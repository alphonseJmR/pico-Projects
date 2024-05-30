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

//  Completed
//  Pwm_set_channel_levels takes uint PWM_PIN, uint8_t Channel input value, and uint16_t wrap set value.
void pwm_set_motor_config(motor_flags_status *flag, pwm_settings_t *pwm){
   
    printf("\nPwm Set Motor Config Function Active.\n");

    if(flag->vdtp == true){
        if(pwm->pwm_pin_one != UNDEFINED){
            printf("Setting PWM on %i.\n", pwm->pwm_pin_one);

            printf("Wrap One: %i.\n", pwm->wrap_output_level_one);
            pwm_set_wrap(pwm->slice_one, pwm->wrap_output_level_one);

            printf("Duty Cycle One: %f.\n", pwm->duty_cycle_output_one);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, pwm->duty_cycle_output_one);

          //  pwm_set_enabled(pwm->slice_one, true);
        }
    }else if(flag->vdti == true){
        printf("PWM Idle Setting.\n");
        if(pwm->pwm_pin_one != UNDEFINED){

            pwm_set_wrap(pwm->slice_one, pwm->wrap_output_level_one);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, 0);
            pwm_set_enabled(pwm->slice_one, true);
        }
        if(pwm->pwm_pin_two != UNDEFINED){
            
            pwm_set_wrap(pwm->slice_two, pwm->wrap_output_level_two);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, 0);
            pwm_set_enabled(pwm->slice_two, true);
        }
    }else if(flag->vdtn == true){

        if(pwm->pwm_pin_two != UNDEFINED){
            pwm_set_wrap(pwm->slice_two, pwm->wrap_output_level_two);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, pwm->duty_cycle_output_two);
            pwm_set_enabled(pwm->slice_two, true);
        }
    }
}

void test_motor_activation(pwm_settings_t *pwm, motor_flags_status *flag, adc_port_values *adc, rotary_encoder_struct *rotary){

    printf("\nMotor Test Activation Function.\n");
    pwm->rotary_wrap_input_level = rotary->rotary_total;
    printf("PWM Rotary Buffer Value: %i.\n", pwm->rotary_wrap_input_level);

    pwm->analog_vertical_input_level = adc->adc1_mapped_value;
    printf("\nPWM Buffered VERTICAL: %i.\n", pwm->analog_vertical_input_level);
    pwm->analog_horizontal_input_level = adc->adc2_mapped_value;
    printf("PWM Buffered HORIZONTAL: %i.\n\n", pwm->analog_horizontal_input_level);

    motor_flag_status(pwm, flag);
    motor_drive_function(pwm, flag);
    drive_motor_wrap_set(pwm);
    
   // servo_turn(pwm->analog_horizontal_input_level, &servo_configuration);

    pwm_set_motor_config(flag, pwm);
    motor_data_display_buffer(pwm);
}



#endif
