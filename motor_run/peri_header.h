/*
  PERI_HEADER file written by Alphonse Reitz.
  @alphonseJmR
*/
#ifndef PERI_FUNCS
#define PERI_FUNCS

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "lcd_16x2.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

char top_string[16];
char bottom_string[16];

enum pico_pins {
  UNDEFINED = -1,
  GPIO_ZERO,
  GPIO_ONE,
  GPIO_TWO,
  GPIO_THREE,
  GPIO_FOUR,
  GPIO_FIVE,
  GPIO_SIX,
  GPIO_SEVEN,
  GPIO_EIGHT,
  GPIO_NINE,
  GPIO_TEN,
  GPIO_ELEVEN,
  GPIO_TWELVE,
  GPIO_THIRTEEN,
  GPIO_FOURTEEN,
  GPIO_FIFTEEN,
  GPIO_SIXTEEN,
  GPIO_SEVENTEEN,
  GPIO_EIGHTEEN,
  GPIO_NINETEEN,
  GPIO_TWENTY,
  GPIO_TWENTY_ONE,
  GPIO_TWENTY_TWO,
  GPIO_TWENTY_SIX = 26,
  GPIO_TWENTY_SEVEN = 27, 
  GPIO_TWENTY_EIGHT = 28
};

typedef struct {

  const uint step_a;                //  Clockwise rotation is a,c,b,d.
  const uint step_b;
  const uint step_c;                //  Counter-clockwise rotation is d,b,c,a.
  const uint step_d;
  uint16_t steps_per_revolution;    // User defined variable.  28BYJ-48 is 2038 steps.
  uint16_t current_absolute_revolution;     //  C_A_R = C_CW_R - C_CCW_R
  uint16_t current_CW_revolutions;     //  Increment per steps_per_revolution steps.
  uint16_t CW_revolution_steps;
  uint16_t current_CCW_revolutions;
  uint16_t CCW_revolution_steps;
  uint16_t step_value;              //  To be set equal to rotary input value.
  uint16_t previous_step;           //  To check rotary value.
  int current_step;            //  Current step = previous_step - step_value.
  uint8_t max_CW_revolutions;
  uint8_t max_CCW_revolutions;

}s_motor_t;
s_motor_t stepping;

typedef struct {
const uint rotary_button_clk;
const uint rotary_button_dt;
const uint rotary_button_button;
const uint analog_button_one;
const uint analog_button_two;
const uint button_one;
const uint button_two;
const uint button_three;
}button_types;
button_types enabled_buttons;

typedef struct {
  volatile bool rotary_button_status;
  volatile bool button_one_status;
  volatile bool button_two_status;
  volatile bool button_three_status;
  volatile bool analog_one_status;
  volatile bool analog_two_status;
}buttons_onoff;
buttons_onoff button_status;

typedef struct {
  
volatile bool previous_rotary_clk;
volatile bool previous_rotary_dt;
volatile bool current_rotary_clk;
volatile bool current_rotary_dt;
volatile uint16_t rotary_value;
const uint16_t max_rotation_value;
const uint32_t minimum_required_interrupt;
uint16_t rotary_total;

} rotary_encoder_struct;
rotary_encoder_struct rotary;

typedef struct {
  uint register_one_data;
  uint register_one_latch;
  uint register_one_enable;
  uint register_two_data;
  uint register_two_latch;
  uint register_two_enable;
  uint register_clk_line;
} register_pins;
register_pins reg_pins;

typedef struct {
  uint8_t register_value_zero;
  uint8_t register_value_one;
  uint8_t register_value_two;
  uint8_t register_value_three;
  uint8_t register_value_four;
  uint8_t register_value_five;
  uint8_t register_value_six;
  uint8_t register_value_seven;
} uint8_variables;
uint8_variables u_vars;

typedef struct {

  uint32_t bcb_current_interrupt;
  uint32_t rei_current_interrupt;
  uint32_t rotary_initial;
  uint32_t rotary_last;
  uint32_t button_initial;
  uint32_t button_last;
  uint32_t clk_last;
  uint32_t dt_last;
  int clk_max;
  int dt_max;
  int button_max;
  int interruption_max; 
  uint32_t current_interruption;
  uint32_t initial_interruption;
  uint32_t last_interruption;

}interrupt_times_t;
  interrupt_times_t callback_times;

typedef struct {
  uint adc0_pin;
  uint adc1_pin;
  uint adc2_pin;
  uint16_t adc0_raw_read;
  uint16_t adc1_raw_read;
  uint16_t adc2_raw_read;
  uint16_t adc0_min_in_map_value;
  uint16_t adc0_max_in_map_value;
  uint16_t adc1_min_in_map_value;
  uint16_t adc1_max_in_map_value;
  uint16_t adc2_min_in_map_value;
  uint16_t adc2_max_in_map_value;
  uint16_t adc0_min_out_map_value;
  uint16_t adc0_max_out_map_value;
  uint16_t adc1_min_out_map_value;
  uint16_t adc1_max_out_map_value;
  uint16_t adc2_min_out_map_value;
  uint16_t adc2_max_out_map_value;
  uint16_t adc0_mapped_value;
  uint16_t adc1_mapped_value;
  uint16_t adc2_mapped_value;
} adc_port_values;
adc_port_values pico_adc;

typedef struct {
  const uint8_t battery_high;
  const uint8_t battery_medium;
  const uint8_t battery_low;
  const uint8_t battery_critical;
  bool battery_indication_status;
  const uint battery_red;
  const uint battery_blue;
  const uint battery_green;
}battery_data_t;
battery_data_t bat_status;

typedef struct {

  float servo_initial_position;     //  Initialize to 1500 for most servo motors.
  bool servo_inital_positioning;    //  Set true until initialization is finished. 
  float servo_one_position;         //  Variable to set servo one position.
  float servo_two_position;         //  Variable to set servo two position.
  float servo_three_position;       //  Variable to set servo three position.
  float servo_four_position;        //  Variable to set servo four position.
  uint32_t mcu_clock;               //  Variable to set mcu's clock speed.  RP2040(Pi Pico) is 125000000.

  uint servo_one_slice;             //  Hold slice of servo one pin.
  uint servo_two_slice;             //  Hold slice of servo two pin.
  uint servo_three_slice;           //  Hold slice of servo three pin.
  uint servo_four_slice;            //  Hold slice of servo four pin.

  uint servo_one_chan;              //  Hold channel of servo one pin.
  uint servo_two_chan;              //  Hold channel of servo two pin.
  uint servo_three_chan;            //  Hold channel of servo three pin.
  uint servo_four_chan;             //  Hold channel of servo four pin.

  float so_clkdiv;                  //  Set servo one clk divisior.  Typically 64.
  uint16_t so_wrap;                 //  Set servo one wrap value.  Typically 39062.
  float st_clkdiv;                  //  Set servo two clk divisior.  Typically 64.
  uint16_t st_wrap;                 //  Set servo two wrap value.  Typically 39062.
  float stt_clkdiv;                 //  Set servo three clk divisior.  Typically 64.
  uint16_t stt_wrap;                //  Set servo three wrap value.  Typically 39062.
  float sf_clkdiv;                  //  Set servo four clk divisior.  Typically 64.
  uint16_t sf_wrap;                 //  Set servo four wrap value.  Typically 39062.

  uint servo_one_pin;       //  .servo_one_pin = GPIO_(insert correct pin)
  uint servo_two_pin;       //  .servo_two_pin = GPIO_(insert correct pin)
  uint servo_three_pin;     //  .servo_three_pin = GPIO_(insert correct pin)
  uint servo_four_pin;      //  .servo_four_pin = GPIO_(insert correct pin)

}servo_motor_config_t;
servo_motor_config_t servo_configuration;

typedef struct {
  uint trigger_one;
  uint trigger_two;
  uint echo_one;
  uint echo_two;
  uint32_t usonic_one_start;
  uint32_t usonic_two_start;
  uint32_t usonic_one_end;
  uint32_t usonic_two_end;
  uint32_t pulse_duration_one;
  uint32_t pulse_duration_two;
  double distance_one;
  double distance_two;
  uint16_t returned_distance_one;
  uint16_t returned_distance_two;

}ultra_sonic_data_t;
ultra_sonic_data_t usonic;

typedef struct {

  uint32_t first_light_fall;      //  Start time since photoresistor crossed threshold.
  uint32_t current_darkness;      //  Current time since photoresistor crossed.
  uint32_t elapsed_darkness;      //  Current time - first light fall.
  uint32_t darkness_threshold;    //  Time required to set backlight toggle value.

}back_timed_t;
back_timed_t back_time;


// ===== Functions Below ========

/*
    long impliments the variable to handle large values, can be changed if precision is necessary or easy.
    Parameters:     --long type implied--
    x:          Input for both analog.vertical & analog.horizontal.
    in_min:     Minimum value that an ADC can input. i.e {in_min = 0 || in_min = -1024}
    in_max:     Maximum value that an ADC can input. i.e {in_max = 1024 || in_max = 4096}
    out_min:    Minimum value to be set by MAP function. i.e {out_min = 0 || out_min = -1024}
    out_max:    Maximum value to be set by MAP function. i.e {out_max = 255 || out_max = 4096}

    EXAMPLE:
    x = 3600    in_min = 0      in_max = 4096       out_min = 200       out_max = 600

    (3600-0)*(600-200)/(4096-0)+200

    ===
    
    551.5625 the .5625 can be dropped if desired.
*/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//  Use to console print the binary form of the inputted variable.  Able to accept UINT, UINT8_T, UINT16_T, UINT32_T data.
//  Example print_binary(0, 0, 2056, 0);  Would print the binary form of the uint16_t value 2056.
void print_binary(uint byte_input, uint8_t two_byte_input, uint16_t four_byte_input, uint32_t eight_byte_input) {
    printf("\n");
    if(byte_input > 0){
      for (int i = 3; i >= 0; i--) {
        if ((byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }else {
      printf("\nNo UINT data.\n");
    }
    if(two_byte_input > 0){
      for (int i = 7; i >= 0; i--) {
        if ((two_byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }else {
      printf("\nNo UINT8_T data.\n");
    }
    if(four_byte_input > 0){
      for (int i = 15; i >= 0; i--) {
        if ((four_byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }else {
      printf("\nNo UINT16_T data.\n");
    }
    if(eight_byte_input > 0){
      for (int i = 31; i >= 0; i--) {
        if ((eight_byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }else {
      printf("\nNo UINT32_T data.\n");
    }
    
    printf("\n");
}

// Large Button Call_back Function
void enabled_button_callback(uint gpio, uint32_t events){

  // printf("Interruption Occured:\n \tPin: %d.\n\tEvent: %d.\n", gpio, events);
 
  //  Main callback interruption time.
  callback_times.current_interruption = time_us_32();
  callback_times.last_interruption = callback_times.current_interruption - callback_times.initial_interruption; 

  // Callback time for rotary encoder interruption time (rei).
  callback_times.rei_current_interrupt = time_us_32();
  callback_times.rotary_last = callback_times.rei_current_interrupt - callback_times.rotary_initial;

  // Callback time for enabled buttons interruption time.
  callback_times.bcb_current_interrupt = time_us_32();
  callback_times.button_last = callback_times.bcb_current_interrupt - callback_times.button_initial;

  if(callback_times.last_interruption >= callback_times.interruption_max){

      printf("Interruption occured at:\n \tPin: %i.\n\tEvent: %i.\n", gpio, events);
    callback_times.initial_interruption = callback_times.current_interruption;
    gpio_acknowledge_irq(gpio, events);
  
  if(callback_times.rotary_last > rotary.minimum_required_interrupt){
    callback_times.rotary_initial = callback_times.rei_current_interrupt;
  
    if(gpio == enabled_buttons.rotary_button_clk && (events & 0x04)){
      rotary.current_rotary_clk = gpio_get(enabled_buttons.rotary_button_clk);
      rotary.current_rotary_dt = gpio_get(enabled_buttons.rotary_button_dt);
        printf("Clk Edge Detected.\n");
      if(rotary.current_rotary_clk != rotary.previous_rotary_clk){
          printf("Clk Status Changed.\n");
          printf("CLK Current Status: %i.\n", rotary.current_rotary_clk);
          printf("DT Current Status: %i.\n", rotary.current_rotary_dt);
        if(rotary.current_rotary_dt != rotary.current_rotary_clk){
          printf("B != A.\n");
            if(rotary.rotary_total <= rotary.max_rotation_value){
          rotary.rotary_total++;
            }else {
              printf("Maximum Rotation Value.\n");
            }
         
        }else {
            printf("B == A.\n");
            if(rotary.rotary_total > 0){
            rotary.rotary_total--;
            } else {
              printf("Minimum Rotation Value.\n");
          }
           
      }
    }
      }
      printf("Rotary Total Value-> %i.\n", rotary.rotary_total);
    rotary.previous_rotary_clk = rotary.current_rotary_clk;
    rotary.previous_rotary_dt = rotary.current_rotary_dt;
    } 
  
  }

  if(gpio == enabled_buttons.rotary_button_button){

      printf("Rotary Button Press Occured.\n");
      gpio_acknowledge_irq(gpio, events);
      callback_times.button_initial = callback_times.bcb_current_interrupt;
      if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.rotary_button_status = true;
      }
  }
  
  if(gpio == enabled_buttons.analog_button_one){
    
      printf("Analog Button One Press Occured.\n");
      gpio_acknowledge_irq(gpio, events);
      callback_times.button_initial = callback_times.bcb_current_interrupt;
      if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.analog_one_status = true;
      }
  } 
  
  if(gpio == enabled_buttons.analog_button_two){

      printf("Analog Button Two Press Occured.\n");
      gpio_acknowledge_irq(gpio, events);
        callback_times.button_initial = callback_times.bcb_current_interrupt;
      if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.analog_two_status = true;
      }
  } 
  
  if(gpio == enabled_buttons.button_one){

    printf("Button One Press Occured.\n");
    gpio_acknowledge_irq(gpio, events);
      callback_times.button_initial = callback_times.bcb_current_interrupt;
    if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.button_one_status = true;
      }
  } 
  
  if(gpio == enabled_buttons.button_two){
    
    printf("Button Two Press Occured.\n");
      gpio_acknowledge_irq(gpio, events);
        callback_times.button_initial = callback_times.bcb_current_interrupt;
    if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.button_two_status = true;
      }
  } 
  
 }

//  Adc_pin_call function takes in a struct and based on the set values, can call all three ADC ports, and map their respective values.
void adc_pin_call(adc_port_values *config) {
  /*
    Select inputs according to ADC enabled pins.  Perform ADC call on inputs 0, 1, or 2. 
    Select which input to read data from (0 | 1 | 2).
  */
  if(config->adc0_pin != (int)NULL) {
    adc_select_input(0);
    config->adc0_raw_read = adc_read();
    printf("\n\n\tRaw value of ADC0: %d.\n", config->adc0_raw_read);
    }
  if(config->adc1_pin != (int)NULL) {
    adc_select_input(1);
    config->adc1_raw_read = adc_read();
    printf("\t\tRaw value of ADC1: %d.\n", config->adc1_raw_read);
    }
  if(config->adc2_pin != (int)NULL) {
    adc_select_input(2);
    config->adc2_raw_read = adc_read();
    printf("\t\t\tRaw value of ADC2: %d.\n\n", config->adc2_raw_read);
    }
    
    //  Convert raw analog input data of vertical & horizontal using MAP function.
    //  Store in respectively named analog.()_raw_conversion variable.
    if(config->adc0_raw_read > 0){
    config->adc0_mapped_value = map(config->adc0_raw_read, config->adc0_min_in_map_value, config->adc0_max_in_map_value, config->adc0_min_out_map_value, config->adc0_max_out_map_value);
    } else {
      printf("\tADC0 Port Has No Value.\n");
    }
    if(config->adc1_raw_read > 0){
    config->adc1_mapped_value = map(config->adc1_raw_read, config->adc1_min_in_map_value, config->adc1_max_in_map_value, config->adc1_min_out_map_value, config->adc1_max_out_map_value);
    }  else {
      printf("\tADC1 Port Has No Value.\n");
    }
    if(config->adc2_raw_read > 0){
    config->adc2_mapped_value = map(config->adc2_raw_read, config->adc2_min_in_map_value, config->adc2_max_in_map_value, config->adc2_min_out_map_value, config->adc2_max_out_map_value);
    }  else {
      printf("\tADC2 Port Has No Value.\n");
    }

    
};

void adc_pin_setup(adc_port_values *config) {

  printf("\nADC Pin Initialization.\n");

  if(config->adc0_pin != (int)NULL){
  adc_gpio_init(config->adc0_pin);
  printf("ADC0 Pin Initialized.\n");
  printf("ADC0 PIN: %d.\n\n", config->adc0_pin);
  }else {
    printf("ADC0 Not Initialized.\n");
  }

  if(config->adc1_pin != (int)NULL){
  adc_gpio_init(config->adc1_pin);
  printf("ADC1 Pin Initialized.\n");
  printf("ADC1 PIN: %d.\n\n", config->adc1_pin);
  }else {
    printf("ADC1 Not Initialized.\n");
  }

  if(config->adc2_pin != (int)NULL){
  adc_gpio_init(config->adc2_pin);
  printf("ADC2 Pin Initialized.\n");
  printf("ADC2 PIN: %d.\n\n", config->adc2_pin);
  }else {
    printf("ADC2 Not Initialized.\n");
  }

}

/*  Button Interrupt Init
  Sets GPIO function enabled with callback for up to four gpio interrupts.
  Accepts a struct of button_types
  Using if statements to determine if struct values are null or not
  Initializes interrupts for gpio pins if true.
*/
void button_interrupt_init(button_types *types){

  printf("\n\nInterrupt Button Initializing.\n");

        if(types->rotary_button_button != (-1)){
          gpio_init(types->rotary_button_button);
      gpio_set_dir(types->rotary_button_button, GPIO_IN);
     // gpio_pull_up(types->rotary_button_button);
        gpio_set_irq_enabled_with_callback(types->rotary_button_button, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n\n", types->rotary_button_button);
    } else {
                printf("Rotary Button Not Used.\n\n");
    }

        if(types->analog_button_one != (-1)){
      gpio_init(types->analog_button_one);
      gpio_set_dir(types->analog_button_one, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button One: Pin %d.  Initialized.\n\n", (types->analog_button_one));
    } else {
        printf("Analog Button One Not Used.\n\n");
    }

    if(types->analog_button_two != (-1)){
      gpio_init(types->analog_button_two);
      gpio_set_dir(types->analog_button_two, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button Two: Pin %d.  Initialized.\n\n", types->analog_button_two);
    } else {
        printf("Analog Button Two Not Used.\n\n");
    }

    if(types->button_one != (-1)){
      gpio_init(types->button_one);
      gpio_set_dir(types->button_one, GPIO_IN);
      gpio_pull_up(types->button_one);
      gpio_set_irq_enabled_with_callback(types->button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n\n", types->button_one);
    } else {
                printf("Button OneNot Used.\n\n");
    }
    if(types->button_two != (-1)){
      gpio_init(types->button_two);
      gpio_set_dir(types->button_two, GPIO_IN);
      gpio_pull_down(types->analog_button_two);
      gpio_set_irq_enabled_with_callback(types->button_two, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n\n", types->button_two);
    } else {
                printf("Button Two Not Used.\n\n");
    }

    if(types->button_three != (-1)){
      gpio_init(types->button_three);
      gpio_set_dir(types->button_three, GPIO_IN);
      gpio_pull_down(types->button_three);
      gpio_set_irq_enabled_with_callback(types->button_three, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n\n", types->button_three);
    } else {
                printf("Button Three Not Used.\n\n");
    }
}

void rotary_encoder_init(button_types *types){
  
      if(types->rotary_button_clk != (-1)){
      gpio_init(types->rotary_button_clk);
      gpio_set_dir(types->rotary_button_clk, GPIO_IN);
      gpio_pull_up(types->rotary_button_clk);
      gpio_set_irq_enabled_with_callback(types->rotary_button_clk, 0x04, true, &enabled_button_callback);
        printf("Button Type: Rotary Clk: Pin %d.  Initialized.\n\n", types->rotary_button_clk);
    } else {
                printf("Rotary Clk Not Used.\n\n");
    }

        if(types->rotary_button_dt != (-1)){
          gpio_init(types->rotary_button_dt);
      gpio_set_dir(types->rotary_button_dt, GPIO_IN);
      gpio_pull_up(types->rotary_button_dt);
      gpio_set_irq_enabled_with_callback(types->rotary_button_dt, 0x04, true, &enabled_button_callback);
        printf("Button Type: Rotary DT: Pin %d.  Initialized.\n\n", types->rotary_button_dt);
    } else {
                printf("Rotary DT Not Used.\n\n");
    }

}

void battery_light_init(battery_data_t *data){
  
  if(data->battery_red != (-1)){
    printf("Red Indicator Initialilzing.");
    gpio_init(data->battery_red);
    gpio_set_dir(data->battery_red, GPIO_OUT);
    
  }
  if(data->battery_blue != (-1)){
    printf("Blue Indicator Initialilzing.");
    gpio_init(data->battery_blue);
    gpio_set_dir(data->battery_blue, GPIO_OUT);
  }
  if(data->battery_green != (-1)){
    printf("Green Indicator Initialilzing.");
    gpio_init(data->battery_green);
    gpio_set_dir(data->battery_green, GPIO_OUT);
  }

}

void battery_status_check(battery_data_t *level, adc_port_values *read){

  if(read->adc1_mapped_value >= level->battery_high) {
    gpio_put(level->battery_green, 1);
    gpio_put(level->battery_blue, 0);
    gpio_put(level->battery_red, 0);
  }
  if((read->adc1_mapped_value >= level->battery_medium) && (read->adc1_mapped_value < level->battery_high)){
    gpio_put(level->battery_blue, 1);
    gpio_put(level->battery_green, 0);
    gpio_put(level->battery_red, 0);
  }
  if((read->adc1_mapped_value >= level->battery_low) && (read->adc1_mapped_value < level->battery_medium)){
    gpio_put(level->battery_red, 1);
    gpio_put(level->battery_green, 0);
    gpio_put(level->battery_blue, 0);
  }
  if((read->adc1_mapped_value > 0) && (read->adc1_mapped_value < level->battery_critical)){

    for(int i = 0; i < 6; i++){
      gpio_put(level->battery_red, 1);
      gpio_put(level->battery_red, 0);
    }

  }

}

void ultra_sonic_sensor_pin_setup(ultra_sonic_data_t *pins) {
  if(pins->trigger_one != (-1)){
    gpio_init(pins->trigger_one);
    gpio_set_dir(pins->trigger_one, GPIO_OUT);
    printf("Ultra Sonic Trigger One:\tInitialized.\n");
  }else {
    printf("Ultra Sonic Trigger One:\tNot Initialized.\n");
  }
  if(pins->trigger_two != (-1)){
    gpio_init(pins->trigger_two);
    gpio_set_dir(pins->trigger_two, GPIO_OUT);
    printf("Ultra Sonic Trigger Two:\tInitialized.\n");
  }else {
    printf("Ultra Sonic Trigger Two:\tNot Initialized.\n");
  }
  if(pins->echo_one != (-1)){
    gpio_init(pins->echo_one);
    gpio_set_dir(pins->echo_one, GPIO_IN);
    printf("Ultra Sonice Echo One:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo One:\tNot Initialized.\n");
  }
  if(pins->echo_two != (-1)){
    gpio_init(pins->echo_two);
    gpio_set_dir(pins->echo_two, GPIO_IN);
    printf("Ultra Sonice Echo Two:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo Two:\tNot Initialized.\n");
  }
}

void shift_register_pin_init(register_pins *config){

  if(config->register_one_data != (int)NULL){
    gpio_init(config->register_one_data);
    gpio_set_dir(config->register_one_data, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_one_data);
  }
  if(config->register_one_latch != (int)NULL){
    gpio_init(config->register_one_latch);
    gpio_set_dir(config->register_one_latch, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_one_latch);
  }
  if(config->register_one_enable != (int)NULL){
    gpio_init(config->register_one_enable);
    gpio_set_dir(config->register_one_enable, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_one_enable);
  }
    if(config->register_two_data != (int)NULL){
    gpio_init(config->register_two_data);
    gpio_set_dir(config->register_two_data, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_two_data);
  }
  if(config->register_two_latch != (int)NULL){
    gpio_init(config->register_two_latch);
    gpio_set_dir(config->register_two_latch, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_two_latch);
  }
  if(config->register_two_enable != (int)NULL){
    gpio_init(config->register_two_enable);
    gpio_set_dir(config->register_two_enable, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_two_enable);
  }
  if(config->register_clk_line != (int)NULL){
    gpio_init(config->register_clk_line);
    gpio_set_dir(config->register_clk_line, GPIO_OUT);
    printf("GPIO PIN %d Initialized.  Direction: OUTPUT.\n", config->register_clk_line);
  }
}

//  Function to create pulse for shift latching.
//  Takes a uint GPIO pin to allow multiple shift registers.
void latch_data(register_pins *config) {
  if(config->register_one_latch != (int)NULL){
    gpio_put(config->register_one_latch, 1); // Generate latch pulse for register one.
    gpio_put(config->register_one_latch, 0);
  }
  if(config->register_two_latch != (int)NULL){
    gpio_put(config->register_two_latch, 1); // Generate latch pulse for register two.
    gpio_put(config->register_two_latch, 0);
  }
}

//  Function to create pulse for clock pulses.
//  Takes a uint GPIO pin to allow multiple shift registers.
void clk_pulse(register_pins *config) {

  if(config->register_clk_line != (int)NULL){
    gpio_put(config->register_clk_line, 1); // Generate latch pulse for register's.
    gpio_put(config->register_clk_line, 0);
  }
}

/*
    ::serial_register_ouput function::
    Params:
    uint gpio_display - Reference to gpio pin tied to register display enable.
    uint gpio_pin -     Reference to serial data output pin.  Currently labeled serialData.  Additional can be added by muxing clock, notOE, and reg_clk.
    bool data_in[] -    With use for RGB led register and sevsegdis in mind.

*/

void serial_register_output(register_pins *config, uint8_variables *vars) {

    printf("Serial Register Function Initialized.\n\n");
    //  Set display enable high for duration of data transfer.

    if(config->register_one_data != (int)NULL){

      printf("Register One Data Outputting.\n");

    gpio_put(config->register_one_enable, 1);

    if(vars->register_value_zero != (int)NULL){
      printf("Value of shift register buffer: 0b%08x.\n", vars->register_value_zero);
    }
    
    //  Create loop to shift data into selected register output.
    for(int a = 0; a < 8; a++){
    //  Put data_in line high or low, pulse register clock.
    ((vars->register_value_zero >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);     //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
    printf("Shifting: 0b%01x.\n", (vars->register_value_zero >> a));
    clk_pulse(config);                               //  Pulse clock per iteration.              
    }

    latch_data(config);                            //  After loop iterates, call latch function to pulse register latch.
    clk_pulse(config);                               //  Pulse clock finish input loop.

    gpio_put(config->register_one_enable, 0);                        //  Set display enable low to enable register output pins.
        sleep_ms(100);

    }

    if(config->register_two_data != (int)NULL){

      printf("Register One Data Outputting.\n");

    gpio_put(config->register_two_enable, 1);

    if(vars->register_value_one != (int)NULL){
      printf("Value of shift register buffer: 0b%08x.\n", vars->register_value_one);
    }
    
    //  Create loop to shift data into selected register output.
    for(int a = 0; a < 8; a++){
    //  Put data_in line high or low, pulse register clock.
    ((vars->register_value_one >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);     //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
    printf("Shifting: 0b%01x.\n", (vars->register_value_one >> a));
    clk_pulse(config);                               //  Pulse clock per iteration.              
    }

    latch_data(config);                            //  After loop iterates, call latch function to pulse register latch.
    clk_pulse(config);                               //  Pulse clock finish input loop.

    gpio_put(config->register_one_enable, 0);                        //  Set display enable low to enable register output pins.
        sleep_ms(100);

    }

}

void ultra_sonic_distance(ultra_sonic_data_t *data){

    printf("\n\tUltrasonic Sensor Function.\n");

    if((data->trigger_one != (-1)) && (data->echo_one != (-1))){

        gpio_put(data->trigger_one, 1);
        sleep_us(10);
        gpio_put(data->trigger_one, 0);

        // Wait for the echo pulse
        while (gpio_get(data->echo_one) == 0) {}
        data->usonic_one_start = time_us_32();

        while (gpio_get(data->echo_one) == 1) {}
        data->usonic_one_end = time_us_32();

        // Calculate the distance
        data->pulse_duration_one = data->usonic_one_end - data->usonic_one_start;
        data->distance_one = (data->pulse_duration_one / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", data->distance_one);
        data->returned_distance_one = data->distance_one;
    }
    if((data->trigger_two != (-1)) && (data->echo_two != (-1))){

        gpio_put(data->trigger_two, 1);
        sleep_us(10);
        gpio_put(data->trigger_two, 0);

        // Wait for the echo pulse
        while (gpio_get(data->echo_two) == 0) {}
        data->usonic_one_start = time_us_32();

        while (gpio_get(data->echo_two) == 1) {}
        data->usonic_one_end = time_us_32();

        // Calculate the distance
        data->pulse_duration_two = data->usonic_two_end - data->usonic_two_start;
        data->distance_two = (data->pulse_duration_two / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", data->distance_two);
        data->returned_distance_two = data->distance_two;
    }
}

void stepper_init(s_motor_t *pins){

  printf("Initializing Stepper Motor Pins.\n");

  if(pins->step_a != -1){
    printf("Step A: Initializing.\n");
    gpio_init(pins->step_a);
    gpio_set_dir(pins->step_a, GPIO_OUT);
    printf("Step A: Initialized.\n");
  }else {
    printf("Step A: Not Initialized.\n");
  }
  if(pins->step_b != -1){
    printf("Step B: Initializing.\n");
    gpio_init(pins->step_b);
    gpio_set_dir(pins->step_b, GPIO_OUT);
    printf("Step B: Initialized.\n");
  }else {
    printf("Step B: Not Initialized.\n");
  }
  if(pins->step_c != -1){
    printf("Step C: Initializing.\n");
    gpio_init(pins->step_c);
    gpio_set_dir(pins->step_c, GPIO_OUT);
    printf("Step C: Initialized.\n");
  }else {
    printf("Step C: Not Initialized.\n");
  }
  if(pins->step_d != -1){
    printf("Step D: Initializing.\n");
    gpio_init(pins->step_d);
    gpio_set_dir(pins->step_d, GPIO_OUT);
    printf("Step D: Initialized.\n");
  }else {
    printf("Step D: Not Initialized.\n");
  }
}

void stepped(s_motor_t *steps){

  printf("Stepping Function.\n");

  steps->current_step = steps->step_value - steps->previous_step;
  printf("Current Steps: %i.\nStep Value: %i.\nPrevious Step: %i.\n", steps->current_step, steps->step_value, steps->previous_step);

  printf("Moving %i Steps.\n", steps->current_step);
  printf("Current revolutions: %i.\n", steps->current_absolute_revolution);

  if(steps->current_step >= steps->previous_step){

    if(steps->current_CW_revolutions <= steps->max_CW_revolutions){

    for(int i = 0; i < (steps->current_step / 4); i++){

      gpio_put(steps->step_a, 1);
        sleep_ms(5);
      gpio_put(steps->step_a, 0);

      gpio_put(steps->step_b, 1);
        sleep_ms(5);
      gpio_put(steps->step_b, 0);

      gpio_put(steps->step_c, 1);
        sleep_ms(5);
      gpio_put(steps->step_c, 0);
      
      gpio_put(steps->step_d, 1);
        sleep_ms(5);
      gpio_put(steps->step_d, 0);

      if(i % 4 == 0){
        steps->CW_revolution_steps++;
        printf("Current CW_Full_Steps: %i.\n", steps->CW_revolution_steps);
      }
    }
      
      if(steps->CW_revolution_steps % 32 == 0){
        steps->current_CW_revolutions++;
      }
    }

  }else if(steps->previous_step >= steps->current_step){

    if(steps->current_CCW_revolutions <= steps->max_CCW_revolutions){
    for(int j = (steps->current_step / 4); j >= 0; j--){

      gpio_put(steps->step_d, 1);
        sleep_ms(5);
      gpio_put(steps->step_d, 0);

      gpio_put(steps->step_c, 1);
        sleep_ms(5);
      gpio_put(steps->step_c, 0);

      gpio_put(steps->step_b, 1);
        sleep_ms(5);
      gpio_put(steps->step_b, 0);

      gpio_put(steps->step_a, 1);
        sleep_ms(5);
      gpio_put(steps->step_a, 0);

      if(j % 4 == 0){
        steps->CCW_revolution_steps--;
      }

    }

      if(steps->CCW_revolution_steps % 32 == 0){
        steps->current_CCW_revolutions--;
      }
    }
  }else if(steps->current_step == 0){
    printf("No steps moved.\n");
  }

    steps->previous_step = steps->step_value;
    steps->current_absolute_revolution = steps->current_CW_revolutions - steps->current_CCW_revolutions;
    
}

void set_servo_position(uint pin_selection, adc_port_values *adc){

  printf("\n\nSetting Servo Position.\n");

    printf("Horizontal Input Value: %d.\n", adc->adc2_mapped_value);
    float corrected_value = (adc->adc2_mapped_value) * 9.45f;
    printf("Horizontal Corrected Value: %f.\n", corrected_value);
    servo_configuration.servo_one_position = corrected_value;
    

   if(pin_selection == servo_configuration.servo_one_pin){

    printf("Servo Moved to Position: %f.\n", (servo_configuration.servo_one_position/20000.f)*39062.f);
    
      pwm_set_gpio_level(pin_selection, (servo_configuration.servo_one_position/20000.f)*39062.f);
  }

  if(pin_selection == servo_configuration.servo_two_pin){    
      pwm_set_gpio_level(pin_selection, (servo_configuration.servo_two_position/20000.f)*39062.f);  
  }

  if(pin_selection == servo_configuration.servo_three_pin){
      pwm_set_gpio_level(pin_selection, (servo_configuration.servo_three_position/20000.f)*39062.f);
  }

  if(pin_selection == servo_configuration.servo_four_pin){    
      pwm_set_gpio_level(pin_selection, (servo_configuration.servo_four_position/20000.f)*39062.f);  
  }
}

void set_servo_initial_position(servo_motor_config_t *config){

  printf("\n\nInitializing Servo Position.\n");

  uint16_t initial_smp;
  printf("Initial Servo Position: %f.\n", config->servo_initial_position);
  initial_smp = ((config->servo_initial_position/20000.f)*39062.f);

  if(config->servo_inital_positioning == true){

    if(config->servo_one_pin != UNDEFINED){
      pwm_set_gpio_level(config->servo_one_pin, initial_smp);
      printf("Servo Motor One Initialized to: %lu.\n", initial_smp);
    }
  
  if(config->servo_two_pin != UNDEFINED){
      pwm_set_gpio_level(config->servo_two_pin, initial_smp);
      printf("Servo Motor Two Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_three_pin != UNDEFINED){
      pwm_set_gpio_level(config->servo_three_pin, initial_smp);
      printf("Servo Motor Three Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_four_pin != UNDEFINED){
      pwm_set_gpio_level(config->servo_four_pin, initial_smp);
      printf("Servo Motor Four Initialized to: %li.\n", initial_smp);
    }
  }
  config->servo_inital_positioning = false;
}

void servo_initialization(servo_motor_config_t *configuration){

  printf("\n\nServo Initialization.\n");

  pwm_config config = pwm_get_default_config();

  printf("\n\nServo Wrap Valid Check: %u.\n", configuration->so_wrap);

  if(configuration->servo_one_pin != UNDEFINED){

    printf("Servo One:  Initializing.\n");
    printf("Servo One Pin: %i.\n", configuration->servo_one_pin);

    // gpio_init(configuration->servo_one_pin);
    gpio_set_function(configuration->servo_one_pin, GPIO_FUNC_PWM);

    configuration->servo_one_slice = pwm_gpio_to_slice_num(configuration->servo_one_pin);
    pwm_config_set_clkdiv(&config, configuration->so_clkdiv);
    pwm_config_set_wrap(&config, configuration->so_wrap);
    pwm_init(configuration->servo_one_slice, &config, true);

    printf("\tServo One Slice: %d.\n\t\tServo One Clkdiv: %f.\n\t\t\tServo One Wrap: %u.\n", configuration->servo_one_slice, configuration->so_clkdiv, configuration->so_wrap);
    printf("\nSERVO ONE INITIALIZED.\n\n");

  }else {
    printf("Servo Motor One:\tNot Initialized.\n");
  }
  if(configuration->servo_two_pin != UNDEFINED){

    printf("\nServo Two:  Initializing.\n");

   // gpio_init(configuration->servo_two_pin);
    gpio_set_function(configuration->servo_two_pin, GPIO_FUNC_PWM);
    configuration->servo_two_slice = pwm_gpio_to_slice_num(configuration->servo_two_pin);
    pwm_config_set_clkdiv(&config, configuration->st_clkdiv);
    pwm_config_set_wrap(&config, configuration->st_wrap);
    pwm_init(configuration->servo_two_slice, &config, true);
  }else {
    printf("Servo Motor Two:\tNot Initialized.\n");
  }
  if(configuration->servo_three_pin != UNDEFINED){

    gpio_set_function(configuration->servo_three_pin, GPIO_FUNC_PWM);
    configuration->servo_three_slice = pwm_gpio_to_slice_num(configuration->servo_three_pin);
    pwm_config_set_clkdiv(&config, configuration->stt_clkdiv);
    pwm_config_set_wrap(&config, configuration->stt_wrap);
    pwm_init(configuration->servo_three_slice, &config, true);
  }else {
    printf("Servo Motor Three:\tNot Initialized.\n");
  }
  if(configuration->servo_four_pin != UNDEFINED){

    gpio_set_function(configuration->servo_four_pin, GPIO_FUNC_PWM);
    configuration->servo_four_slice = pwm_gpio_to_slice_num(configuration->servo_four_pin);
    pwm_config_set_clkdiv(&config, configuration->sf_clkdiv);
    pwm_config_set_wrap(&config, configuration->sf_wrap);
    pwm_init(configuration->servo_four_slice, &config, true);
  }else {
    printf("Servo Motor Four:\tNot Initialized.\n");
  }
  printf("End Servo Initialization.\n");
  
  set_servo_initial_position(configuration);
}
/*
//  Use for more detailed control of the servo motor.
uint32_t pwm_set_freq_duty(servo_motor_config_t *configed){
        uint32_t divider16 = configed->mcu_clock / configed->so_frequency / 4096 + (configed->mcu_clock % (configed-> * 4096) != 0);
        if (divider16 / 16 == 0) {
            divider16 = 16;
            wrap = clock * 16 / divider16 / f - 1;
            pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
            pwm_set_wrap(slice_num, wrap);
            pwm_set_chan_level(slice_num, chan, wrap * d / 100);
        return wrap;
        }
}
*/



#endif