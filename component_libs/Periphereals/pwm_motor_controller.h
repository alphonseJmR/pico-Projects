#ifndef ALL_MOTOR_PWM_CONTROLLER_S_H
#define ALL_MOTOR_PWM_CONTROLLER_S_H

#include <stdio.h>
#include "resources/pico_pin_enum.h"

// #define SERVO_S
// #define STEPPER_S
#define DC_S
// #define BLDC_S   In the future.  

#ifdef SERVO_S
#include "Motors/servo_motors.h"
#endif


#ifdef STEPPER_S
#include "Motors/stepper_motors.h"

#endif


#ifdef DC_S
#include "Motors/dc_motors.h"

//  Initialise PWM Pins for user selected GPIO pins.  DC_MOTORS
//  ADD A RETURN VALUE FOR ERROR CHECKING AT SOME POINT, means making a new error.h :)
//  Just realized dc_motor_pwm_configuration_s needs dual copies of the slice-level for the second motor.
void initialise_dc_pwm_pins(dc_motor *dc){

    printf("\nPWM Initialization.\n");

    if(dc->my_pins->motor_one_forward != UNDEFINED){
        printf("First PWM: %i.\n", dc->my_pins->motor_one_forward);
      
      gpio_set_function(dc->my_pins->motor_one_forward, GPIO_FUNC_PWM);
    
      dc->my_pwm->dc_slice_one = pwm_gpio_to_slice_num(dc->my_pins->motor_one_forward);
      printf("PWM Pin %i's slice: %i.\n", dc->my_pins->motor_one_forward , dc->my_pwm->dc_slice_one);

      dc->my_pwm->dc_chan_one = pwm_gpio_to_channel(dc->my_pins->motor_one_forward);
        printf("PWM Pin %i's chan: %i.\n", dc->my_pins->motor_one_forward , dc->my_pwm->dc_chan_one);

      pwm_set_wrap(dc->my_pwm->dc_slice_one, dc->my_pwm->dc_wrap_one);
        printf("Initial Wrap Set: %u.\n", dc->my_pwm->dc_wrap_one);

      pwm_set_chan_level(dc->my_pwm->dc_slice_one, dc->my_pwm->dc_wrap_one, dc->my_pwm->dc_one_level);
        printf("Initial Duty Cycle: %f.\n", dc->my_pwm->dc_one_level);

      pwm_set_enabled(dc->my_pins->motor_one_forward, true);
        printf("PWM Enabled.\n");
    }

    if(dc->my_pins->motor_two_forward != UNDEFINED){
        printf("Second PWM: %i.\n", dc->my_pins->motor_two_forward);
     
      gpio_set_function(dc->my_pins->motor_two_forward, GPIO_FUNC_PWM);

      dc->my_pwm->dc_slice_two = pwm_gpio_to_slice_num(dc->my_pins->motor_two_forward);
        printf("PWM Pin %i's slice: %i.\n", dc->my_pins->motor_two_forward, dc->my_pwm->dc_slice_two);

      dc->my_pwm->dc_chan_two = pwm_gpio_to_channel(dc->my_pins->motor_two_forward);
        printf("PWM Pin %i's chan: %i.\n", dc->my_pins->motor_two_forward, dc->my_pwm->dc_chan_two);

      pwm_set_wrap(dc->my_pwm->dc_slice_two, dc->my_pwm->dc_wrap_two);
        printf("Initial Wrap Set: %u.\n", dc->my_pwm->dc_wrap_two);

      pwm_set_chan_level(dc->my_pwm->dc_slice_two, dc->my_pwm->dc_chan_two, dc->my_pwm->dc_two_level);
        printf("Initial Duty Cycle: %f.\n", dc->my_pwm->dc_two_level);

      pwm_set_enabled(dc->my_pwm->dc_slice_two, true);
        printf("PWM Enabled.\n");
    }

}

#endif




#endif