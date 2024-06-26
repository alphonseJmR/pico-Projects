#ifndef SERVO_MOTOR_S_H
#define SERVO_MOTOR_S_H

#include <stdio.h>
#include <stdint.h>
#include "../resources/pico_pin_enum.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define input_Test(a, b, c) (bool)( ( ((uint16_t)a <= (uint16_t)c ) & ((uint16_t)b >= (uint16_t)c) ))

//  Hold two pins for now, more can be added fairly easily.
typedef struct servo_motor_pin_s {
  
  uint8_t servo_pin_one;
  uint8_t servo_pin_two;

}servo_pin;

typedef struct servo_motor_pwm_configuration_s {
  
  uint8_t servo_slice_one;
  uint8_t servo_chan_one;
  uint16_t servo_wrap_one;
  uint16_t servo_one_level;

  uint8_t servo_slice_two;
  uint8_t servo_chan_two;
  uint16_t servo_wrap_two;
  uint16_t servo_two_level;

  uint16_t servo_middle_ground;

}servo_pwm;

typedef struct servo_motor_configuration_s {

  servo_pin my_perv;
  servo_pwm my_serv;

}servo_motor;


void set_servo_position(uint8_t pin_selection, servo_motor *moto, uint16_t position){
  

   if(pin_selection == moto->my_perv.servo_pin_one){

      printf("Servo Motor Input Position Value: %i.\n", position);
      pwm_set_gpio_level(pin_selection, ((position / 20000.f) * 39062.f));
      printf("Servo Motor Position: %.\n", ((position / 20000.f) * 39062.f));

  }

  if(pin_selection == moto->my_perv.servo_pin_two){   

      pwm_set_gpio_level(pin_selection, (position /20000.f)*39062.f);  

  }

}


void set_servo_initial_position(servo_motor *config){

  uint16_t initial_smp;
  initial_smp = ((config->my_serv.servo_middle_ground/20000.f)*39062.f);

    if(config->my_perv.servo_pin_one != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_one, initial_smp);
      printf("Servo Motor One Initialized to: %li.\n", initial_smp);
    }
  
  if(config->my_perv.servo_pin_two != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_two, initial_smp);
      printf("Servo Motor Two Initialized to: %li.\n", initial_smp);
    }
/*
  if(config->servo_three_pin != -1){
      pwm_set_gpio_level(config->servo_three_pin, initial_smp);
      printf("Servo Motor Three Initialized to: %li.\n", initial_smp);
    }
  
  if(config->servo_four_pin != -1){
      pwm_set_gpio_level(config->servo_four_pin, initial_smp);
      printf("Servo Motor Four Initialized to: %li.\n", initial_smp);
    }
*/
}


void servo_initialization(servo_motor *configuration){

  printf("\n////\tServo Motor Initialisation\t////\n");

  pwm_config config = pwm_get_default_config();

  if(configuration->my_perv.servo_pin_one != UNDEFINED){

    gpio_set_function(configuration->my_perv.servo_pin_one, GPIO_FUNC_PWM);
      printf("Initializing Servo Pin: %i.\n", configuration->my_perv.servo_pin_one);
    configuration->my_serv.servo_slice_one = pwm_gpio_to_slice_num(configuration->my_perv.servo_pin_one);
      printf("Servo Slice: %i.\n", configuration->my_serv.servo_slice_one);
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, configuration->my_serv.servo_wrap_one);
    pwm_init(configuration->my_serv.servo_slice_one, &config, true);
    
    printf("////\tServo One: Initialised\t////\n");
    
  }else {
    printf("Servo Motor One:\tNot Initialized.\n");
  }

  if(configuration->my_perv.servo_pin_two != UNDEFINED){

    gpio_set_function(configuration->my_perv.servo_pin_two, GPIO_FUNC_PWM);
    configuration->my_serv.servo_slice_two = pwm_gpio_to_slice_num(configuration->my_perv.servo_pin_two);
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, configuration->my_serv.servo_wrap_two);
    pwm_init(configuration->my_perv.servo_pin_two, &config, true);

    printf("////\tServo Two: Initialised\t////\n");
  }else {
    printf("Servo Motor Two:\tNot Initialized.\n");
  }

/*
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

*/

  set_servo_initial_position(configuration);
}

#endif