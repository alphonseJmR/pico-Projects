#ifndef PWM_MY_INTERRUPT_H
#define PWM_MY_INTERRUPT_H

#include <stdio.h>
#include <stdint.h>
#include "hardware/gpio.h"

typedef enum function_ack {
    function_error = 0,
    function_ack = 1,
    function_cascaded = 2
} f_ack;

typedef struct pwm_manager_s {

    pwm_interrupt_functions_s interrupts;
    interrupt_args_s inputs;
    pwm_interrupt_pins_s input_pins;

}pwm_manager_s;

typedef struct pwm_interrupt_functions_s {

f_ack (*set_gpio_pwm_func)(interrupt_args_s interrupt_input);  // (void) (uint slice_num);
f_ack (*pwm_irq_enable)(interrupt_args_s interrupt_input);  // (void) (uint slice_num, bool enable)
f_ack (*pwm_irq_mask_enable)(interrupt_args_s interrupt_input);  //(void) (uint32_t slice_mask, bool enable)
f_ack (*pwm_irq_clear)(interrupt_args_s interrupt_input);  // (void) (uint slice_num)
uint32_t (*pwm_get_irq_mask_status)(void);
f_ack (*pwm_slice_gpio)(interrupt_args_s interrupt_input);  //(void) (uint slice_num)

}pwm_interrupt_functions_s;

typedef struct interrupt_args_s {
    
    uint slice_number;
    uint32_t mask;
    uint32_t slice_mask;
    bool enabled;

}interrupt_args_s;

typedef struct pwm_interrupt_pins_s {
    
    uint pin_one;
    uint pin_two;
    uint pin_three;
    uint pin_four;

}pwm_interrupt_pins_s;

f_ack pwm_irq_enable(interrupt_args_s *inputs){

    pwm_set_irq_enabled(inputs->slice_number, inputs->enabled);

    return function_ack;

}

f_ack pwm_irq_mask_enable(interrupt_args_s *inputs){

    pwm_set_irq_mask_enabled(inputs->slice_mask, inputs->enabled);

    return function_ack;

}

f_ack pwm_irq_clear(interrupt_args_s *inputs){

    pwm_clear_irq(inputs->slice_number);

    return function_ack;

}

uint32_t pwm_get_irq_mask_status(void) {

    uint32_t irq_buffer_mask;
    irq_buffer_mask = 0x00000000;

    irq_buffer_mask = pwm_get_irq_status_mask();

    return irq_buffer_mask;

}

f_ack set_gpio_pwm_func(pwm_interrupt_pins_s *pins){

  if(pins->pin_one != UNDEFINED){
    gpio_init(pins->pin_one);
    gpio_set_function(pins->pin_one, GPIO_FUNC_PWM);
  }

  if(pins->pin_two != UNDEFINED){
    gpio_init(pins->pin_two);
    gpio_set_function(pins->pin_two, GPIO_FUNC_PWM);
  }

  if(pins->pin_three != UNDEFINED){
    gpio_init(pins->pin_one);
    gpio_set_function(pins->pin_one, GPIO_FUNC_PWM);
  }

  if(pins->pin_three != UNDEFINED){
    gpio_init(pins->pin_two);
    gpio_set_function(pins->pin_two, GPIO_FUNC_PWM);
  }

    return function_ack;
}


#endif
