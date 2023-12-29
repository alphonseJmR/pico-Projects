#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
#include "peri_header.h"
#include "dc_motors.h"
#include "nrf24_driver.h"
#include "rc_nrf_configs.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int rgb_cycle;

char rf_channel[16];
char rf_data[16];
char rf_function[16];
char rf_transmitting[16];

  // Define the LCD pins struct for your specific configuration
LCDPins lcd_pins = {

    .rs_pin = GPIO_ELEVEN,        // Replace with the GPIO pin number for RS
    .e_pin = GPIO_TWELVE,         // Replace with the GPIO pin number for E
    .back_light = GPIO_THIRTEEN,   // Replace with the GPIO pin number for LCD backlight.
    .data4_pin = GPIO_SEVEN,    // Replace with the GPIO pin number for Data4
    .data5_pin = GPIO_EIGHT,    // Replace with the GPIO pin number for Data5
    .data6_pin = GPIO_NINE,    // Replace with the GPIO pin number for Data6
    .data7_pin = GPIO_TEN     // Replace with the GPIO pin number for Data7

};

register_pins reg_pins = {

  .register_one_data = GPIO_TWENTY,
  .register_one_latch = GPIO_ONE,
  .register_one_enable = GPIO_EIGHTEEN,
  .register_two_data = UNDEFINED,
  .register_two_latch = UNDEFINED,
  .register_two_enable = UNDEFINED,
  .register_clk_line = GPIO_NINETEEN

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
  .interruption_max = 700
};

rotary_encoder_struct rotary = {

  .max_rotation_value = 100,
  .minimum_required_interrupt = 1500

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
    .power = RF_PWR_NEG_12DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US 
  };

motor_flags_status cfl= {

  .vdti = true,
  .vdtn = false,
  .vdtp = false,
  .hdti = true,
  .hdtl = false,
  .hdtr = false

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
  rotary_encoder_init(&enabled_buttons);
  button_interrupt_init(&enabled_buttons);
  shift_register_pin_init(&reg_pins);
  lcd_init(&lcd_pins);
  lcd_4_bit_init(&lcd_pins);
  
    gpio_put(lcd_pins.rs_pin, 0);
    lcd_clear(&lcd_pins);
    lcd_home(&lcd_pins);

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


while(1) {

  battery_status_to_shift(&u_vars, &pico_adc, &bat_status, 1);
   write_display(&lcd_pins, bat_status.battery_level, bat_status.battery_printer);

  adc_pin_call(&pico_adc);
  cycle_handler(&u_vars, &enabled_buttons, rgb_cycle);
  
  payload_buffer(&load_o,&pico_adc, &rotary, &u_vars);
  serial_register_output(&reg_pins, &u_vars, 1, NULL);

  sprintf(rf_function, "RF Switching");
  sprintf(rf_transmitting, "Transmitting Now");
   write_display(&lcd_pins, rf_function, rf_transmitting);

  for(int zv = 0; zv < 4; zv++){

    switch(zv){

      case 0:
      sprintf(rf_channel, "Pipeline Zero.");
            printf("Transmitter Switch Case 0.\n");

    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_0 address
    printf("\n Sending: 0b%08x.\n", load_o.payload_zero);
    success = my_nrf.send_packet(&load_o.payload_zero, sizeof(load_o.payload_zero));
    sprintf(rf_data, "Data Sent: %u.", load_o.payload_zero);

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, load_o.payload_zero);
      write_display(&lcd_pins, rf_channel, rf_data);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");

     write_display(&lcd_pins, error_char_t, rf_data);
    }
        sleep_ms(300);

        break;
        case 1:
        sprintf(rf_channel, "Pipeline One.");
            printf("Transmitter Switch Case 1.\n");

    // send to receiver's DATA_PIPE_1 address
    my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_1 address
    printf("\n Sending: 0b%08x.\n", load_o.payload_one_t.vertical_buffer);
    printf("\n Sending: 0b%08x.\n", load_o.payload_one_t.horizontal_buffer);
    success = my_nrf.send_packet(&load_o.payload_one_t, sizeof(load_o.payload_one_t));

    sprintf(rf_data, "VD: %u HD: %u", load_o.payload_one_t.vertical_buffer, load_o.payload_one_t.horizontal_buffer);

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
        printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x & 0b%04x.\n",time_reply - time_sent, load_o.payload_one_t.vertical_buffer, load_o.payload_one_t.horizontal_buffer);
        write_display(&lcd_pins, rf_channel, rf_data);
    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
      write_display(&lcd_pins, error_char_t, rf_data);
    }
        sleep_ms(300);
                
        break;
        case 2:

        sprintf(rf_channel, "Pipeline Two.");
                printf("Transmitter Switch Case 2.\n");

    // send to receiver's DATA_PIPE_2 address
    my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_2 address
    printf("\n Sending: 0b%04x.\n", load_o.payload_two);
    success = my_nrf.send_packet(&load_o.payload_two, sizeof(load_o.payload_two));
    sprintf(rf_data, "Payload: %u", load_o.payload_two);
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, load_o.payload_two);
      write_display(&lcd_pins, rf_channel, rf_data);

    }  else {

      printf("\nPacket not sent:- Receiver not available.\n");
      write_display(&lcd_pins, error_char_t, rf_data);
    }
        sleep_ms(300);

        break;
        case 3:

        sprintf(rf_channel, "Pipeline Three.");
        printf("Transmitter Switch Case 3.\n");

    // send to receiver's DATA_PIPE_3 address
    my_nrf.tx_destination((uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_3 address
    printf("\n Sending: 0b%08x.\n", load_o.payload_three);
    success = my_nrf.send_packet(&load_o.payload_three, sizeof(load_o.payload_three));
     sprintf(rf_data, "Payload: %u", load_o.payload_three);

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, load_o.payload_three);
      write_display(&lcd_pins, rf_channel, rf_data);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
      write_display(&lcd_pins, error_char_t, rf_data);
    }
        sleep_ms(300);

        break;
        default:
            printf("Transmitter Switch Default.\n");
            write_display(&lcd_pins, error_char_t, error_char_b);
        break;
        }
    }
   // sleep_ms(500);

}
tight_loop_contents();
}
