#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
#include "peri_header.h"
#include "nrf24_driver.h"
#include "rc_nrf_configs.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int rgb_cycle;


rf_lcd_array_s rf_arr = {

  .rf_function = {' ', 'R', 'F', ' ', 'S', 'w', 'i', 't', 'c', 'h', 'i', 'n', 'g', ' ', ' '},  //  Empty [16] array for const use.
  .rf_transmitting = {'T', 'r', 'a', 'n', 's', 'm', 'i', 't', 't', 'i', 'n', 'g', '.', '.', '.'},
  .error_char_t =  {'C', 'h', 'a', 'n', 'n', 'a', 'l', ' ', 'E', 'r', 'r', 'o', 'r'},
  .error_char_b = {'D', 'a', 't', 'a', ' ', 'E', 'r', 'r', 'o', 'r'}
  
};

register_pins reg_pins = {

  .register_one_data = GPIO_SEVEN,
  .register_one_latch = GPIO_NINE,
  .register_one_enable = GPIO_EIGHT,
  .register_two_data = UNDEFINED,
  .register_two_latch = UNDEFINED,
  .register_two_enable = UNDEFINED,
  .register_clk_line = GPIO_THIRTEEN

};

button_types enabled_buttons = {
  .rotary_button_clk = GPIO_SIXTEEN,
  .rotary_button_dt = GPIO_FIFTEEN,
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
  .adc1_max_out_map_value = 1024,

  .adc2_max_in_map_value = 4095,
  .adc2_min_in_map_value = 0,
  .adc2_min_out_map_value = 0,
  .adc2_max_out_map_value = 512
};

interrupt_times_t callback_times = {
  .clk_max = 1500,
  .dt_max = 1500,
  .button_max = 1000,
  .interruption_max = 1500
};

rotary_encoder_struct rotary = {

  .max_rotation_value = 500,
  .minimum_required_interrupt = 2000

};

pin_manager_t my_pins = { 

    .sck = GPIO_TWO,
    .mosi = GPIO_THREE, 
    .miso = GPIO_FOUR, 
    .csn = GPIO_FIVE, 
    .ce = GPIO_SIX 

  };

nrf_manager_t my_config = {
    // RF Channel 
    .channel = 120,

    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,

    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,

    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_1MBPS,

    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_6DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_750US 
  };

battery_data_t bat_status = {

  .battery_high = 0xFF,
  .battery_medium = 0xB9,
  .battery_low = 0x55,
  .battery_critical = 0x1E,
  .battery_red = 0x20,
  .battery_blue = 0x40,
  .battery_green = 0x80,
  .battery_level = {'B', 'a', 't', 't', 'e', 'r', 'y', ' ', 'L', 'e', 'v', 'e', 'l'}

};

int main() {

  stdio_init_all();

  sleep_ms(7000);
  sleep_ms(7000);
    
  adc_init();
  adc_pin_setup(&pico_adc);
//  Initialize pin setup for rotary encoder.
  rotary_encoder_init(&enabled_buttons);
  button_interrupt_init(&enabled_buttons);

//  Initialize pin setup for cd74hc595e(s).
  shift_register_pin_init(&reg_pins);

//  Initialize LCD for control interface.
  lcd_initialize(&reg_pins);

  // SPI baudrate
  uint32_t my_baudrate = 5000000;

  nrf_client_t my_nrf;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise(NULL)) 
  my_nrf.initialise(&my_config);

  // set to Standby-I Mode
  my_nrf.standby_mode();

  // result of packet transmission
  fn_status_t success = 0;

  uint64_t time_sent = 0; // time packet was sent
  uint64_t time_reply = 0; // response time after packet sent

  initialize_print_debug(&my_config, &my_pins, my_baudrate);

while(1) {

  printf("\n");
  adc_pin_call(&pico_adc);
  u_vars.register_value_one = 0x07;

  battery_status_to_shift(&u_vars, &pico_adc, &bat_status, 1);
  cycle_handler(&u_vars, &enabled_buttons, rgb_cycle);
  
  payload_buffer(&load_o,&pico_adc, &rotary, &u_vars);

  //  Begin Transmitting RF data.

  byte_shifting(com_char_e, 0x64, &reg_pins);


  for(int zv = 0; zv < 1; zv++)
  {
    
      sprintf(rf_arr.rf_channel, "Pipeline Zero.");
      printf("Pipeline 0.\n");

    // send to receiver's DATA_PIPE_0 address
      my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});
      printf("Step 1.\n");
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
      printf("Step 2.\n");
    // send packet to receiver's DATA_PIPE_0 address
      printf("\n Sending: 0b%04x.\n", load_o.payload_zero);
      success = my_nrf.send_packet(&load_o.payload_zero, sizeof(load_o.payload_zero));
      printf("Step 3.\n");
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
      printf("Step 4.\n");
      sprintf(rf_arr.rf_data, "Attempted: %u", load_o.payload_zero);
      printf("Step 5.\n");
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, load_o.payload_zero);
    lcd_screen_write(&reg_pins, com_char_e, rf_arr.rf_channel, rf_arr.rf_data);

    } 
    else
    {

      printf("\nPacket not sent:- Receiver not available.\n");
      lcd_screen_write(&reg_pins, com_char_e, rf_arr.error_char_t, rf_arr.rf_data);

    }


    

    sprintf(rf_arr.rf_channel, "Pipeline One.");
      printf("Pipeline 1.\n");
    // send to receiver's DATA_PIPE_1 address
      my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_1 address
      printf("\n Sending: 0b%04x.\n", load_o.payload_one_t.vertical_buffer);
      printf("\n Sending: 0b%04x.\n", load_o.payload_one_t.horizontal_buffer);
      success = my_nrf.send_packet(&load_o.payload_one_t, sizeof(load_o.payload_one_t));\
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
      sprintf(rf_arr.rf_data, "Vertical: %u", load_o.payload_one_t.vertical_buffer);
      sprintf(rf_arr.rf_data_two, "Horizontal: %u", load_o.payload_one_t.horizontal_buffer);
    if (success)
    {
        printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x & 0b%04x.\n",time_reply - time_sent, load_o.payload_one_t.vertical_buffer, load_o.payload_one_t.horizontal_buffer);
          lcd_screen_write(&reg_pins, com_char_e, rf_arr.rf_channel, rf_arr.rf_data);

    }
    else
    {

      printf("\nPacket not sent:- Receiver not available.\n");
        lcd_screen_write(&reg_pins, com_char_e, rf_arr.error_char_t, rf_arr.rf_data);

    }
          
    sprintf(rf_arr.rf_channel, "Pipeline Two.");
      printf("Pipeline 2.\n");
    // send to receiver's DATA_PIPE_2 address
      my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_2 address
      printf("\n Sending: 0b%04x.\n", load_o.payload_two);
      success = my_nrf.send_packet(&load_o.payload_two, sizeof(load_o.payload_two));
    sprintf(rf_arr.rf_data, "Attempted: %u", load_o.payload_two);
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, load_o.payload_two);
        lcd_screen_write(&reg_pins, com_char_e, rf_arr.rf_channel, rf_arr.rf_data);

    }
    else
    {

      printf("\nPacket not sent:- Receiver not available.\n");
        lcd_screen_write(&reg_pins, com_char_e, rf_arr.error_char_t, rf_arr.rf_data);
    }

    sprintf(rf_arr.rf_channel, "Pipeline Three.");
      printf("Pipeline Three.\n");
    // send to receiver's DATA_PIPE_3 address
      my_nrf.tx_destination((uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_3 address
      printf("\n Sending: 0b%08x.\n", load_o.payload_three);
      success = my_nrf.send_packet(&load_o.payload_three, sizeof(load_o.payload_three));
    sprintf(rf_arr.rf_data, "Attempted: %u", load_o.payload_three);
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, load_o.payload_three);
        lcd_screen_write(&reg_pins, com_char_e, rf_arr.rf_channel, rf_arr.rf_data);

    }
    else
    {

      printf("\nPacket not sent:- Receiver not available.\n");
        lcd_screen_write(&reg_pins, com_char_e, rf_arr.error_char_t, rf_arr.rf_data);
    }

    }    
  
    sleep_ms(950);
    printf("Next Iteration?\n");
}
tight_loop_contents();
}
