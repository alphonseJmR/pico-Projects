#ifndef SERVO_MOTOR_S_H
#define SERVO_MOTOR_S_H

#include <stdio.h>
#include <stdint.h>
#include "../resources/pico_pin_enum.h"
#include "../../hi_lvl_resources/project_struct_s.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define input_Test(a, b, c) ((bool)( ( ((uint16_t)a <= (uint16_t)c ) && ((uint16_t)b >= (uint16_t)c) )))
#define idle_return(a, b, c) (uint16_t)( (((uint16_t)a >= b) && ((uint16_t)a <= c) ? 505 : a))


uint16_t hori_testers[12] = {

  0x0000, 0x0038,
  0x0039, 0x00CA,
  0x00CB, 0x01DB,
  0x0213, 0x0296,
  0x0297, 0x0328,
  0x0329, 0x0400

};

uint16_t ms18_turn_degs[6] = {

0x0248,
0x036C,
0x03FE,
0x076A,
0x088E,
0x09B2

};

uint16_t servo_turn_table[6] = {

  0x0474,
  0x06AE,
  0x07CC,
  0x0B23,
  0x0E7A,
  0x10B5

};


void set_servo_position(uint8_t pin_selection, servo_motor *moto, uint16_t position){
  
   if(pin_selection == moto->my_perv.servo_pin_one){

    pwm_set_gpio_level(pin_selection, position);
  //    printf("Servo Motor Position: %.\n", position);

  }

  if(pin_selection == moto->my_perv.servo_pin_two){   

      pwm_set_gpio_level(pin_selection, position);  

  }

}

void new_servo_turn(uint8_t pin, uint16_t pos){

  if(pos != 2851){
    set_servo_position(pin, &my_servo, pos);
  }else {
    set_servo_position(pin, &my_servo, 2851);
  }

}

void set_servo_initial_position(servo_motor *config){

  if(config->my_perv.servo_pin_one != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_one, 2851);
      printf("Servo Motor One Initialized to: %li.\n", 2851);
  }
  
  if(config->my_perv.servo_pin_two != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_two, 2851);
      printf("Servo Motor Two Initialized to: %li.\n", 2851);
    }

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


uint16_t servo_pre_calc(uint16_t hori_data, uint16_t *tab){

  bool test_status;
    test_status = false;
  uint16_t buffered_degree;
    buffered_degree = 0;

  size_t check_size = ( (sizeof(hori_testers) / sizeof(uint16_t)) );
//  printf("Size of Hori_checker: %i.\n", check_size);

//    printf("\nRaw Horizontal Data: %i.\n", hori_data);

  test_status = input_Test(480, 525, hori_data);

    if(test_status){
    
      buffered_degree = 2851;
      test_status = false;

    }else{

    for(int i = 0; i < check_size; i += 2){

      test_status = input_Test(hori_testers[i], hori_testers[i+1], hori_data);
        if(test_status){

          buffered_degree = tab[(i/2)];
          printf("Current Servo Output: %i\n", buffered_degree);
          i = check_size;

        }else{

          printf("Unmatched.\n");


        }
    }
  }

  return buffered_degree;

}


uint16_t set_servo_turn(servo_motor *servo, volatile uint16_t horizontal_data, uint16_t prev_pos){

//  printf("\n////\tSet Servo Turn\t////\n");
  printf("///\tPrevious Position: %i.\t///\n", prev_pos);

  bool idle_turn;
  idle_turn = false;
    //  Check if input is idle value.
  idle_turn = input_Test(1400, 1500, horizontal_data);
  
  if(!idle_turn){
    (!idle_turn) ? printf("Servo Turning.\n") :  printf("Servo Turn Idle\n");

  set_servo_position(10, servo, horizontal_data);
//    printf("Re-Enabling Servo PWM");
  pwm_set_enabled(servo->my_serv.servo_slice_one, true);
      sleep_us(50);

  }else{

        set_servo_position(10, servo, 2851);
        //  pwm_set_counter(my_servo.my_serv.servo_slice_one, 0);
        sleep_us(50);
        pwm_set_enabled(servo->my_serv.servo_slice_one, false);

  }

  return prev_pos;
}


#endif
