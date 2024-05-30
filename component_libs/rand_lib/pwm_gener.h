#ifndef PWM_GENERATOR
#define PWM_GENERATOR

#include <stdio.h>
#include <stdint.h>
#include "./Screen_lib/_1602A_lcd/lcd_16x2.h"
#include "hardware/gpio.h"

// Header should eventually have a setting for all values between 0x00-0xFF.
//  Currently, 2-12-24, only 16 will be created.
//  The ~ are there as the resolution used is only .2f.  Example, 0x2C 1st: ~<= .11 could be .107 and still be working.

#define zero 0x00       //  1st: ~>= .13 2nd: ~>= .05 3rd: ~>= .07
#define one 0x02        //  1st: ~<= .06 2nd: ~>= .05 3rd: ~>= .01
#define two 0x04        //  1st: ~>= .05 2nd: > 0 3rd: ~>= .01
#define three 0x08      //  1st: <= 1.0 2nd: <= 1.0 3rd: ~<= .99
#define four 0x0A       //  1st: ~<= .75 2nd: ~<= .87 3rd: ~<== .80
#define five 0x0C       //  1st: ~<= .62 2nd: ~<== .71 3rd: ~<= .67
#define six 0x0E        //  1st: ~<= .53 2nd: ~<== .60 3rd: ~<= .58
#define seven 0x20      //  1st: ~<= .46 2nd: ~<= .53 3rd: ~<== .50
#define eight 0x22      //  1st: ~<== .40 2nd: ~<== .46 3rd: ~< .45
#define nine 0x24       //  1st: ~<= .15 2nd: ~<= .15 3rd: ~<= .14
#define ten 0x28        
#define eleven 0x2A     //  1st: ~<= .04 2nd: ~<= .04 3rd: ~<= .02
#define twelve 0x2C     //  1st: ~< .11 2nd: ~< .12 3rd: ~<= .12
#define thirteen 0x2E   //  1st: ~< .08 2nd: ~< .09 3rd: ~< .09
#define fourteen 0x40   //  1st: ~<= .04 2nd: ~<= .04 3rd: ~<= .02
#define fifteen 0x42

#define div_One 244.f
#define div_Two 100.f
#define default_Div 1.f

char top_line[16];
char first_line[16];
char bottom_line[16];
char third_line[16];
char empty_line[16];

char current_wrap[16];
char current_clkdiv[16];

uint8_t configure_output;

uint slice_one;
uint chan_one;
uint slice_two;
uint chan_two;
uint slice_three;
uint chan_three;

uint16_t pwm_values_array[16][3] = {

    {10, 20, 30},
    {40, 50, 60},
    {70, 80, 90},
    {100, 110, 120},
    {130, 140, 150},
    {160, 170, 180},
    {190, 200, 210},
    {220, 230, 240},
    {250, 260, 270},
    {280, 290, 300},
    {400, 500, 600},
    {700, 800, 900},
    {1000, 1100, 1200},
    {1300, 1400, 1500},
    {2000, 2500, 3000},
    {4000, 5000, 6000}

};

uint16_t clk_div_cluster[16][3] = {

    {10, 50, 100},
    {20, 100, 200},
    {30, 200, 400},
    {40, 300, 600},
    {50, 400, 800},
    {60, 500, 900},
    {70, 600, 1000},
    {80, 700, 1100},
    {90, 800, 1200},
    {100, 900, 1300},
    {110, 1000, 1500},
    {120, 2000, 3000},
    {130, 3000, 6000},
    {140, 4000, 12000},
    {150, 5000, 24000},
    {200, 6000, 48000}

};

typedef struct pwm_gen_pins {

    uint pwm_serial_in_wrap;
    uint pwm_serial_in_clkdiv;
    uint piso_clk;    // Pulled high normally
    uint piso_latch;  // a.k.a sh/!ld  // Pulled High Normally
    uint clk_inh;   // Pulled High normally
    uint piso_pwm_clk;
    uint piso_slice;
    uint piso_chan;

    uint pwm_one;
    uint pwm_two;
    uint pwm_three;

    bool gpio_state_one;
    bool gpio_state_two;

  uint8_t wrap_configure_value;
  uint8_t clkdiv_configure_value;

  uint8_t previous_configure_value;
  uint8_t previous_clkdiv_configure_value;

  uint16_t configured_input_mesh;
  uint16_t previous_input_mesh;

}pwm_gen_pins;
pwm_gen_pins pwm_pins;


uint8_t lsb_to_msb(uint8_t input, uint8_t output){

  printf("\n\nLSB Current Input: 0x%02x.\n", input);

  output += (input & 0x01) ? 0x80 : 0x00;
  output += (input & 0x02) ? 0x20 : 0x00;
  output += (input & 0x04) ? 0x40 : 0x00;
  output += (input & 0x08) ? 0x10 : 0x00;
  output += (input & 0x10) ? 0x08 : 0x00;
  output += (input & 0x20) ? 0x02 : 0x00;
  output += (input & 0x40) ? 0x04 : 0x00;
  output += (input & 0x80) ? 0x01 : 0x00;

  printf("LSB->MSB Complete: 0x%02x.\n", output);
  return output;
} 


void piso_pin_init(pwm_gen_pins *pins){

  printf("Piso Register Pin Initialization.\n\n");

  if(pins->pwm_serial_in_wrap != UNDEFINED){

   printf("Initializing Pin: %i, PWM Serial In Wrap.\n", pins->pwm_serial_in_wrap);
    gpio_init(pins->pwm_serial_in_wrap);
    gpio_set_dir(pins->pwm_serial_in_wrap, GPIO_IN);
      gpio_pull_down(pins->pwm_serial_in_wrap);
   printf("Pin Initialized.\n");
   
  }
  if(pins->pwm_serial_in_clkdiv != UNDEFINED){

   printf("Initializing Pin: %i, PWM Serial Clkdiv.\n", pins->pwm_serial_in_clkdiv);
    gpio_init(pins->pwm_serial_in_clkdiv);
    gpio_set_dir(pins->pwm_serial_in_clkdiv, GPIO_IN);
      gpio_pull_down(pins->pwm_serial_in_clkdiv);
   printf("Pin Initialized.\n");

  }
  if(pins->piso_clk != UNDEFINED){

   printf("Initializing Pin: %i, Piso Clk.\n", pins->piso_clk);
    gpio_init(pins->piso_clk);
    gpio_set_dir(pins->piso_clk, GPIO_OUT);
    //  gpio_pull_down(pins->piso_clk);
   printf("Pin Initialized.\n");

  }
  if(pins->piso_latch != UNDEFINED){
     printf("Initializing Pin: %i, Piso Latch.\n", pins->piso_latch);
    gpio_init(pins->piso_latch);
    gpio_set_dir(pins->piso_latch, GPIO_OUT);
      gpio_pull_up(pins->piso_latch);
   printf("Pin Initialized.\n");

  }
  if(pins->clk_inh != UNDEFINED){
     printf("Initializing Pin: %i, ClkInh.\n", pins->clk_inh);
    gpio_init(pins->clk_inh);
    gpio_set_dir(pins->clk_inh, GPIO_OUT);
      gpio_pull_up(pins->clk_inh);
   printf("Pin Initialized.\n");

  }
  
  if(pins->piso_pwm_clk != UNDEFINED){

    printf("\nInitializing Pin: %i, Piso PWM Clk.\n\n", pins->piso_pwm_clk);
  //  gpio_init(pins->piso_pwm_clk);
    gpio_set_function(pins->piso_pwm_clk, GPIO_FUNC_PWM);

    pins->piso_slice = pwm_gpio_to_slice_num(pins->piso_pwm_clk);
      printf("Piso Slice: %i.\n", pins->piso_slice);
    pins->piso_chan = pwm_gpio_to_channel(pins->piso_pwm_clk);
      printf("Piso Chan: %c.\n\n", (pins->piso_chan == 0) ? 'A' : 'B');

      pwm_set_clkdiv(pins->piso_slice, 212.f);
      pwm_set_counter(pins->piso_slice, 0x0800);

      pwm_set_wrap(pins->piso_slice, 0x00FF);
      pwm_set_chan_level(pins->piso_slice, pins->piso_chan, .5f);


      pwm_set_enabled(pins->piso_slice, true);

      printf("PWM Settings\n\tClkdiv: 8.f\n\tWrap: 4\n\tChan_Level: 2\n");

        sleep_us(500);

      printf("\nInitialization of Piso Pins Complete.\n\n");

    

  }

}


void piso_clk_pulse(pwm_gen_pins *pin){

  gpio_put(pin->piso_clk, 1);
    sleep_us(100);
  gpio_put(pin->piso_clk, 0);
    sleep_us(100);

}


void piso_clk_inh(pwm_gen_pins *pins){

  gpio_put(pins->piso_clk, 1);
    sleep_us(100);

  gpio_put(pins->clk_inh, 1);
    sleep_us(100);
  gpio_put(pins->piso_clk, 0);
    sleep_us(100);

}


void piso_latchtivate(pwm_gen_pins *pin){

  printf("\nPiso Latch.\n");

  gpio_put(pin->piso_latch, 1);
    sleep_us(100);
  gpio_put(pin->piso_latch, 0);
    sleep_us(100);
  gpio_put(pin->piso_latch, 1);

}


pwm_gen_pins piso_collect(pwm_gen_pins *pins){

  printf("\n\nPiso Collection.\n");

  pins->wrap_configure_value = 0x00;
  pins->clkdiv_configure_value = 0x00;

  uint8_t wrap_buffer;
  wrap_buffer = 0x00;

  uint8_t clkdiv_buffer;
  clkdiv_buffer = 0x00;

  piso_clk_inh(pins);

  piso_latchtivate(pins);
  
  if((pins->pwm_serial_in_wrap != UNDEFINED) || (pins->pwm_serial_in_clkdiv != UNDEFINED)){
        
      gpio_put(pins->clk_inh, 0);
        sleep_us(100);

      gpio_put(pins->clk_inh, 1);

    for(int i = 0; i < 8; i++){

    piso_clk_pulse(pins);

      printf("Collecting bit: %i.\n", i);

    pins->gpio_state_one = (gpio_get(pins->pwm_serial_in_wrap) != 0) ? 1 : 0;
    pins->gpio_state_two = (gpio_get(pins->pwm_serial_in_clkdiv) != 0) ? 1 : 0;

      printf("\nCurrent Wrap GPIO state: %i.\n", pins->gpio_state_one);
      printf("Current Clkdiv GPIO state: %i.\n", pins->gpio_state_two);

    wrap_buffer += (pins->gpio_state_one == 1) ? (0x01 << i) : (0 << i);
    clkdiv_buffer += (pins->gpio_state_two == 1) ? (0x01 << i) : (0 << i);

  //    printf("Piso Wrap Buffer Value: 0x%02x.\n", wrap_buffer);
  //    printf("Piso Clkdiv Buffer Value: 0x%02x.\n\n", clkdiv_buffer);

  //  piso_clk_pulse(pins);
      sleep_us(100);
        
    }

    piso_clk_pulse(pins);

  //  printf("\n\nWrap and Clkdiv Input Collected.\n");
    
  }
  
  pins->wrap_configure_value = lsb_to_msb(wrap_buffer, pins->wrap_configure_value);
 // print_binary(pins->wrap_configure_value);
  pins->clkdiv_configure_value = lsb_to_msb(clkdiv_buffer, pins->clkdiv_configure_value);
 // print_binary(pins->clkdiv_configure_value);
  
  return *pins;

  }


uint16_t pwm_configured_inputs(pwm_gen_pins *values, uint8_t wrap_out, uint8_t clkdiv_out){

uint16_t mesh_out;

  wrap_out = 0x00;
  clkdiv_out = 0x00;
  mesh_out = 0x0000;

  piso_collect(values);

  wrap_out = values->wrap_configure_value;
//  print_binary(wrap_out);
  clkdiv_out = values->clkdiv_configure_value;
//  print_binary(clkdiv_out);
  mesh_out = uints_mesh(wrap_out, clkdiv_out);
  print_binary(mesh_out);

    printf("\nMesh Value: 0x%04x.\n", mesh_out);

  return mesh_out;

}


void pwm_clkdiv_setter(uint slice_o, uint slice_tw, uint slice_th, uint16_t div_one, uint16_t div_two, uint16_t div_three){

  printf("\n\nSetting ClkDiv's now.\n");
   
    float new_one, new_two, new_three;
    uint16_t pwm_cnt;

    new_one = (float)div_one;
    new_two = (float)div_two;
    new_three = (float)div_three;

    pwm_set_clkdiv(slice_o, new_one);
      pwm_cnt = pwm_get_counter(slice_o);
        printf("Slice One CLK Div: %f\n\tSlice Count: 0x%04x\n", new_one, pwm_cnt);

    pwm_set_clkdiv(slice_tw, new_two);
      pwm_cnt = pwm_get_counter(slice_tw);
        printf("Slice Two CLK Div: %f\n\tSlice Count: 0x%04x\n", new_two, pwm_cnt);

    pwm_set_clkdiv(slice_th, new_three);
      pwm_cnt = pwm_get_counter(slice_th);
        printf("Slice Three CLK Div: %f\n\tSlice Count: 0x%04x\n", new_three, pwm_cnt);

  printf("\nClk Div Setting Configured.\n\n");

}


void pwm_initialize_three(pwm_gen_pins *pins, uint8_t configed, uint16_t dOne, uint16_t dTwo, uint16_t dThree){

    printf("\nPWM Initialization Function.\n");

  printf("Initializing PWM One.\n");
    gpio_set_function(pins->pwm_one, GPIO_FUNC_PWM);
    slice_one = pwm_gpio_to_slice_num(pins->pwm_one);
    chan_one = pwm_gpio_to_channel(pins->pwm_one);

  printf("Initializing PWM Two.\n");
    gpio_set_function(pins->pwm_two, GPIO_FUNC_PWM);
    slice_two = pwm_gpio_to_slice_num(pins->pwm_two);
    chan_two = pwm_gpio_to_channel(pins->pwm_two);

  printf("Initializing PWM Three.\n");
    gpio_set_function(pins->pwm_three, GPIO_FUNC_PWM);
    slice_three = pwm_gpio_to_slice_num(pins->pwm_three);
    chan_three = pwm_gpio_to_channel(pins->pwm_three);

  pwm_clkdiv_setter(slice_one, slice_two, slice_three, dOne, dTwo, dThree);

    switch(configed){

        case zero:
          printf("Case Zero.\n");
            pwm_set_wrap(slice_one, pwm_values_array[0][0]);
              //printf("Value of PWM Array[0][0]: %i.\n", pwm_values_array[0][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[0][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[0][1]);
              //printf("Value of PWM Array[0][1]: %i.\n", pwm_values_array[0][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[0][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[0][2]);
              //printf("Value of PWM Array[0][2]: %i.\n", pwm_values_array[0][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[0][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case one:
          printf("Case One.\n");
            pwm_set_wrap(slice_one, pwm_values_array[1][0]);
            pwm_set_chan_level(slice_one, chan_one, (uint16_t)(pwm_values_array[1][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[1][1]);
            pwm_set_chan_level(slice_two, chan_two, (uint16_t)(pwm_values_array[1][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[1][2]);
            pwm_set_chan_level(slice_three, chan_three, (uint16_t)(pwm_values_array[1][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case two:
          printf("Case Two.\n");
            pwm_set_wrap(slice_one, pwm_values_array[2][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[2][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[2][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[2][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[2][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[2][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case three:
          printf("Case Three.\n");
            pwm_set_wrap(slice_one, pwm_values_array[3][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[3][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[3][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[3][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[3][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[3][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case four:
          printf("Case Four.\n");
            pwm_set_wrap(slice_one, pwm_values_array[4][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[4][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[4][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[4][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[4][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[4][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case five:
          printf("Case Five.\n");
            pwm_set_wrap(slice_one, pwm_values_array[5][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[5][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[5][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[5][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[5][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[5][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case six:
          printf("Case Six.\n");
            pwm_set_wrap(slice_one, pwm_values_array[6][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[6][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[6][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[6][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[6][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[6][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case seven:
          printf("Case Seven.\n");
            pwm_set_wrap(slice_one, pwm_values_array[7][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[7][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[7][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[7][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[7][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[7][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case eight:
           printf("Case Eight.\n");
            pwm_set_wrap(slice_one, pwm_values_array[8][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[8][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[8][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[8][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[8][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[8][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case nine:
          printf("Case Nine.\n");
            pwm_set_wrap(slice_one, pwm_values_array[9][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[9][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[9][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[9][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[9][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[9][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case ten:
          printf("Case Ten.\n");
            pwm_set_wrap(slice_one, pwm_values_array[10][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[10][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[10][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[10][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[10][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[10][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case eleven:
          printf("Case Eleven.\n");
            pwm_set_wrap(slice_one, pwm_values_array[11][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[11][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[11][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[11][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[11][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[11][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case twelve:
          printf("Case Twelve.\n");
            pwm_set_wrap(slice_one, pwm_values_array[12][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[12][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[12][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[12][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[12][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[12][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case thirteen:
          printf("Case Thirteen.\n");
            pwm_set_wrap(slice_one, pwm_values_array[13][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[13][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[13][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[13][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[13][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[13][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case fourteen:
          printf("Case Fourteen.\n");
            pwm_set_wrap(slice_one, pwm_values_array[14][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[14][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[14][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[14][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[14][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[14][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        case fifteen:
          printf("Case Fifteen.\n");
            pwm_set_wrap(slice_one, pwm_values_array[15][0]);
            pwm_set_chan_level(slice_one, chan_one, (pwm_values_array[15][0] * .5));
            pwm_set_enabled(slice_one, true);

            pwm_set_wrap(slice_two, pwm_values_array[15][1]);
            pwm_set_chan_level(slice_two, chan_two, (pwm_values_array[15][1] * .5));
            pwm_set_enabled(slice_two, true);

            pwm_set_wrap(slice_three, pwm_values_array[15][2]);
            pwm_set_chan_level(slice_three, chan_three, (pwm_values_array[15][2] * .5));
            pwm_set_enabled(slice_three, true);
        break;

        default:

            printf("The Configuration output was either null, or an invalid selection.\n");
        break;
    }


}


void pwm_dc_set(adc_port_values *adc, uint8_t config_setting){

    printf("PWM Duty Cycle Set Function.\n\n");

    switch(config_setting){

    case zero:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[0][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[0][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[0][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[0][2]));
    break;

    case one:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[1][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[1][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[1][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[1][2]));
    break;

    case two:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[2][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[2][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[2][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[2][2]));
    break;

    case three:
        pwm_set_chan_level(slice_one, chan_one,(adc->adc0_mapped_float * pwm_values_array[3][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[3][0])));
        pwm_set_chan_level(slice_two, chan_two, (adc->adc1_mapped_float * pwm_values_array[3][1]));
        pwm_set_chan_level(slice_three, chan_three, (adc->adc2_mapped_float * pwm_values_array[3][2]));
    break;

    case four:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[4][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[4][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[4][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[4][2]));
    break;

    case five:
    printf("Case 0x0C.\n");
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[5][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[5][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[5][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[5][2]));
    break;

    case six:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[6][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[6][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[6][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[6][2]));
    break;

    case seven:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[7][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[7][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[7][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[7][2]));
    break;

    case eight:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[8][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[8][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[8][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[8][2]));
    break;

    case nine:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[9][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[9][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[9][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[9][2]));
    break;

    case ten:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[10][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[10][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[10][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[10][2]));
    break;

    case eleven:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[11][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[11][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[11][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[11][2]));
    break;

    case twelve:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[12][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[12][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[12][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[12][2]));
    break;

    case thirteen:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[13][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[13][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[13][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[13][2]));
    break;

    case fourteen:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[14][0]));
          printf("Calculation: %04x.\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[14][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[14][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[14][2]));
    break;

    case fifteen:
        pwm_set_chan_level(slice_one, chan_one, (uint16_t)(adc->adc0_mapped_float * pwm_values_array[15][0]));
          printf("Calculation: %.1f\n", ((uint16_t)(adc->adc0_mapped_float * pwm_values_array[15][0])));
        pwm_set_chan_level(slice_two, chan_two, (uint16_t)(adc->adc1_mapped_float * pwm_values_array[15][1]));
        pwm_set_chan_level(slice_three, chan_three, (uint16_t)(adc->adc2_mapped_float * pwm_values_array[15][2]));
    break;

    default:

        printf("Error in PWM Channel setting.\n");
    break;
    }
}


void program_by_print(LCDPins *pins, char *top, char *bottom){
    write_display(pins, top, bottom);
}
//  program_by_print(&lcdpins, by_line, my_tag);


void pwm_initial_view(LCDPins *l_pins, uint8_t config, char *top, char *bottom, pwm_gen_pins *pins){

    printf("\nViewing pins and configure value.\n");

    sprintf(top, "Pins: %i, %i, %i",pins->pwm_one, pins->pwm_two, pins->pwm_three);
    sprintf(bottom, "Config Set: %02x", config);

    write_display(l_pins, top, bottom);
        sleep_ms(750);
}
//  pwm_initial_view(configuration_output, top_line, bottom_line, &pwm_pins);


void pwm_adjustment_view(LCDPins *l_pins, adc_port_values *adc, char *top, char *bottom, char *third, char *empty){

    printf("PWM Adjustment View Function.\n");

    memset(top, 0, sizeof(top));

    sprintf(top, "First DC: %.2f", (adc->adc0_mapped_float));
        printf("ADC0 Mapped Value: %.2f\n", adc->adc0_mapped_float);
    sprintf(bottom, "Second DC: %.2f", (float)(adc->adc1_mapped_float));
        printf("ADC1 Mapped Value: %.2f\n", adc->adc1_mapped_float);
    sprintf(third, "Third DC: %.2f", (float)(adc->adc2_mapped_float));
        printf("ADC2 Mapped Value: %.2f\n", adc->adc2_mapped_float);

      sleep_us(100);

    write_display(l_pins, top, bottom);
        sleep_ms(750);

    write_display(l_pins, third, empty);
        sleep_ms(500);

}


void user_input_view(LCDPins *l_pins, pwm_gen_pins *value, char *top_line, char *bottom_line){

    printf("\nUser Input View Function.\n");

    sprintf(top_line, "U Wrap: 0x%04x", value->wrap_configure_value);
        printf("Current User Inputted Wrap Value: 0x%04x\n", value->wrap_configure_value);

    sprintf(bottom_line, "ClkDiv: 0x%04x", value->clkdiv_configure_value);
        printf("Current User Inputted ClkDiv: 0x%04x\n", value->clkdiv_configure_value);

      sleep_us(100);

    write_display(l_pins, top_line, bottom_line);
        sleep_ms(750);

  printf("\n\n");

}


uint16_t piso_parity_check(pwm_gen_pins *values){

  uint16_t input_one_buffer, input_two_buffer, input_three_buffer;

  uint16_t output;

  uint8_t w_val, d_val;

  input_one_buffer = 0x0000;
  input_two_buffer = 0x0000;
  input_three_buffer = 0x0000;

  w_val = 0x00;
  d_val = 0x00;

  input_one_buffer = pwm_configured_inputs(values, w_val, d_val);
    printf("Parity Check One: 0x%04x\n", input_one_buffer);

  w_val = 0x00;
  d_val = 0x00;

  input_two_buffer = pwm_configured_inputs(values, w_val, d_val);
    printf("Parity Check Two: 0x%04x\n", input_two_buffer);

  w_val = 0x00;
  d_val = 0x00;

  input_three_buffer = pwm_configured_inputs(values, w_val, d_val);
    printf("Parity Check Three: 0x%04x\n", input_three_buffer);

    if((input_one_buffer == input_two_buffer) || (input_one_buffer == input_three_buffer)){

      output = (input_one_buffer & input_two_buffer) ? input_two_buffer : input_three_buffer;

      return output;

    }else {

      return 0x0001;

    }

}

void reset_to_previous_configure(pwm_gen_pins *values, adc_port_values *adc, uint slice_one, uint slice_two, uint slice_three){

    printf("\n\nResetting configuration.\n\n");

  uint16_t in_buf;
  uint8_t w_buf;
  uint8_t c_buf;

  in_buf = values->previous_input_mesh;
  w_buf = ((in_buf >> 8) & 0x00FF);
  c_buf = (in_buf & 0x00FF);

    printf("Input Mesh: 0x%04x\n\tSliced Wrap: 0x%02x.\n\tSliced ClkDiv: 0x%02x\n", values->configured_input_mesh, w_buf, c_buf);


  printf("Setting ClkDiv:\n\tOne: 0x%02x\n\tTwo: %.2f\n\tThree: %.2f\n", (in_buf & 0x00FF), default_Div, div_Two);
    if((in_buf == 0)){
        //  set minimum range.
       in_buf = 10.f;
  pwm_clkdiv_setter(slice_one, slice_two, slice_three, (in_buf & 0x00FF), default_Div, div_Two);
    }
  printf("Resetting Duty Cycle Configuration.\n\tPrevious Wrap: 0x%02x\n", w_buf);
    if(w_buf == 0){
      w_buf = 32;
  pwm_dc_set(adc, w_buf);
    }
    printf("\n\nEnd of configuration reset.\n\n");
}



#endif
