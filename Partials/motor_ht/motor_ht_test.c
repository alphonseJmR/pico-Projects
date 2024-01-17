#include <stdio.h>
#include "lcd_16x2.h"
#include "peri_header.h"
#include "dc_motors.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int test_run;

  // Define the LCD pins struct for your specific configuration
LCDPins lcd_pins = {
    .rs_pin = GPIO_FOUR,        // Replace with the GPIO pin number for RS
    .e_pin = GPIO_FIVE,         // Replace with the GPIO pin number for E
    .back_light = GPIO_THREE,   // Replace with the GPIO pin number for LCD backlight.
    .data4_pin = GPIO_SIX,    // Replace with the GPIO pin number for Data4
    .data5_pin = GPIO_SEVEN,    // Replace with the GPIO pin number for Data5
    .data6_pin = GPIO_EIGHT,    // Replace with the GPIO pin number for Data6
    .data7_pin = GPIO_NINE     // Replace with the GPIO pin number for Data7
};

button_types enabled_buttons = {
  .rotary_button_clk = GPIO_FIFTEEN,
  .rotary_button_dt = GPIO_SIXTEEN,
  .rotary_button_button = GPIO_SEVENTEEN,
  .analog_button_one = GPIO_FOURTEEN,
  .analog_button_two = UNDEFINED,
  .button_one = UNDEFINED,
  .button_two = UNDEFINED,
  .button_three = UNDEFINED
};

adc_port_values pico_adc = {
  .adc0_pin = GPIO_TWENTY_SIX,
  .adc1_pin = GPIO_TWENTY_SEVEN,
  .adc2_pin = GPIO_TWENTY_EIGHT,
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
    
pwm_settings_t pwm_header_config = {
  .pwm_pin_one = GPIO_TWENTY,
  .pwm_pin_two = UNDEFINED,
  .pwm_pin_three = UNDEFINED,
  .pwm_pin_four = UNDEFINED,
  .pwm_pin_five = UNDEFINED,
  .pwm_pin_six = UNDEFINED,
  .wrap_one_init = 5000,
  .duty_cycle_one_init = .5
};

servo_motor_config_t servo_configuration = {

  .servo_initial_position = 1500,
  .servo_inital_positioning = true,
  .mcu_clock = 125000000,
  .so_clkdiv = 64,
  .so_wrap = 0x9896,
  .servo_one_pin = GPIO_EIGHTEEN,
  .servo_two_pin = UNDEFINED,
  .servo_three_pin = UNDEFINED,
  .servo_four_pin = UNDEFINED

};

back_timed_t back_time = {
  .darkness_threshold = 2500000
};

s_motor_t stepping = {
  .step_a = GPIO_TEN,
  .step_b = GPIO_ELEVEN,
  .step_c = GPIO_TWELVE,
  .step_d = GPIO_THIRTEEN,
  .max_CW_revolutions = 50,
  .max_CCW_revolutions = 50
};

int main() {

  test_run = 0;

  stdio_init_all();

  sleep_ms(7000);
  sleep_ms(7000);
    
  adc_init();
  adc_pin_setup(&pico_adc);

  rotary_encoder_init(&enabled_buttons);
  button_interrupt_init(&enabled_buttons);
  stepper_init(&stepping);

  servo_initialization(&servo_configuration);

  lcd_init(&lcd_pins);
  lcd_4_bit_init(&lcd_pins);

  initial_pwm_pin_init(&pwm_header_config);
  
  gpio_put(lcd_pins.rs_pin, 0);
    sleep_ms(5);
  lcd_clear(&lcd_pins);
    sleep_ms(20);
  lcd_home(&lcd_pins);
    sleep_ms(20);

while(1) {


  adc_pin_call(&pico_adc);
  
  drive_motor_wrap_set(&pwm_header_config, rotary.rotary_total);
  motor_flag_status(&pwm_header_config, &motor_flags, &pico_adc);
  motor_drive_function(&pwm_header_config, &pico_adc);
  motor_data_display_buffer(&pwm_header_config);
 
  if(rotary.rotary_value <= 101){
    sprintf(top_string, "R Encoder Value");
    sprintf(bottom_string, "Current: %i", (rotary.rotary_total * 5));
    write_display(&lcd_pins, top_string, bottom_string, 0, 0);
  }else{
    sprintf(top_string, "Maximum Value.");
    }

  (pico_adc.adc0_mapped_value <= 127) ? gpio_put(lcd_pins.back_light, 0) : photoresistor_status_toggle(&back_time, &pico_adc, &lcd_pins);

  sleep_ms(100);
  write_display(&lcd_pins, pwm_header_config.wrap_value_one, pwm_header_config.duty_cycle_one, 0, 0);

  if(button_status.rotary_button_status == true){

    stepping.step_value = (rotary.rotary_total * 5);
    printf("Stepping: %i.\n", (stepping.step_value));
    stepped(&stepping);
    button_status.rotary_button_status = false;

  }

  if(test_run >= 5){
    pwm_set_motor_config(&motor_flags, &pwm_header_config);
      printf("Set Servo Position?\n");
    set_servo_position(GPIO_EIGHTEEN, &pico_adc);
  }

  test_run++;

  if(test_run > 100){
    test_run = 0;
  }
  printf("\n\n\t\tTest Run: %i.\n\n", test_run);
}
// tight_loop_contents();
}
