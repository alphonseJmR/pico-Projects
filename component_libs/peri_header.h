#ifndef PERI_FUNCS
#define PERI_FUNCS

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "lcd_16x2.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define stage_inat 0x00
#define stage_intl 0x02
#define stage_indp 0x04
#define stage_comm 0x08
#define stage_char 0x10

#define comm_start 0x40
#define comm_end 0x70

#define e_toggle 0x20
#define com_sel = 040
#define backlight_toggle 0x80

#define com_com_e 0xA0
#define com_com_f 0x80
#define com_char_e 0xE0
#define com_char_f 0xC0

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
 const uint register_one_data;
 const uint register_one_latch;
 const uint register_one_enable;
 const uint register_two_data;
 const uint register_two_latch;
 const uint register_two_enable;
 const uint register_clk_line;
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
  char battery_level[16];
  char battery_printer[16];
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

  uint servo_one_pin;       //  .servo_one_pin = servo_one
  uint servo_two_pin;       //  .servo_two_pin = servo_two
  uint servo_three_pin;     //  .servo_three_pin = servo_three
  uint servo_four_pin;      //  .servo_four_pin = servo_four

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
void print_binary(uint byte_input, uint8_t two_byte_input) {
  
    printf("\n");
    if(byte_input > 0){
      printf("1 Byte:\n");
      for (int i = 3; i >= 0; i--) {
        if ((byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }

    if(two_byte_input > 0){
      printf("\n2 Bytes:\n");
      for (int i = 7; i >= 0; i--) {
        if ((two_byte_input >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
      }
    }

    printf("\n");
    }

//  Initial idea -> Based on reg select, the register output can be selected.  Then conditionals subject adc battery data to translation for register ready output.
void battery_status_to_shift(uint8_variables *vars, adc_port_values *adc, battery_data_t *level, uint8_t register_select){

  switch(register_select){

    case 0:
    vars->register_value_zero = 0x00;
      if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_zero += level->battery_green;
    vars->register_value_zero += level->battery_blue;
    vars->register_value_zero += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_zero += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_zero += level->battery_blue;

      }else {
    
    vars->register_value_zero += level->battery_red;

  }
      break;

    case 1:
    vars->register_value_one = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_one += level->battery_green;
    vars->register_value_one += level->battery_blue;
    vars->register_value_one += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_one += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_one += level->battery_blue;

      }else {
    
    vars->register_value_one += level->battery_red;

  }
      break;

    case 2:
    vars->register_value_two = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_two += level->battery_green;
    vars->register_value_two += level->battery_blue;
    vars->register_value_two += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_two += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_two += level->battery_blue;

      }else {
    
    vars->register_value_two += level->battery_red;

  }
      break;

    case 3:
    vars->register_value_three = 0x00;
      if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_three += level->battery_green;
    vars->register_value_three += level->battery_blue;
    vars->register_value_three += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_three += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_three += level->battery_blue;

      }else {
    
    vars->register_value_three += level->battery_red;

  }
      break;

    case 4:
    vars->register_value_four = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_four += level->battery_green;
    vars->register_value_four += level->battery_blue;
    vars->register_value_four += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_four += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_four += level->battery_blue;

      }else {
    
    vars->register_value_four += level->battery_red;

  }
      break;

    case 5:
    vars->register_value_five = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_five += level->battery_green;
    vars->register_value_five += level->battery_blue;
    vars->register_value_five += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_five += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_five += level->battery_blue;

      }else {
    
    vars->register_value_five += level->battery_red;

  }
      break;

    case 6:
    vars->register_value_six = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_six += level->battery_green;
    vars->register_value_six += level->battery_blue;
    vars->register_value_six += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_six += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_six += level->battery_blue;

      }else {
    
    vars->register_value_six += level->battery_red;

  }
      break;

    case 7:
    vars->register_value_seven = 0x00;
       if(adc->adc0_mapped_value > 0xC8 && adc->adc0_mapped_value < level->battery_high){
    
    vars->register_value_seven += level->battery_green;
    vars->register_value_seven += level->battery_blue;
    vars->register_value_seven += level->battery_red;

      }else if(adc->adc0_mapped_value > 0x84 && adc->adc0_mapped_value < level->battery_medium){

    vars->register_value_seven += level->battery_green;

      }else if(adc->adc0_mapped_value > 0x32 && adc->adc0_mapped_value < level->battery_low){

    vars->register_value_seven += level->battery_blue;

      }else {
    
    vars->register_value_seven += level->battery_red;

  }
      break;

    default:
    printf("Invalid Register Selection.\n");

  }
    sprintf(bat_status.battery_printer, "Level: %u", adc->adc0_mapped_value);
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

      printf("\nInterruption occured:\n\tLast Interrupt: %u.\n", callback_times.last_interruption);
    callback_times.initial_interruption = callback_times.current_interruption;
    gpio_acknowledge_irq(gpio, events);
  
  if(callback_times.rotary_last > rotary.minimum_required_interrupt){
    callback_times.rotary_initial = callback_times.rei_current_interrupt;
  
    if(gpio == enabled_buttons.rotary_button_clk && (events & 0x04)){
      rotary.current_rotary_clk = gpio_get(enabled_buttons.rotary_button_clk);
      rotary.current_rotary_dt = gpio_get(enabled_buttons.rotary_button_dt);
      
      if(rotary.current_rotary_clk != rotary.previous_rotary_clk){
        //  printf("Clk Status Changed.\n");
        //  printf("CLK Current Status: %i.\n", rotary.current_rotary_clk);
        //  printf("DT Current Status: %i.\n", rotary.current_rotary_dt);
        if(rotary.current_rotary_dt != rotary.current_rotary_clk){
        //  printf("B != A.\n");
            if(rotary.rotary_total <= rotary.max_rotation_value){
          rotary.rotary_total++;
            }else {
              printf("Maximum Rotation Value.\n");
            }
         
        }else {
          //  printf("B == A.\n");
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
  if(config->adc0_pin != UNDEFINED) {
    adc_select_input(0);
    config->adc0_raw_read = adc_read();
    printf("\n\n\tRaw value of ADC0: %d.\n", config->adc0_raw_read);
    }
  if(config->adc1_pin != UNDEFINED) {
    adc_select_input(1);
    config->adc1_raw_read = adc_read();
    printf("\t\tRaw value of ADC1: %d.\n", config->adc1_raw_read);
    }
  if(config->adc2_pin != UNDEFINED) {
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

  if(config->adc0_pin != UNDEFINED){
  adc_gpio_init(config->adc0_pin);
  printf("ADC0 Pin Initialized.\n");
  printf("ADC0 PIN: %d.\n\n", config->adc0_pin);
  }else {
    printf("ADC0 Not Initialized.\n");
  }

  if(config->adc1_pin != UNDEFINED){
  adc_gpio_init(config->adc1_pin);
  printf("ADC1 Pin Initialized.\n");
  printf("ADC1 PIN: %d.\n\n", config->adc1_pin);
  }else {
    printf("ADC1 Not Initialized.\n");
  }

  if(config->adc2_pin != UNDEFINED){
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

  printf("\nInterrupt Button Initializing.\n\n");

        if(types->rotary_button_button != -1){
          gpio_init(types->rotary_button_button);
      gpio_set_dir(types->rotary_button_button, GPIO_IN);
     // gpio_pull_up(types->rotary_button_button);
        gpio_set_irq_enabled_with_callback(types->rotary_button_button, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary\n\tPin: %d: Initialized.\n\n", types->rotary_button_button);
    } else {
                printf("Rotary Button Not Used.\n\n");
    }

        if(types->analog_button_one != -1){
      gpio_init(types->analog_button_one);
      gpio_set_dir(types->analog_button_one, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button One\n\tPin %d: Initialized.\n\n", (types->analog_button_one));
    } else {
        printf("Analog Button One Not Used.\n\n");
    }

    if(types->analog_button_two != -1){
      gpio_init(types->analog_button_two);
      gpio_set_dir(types->analog_button_two, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button Two\n\tPin %d: Initialized.\n\n", types->analog_button_two);
    } else {
        printf("Analog Button Two Not Used.\n\n");
    }

    if(types->button_one != -1){
      gpio_init(types->button_one);
      gpio_set_dir(types->button_one, GPIO_IN);
      gpio_pull_up(types->button_one);
      gpio_set_irq_enabled_with_callback(types->button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_one);
    } else {
                printf("Button One Not Used.\n\n");
    }
    if(types->button_two != -1){
      gpio_init(types->button_two);
      gpio_set_dir(types->button_two, GPIO_IN);
      gpio_pull_down(types->analog_button_two);
      gpio_set_irq_enabled_with_callback(types->button_two, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_two);
    } else {
                printf("Button Two Not Used.\n\n");
    }

    if(types->button_three != -1){
      gpio_init(types->button_three);
      gpio_set_dir(types->button_three, GPIO_IN);
      gpio_pull_down(types->button_three);
      gpio_set_irq_enabled_with_callback(types->button_three, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_three);
    } else {
                printf("Button Three Not Used.\n\n");
    }
}

void rotary_encoder_init(button_types *types){
  
      if(types->rotary_button_clk != -1){
      gpio_init(types->rotary_button_clk);
      gpio_set_dir(types->rotary_button_clk, GPIO_IN);
      gpio_pull_up(types->rotary_button_clk);
      gpio_set_irq_enabled_with_callback(types->rotary_button_clk, 0x04, true, &enabled_button_callback);
        printf("Rotary Encoder Clk: Pin %d.\n\tInitialized.\n\n", types->rotary_button_clk);
    } else {
                printf("Rotary Clk Not Used.\n\n");
    }

        if(types->rotary_button_dt != -1){
          gpio_init(types->rotary_button_dt);
      gpio_set_dir(types->rotary_button_dt, GPIO_IN);
      gpio_pull_up(types->rotary_button_dt);
      gpio_set_irq_enabled_with_callback(types->rotary_button_dt, 0x04, true, &enabled_button_callback);
        printf("Rotary Encoder DT: Pin %d.\n\tInitialized.\n\n", types->rotary_button_dt);
    } else {
                printf("Rotary DT Not Used.\n\n");
    }

}

void battery_light_init(battery_data_t *data){
  
  if(data->battery_red != -1){
    printf("Red Indicator Initialilzing.");
    gpio_init(data->battery_red);
    gpio_set_dir(data->battery_red, GPIO_OUT);
    
  }
  if(data->battery_blue != -1){
    printf("Blue Indicator Initialilzing.");
    gpio_init(data->battery_blue);
    gpio_set_dir(data->battery_blue, GPIO_OUT);
  }
  if(data->battery_green != -1){
    printf("Green Indicator Initialilzing.");
    gpio_init(data->battery_green);
    gpio_set_dir(data->battery_green, GPIO_OUT);
  }

}

void ultra_sonic_sensor_pin_setup(ultra_sonic_data_t *pins) {
  if(pins->trigger_one != -1){
    gpio_init(pins->trigger_one);
    gpio_set_dir(pins->trigger_one, GPIO_OUT);
    printf("Ultra Sonic Trigger One:\tInitialized.\n");
  }else {
    printf("Ultra Sonic Trigger One:\tNot Initialized.\n");
  }
  if(pins->trigger_two != -1){
    gpio_init(pins->trigger_two);
    gpio_set_dir(pins->trigger_two, GPIO_OUT);
    printf("Ultra Sonic Trigger Two:\tInitialized.\n");
  }else {
    printf("Ultra Sonic Trigger Two:\tNot Initialized.\n");
  }
  if(pins->echo_one != -1){
    gpio_init(pins->echo_one);
    gpio_set_dir(pins->echo_one, GPIO_IN);
    printf("Ultra Sonice Echo One:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo One:\tNot Initialized.\n");
  }
  if(pins->echo_two != -1){
    gpio_init(pins->echo_two);
    gpio_set_dir(pins->echo_two, GPIO_IN);
    printf("Ultra Sonice Echo Two:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo Two:\tNot Initialized.\n");
  }
}

void shift_register_pin_init(register_pins *config){

  printf("\n\t\tCD74HC595E Pin Initialization.\n\n");

  printf("Initializing Register One.\n");
  if(config->register_one_data != UNDEFINED){
    printf("Register One Data Initializing.\n\n");
    gpio_init(config->register_one_data);
    gpio_set_dir(config->register_one_data, GPIO_OUT);
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_data);
  }
  if(config->register_one_latch != UNDEFINED){
    gpio_init(config->register_one_latch);
    gpio_set_dir(config->register_one_latch, GPIO_OUT);
    printf("Register One Latch Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_latch);
  }
  if(config->register_one_enable != UNDEFINED){
    gpio_init(config->register_one_enable);
    gpio_set_dir(config->register_one_enable, GPIO_OUT);
    printf("Register One Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_enable);
  }
  printf("Initializing Register Two.\n");
  if(config->register_two_data != UNDEFINED){
    gpio_init(config->register_two_data);
    gpio_set_dir(config->register_two_data, GPIO_OUT);
    printf("Register Two Data Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_data);
  }
  if(config->register_two_latch != UNDEFINED){
    gpio_init(config->register_two_latch);
    gpio_set_dir(config->register_two_latch, GPIO_OUT);
    printf("Register Two Latch InitInitializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_latch);
  }
  if(config->register_two_enable != UNDEFINED){
    gpio_init(config->register_two_enable);
    gpio_set_dir(config->register_two_enable, GPIO_OUT);
    printf("Register Two Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_enable);
  }
  if(config->register_clk_line != UNDEFINED){
    gpio_init(config->register_clk_line);
    gpio_set_dir(config->register_clk_line, GPIO_OUT);
    printf("Register Clk Line Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n\n", config->register_clk_line);
  }
  printf("Register pins initialized.\n");
}

//  Function to create pulse for shift latching.
//  Takes a uint GPIO pin to allow multiple shift registers.
void latch_data(register_pins *config) {
  if(config->register_one_latch != UNDEFINED){
    gpio_put(config->register_one_latch, 1); // Generate latch pulse for register one.
      sleep_us(200);
    gpio_put(config->register_one_latch, 0);
  }
  if(config->register_two_latch != UNDEFINED){
    gpio_put(config->register_two_latch, 1); // Generate latch pulse for register two.
      sleep_us(200);
    gpio_put(config->register_two_latch, 0);
  }
}

//  Function to create pulse for clock pulses.
//  Takes a uint GPIO pin to allow multiple shift registers.
void clk_pulse(register_pins *config) {

  if(config->register_clk_line != UNDEFINED){
    gpio_put(config->register_clk_line, 1); // Generate latch pulse for register's.
      sleep_us(200);
    gpio_put(config->register_clk_line, 0);
  }
}

//  Function takes register_pins struct, uint8_variable data to output, int between 0-7 for both selections.  Use -1 on either for non-use.
void serial_register_output(register_pins *config, uint8_variables *vars, int register_selection, int reg_two_sel) {

  printf("Serial Register Function Initialized.\n\n");
    //  Set display enable high for duration of data transfer.
    if(config->register_one_data != UNDEFINED){
  printf("Register One Data Line Outputting.\n");
  printf("Currently outputting register %i.\n", register_selection);
    gpio_put(config->register_one_enable, 1);
      switch(register_selection){

        case -1:
          printf("Register One Not Selected.\n");
          break;

        case 0:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_zero >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_zero >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 1:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_one >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_one >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 2:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_two >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_two >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 3:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_three >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_three >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 4:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_four >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_four >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 5:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_five >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_five >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 6:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_six >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_six >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 7:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_seven >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_seven >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        default:
          printf("Shift Register One Not Selected / Error Occured.\n");

    }
    }

    if(config->register_two_data != UNDEFINED){
  printf("Register One Data Outputting.\n");
    gpio_put(config->register_two_enable, 1);

            switch(reg_two_sel){

              case -1:
              printf("Register Two Not Used.\n");
              break;

        case 0:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_zero >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_zero >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 1:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_one >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_one >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 2:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_two >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_two >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 3:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_three >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_three >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 4:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_four >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_four >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 5:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_five >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_five >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 6:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_six >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_six >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 7:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_seven >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_seven >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        default:
          printf("Shift Register Two Not Selected / Error Occured.\n");

    }
    
    }

}

uint8_t uint_flip(uint8_t input_byte){
  uint8_t flipped_val;
  flipped_val = 0x00;

  flipped_val += (0x01 & (input_byte >> 7)) ? (0x01 << 4) : (0x00 << 4);
  flipped_val += (0x01 & (input_byte >> 6)) ? (0x01 << 5) : (0x00 << 5);
  flipped_val += (0x01 & (input_byte >> 5)) ? (0x01 << 6) : (0x00 << 6);
  flipped_val += (0x01 & (input_byte >> 4)) ? (0x01 << 7) : (0x00 << 7);

  flipped_val += (0x01 & (input_byte >> 3)) ? (0x01 << 0) : (0x00 << 0);
  flipped_val += (0x01 & (input_byte >> 2)) ? (0x01 << 1) : (0x00 << 1);
  flipped_val += (0x01 & (input_byte >> 1)) ? (0x01 << 2) : (0x00 << 2);
  flipped_val += (0x01 & (input_byte >> 0)) ? (0x01 << 3) : (0x00 << 3);

  return flipped_val;
}

//  Pre-set to register so as to pull high/low the required pins for the lcd to accept data.
//  Command is user-configed.  RS, E, Backlight - 0x40, 0x20, 0x10.
void lcd_register_prep(uint8_t command, register_pins *pins){

    uint8_t command_transform;

    command_transform = ((command & 0xF0));
    for(int j = 0; j < 8; j++){
                ((command_transform >> j) & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_us(30);
                clk_pulse(pins);
            }
            latch_data(pins);
            sleep_us(30);
            clk_pulse(pins);
            sleep_us(200);
}

void byte_shifting(uint8_t command, uint8_t byte_input, register_pins *pins){

  printf("Bit Shifter.\n");

    uint8_t upper_bits;
    uint8_t lower_bits;
    uint8_t command_par;

    command_par = (command & 0xFF);
    printf("Input: %02x.\n", command_par);
    bool com_t;

    (command_par == 0xE0) ? printf("Character Command.\n") : printf("Com Command.\n");
    com_t = (command_par == 0xA0) ? true : false;

    //  Call lcd_display_prep first, as this sets the lcd ready for the data input, whether command or char.
    //  A loop of two, to first shift the upper four bits of the data, then the bottom four bits.
 
    for(int i = 0; i < 2; i++){
      
      gpio_put(pins->register_one_enable, 1);

        lcd_register_prep(com_com_e, pins);

      //  printf("Pre Command.\n");
        //  We don't want to send both upper and lower bits twice, so we'll use the iterations to send the sections.
        if(i == 0){
          printf("First Shift / nibble.\n");
            //  set the variable upper_bits to the output expression.
            upper_bits = ((command & 0xF0) | ((uint_flip(byte_input) >> 4) & 0x0F));
            printf("Shifting: %02x.\n", upper_bits);
            for(int j = 0; j < 8; j++){
                    //  per iteration, check if upper_bits is a 1 or 0, and put the gpio_pin in the appropriate position.
                (upper_bits >> j & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_us(100);
                clk_pulse(pins);
            }
            latch_data(pins);
            clk_pulse(pins);
            (com_t) ? printf("Send COMMAND finish.\n") : printf("Send CHARACTER finish.\n");
            (com_t) ? lcd_register_prep(com_com_f, pins) : lcd_register_prep(com_char_f, pins);
            
            printf("\n\n");
        }
        
        if(i == 1){
          printf("Second Shift / nibble.\n");
            
            lower_bits = ((command & 0xF0) | (uint_flip(byte_input) & 0x0F));
            printf("Shifting: %02x.\n", lower_bits);
            for(int j = 0; j < 8; j++){
                (lower_bits >> j & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_us(100);
                clk_pulse(pins);
            }  
            latch_data(pins);
            clk_pulse(pins);
        (com_t) ? printf("Send COMMAND finish.\n") : printf("Send CHARACTER finish.\n");
        (com_t) ? lcd_register_prep(com_com_f, pins) : lcd_register_prep(com_char_f, pins);
        
          printf("\n\n");
      }

      gpio_put(pins->register_one_enable, 0);
    }
}

void lcd_initialize(register_pins *pins){

  printf("Register -> LCD Initialization.\n\n");

  byte_shifting(com_com_e, 0x02, pins);
    sleep_ms(20);
    printf("Shifted command: 0x02.\n");

  byte_shifting(com_com_e, 0x03, pins);
    sleep_ms(10);
    printf("Shifted command: 0x03.\n");

  byte_shifting(com_com_e, 0x03, pins);
    sleep_ms(5);
    printf("Shifted command: 0x03.\n");

  byte_shifting(com_com_e, 0x03, pins);
    sleep_ms(10);
    printf("Shifted command: 0x03.\n");

  byte_shifting(com_com_e, 0x28, pins);
    sleep_ms(20);
    printf("Shifted command: 0x28.\n");

  byte_shifting(com_com_e, 0x08, pins);
    sleep_ms(5);
    printf("Shifted command: 0x08.\n");

  byte_shifting(com_com_e, 0x01, pins);
    sleep_ms(5);
    printf("Shifted command: 0x01.\n");

  byte_shifting(com_com_e, 0x06, pins);
    sleep_ms(10);
    printf("Shifted command: 0x06.\n\n");

  printf("Register -> LCD Initialized.\n");
}

void lcd_single_line_out(register_pins *pins, uint8_t comm, const char *display){

  //  printf("\n\nlcd display array size: %u.\n\n", sizeof(display));
  printf("Single line lcd out func ->\n");
    for(int i = 0; i < sizeof(display); i++){
      
        //  printf("Outputting: %u.\n", display[i]);
          printf("\n");
            byte_shifting(comm, display[i], pins);
              sleep_us(500);

    }
}

void lcd_screen_write(register_pins *pins, uint8_t com, const char *line_one, const char *line_two){

  printf(" screen write func\n");
  
  //  Clear LCD display.
  byte_shifting(com_com_e, 0x01, pins);
  sleep_ms(2);
  //  LCD return home.
  byte_shifting(com_com_e, 0x02, pins);
 sleep_us(100);
  //  Print first line to LCD display.
  lcd_single_line_out(pins, com_char_e, line_one);
   sleep_us(100);

  //  Bring LCD display to second line.
  byte_shifting(com_com_e, 0x80, pins);
   sleep_us(100);
  byte_shifting(com_com_e, 0x40, pins);
  sleep_us(100);

  //  Print second line to LCD display.
  lcd_single_line_out(pins, com_char_e, line_two);
    sleep_us(100);
}


void ultra_sonic_distance(ultra_sonic_data_t *data){

    printf("\n\tUltrasonic Sensor Function.\n");

    if((data->trigger_one != -1) && (data->echo_one != -1)){

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
    if((data->trigger_two != -1) && (data->echo_two != -1)){

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

void set_servo_position(uint pin_selection){
  

   if(pin_selection == servo_configuration.servo_one_pin){
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

  uint32_t initial_smp;
  initial_smp = ((config->servo_initial_position/20000.f)*39062.f);

  if(config->servo_inital_positioning == true){

    if(config->servo_one_pin != -1){
      pwm_set_gpio_level(config->servo_one_pin, initial_smp);
      printf("Servo Motor One Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_two_pin != -1){
      pwm_set_gpio_level(config->servo_two_pin, initial_smp);
      printf("Servo Motor Two Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_three_pin != -1){
      pwm_set_gpio_level(config->servo_three_pin, initial_smp);
      printf("Servo Motor Three Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_four_pin != -1){
      pwm_set_gpio_level(config->servo_four_pin, initial_smp);
      printf("Servo Motor Four Initialized to: %li.\n", initial_smp);
    }
  }
  
}

void servo_initialization(servo_motor_config_t *configuration){

  pwm_config config = pwm_get_default_config();

  if(configuration->servo_one_pin != -1){

    gpio_set_function(configuration->servo_one_pin, GPIO_FUNC_PWM);
    configuration->servo_one_slice = pwm_gpio_to_slice_num(configuration->servo_one_pin);
    pwm_config_set_clkdiv(&config, configuration->so_clkdiv);
    pwm_config_set_wrap(&config, configuration->so_wrap);
    pwm_init(configuration->servo_one_pin, &config, true);
  }else {
    printf("Servo Motor One:\tNot Initialized.\n");
  }
  if(configuration->servo_two_pin != -1){

    gpio_set_function(configuration->servo_two_pin, GPIO_FUNC_PWM);
    configuration->servo_two_slice = pwm_gpio_to_slice_num(configuration->servo_two_pin);
    pwm_config_set_clkdiv(&config, configuration->st_clkdiv);
    pwm_config_set_wrap(&config, configuration->st_wrap);
    pwm_init(configuration->servo_two_pin, &config, true);
  }else {
    printf("Servo Motor Two:\tNot Initialized.\n");
  }
  if(configuration->servo_three_pin != -1){

    gpio_set_function(configuration->servo_three_pin, GPIO_FUNC_PWM);
    configuration->servo_three_slice = pwm_gpio_to_slice_num(configuration->servo_three_pin);
    pwm_config_set_clkdiv(&config, configuration->stt_clkdiv);
    pwm_config_set_wrap(&config, configuration->stt_wrap);
    pwm_init(configuration->servo_three_pin, &config, true);
  }else {
    printf("Servo Motor Three:\tNot Initialized.\n");
  }
  if(configuration->servo_four_pin != -1){

    gpio_set_function(configuration->servo_four_pin, GPIO_FUNC_PWM);
    configuration->servo_four_slice = pwm_gpio_to_slice_num(configuration->servo_four_pin);
    pwm_config_set_clkdiv(&config, configuration->sf_clkdiv);
    pwm_config_set_wrap(&config, configuration->sf_wrap);
    pwm_init(configuration->servo_four_pin, &config, true);
  }else {
    printf("Servo Motor Four:\tNot Initialized.\n");
  }
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