#ifndef SERVO_MOTOR_S_H
#define SERVO_MOTOR_S_H

#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"

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

}servo_pwm;

typedef struct servo_motor_configuration_s {

  struct servo_pin;
  struct servo_pwm;

}servo_motor;

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

void servo_turn(uint16_t turn_value, servo_motor_config_t *servo){

    if(idleValue(turn_value, 0x1CC, 0x1E0)){
        printf("Idle Horizontal Value.  No Turn.\n");
    }else {
        servo->servo_one_position = turn_value;
        set_servo_position(servo->servo_one_pin);
    }

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

servo_motor_config_t servo_configuration = {

  .servo_one_pin = GPIO_EIGHTEEN,
  .servo_initial_position = 1500,
  .servo_inital_positioning = true,
  .mcu_clock = 125000000,
  .so_clkdiv = 64,
  .so_wrap = 39062,
  .servo_two_pin = UNDEFINED,
  .servo_three_pin = UNDEFINED,
  .servo_four_pin = UNDEFINED

};



#endif