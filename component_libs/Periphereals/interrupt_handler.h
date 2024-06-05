#ifndef INTERRUPT_HANDLER_S_H
#define INTERRUPT_HANDELR_S_H

#include <stdio.h>
#include <stdint.h>
#include "resources/pico_pin_enum.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

typedef struct {
const uint rotary_button_clk;
const uint rotary_button_dt;
const uint rotary_button_button;
const uint analog_button_one;
const uint analog_button_two;
const uint button_one;
const uint button_two;
const uint button_three;
const uint button_four;
const uint button_five;
const uint button_six;
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


// Large Button Call_back Function
void enabled_button_callback(uint gpio, uint32_t events){

   printf("Original Callback Function Occured:\n \tPin: %d.\n\tEvent: %d.\n", gpio, events);
 
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
          rotary.rotary_total += 25;
            }else {
              printf("Maximum Rotation Value.\n");
            }
         
        }else {
          //  printf("B == A.\n");
            if(rotary.rotary_total > 0){
            rotary.rotary_total -= 25;
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
 }


void rotary_encoder_init(button_types *types){
  
      if(types->rotary_button_clk != UNDEFINED){
      gpio_init(types->rotary_button_clk);
      gpio_set_dir(types->rotary_button_clk, GPIO_IN);
      gpio_pull_up(types->rotary_button_clk);
      gpio_set_irq_enabled_with_callback(types->rotary_button_clk, 0x04, true, &enabled_button_callback);
        printf("Rotary Encoder Clk: Pin %d.\n\tInitialized.\n\n", types->rotary_button_clk);
    } else {
                printf("Rotary Clk Not Used.\n\n");
    }

        if(types->rotary_button_dt != UNDEFINED){
          gpio_init(types->rotary_button_dt);
      gpio_set_dir(types->rotary_button_dt, GPIO_IN);
      gpio_pull_up(types->rotary_button_dt);
      gpio_set_irq_enabled_with_callback(types->rotary_button_dt, 0x04, true, &enabled_button_callback);
        printf("Rotary Encoder DT: Pin %d.\n\tInitialized.\n\n", types->rotary_button_dt);
    } else {
                printf("Rotary DT Not Used.\n\n");
    }
}


void button_interrupt_init(button_types *types){

  printf("\nInterrupt Button Initializing.\n\n");

        if(types->rotary_button_button != UNDEFINED){
          gpio_init(types->rotary_button_button);
      gpio_set_dir(types->rotary_button_button, GPIO_IN);
     // gpio_pull_up(types->rotary_button_button);
        gpio_set_irq_enabled_with_callback(types->rotary_button_button, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Rotary\n\tPin: %d: Initialized.\n\n", types->rotary_button_button);
    } else {
                printf("Rotary Button Not Used.\n\n");
    }

        if(types->analog_button_one != UNDEFINED){
      gpio_init(types->analog_button_one);
      gpio_set_dir(types->analog_button_one, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button One\n\tPin %d: Initialized.\n\n", (types->analog_button_one));
    } else {
        printf("Analog Button One Not Used.\n\n");
    }

    if(types->analog_button_two != UNDEFINED){
      gpio_init(types->analog_button_two);
      gpio_set_dir(types->analog_button_two, GPIO_IN);
      gpio_pull_up(types->analog_button_one);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button Two\n\tPin %d: Initialized.\n\n", types->analog_button_two);
    } else {
        printf("Analog Button Two Not Used.\n\n");
    }

    if(types->button_one != UNDEFINED){
      gpio_init(types->button_one);
      gpio_set_dir(types->button_one, GPIO_IN);
      gpio_pull_up(types->button_one);
      gpio_set_irq_enabled_with_callback(types->button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_one);
    } else {
                printf("Button One Not Used.\n\n");
    }
    if(types->button_two != UNDEFINED){
      gpio_init(types->button_two);
      gpio_set_dir(types->button_two, GPIO_IN);
      gpio_pull_down(types->analog_button_two);
      gpio_set_irq_enabled_with_callback(types->button_two, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_two);
    } else {
                printf("Button Two Not Used.\n\n");
    }

    if(types->button_three != UNDEFINED){
      gpio_init(types->button_three);
      gpio_set_dir(types->button_three, GPIO_IN);
      gpio_pull_down(types->button_three);
      gpio_set_irq_enabled_with_callback(types->button_three, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_three);
    } else {
                printf("Button Three Not Used.\n\n");
    }
}

#endif