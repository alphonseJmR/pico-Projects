#ifndef BEER_PERI_HEADER_H
#define BEER_PERI_HEADER_H

#include <stdio.h>
#include "beer_list.h"
#include "beer_score.h"

/*  Button Interrupt Init
  Sets GPIO function enabled with callback for up to four gpio interrupts.
  Accepts a struct of button_types
  Using if statements to determine if struct values are null or not
  Initializes interrupts for gpio pins if true.
*/
void interrupt(uint gpio, uint32_t event);


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
      gpio_pull_up(types->analog_button_two);
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &enabled_button_callback);
        printf("Button Type: Analog Button Two\n\tPin %d: Initialized.\n\n", types->analog_button_two);
    } else {
        printf("Analog Button Two Not Used.\n\n");
    }

    if(types->button_one != -1){
      gpio_init(types->button_one);
      gpio_set_dir(types->button_one, GPIO_IN);
      gpio_pull_down(types->button_one);
      gpio_set_irq_enabled_with_callback(types->button_one, 0x04, true, &interrupt);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_one);
    } else {
                printf("Button One Not Used.\n\n");
    }

    if(types->button_two != -1){
      gpio_init(types->button_two);
      gpio_set_dir(types->button_two, GPIO_IN);
      gpio_pull_down(types->button_two);
      gpio_set_irq_enabled_with_callback(types->button_two, 0x04, true, &interrupt);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_two);
    } else {
                printf("Button Two Not Used.\n\n");
    }

    if(types->button_three != -1){
      gpio_init(types->button_three);
      gpio_set_dir(types->button_three, GPIO_IN);
      gpio_pull_down(types->button_three);
      gpio_set_irq_enabled_with_callback(types->button_three, 0x04, true, &interrupt);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_three);
    } else {
                printf("Button Three Not Used.\n\n");
    }

    if(types->button_four != -1){
      gpio_init(types->button_four);
      gpio_set_dir(types->button_four, GPIO_IN);
      gpio_pull_down(types->button_four);
      gpio_set_irq_enabled_with_callback(types->button_four, 0x04, true, &interrupt);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_four);
    } else {
                printf("Button Four Not Used.\n\n");
    }

        if(types->button_five != -1){
      gpio_init(types->button_five);
      gpio_set_dir(types->button_five, GPIO_IN);
      gpio_pull_down(types->button_five);
      gpio_set_irq_enabled_with_callback(types->button_five, 0x04, true, &interrupt);
        printf("Button Type: Press\n\tPin %d: Initialized.\n\n", types->button_five);
    } else {
                printf("Button Five Not Used.\n\n");
    }

    
}



#endif