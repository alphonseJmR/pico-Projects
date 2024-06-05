#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
#include "pwm_gener.h"
#include "hardware/gpio.h"


// Breadboard Correct 1/27/2024 10:22pm
LCDPins lcd_pins = {

    .back_light = GPIO_EIGHT,
    .rs_pin = GPIO_NINE,
    .e_pin = GPIO_TEN,
    .data4_pin = GPIO_ELEVEN,
    .data5_pin = GPIO_TWELVE,
    .data6_pin = GPIO_THIRTEEN,
    .data7_pin = GPIO_FOURTEEN

};

pwm_gen_pins pwm_pins = {

    .pwm_one = GPIO_NINETEEN,
    .pwm_two = GPIO_TWENTY,
    .pwm_three = GPIO_TWENTY_ONE,
    .clk_inh = GPIO_TWO,
    .piso_clk = GPIO_FIVE,
    .piso_latch = GPIO_ONE,
    .pwm_serial_in_clkdiv = GPIO_FOUR,
    .pwm_serial_in_wrap = GPIO_THREE,
    .piso_pwm_clk = GPIO_FIFTEEN
};

adc_port_values pico_adc = {

.adc0_pin = GPIO_TWENTY_SIX,
.adc1_pin = GPIO_TWENTY_SEVEN,
.adc2_pin = GPIO_TWENTY_EIGHT,
.adc0_min_in_map_value = 0,
.adc0_max_in_map_value = 4095,
.adc0_min_out_map_value = 0,
.adc0_max_out_map_value = 1,
.adc1_min_in_map_value = 0,
.adc1_max_in_map_value = 4095,
.adc1_min_out_map_value = 0,
.adc1_max_out_map_value = 1,
.adc2_min_in_map_value = 0,
.adc2_max_in_map_value = 4095,
.adc2_min_out_map_value = 0,
.adc2_max_out_map_value = 1

};

button_types enabled_buttons = {
    .rotary_button_clk = UNDEFINED,
    .rotary_button_dt = UNDEFINED,
    .rotary_button_button = UNDEFINED,
    .analog_button_one = UNDEFINED,
    .analog_button_two = UNDEFINED,
    .button_one = GPIO_SIXTEEN,  // Button one will be the program reset button.
    .button_two = GPIO_SEVENTEEN,
    .button_three = UNDEFINED
};

interrupt_times_t callback_times = {

    .button_max = 1200

};

int main(){

    configure_output = 0x00;
    uint8_t piso_clkdiv;

    piso_clkdiv = 0x00;

    stdio_init_all();
      sleep_ms(7000);
    adc_init();
      sleep_ms(4000);

      printf("PISO Register Pin function should be active here.\n");
    piso_pin_init(&pwm_pins);
      sleep_ms(750);
      printf("Activate Piso clkdiv register input.\n");

      sleep_ms(1200);

      printf("ADC pin setup function should activate here.\n");
    adc_pin_setup(&pico_adc);
      sleep_ms(2000);

      printf("LCD init function should be active here.\n");
    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);
      sleep_ms(1000);

  //  dip_pin_initialize(&pwm_pins);
    program_by_print(&lcd_pins, by_line, my_tag);
      sleep_ms(1000);

    pwm_pins.configured_input_mesh = pwm_configured_inputs(&pwm_pins, configure_output, piso_clkdiv);
    printf("Value of config output: %i.\n", configure_output);
    printf("Value of Piso CLkdiv: %i.\n\n", piso_clkdiv);

      sleep_ms(700);

    pwm_initialize_three(&pwm_pins, configure_output, piso_clkdiv, div_One, div_Two);
    pwm_initial_view(&lcd_pins, configure_output, top_line, bottom_line, &pwm_pins);
    
    button_interrupt_init(&enabled_buttons);


while(1) {

    printf("Resetting Wrap Output.\n");
    printf("Configure clkdiv reset value.\n");
    configure_output = 0x00;
    piso_clkdiv = 0x00;

    pwm_pins.configured_input_mesh = piso_parity_check(&pwm_pins);
    pwm_pins.previous_input_mesh = pwm_pins.configured_input_mesh;

    printf("Previous Mesh Set As: 0x%02x\n", pwm_pins.previous_input_mesh);
    
    configure_output = ((pwm_pins.configured_input_mesh >> 8) & 0x00FF);
    piso_clkdiv = (pwm_pins.configured_input_mesh & 0x00FF);

    printf("Configured wrap (new) value: 0x%02x.\n", configure_output);
    printf("Configured clkdiv (new) value: 0x%02x.\n\n", piso_clkdiv);


  while(button_status.button_one_status != true) {

    adc_pin_call(&pico_adc);
    pwm_adjustment_view(&lcd_pins, &pico_adc, first_line, bottom_line, third_line, empty_line);
    pwm_dc_set(&pico_adc, configure_output);
    user_input_view(&lcd_pins, &pwm_pins, current_wrap, current_clkdiv);

    if(button_status.button_two_status == true) {
          piso_latchtivate(&pwm_pins);
          reset_to_previous_configure(&pwm_pins, &pico_adc, slice_one, slice_two, slice_three);

      //  Inverting backlight.
      printf("\n\nInverting Backlight Toggle.\n");
      gpio_put(lcd_pins.back_light, (lcd_pins.e_pin != 0) ? 1 : 0);

    }

    button_status.button_two_status = false;
    
  }

  button_status.button_one_status = false;
}
tight_loop_contents();
}