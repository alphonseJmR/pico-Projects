#include <stdio.h>
#include "stdlib.h"
#include "Poly_Tools.h"
#include "nrf24_driver.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define mapped_between(a,b,c) (bool)( (a >= b) && (a < c))
#define turret_button_one turret_buttons.t_buttons.b_ze_status
#define turret_button_two turret_buttons.t_buttons.b_on_status
#define fire_angle ((turret_load.ready_load.load_vert_data.vert_upper << 8) | turret_load.ready_load.load_vert_data.vert_lower)
#define turret_turn (turret_load.ready_load.load_hori_data.hori_lower) 

//  Laser Pattern # choices
#define lp_one   0x01
#define lp_two   0x02
#define lp_three 0x03

volatile bool _menu;
volatile bool _menu_selected;
uint8_t menu_selection;
uint8_t prev_menu;


register_lcd _1602_pins = {
  .lcd_reg_data = GPIO_TWO,
  .lcd_reg_latch = GPIO_FOUR,
  .lcd_reg_enable = GPIO_THREE,
  .lcd_reg_clk = GPIO_FIVE,
  .lcd_rs = GPIO_SIX,
  .lcd_e = GPIO_SEVEN
};

uint16_t calc_servo_output_angle(uint8_t analog_input){
  uint16_t output_buffer = 0;
  
    if(mapped_between(analog_input, 0, 28)){
      output_buffer = turret_tilt_angle[0];
    }else if(mapped_between(analog_input, 28, 56)){
      output_buffer = turret_tilt_angle[1];
    }else if(mapped_between(analog_input, 56, 84)){
      output_buffer = turret_tilt_angle[2];
    }else if(mapped_between(analog_input, 84, 112)){
      output_buffer = turret_tilt_angle[3];
    }else if(mapped_between(analog_input, 112, 140)){
      output_buffer = turret_tilt_angle[4]; //  This is center position
    }else if(mapped_between(analog_input, 140, 168)){
      output_buffer = turret_tilt_angle[5];
    }else if(mapped_between(analog_input, 168, 196)){
      output_buffer = turret_tilt_angle[6];
    }else if(mapped_between(analog_input, 196, 224)){
      output_buffer = turret_tilt_angle[7];
    }else if(mapped_between(analog_input, 224, 255)){
      output_buffer = turret_tilt_angle[8];
    }else{
        printf("Analog Input Value out of valid range?\n");
    }

    return output_buffer;
}

uint8_t calc_turn_enable(uint8_t analog_input){

  uint8_t buffer_turn;

    printf("Mapped Analog Input Value: %i.\n", analog_input);

  if(mapped_between(analog_input, 0, 85)){
    buffer_turn = 2;
  }else if(mapped_between(analog_input, 85, 170)){
    buffer_turn = 0;
  }else if(mapped_between(analog_input, 170, 255)){
    buffer_turn = 1;
  }

  return buffer_turn;
}

void buffer_turret_data(adc_port_values *read, payload_data *turret, uint8_t pattern){

  uint16_t splitter = 0x0000;

  adc_pin_call(read);
  //  Buffer the vertical turret value.
  //  splitter = calc_servo_output_angle(read->adc1_mapped_value);
  turret->ready_load.load_vert_data.vert_upper = 0x00;
  turret->ready_load.load_vert_data.vert_lower = (read->adc1_mapped_value & 0x00FF);
    printf("Lower Vert Data: 0x%02x.\n", turret->ready_load.load_vert_data.vert_lower);
    splitter = 0x0000;

    splitter = (calc_turn_enable(read->adc2_mapped_value) & 0x00FF);
  turret->ready_load.load_hori_data.hori_upper = 0x0000;
  turret->ready_load.load_hori_data.hori_lower = (splitter & 0x00FF);
  
  turret->ready_load.raw_data.raw_upper = 0x0000;
  turret->ready_load.raw_data.raw_lower = (pattern & 0x00FF);
    if(turret_button_two){
      turret->ready_load.raw_data.raw_lower |= 0x08;
      turret_button_two = false;
    }else{
      printf("Fire weapons not active.\n");
    }
  
}

int main() {

  _menu = false;
  _menu_selected = false;
  menu_selection = 1;
  prev_menu = 1;

  stdio_init_all();

  printf("RC Controller Program Initialising.\n");
//  Sleep arbitrary amount of seconds to allow user to connect to serial monitor.
      sleep_ms(7000);
      sleep_ms(7000);

//  Initialise ADC PORT's.
      adc_init();
      adc_pin_setup(&turret_adc);
        sleep_ms(200);

//  Initialize pin setup for rotary encoder.
      interruption_input_initialisation(&turret_buttons);
        sleep_ms(200);

//  Initialize pin setup for cd74hc595e(s).
      setup_my_registers(&_1602_pins);
        sleep_ms(200);

//  Initialize LCD for control interface.
      set_lcd_initialisation(&_1602_pins);
        sleep_ms(200);
  write_intro_screen(&_1602_pins, &intro_screen);
      sleep_ms(2500);
  set_lcd_screen_clear(&_1602_pins);

  //  Initialization using inputtable function to setup
  nrf_client_t my_nrf;


    
while(1)
{
  //  This will hopefully only run once, and once nrf_set_complete is set true, it won't execute again.
  while(!controller_init.nrf_setup_complete){

    while(!controller_init.channel_complete){
      if(turret_button_one){
        controller_init.channel_complete = true;
        controller_init.channel_set = controller_init.setting_val;
        break;
      }else{
        sprintf(controller_init.top_line, "NRF Channel:");
        controller_init.setting_val = analog_menu(&turret_adc, 10);
        sprintf(controller_init.bottom_line, "   %i", controller_init.setting_val);
        set_screen_write(&_1602_pins, controller_init.top_line, controller_init.bottom_line);
      }
    }
    turret_button_one = false;
    controller_init.setting_val = 0;

    while(!controller_init.data_rate_complete){
      if(turret_button_one){
        controller_init.data_rate_complete = true;
        controller_init.data_set = controller_init.setting_val;
        break;
      }else{
        sprintf(controller_init.top_line, "NRF Datarate:");
        controller_init.setting_val = analog_menu(&turret_adc, 3);
        sprintf(controller_init.bottom_line, "   %i", controller_init.setting_val);
        set_screen_write(&_1602_pins, controller_init.top_line, controller_init.bottom_line);
      }
    }
    turret_button_one = false;
    controller_init.setting_val = 0;

    while(!controller_init.power_complete){
      if(turret_button_one){
        controller_init.power_complete = true;
        controller_init.power_set = controller_init.setting_val;
        break;
      }else{
        sprintf(controller_init.top_line, "NRF Power:");
        controller_init.setting_val = analog_menu(&turret_adc, 4);
        sprintf(controller_init.bottom_line, "    %i", controller_init.setting_val);
        set_screen_write(&_1602_pins, controller_init.top_line, controller_init.bottom_line);
      }
    }
    turret_button_one = false;
    controller_init.setting_val = 0;

    if(check_completes(&controller_init)){
      set_nrf_init_vals(&my_config, &controller_init);
      nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);
    controller_init.nrf_setup_complete = true;
    break;
    }else{
      printf("Error in NRF setup function.\n");
    }

}

  while(!_menu){
    if(turret_button_one){
      _menu = true;
        printf("\n\nBreaking To Menu\n\n");
          sleep_ms(1000);
      break;
    }else{
        printf("Normal NRF Operations\n");
    if(nrf_testing.nrf_initial_bypass){
      nrf_testing.nrf_initial_bypass = initial_bypass(&nrf_testing);
    }else {
      nrf_reset(&my_nrf, &my_pins, &my_config, 5000000, &nrf_testing);
    }

    while(!nrf_testing.reset_nrf){
      if(turret_button_one){
      _menu = true;
        printf("\n\nBreaking To Menu\n\n");
          sleep_ms(1000);
      break;
    }else{
        printf("\n");
      buffer_turret_data(&turret_adc, &turret_load, menu_selection);
      nrf_rc_update(&my_nrf, &nrf_testing, &turret_load);

        write_data(&_1602_pins, fire_angle, turret_turn);
    }
    }
    }
  //  turret_button_one = false;
  }


reset_button_status(&turret_buttons);

  while(_menu){
    printf("\nInside Menu Loop.\n");
      sleep_ms(1000);
    menu_selection = 1;
      while(!_menu_selected){
        if(turret_button_two){
          _menu_selected = true;
          break;
        }else{
          menu_selection = stick_menu(&turret_adc, prev_menu);
          prev_menu = menu_selection;
          printf("Current Menu Selection: %i.\n", menu_selection);
        switch(menu_selection){
          case 1:
            write_pattern_selection(&_1602_pins, &pattern_one);
          break;

          case 2:
            write_pattern_selection(&_1602_pins, &pattern_two);
          break;

          case 3:
            write_pattern_selection(&_1602_pins, &pattern_three);
          break;

          default:
            printf("Menu Selector Didn't Work.\n");
          break;
      }
     }
    }
      _menu = false;
    //  turret_button_two = false;
      break;
  }
  _menu_selected = false;
  reset_button_status(&turret_buttons);

}
tight_loop_contents();
}
