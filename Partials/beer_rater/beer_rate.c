#include <stdio.h>
#include "lcd_16x2.h"
#include "peri_header.h"
#include "beer_list.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define lcd_backlight_toggle 14

int initial_run_count;


int main() {
    initial_run_count = 0;
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
  .rotary_button_clk = 2,
  .rotary_button_dt = 4,
  .rotary_button_button = 5,
  .rotary_current_value = 2,
  .rotary_maximum_value = 20,
  .rotary_minimum_value = 0,
  .analog_button_one = (int)NULL,
  .analog_button_two = (int)NULL,
  .button_one = 3,
  .button_two = (int)NULL,
  .button_three = (int)NULL
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
 button_interrupt_times interrupt_times = {
  .rotary_clk_max = 1500,
  .rotary_dt_max = 1500,
  .rotary_button_max = 1000,
  .analog_one_max = (int)NULL,
  .analog_two_max = (int)NULL,
  .button_one_max = 1000,
  .button_two_max = (int)NULL,
  .button_three_max = (int)NULL
 };

    stdio_init_all();

    sleep_ms(7000);
    adc_init();
    adc_pin_setup(&pico_adc);

    button_interrupt_init(&enabled_buttons);

    int display_cycle_count = 0;

    // Initialize the LCD using the configured pins
    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);

    gpio_init(lcd_backlight_toggle);
    gpio_set_dir(lcd_backlight_toggle, GPIO_OUT);

    // Your LCD control code here...


  
while(1) {

    adc_pin_call(&pico_adc);

    printf("\nVarious values.\n\n");
    
    printf("Photoresistor Value: %i.\n", pico_adc.adc0_mapped_value);
    printf("Rotary Encoder Value: %d.\n", enabled_buttons.rotary_current_value);
    printf("Last Status Rotary Value: %d.\n", last_status.basic_rotary_value);
    printf("Current CLK value: %d.\n", buttons_onoff.rotary_clk_status);
    printf("Current DT value: %d.\n", buttons_onoff.rotary_dt_status);
    printf("CLK Interrupt Time: %d.\n", last_interrupt.rotary_clk_last_interrupt);
    printf("DT Interrupt Time: %d.\n\n", last_interrupt.rotary_dt_last_interrupt);
    printf("Rotary button Status: %d.\n", enabled_buttons.rotary_button_button);
    printf("Button Status: %d.\n", enabled_buttons.button_one);
    printf("Current Menu Values.\n");
    printf("\tTop Menu: %d.\n", menu_values.top_menu_value);
    printf("\tSecond Menu: %d.\n", menu_values.second_menu_value);
    printf("\tThird Menu: %d.\n", menu_values.third_menu_value);
    printf("\tFourth Menu: %d.\n", menu_values.fourth_menu_value);
    printf("Menu Level: %d.\n\n", menu_values.menu_level);
    (pico_adc.adc0_mapped_value >= 175) ? gpio_put(lcd_backlight_toggle, 1) : gpio_put(lcd_backlight_toggle, 0);

    if(initial_run_count == 0){
     write_display(&lcd_pins, intro_top_line, intro_bottom_line, 0, 0);

    sleep_ms(5000);
    gpio_put(lcd_pins.rs_pin, 0);
        sleep_ms(5);
    lcd_clear(&lcd_pins);
        sleep_ms(20);
    lcd_home(&lcd_pins);
        sleep_ms(20);

    write_display(&lcd_pins, about_top_line, about_bottom_line, 0, 0);
    sleep_ms(2000);
    initial_run_count++;
    }
    
    top_menu(&lcd_pins, &menu_values);
    buttons_onoff.rotary_button_status = false;

    sleep_ms(1000);
    
}
}
