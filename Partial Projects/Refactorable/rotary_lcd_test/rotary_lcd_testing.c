#include <stdio.h>
#include "lcd_16x2.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

char top_string[16] = {' ', ' ', 'R', 'o', 't', 'a', 'r', 'y', ' ', 'V', 'a', 'l', 'u', 'e', ' ', ' '};

  // Define the LCD pins struct for your specific configuration
LCDPins lcd_pins = {
    .rs_pin = 8,        // Replace with the GPIO pin number for RS
    .e_pin = 9,         // Replace with the GPIO pin number for E
    .back_light = 14,   // Replace with the GPIO pin number for LCD backlight.
    .data4_pin = 10,    // Replace with the GPIO pin number for Data4
    .data5_pin = 11,    // Replace with the GPIO pin number for Data5
    .data6_pin = 12,    // Replace with the GPIO pin number for Data6
    .data7_pin = 13     // Replace with the GPIO pin number for Data7
};

button_types enabled_buttons = {
  .rotary_button_clk = rotary_clk,
  .rotary_button_dt = rotary_dt,
  .rotary_button_button = rotary_button,
  .analog_button_one = analog_one,
  .analog_button_two = analog_two,
  .button_one = button_one,
  .button_two = button_two,
  .button_three = button_three
};

battery_data_t bat_status = {
  .battery_high = 0xFF,
  .battery_medium = 0x96,
  .battery_low = 0x2D,
  .battery_critical = 0x0A,
  .battery_red = rgb_red,
  .battery_blue = rgb_blue,
  .battery_green = rgb_green
};

adc_port_values pico_adc = {
  .adc0_pin = 26,
  .adc1_pin = 27,
  .adc2_pin = (int)NULL,
  .adc0_raw_read = 0,
  .adc1_raw_read = 0,
  .adc2_raw_read = 0,
  .adc0_max_in_map_value = 4095,
  .adc0_min_in_map_value = 0,
  .adc0_min_out_map_value = 0,
  .adc0_max_out_map_value = 255,
  .adc1_max_in_map_value = 4095,
  .adc1_min_in_map_value = 0,
  .adc1_min_out_map_value = 0,
  .adc1_max_out_map_value = 255,
  .adc2_max_in_map_value = 4095,
  .adc2_min_in_map_value = 0,
  .adc2_min_out_map_value = 0,
  .adc2_max_out_map_value = 255
};

interrupt_times_t callback_times = {
  .clk_max = 1500,
  .dt_max = 1500,
  .button_max = 1000,
  .interruption_max = 1200
};

rotary_encoder_struct rotary = {
  .max_rotation_value = 100,
  .minimum_required_interrupt = 1500
};
    
int main() {

  stdio_init_all();

  bat_status.battery_indication_status = true;

  sleep_ms(7000);
  sleep_ms(7000);
    
  adc_init();
  adc_pin_setup(&pico_adc);
  battery_light_init(&bat_status);
  rotary_encoder_init(&enabled_buttons);
  button_interrupt_init(&enabled_buttons);
  lcd_init(&lcd_pins);
  lcd_4_bit_init(&lcd_pins);

  gpio_put(lcd_pins.rs_pin, 0);
    sleep_ms(5);
  lcd_clear(&lcd_pins);
    sleep_ms(20);
  lcd_home(&lcd_pins);
    sleep_ms(20);

while(1) {

  if(bat_status.battery_indication_status == true){
    battery_status_check(&bat_status, &pico_adc);
    printf("Battery Level: %i.\n", pico_adc.adc1_mapped_value);
  }

  adc_pin_call(&pico_adc);

  if(rotary.rotary_value >= 0){
      sprintf(bottom_string, "Current: %d", rotary.rotary_total);
  }else{
      printf("Rotary Value ERROR.\n");
    }

  (pico_adc.adc0_mapped_value <= 127) ? gpio_put(lcd_pins.back_light, 1) : gpio_put(lcd_pins.back_light, 0);

  write_display(&lcd_pins, top_string, bottom_string, 0, 0);

  sleep_ms(50);
  if(button_status.rotary_button_status == true){
  button_status.rotary_button_status = false;
  }
 
}
}
