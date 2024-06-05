#ifndef DC_MOTOR_HEADER_S_H
#define DC_MOTOR_HEADER_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico_pin_enum.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define idleValue(x, y, z) (bool)(((uint16_t)y >= (uint16_t)x) && ((uint16_t)x <= (uint16_t)z))
#define pwmAbove(a, b) (bool)((uint16_t)a > (uint16_t)b)
#define pwmBelow(a, b) (bool) ((uint16_t)a < (uint16_t)b)


typedef struct {

    uint pwm_pin_one;
    uint pwm_pin_two;

    uint slice_one;
    uint slice_two;

    uint chan_one;
    uint chan_two;

    uint16_t wrap_one_init;
    uint16_t wrap_two_init;

    float duty_cycle_one_init;
    float duty_cycle_two_init;

    char wrap_value_one[16];
    char wrap_value_two[16];

    char duty_cycle_one[16];
    char duty_cycle_two[16];

    uint16_t rotary_wrap_input_level;          //   Value taken from rotary encoder.
    uint16_t analog_vertical_input_level;      //   Vertical value taken from analog joystick.  Premapped.
    uint16_t analog_horizontal_input_level;    //   Horizontal value from analog joystick.      Premapped.

    uint16_t wrap_output_level_one;
    uint16_t wrap_output_level_two;

    float duty_cycle_output_one;
    float duty_cycle_output_two;

}pwm_settings_t;
pwm_settings_t pwm_header_config;

void motor_data_display_buffer(pwm_settings_t *pwm){

    if(pwm->pwm_pin_one != UNDEFINED){

        if(pwm->wrap_output_level_one > 0){
            sprintf(pwm->wrap_value_one, "Wrap One: %i", pwm->wrap_output_level_one);
        }
        if(pwm->duty_cycle_output_one > 0) {
            sprintf(pwm->duty_cycle_one, "D Cycle: %f", pwm->duty_cycle_output_one);
        }
    }
    if(pwm->pwm_pin_two != UNDEFINED){

        if(pwm->wrap_output_level_two > 0){
            sprintf(pwm->wrap_value_two, "Wrap Two: %i", pwm->wrap_output_level_two);
        }
        if(pwm->duty_cycle_output_two > 0) {
            sprintf(pwm->duty_cycle_two, "D Cycle: %f", pwm->duty_cycle_output_two);
        }
    }

}

//  Completed
void initial_pwm_pin_init(pwm_settings_t *pwm){

    printf("\nPWM Initialization.\n");

    if(pwm->pwm_pin_one != UNDEFINED){
        
        printf("First PWM: %i.\n", pwm->pwm_pin_one);
      
      gpio_set_function(pwm->pwm_pin_one, GPIO_FUNC_PWM);
    
      pwm->slice_one = pwm_gpio_to_slice_num(pwm->pwm_pin_one);
        printf("PWM Pin %i's slice: %i.\n", pwm->pwm_pin_one, pwm->slice_one);

      pwm->chan_one = pwm_gpio_to_channel(pwm->pwm_pin_one);
        printf("PWM Pin %i's chan: %i.\n", pwm->pwm_pin_one, pwm->chan_one);

      pwm_set_wrap(pwm->slice_one, pwm->wrap_one_init);
        printf("Initial Wrap Set: %u.\n", pwm->wrap_one_init);

      pwm_set_chan_level(pwm->slice_one, pwm->chan_one, pwm->duty_cycle_one_init);
        printf("Initial Duty Cycle: %f.\n", pwm->duty_cycle_one_init);

      pwm_set_enabled(pwm->slice_one, true);
        printf("PWM Enabled.\n");
    }

    if(pwm->pwm_pin_two != UNDEFINED){
        printf("Second PWM: %i.\n", pwm->pwm_pin_two);
     
      gpio_set_function(pwm->pwm_pin_two, GPIO_FUNC_PWM);

      pwm->slice_two = pwm_gpio_to_slice_num(pwm->pwm_pin_two);
        printf("PWM Pin %i's slice: %i.\n", pwm->pwm_pin_two, pwm->slice_two);

      pwm->chan_two = pwm_gpio_to_channel(pwm->pwm_pin_two);
        printf("PWM Pin %i's chan: %i.\n", pwm->pwm_pin_two, pwm->chan_two);

      pwm_set_wrap(pwm->slice_two, pwm->wrap_two_init);
        printf("Initial Wrap Set: %u.\n", pwm->wrap_two_init);

      pwm_set_chan_level(pwm->slice_two, pwm->chan_two, pwm->duty_cycle_two_init);
        printf("Initial Duty Cycle: %f.\n", pwm->duty_cycle_two_init);

      pwm_set_enabled(pwm->slice_two, true);
        printf("PWM Enabled.\n");
    }

}



#endif
