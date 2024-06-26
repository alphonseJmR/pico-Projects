#ifndef INTERRUPT_MAIN_HANDLER_S_H
#define INTERRUPT_MAIN_HANDLER_S_H

#include <stdio.h>
#include <stdint.h>
#include "interrupt_structs.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "Periphereals/resources/pico_pin_enum.h"

uint8_t interruption_input_initialisation(input_types *ins);
void button_interrupt_handler(input_types *butts, uint gpio);
bool buttons_timer(input_types *my_ren);
bool callback_timer(input_types *my_ren);
void rotary_encoder_handler(input_types *ren, uint gpio, uint32_t events);
void main_interrupt_handler(uint gpio, uint32_t events);
uint16_t button_incrementer(input_types *butts);


input_types my_types = {

  .r_en = {
    .CLK = UNDEFINED,
    .DT = UNDEFINED,
    .REB = UNDEFINED,
    .max_rota_val = 500,
    .r_en_max = 2000
    },

  .t_buttons = {
    .b_zero = GPIO_NINETEEN,
    .b_one = GPIO_TWENTY
    
  },

  .button_value = 0

};

bool timed_result;

void main_interrupt_handler(uint gpio, uint32_t events){

    printf("\n////\tMain Interrupt Handler\t////\n");
        gpio_acknowledge_irq(gpio, events);
    if(gpio == my_types.r_en.CLK || gpio == my_types.r_en.DT){
        rotary_encoder_handler(&my_types, gpio, events);
    }else{
        button_interrupt_handler(&my_types, gpio);
    }
}

void rotary_encoder_handler(input_types *ren, uint gpio, uint32_t events){

    printf("\n///\tRotary Encoder Handler\t///\n");
    timed_result = callback_timer(ren);
        printf("\nPIN: %i.", gpio);
      if(timed_result){
          ren->r_en.r_en_initial = ren->r_en.r_en_curr;
        
    if(gpio == ren->r_en.CLK && (events & 0x04)){
        ren->r_en.cur_clk = gpio_get(ren->r_en.CLK);
        ren->r_en.cur_dt = gpio_get(ren->r_en.DT);

      if(ren->r_en.cur_clk != ren->r_en.pre_clk){
        if(ren->r_en.cur_dt != ren->r_en.cur_clk){
          if(ren->r_en.current_value <= ren->r_en.max_rota_val){
            ren->r_en.current_value += 25;
          }else{
            printf("Maximum Rotation Value.\n");
          }
        }else {
            if(ren->r_en.current_value > 0){
                ren->r_en.current_value -= 25;
            }else {
                printf("Minimum Rotation Value.\n");
            }
        }
      }
    }
        printf("Current Rotary Value: %i.\n", ren->r_en.current_value);
            ren->r_en.pre_clk = ren->r_en.cur_clk;
            ren->r_en.pre_dt = ren->r_en.cur_dt;
      }
}

bool callback_timer(input_types *my_ren){

    bool test_result;
    test_result = false;

    my_ren->r_en.r_en_curr = time_us_32();
    my_ren->r_en.r_en_last = (my_ren->r_en.r_en_curr - my_ren->r_en.r_en_initial);

    test_result = (my_ren->r_en.r_en_last >= my_ren->r_en.r_en_max) ? true : false;

    return test_result;
}

bool buttons_timer(input_types *my_ren){

    bool test_result;
    test_result = false;

    my_ren->t_buttons.curr_button = time_us_32();
    my_ren->t_buttons.last_button = (my_ren->t_buttons.curr_button - my_ren->t_buttons.initial_button);

    test_result = (my_ren->t_buttons.last_button >= my_ren->t_buttons.max_button_interrupt) ? true : false;

    return test_result;
}

void button_interrupt_handler(input_types *butts, uint gpio){

    if(gpio == butts->r_en.REB){
        printf("Rotary Button Pressed.\n");
         butts->t_buttons.reb_status = true;

    }else if(gpio == butts->t_buttons.b_zero){
        printf("Button One Pressed.\n");
          butts->t_buttons.b_ze_status = true;

    }else if(gpio == butts->t_buttons.b_one){
        printf("Button Two Pressed.\n");
          butts->t_buttons.b_on_status = true;

    }else if(gpio == butts->t_buttons.b_two){
        printf("Button Three Pressed.\n");
          butts->t_buttons.b_tw_status = true;

    }else if(gpio == butts->t_buttons.b_three){
        printf("Button Four Pressed.\n");
          butts->t_buttons.b_th_status = true;

    }else if(gpio == butts->t_buttons.b_four){
        printf("Button Four Pressed.\n");
          butts->t_buttons.b_fo_status = true;

    }else if(gpio == butts->t_buttons.b_five){
        printf("Button Five Pressed.\n");
          butts->t_buttons.b_fi_status = true;
    }
    
    butts->button_value += button_incrementer(butts);
    
    
}

uint8_t interruption_input_initialisation(input_types *ins){

    uint8_t pin_count;
    pin_count = 0;
    
    if(ins->r_en.CLK != UNDEFINED){
        gpio_init(ins->r_en.CLK);
        gpio_set_dir(ins->r_en.CLK, GPIO_IN);
            gpio_pull_down(ins->r_en.CLK);
        gpio_set_irq_enabled_with_callback(ins->r_en.CLK, 0x04, true, &main_interrupt_handler);
            printf("Button: RCLK: %i Initialised.\n", ins->r_en.CLK);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->r_en.DT != UNDEFINED){
        gpio_init(ins->r_en.DT);
        gpio_set_dir(ins->r_en.DT, GPIO_IN);
            gpio_pull_down(ins->r_en.DT);
        gpio_set_irq_enabled_with_callback(ins->r_en.DT, 0x04, true, &main_interrupt_handler);
            printf("Button: RDT: %i Initialised.\n", ins->r_en.DT);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->r_en.REB != UNDEFINED){
        gpio_init(ins->r_en.REB);
        gpio_set_dir(ins->r_en.REB, GPIO_IN);
            gpio_pull_down(ins->r_en.REB);
        gpio_set_irq_enabled_with_callback(ins->r_en.REB, 0x04, true, &main_interrupt_handler);
            printf("Button: REB: %i Initialised.\n", ins->r_en.REB);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }
    
    if(ins->t_buttons.b_zero != UNDEFINED){
        gpio_init(ins->t_buttons.b_zero);
        gpio_set_dir(ins->t_buttons.b_zero, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_zero);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_zero, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_zero);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_one != UNDEFINED){
        gpio_init(ins->t_buttons.b_one);
        gpio_set_dir(ins->t_buttons.b_one, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_one);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_one, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_one);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_two != UNDEFINED){
        gpio_init(ins->t_buttons.b_two);
        gpio_set_dir(ins->t_buttons.b_two, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_two);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_two, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_two);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_three != UNDEFINED){
        gpio_init(ins->t_buttons.b_three);
        gpio_set_dir(ins->t_buttons.b_three, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_three);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_three, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_three);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_four != UNDEFINED){
        gpio_init(ins->t_buttons.b_four);
        gpio_set_dir(ins->t_buttons.b_four, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_four);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_four, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_four);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_five != UNDEFINED){
        gpio_init(ins->t_buttons.b_five);
        gpio_set_dir(ins->t_buttons.b_five, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_five);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_five, 0x04, true, &main_interrupt_handler);
            printf("Button: %i Initialised.\n", ins->t_buttons.b_five);

        pin_count++;
    }else{
        printf("No Pin Initialised.\n");
    }

    return pin_count;

}

uint16_t button_incrementer(input_types *butts){

    int buffer;
    buffer = 0;
        


    if(butts->t_buttons.b_ze_status){
        if(butts->button_value <= 475){
            printf("Incrementing by 25.\n");
            buffer += 25;
            
        }else {
            butts->button_value = 500;
            printf("Max Value Range.\n");
        }
    }
    butts->t_buttons.b_ze_status = false;

    if(butts->t_buttons.b_on_status){
        if(butts->button_value > 0){
            printf("Decremnting by 25.\n");
            buffer -= 25;
        }else {
            butts->button_value = 0;
            printf("Minimum Range Value.\n");
        }
    }
    butts->t_buttons.b_on_status = false;

    
    return buffer;
}
#endif