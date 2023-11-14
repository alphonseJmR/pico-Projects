#include <stdio.h>
#include <stdint.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

//  Pins for motor controls are set as pin 0 and 1
#define motor_forward 14
#define motor_reverse 15
#define pwm_constant 13


void pwm_pin_init(uint pin){
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
}

int main() {

  stdio_init_all();

  uint slice_num[1];
  uint chan[1];

  pwm_pin_init(14);
  slice_num[0] = pwm_gpio_to_slice_num(motor_forward);
  chan[0] = pwm_gpio_to_channel(motor_forward);
  pwm_set_clkdiv(slice_num[0], 25);
  pwm_set_wrap(slice_num[0], 15000);
  pwm_set_chan_level(slice_num[0], chan[0], 0);
  pwm_set_enabled(slice_num[0], true);

  pwm_pin_init(15);
  slice_num[1] = pwm_gpio_to_slice_num(motor_reverse);
  chan[1] = pwm_gpio_to_channel(motor_reverse);
  pwm_set_clkdiv(slice_num[1], 10);
  pwm_set_wrap(slice_num[1], 15000);
  pwm_set_chan_level(slice_num[1], chan[1], 0);
  pwm_set_enabled(slice_num[1], true);

   pwm_pin_init(13);
  slice_num[2] = pwm_gpio_to_slice_num(pwm_constant);
  chan[2] = pwm_gpio_to_channel(pwm_constant);
  pwm_set_clkdiv(slice_num[2], 20);
  pwm_set_wrap(slice_num[2], 15000);
  pwm_set_chan_level(slice_num[2], chan[2], 0);
  pwm_set_enabled(slice_num[2], true);

 while(1){
  
  pwm_set_chan_level(slice_num[2], chan[2], 7500);
  sleep_ms(5000);
  pwm_set_chan_level(slice_num[2], chan[2], 0);
  sleep_ms(2000);
  pwm_set_chan_level(slice_num[1], chan[1], 12500);
  sleep_ms(5000);
  pwm_set_chan_level(slice_num[1], chan[1], 0);
  sleep_ms(2000);
  pwm_set_chan_level(slice_num[0], chan[0], 5000);
  sleep_ms(5000);
  pwm_set_chan_level(slice_num[0], chan[0], 0);

  sleep_ms(2000);
 }
}