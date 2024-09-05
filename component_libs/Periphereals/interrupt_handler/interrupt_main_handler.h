#ifndef INTERRUPT_MAIN_HANDLER_S_H
#define INTERRUPT_MAIN_HANDLER_S_H

#include <stdio.h>
#include <stdint.h>
#include "../../hi_lvl_resources/project_struct_s.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "Periphereals/resources/pico_pin_enum.h"

#define turret_button_irq_handler
// #define general_button_irq_handler


void set_readied_values(payload_data *load, nrf_status *cons, pre_calcs *pres, pay_size *load_s);
void interruption_input_initialisation(input_types *ins);
//uint8_t interruption_input_initialisation(input_types *ins);
void button_interrupt_handler(input_types *butts, uint gpio, uint32_t events);
//void button_interrupt_handler(input_types *butts, uint gpio);
bool buttons_timer(input_types *my_ren);
bool callback_timer(input_types *my_ren);
void rotary_encoder_handler(input_types *ren, uint gpio, uint32_t events);
void main_interrupt_handler(uint gpio, uint32_t events);
uint16_t button_incrementer(input_types *butts);


repeating_timer_t my_timer;
repeating_timer_t motor_timer;

bool timed_result;


#ifdef turret_button_irq_handler

void main_interrupt_handler(uint gpio, uint32_t events){

    printf("\n////\tMain Interrupt Handler\t////\n");
        gpio_acknowledge_irq(gpio, events);
    button_interrupt_handler(&turret_buttons, gpio, events);
  
}


void button_interrupt_handler(input_types *butts, uint gpio, uint32_t events){

    if(gpio == butts->t_buttons.b_zero){
        
        printf("Button One Pressed.\n");
          butts->t_buttons.b_ze_status = true;
        printf("Button One Currently: %i.\n", (butts->t_buttons.b_ze_status) ? 1 : 0);

    }else if(gpio == butts->t_buttons.b_one){
        printf("Button Two Pressed.\n");
          butts->t_buttons.b_on_status = true;
        printf("Button Two Currently: %i.\n", (butts->t_buttons.b_on_status) ? 1 : 0);
    }
}

void interruption_input_initialisation(input_types *ins){

    if(ins->t_buttons.b_zero != UNDEFINED){
        gpio_init(ins->t_buttons.b_zero);
        gpio_set_dir(ins->t_buttons.b_zero, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_zero);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_zero, 0x04, true, &main_interrupt_handler);
            ins->t_buttons.b_ze_status = false;
            printf("Button: %i Initialised.\n", ins->t_buttons.b_zero);

    }else{
        printf("No Pin Initialised.\n");
    }

    if(ins->t_buttons.b_one != UNDEFINED){
        gpio_init(ins->t_buttons.b_one);
        gpio_set_dir(ins->t_buttons.b_one, GPIO_IN);
            gpio_pull_down(ins->t_buttons.b_one);
        gpio_set_irq_enabled_with_callback(ins->t_buttons.b_one, 0x04, true, &main_interrupt_handler);
            ins->t_buttons.b_on_status = false;
            printf("Button: %i Initialised.\n", ins->t_buttons.b_one);
    }else{
        printf("No Pin Initialised.\n");
    }

}

void reset_button_status(input_types *in){

    printf("Reset Button Status's to false.\n\n");

    in->t_buttons.b_ze_status = false;
    in->t_buttons.b_on_status = false;
}

#endif

#ifdef general_button_irq_handler

void main_interrupt_handler(uint gpio, uint32_t events){

    printf("\n////\tMain Interrupt Handler\t////\n");
        gpio_acknowledge_irq(gpio, events);
  if(my_types.r_en.CLK == UNDEFINED || my_types.r_en.DT == UNDEFINED){

    if(gpio == my_types.r_en.CLK || gpio == my_types.r_en.DT){

        rotary_encoder_handler(&my_types, gpio, events);

    }
  }else {
    button_interrupt_handler(&my_types, gpio);
  }
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


#endif


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

pay_size payload_size_check(pay_size *pays, payload_data *load){

  if(valid_data(load->ready_load.load_duty_data.upper_duty, load->ready_load.load_duty_data.lower_duty)){
    pays->zero_size = ( sizeof(load->ready_load.load_duty_data) );
  }else {
    pays->zero_size = 0;
  }

  if(valid_data(load->ready_load.load_vert_data.vert_upper, load->ready_load.load_vert_data.vert_lower)){
    pays->one_size  = ( sizeof(load->ready_load.load_vert_data) );
  }else {
    pays->one_size = 0;
  }

  if(valid_data(load->ready_load.load_hori_data.hori_upper, load->ready_load.load_hori_data.hori_lower)){
    pays->two_size  = ( sizeof(load->ready_load.load_hori_data) );
  }else {
    pays->two_size = 0;
  }
  
  if(valid_data(load->ready_load.raw_data.raw_upper, load->ready_load.raw_data.raw_lower)){
    pays->three_size = ( sizeof(load->ready_load.raw_data) );
  }

return *pays;
}

pre_calcs reconstitue_recieved(pre_calcs *pres, payload_data *loads, pay_size *iff){

  if(iff->zero_size > 0){

    pres->wrap_val_buffered = ( (loads->ready_load.load_duty_data.upper_duty << 8) | loads->ready_load.load_duty_data.lower_duty );

  }else {
    printf("No Data One Load Zero.\n");
    pres->wrap_val_buffered = 0;
  }

  if(iff->one_size > 0){

    pres->duty_cycle_buffed = ( (loads->ready_load.load_vert_data.vert_upper << 8) | loads->ready_load.load_vert_data.vert_lower );

  }else {
    printf("No Data One Load One.\n");
    pres->duty_cycle_buffed = 0;
  }

  if(iff->two_size > 0){

    pres->servo_val_buffed = ( (loads->ready_load.load_hori_data.hori_upper << 8) | loads->ready_load.load_hori_data.hori_lower );

  }else {
    printf("No Data One Load Two.\n");
    pres->servo_val_buffed = 1460;
  }

  if(iff->three_size > 0){

    pres->raw_adc_rec = ( (loads->ready_load.raw_data.raw_upper << 8) | loads->ready_load.raw_data.raw_lower);

  }
  else {
    printf("No Data One Load Three.\n");
    pres->raw_adc_rec = 0x01F7;
  }

  printf("Reconed Wrap: %i.\n", pres->wrap_val_buffered);
  printf("Reconed Duty: %i.\n", pres->duty_cycle_buffed);
  printf("Reconed Servo: %i.\n", pres->servo_val_buffed);
  printf("Reconed Raw ADC: %i.\n", pres->raw_adc_rec);

  return *pres;

}

void set_readied_values(payload_data *load, nrf_status *cons, pre_calcs *pres, pay_size *load_s){

    *load_s = payload_size_check(load_s, load);

        printf("\n\t//// ////\tNRF Received Input Sizes\t//// ////\n");
        printf("//\tLoad Zero: %i.\n", load_s->zero_size);
        printf("//\tLoad One: %i.\n", load_s->one_size);
        printf("//\tLoad Two: %i.\n", load_s->two_size);
        printf("//\tLoad Three: %i.\n\n", load_s->three_size);

    *pres = reconstitue_recieved(pres, load, load_s);
    
    printf("\nVertical Data: 0x%04x.", pres->duty_cycle_buffed);
    printf("\nHorizontal Data: 0x%04x.\n", pres->servo_val_buffed);

}

#endif