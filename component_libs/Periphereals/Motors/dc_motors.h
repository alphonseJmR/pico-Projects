#ifndef DIRECT_CURRENT_MOTOR_S_H
#define DIRECT_CURRENT_MOTOR_S_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "../resources/pico_pin_enum.h"

#define inputAbove(a, b) (bool)((uint16_t)a > (uint16_t)b)
#define inputBelow(a, b) (bool) ((uint16_t)a < (uint16_t)b)
#define input_Test(a, b, c) (bool)( ( ((uint16_t)a <= (uint16_t)c ) & ((uint16_t)b >= (uint16_t)c) ))

#define analog_idle_min 1900
#define analog_idle_max 2100

bool analog_idle;
uint16_t previous_anal;
uint16_t cur_anal;


uint16_t five_K_inc_wrap[9] = {
    0x1388,
    0x2710,
    0x3A98,
    0x4E20,
    0x61A8,
    0x7530,
    0x88B8,
    0x9C40,
    0xAFC8
};
uint16_t analog_checks[18] = {

    0x00, 0x70,
    0x71, 0xE1,
    0xE2, 0x152,
    0x153, 0x1C3,
    0x1C4, 0x234,
    0x235, 0x2A5,
    0x2A6, 0x316,
    0x317, 0x387,
    0x388, 0x3FF

};
uint16_t rotary_checks[18] = {

    0x01, 0x31,
    0x32, 0x63,
    0x64, 0x95,
    0x96, 0xC7,
    0xC8, 0xF9,
    0xFA, 0x12B,
    0x12C, 0x15D,
    0x15E, 0x18F,
    0x190, 0x1F3

};


//  Holds only two settings for two motors, forward, and reverse.
typedef struct dc_motor_pin_s {

    uint8_t motor_one_forward;
    uint8_t motor_one_reverse;
    
    uint8_t motor_two_forward;
    uint8_t motor_two_reverse;

}dc_pins;

typedef struct dc_motor_pwm_configuration_s {

    uint8_t dc_slice_one;
    uint8_t dc_chan_one;
    uint16_t dc_wrap_one;
    uint16_t dc_one_level;

    uint8_t dc_slice_two;
    uint8_t dc_chan_two;
    uint16_t dc_wrap_two;
    uint16_t dc_two_level;

}dc_pwm;

typedef struct dc_motor_configuration_s {

    dc_pins my_dc_pins;
    dc_pwm my_dc_pwm;

}dc_motor;


void set_new_configuration(dc_motor *dc, uint8_t motor_num, bool direction);
uint16_t set_drive_power(dc_motor *dc, uint16_t rotary_in, uint16_t *duty_tbl);
uint16_t set_drive_wrap(dc_motor *dc, uint16_t analog_in, uint16_t *wrap_tbl_chk, uint16_t *wrap_tbl);
void set_motor_values(dc_motor *DC, uint16_t an_in, uint16_t ro_in);
dc_motor initialise_dc_pwm_pins(dc_motor *dc);
bool check_analog_direction(uint16_t analog_in);


//  Only handles a single motor ATM.  Adding a second is just a conditional.
void set_new_configuration(dc_motor *dc, uint8_t motor_num, bool direction){

    printf("\t////\tSetting New DC PWM Configuration\t////\n");

 if(motor_num == 1){

  if(direction){
      //  Set direction Forward_Drive values.   
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_wrap_one);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_chan_one, dc->my_dc_pwm.dc_one_level);
      sleep_us(2);
      //  Set direction Reverse_Drive to zeros.
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_two, 0);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_chan_two, 0);

  }else {
      //  Set direction Reverse_Drive values.
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_wrap_two);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_chan_two, dc->my_dc_pwm.dc_two_level);
      sleep_us(2);
      //  Set direction Forward_Drive values to zero.
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_one, 0);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_chan_one, 0);

  }
 }
 if(motor_num == 2){

  if(direction){
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_wrap_one);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_chan_one, dc->my_dc_pwm.dc_one_level);
  }else {
    pwm_set_wrap(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_wrap_two);
    pwm_set_chan_level(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_chan_two, dc->my_dc_pwm.dc_two_level);
  }
 }

}


uint16_t set_drive_power(dc_motor *dc, uint16_t rotary_in, uint16_t *duty_tbl){

    uint16_t buffer_out;
    buffer_out = 0;

    for(int i = 0; i <= 18; i += 2){

        buffer_out = (input_Test(duty_tbl[i], duty_tbl[i+1], rotary_in)) ? (dc->my_dc_pwm.dc_wrap_one * ((i / 2) * .1)) : 0;
        i = (buffer_out > 0) ? 17 : i;
        
    }
    printf("Drive Power Now: 0x%04x.\n", buffer_out);


    return buffer_out;
}


uint16_t set_drive_wrap(dc_motor *dc, uint16_t analog_in, uint16_t *wrap_tbl_chk, uint16_t *wrap_tbl){

    uint16_t buffer_out;
    buffer_out = 0;

    for(int i = 0; i < 18; i += 2){

        buffer_out = (input_Test(wrap_tbl_chk[i], wrap_tbl_chk[i+1], analog_in)) ? wrap_tbl[ (i / 2) ] : 0;
        i = (buffer_out > 0) ? 17 : i;
        
    }
    printf("Drive Wrap Now: 0x%04X.\n", buffer_out);

    return buffer_out;

}


void set_motor_values(dc_motor *DC, uint16_t an_in, uint16_t ro_in){

    printf("\n\t////\tSetting New Values For DC Motor\t////\n");
  bool dir;
  dir = false;
  analog_idle = input_Test(495, 525, an_in);
  
  printf("\n// // ///\tCurrent Analog Input: %i\t/// // //\n", an_in);

  if(!analog_idle){
    dir = check_analog_direction(an_in);

  if(dir){

    DC->my_dc_pwm.dc_wrap_one = set_drive_wrap(DC, an_in, analog_checks, five_K_inc_wrap);
    DC->my_dc_pwm.dc_one_level = set_drive_power(DC, ro_in, rotary_checks);
      set_new_configuration(DC, 1, dir);
    
  }else{

    DC->my_dc_pwm.dc_wrap_two = set_drive_wrap(DC, an_in, analog_checks, five_K_inc_wrap);
    DC->my_dc_pwm.dc_two_level = set_drive_power(DC, ro_in, rotary_checks);
      set_new_configuration(DC, 1, dir);
      
    }

  }else{

    printf("Idle value's zeroed.\n");

      pwm_set_chan_level(DC->my_dc_pwm.dc_slice_one, DC->my_dc_pwm.dc_chan_one, 0);
      pwm_set_chan_level(DC->my_dc_pwm.dc_slice_two, DC->my_dc_pwm.dc_chan_two, 0);
      
    printf("Analog Stick Idle.\n");
  }

}

bool check_analog_direction(uint16_t analog_in){

  bool test_buffer_upper;
  bool test_buffer_lower;
  bool output_buffer;

  test_buffer_upper = false;
  test_buffer_lower = false;
  output_buffer = false;

  test_buffer_upper = (inputAbove(analog_in, 525)) ? true : false;
  (test_buffer_upper) ? printf("Upper True\n") : printf("Upper False\n");
  test_buffer_lower = (inputBelow(analog_in, 495)) ? true : false;
  (test_buffer_lower) ? printf("Lower True\n") : printf("Lower False\n");

    output_buffer = (test_buffer_upper && !test_buffer_lower) ? true : false;
  (output_buffer) ? printf("Direction: Drive\n") : printf("Direction: Reverse\n");
  
    return output_buffer;
}

//  Initialise PWM Pins for user selected GPIO pins.  DC_MOTORS
//  ADD A RETURN VALUE FOR ERROR CHECKING AT SOME POINT, means making a new error.h :)
//  Just realized dc_motor_pwm_configuration_s needs dual copies of the slice-level for the second motor.
dc_motor initialise_dc_pwm_pins(dc_motor *dc){

    printf("\nPWM Initialization.\n");

    if(dc->my_dc_pins.motor_one_forward != UNDEFINED){
        printf("First PWM: %i.\n", dc->my_dc_pins.motor_one_forward);
      
      gpio_set_function(dc->my_dc_pins.motor_one_forward, GPIO_FUNC_PWM);

      dc->my_dc_pwm.dc_slice_one = pwm_gpio_to_slice_num(dc->my_dc_pins.motor_one_forward);
      printf("PWM Pin %i's slice: %i.\n", dc->my_dc_pins.motor_one_forward , dc->my_dc_pwm.dc_slice_one);

      dc->my_dc_pwm.dc_chan_one = pwm_gpio_to_channel(dc->my_dc_pins.motor_one_forward);
        printf("PWM Pin %i's chan: %i.\n", dc->my_dc_pins.motor_one_forward , dc->my_dc_pwm.dc_chan_one);

      pwm_set_wrap(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_wrap_one);
        printf("Initial Wrap Set: %u.\n", dc->my_dc_pwm.dc_wrap_one);

      pwm_set_chan_level(dc->my_dc_pwm.dc_slice_one, dc->my_dc_pwm.dc_wrap_one, dc->my_dc_pwm.dc_one_level);
        printf("Initial Duty Cycle: %i.\n", dc->my_dc_pwm.dc_one_level);

      pwm_set_enabled(dc->my_dc_pins.motor_one_forward, true);
        printf("PWM Enabled.\n");
    }

    if(dc->my_dc_pins.motor_one_reverse != UNDEFINED){
        printf("Second PWM: %i.\n", dc->my_dc_pins.motor_one_reverse);
     
      gpio_set_function(dc->my_dc_pins.motor_one_reverse, GPIO_FUNC_PWM);

      dc->my_dc_pwm.dc_slice_two = pwm_gpio_to_slice_num(dc->my_dc_pins.motor_one_reverse);
        printf("PWM Pin %i's slice: %i.\n", dc->my_dc_pins.motor_one_reverse, dc->my_dc_pwm.dc_slice_two);

      dc->my_dc_pwm.dc_chan_two = pwm_gpio_to_channel(dc->my_dc_pins.motor_one_reverse);
        printf("PWM Pin %i's chan: %i.\n", dc->my_dc_pins.motor_one_reverse, dc->my_dc_pwm.dc_chan_two);

      pwm_set_wrap(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_wrap_two);
        printf("Initial Wrap Set: %u.\n", dc->my_dc_pwm.dc_wrap_two);

      pwm_set_chan_level(dc->my_dc_pwm.dc_slice_two, dc->my_dc_pwm.dc_chan_two, dc->my_dc_pwm.dc_two_level);
        printf("Initial Duty Cycle: %i.\n", dc->my_dc_pwm.dc_two_level);

      pwm_set_enabled(dc->my_dc_pwm.dc_slice_two, true);
        printf("PWM Enabled.\n");
    }

    return *dc;

}



#endif