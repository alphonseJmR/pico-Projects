#include <stdio.h>
#include "lcd_16x2.h"
#include "peri_header.h"
#include "beer_list.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define lcd_backlight_toggle 14


void menu_selection(LCDPins *pins, int rotary_value) {
  switch(rotary_value) {
    case 0:
      write_display(pins, t_menu.top_menu_one, t_menu.top_menu_two, 0, 0);
      break;
    case 1:
      write_display(pins, t_menu.top_menu_one, t_menu.top_menu_three, 0, 0);
      break;
    case 2:
      write_display(pins, t_menu.top_menu_one, t_menu.top_menu_four, 0, 0);
      break;
    case 3:
      write_display(pins, t_menu.top_menu_one, t_menu.top_menu_five, 0, 0);
      break;
    case 4:
      write_display(pins, t_menu.top_menu_one, t_menu.top_menu_six, 0, 0);
      break;
    default:
      printf("No menu selection available.\n");
  }
}

void in_menu_selection(LCDPins *pins, int rotary_value){

}

int main() {
  // Define the LCD pins struct for your specific configuration
LCDPins lcd_pins = {
    .rs_pin = 16,      // Replace with the GPIO pin number for RS
    .e_pin = 17,       // Replace with the GPIO pin number for E
    .data4_pin = 8,   // Replace with the GPIO pin number for Data4
    .data5_pin = 9,   // Replace with the GPIO pin number for Data5
    .data6_pin = 10,   // Replace with the GPIO pin number for Data6
    .data7_pin = 11   // Replace with the GPIO pin number for Data7
};
button_types enabled_buttons = {
  .rotary_button_clk = 0,
  .rotary_button_dt = 1,
  .rotary_button_button = 2,
  .rotary_current_value = 0,
  .rotary_maximum_value = 20,
  .rotary_minimum_value = 0,
  .analog_button_one = NULL,
  .analog_button_two = NULL,
  .button_one = 3,
  .button_two = NULL,
  .button_three = NULL
};
adc_port_values pico_adc = {
  .adc0_pin = 26,
  .adc1_pin = 27,
  .adc2_pin = 28,
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
 button_interrupt_times interrupt_times = {
  .rotary_clk_max = 1500,
  .rotary_dt_max = 1500,
  .rotary_button_max = 1000,
  .analog_one_max = 1000,
  .analog_two_max = 1000,
  .button_one_max = NULL,
  .button_two_max = NULL,
  .button_three_max = NULL
 };


    stdio_init_all();
    adc_init();
    adc_pin_setup(&pico_adc);
    button_interrupt_init(&enabled_buttons);
    rotary_pin_setups(&enabled_buttons);

    int display_cycle_count = 0;

    sleep_ms(7000);
    // Initialize the LCD using the configured pins
    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);

    gpio_init(lcd_backlight_toggle);
    gpio_set_dir(lcd_backlight_toggle, GPIO_OUT);

    // Your LCD control code here...
    gpio_put(lcd_pins.rs_pin, 0);
        sleep_ms(5);
    lcd_clear(&lcd_pins);
        sleep_ms(20);
    lcd_home(&lcd_pins);
        sleep_ms(20);

    write_display(&lcd_pins, intro_top_line, intro_bottom_line, 0, 0);
    sleep_ms(5000);

while(1) {


    sleep_ms(1000);
    printf("\n\n\tMy Name.\n\n");
    
}
}
