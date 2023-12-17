#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
#include "peri_header.h"
#include "nrf24_driver.h"
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
  .adc0_max_out_map_value = 1024,
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


int main() {

  test_run = 0;

  stdio_init_all();

  sleep_ms(7000);
  sleep_ms(7000);
    
  adc_init();
  adc_pin_setup(&pico_adc);

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


  adc_pin_call(&pico_adc);
  
    rgb_color_cycle(analog.analog_button_status, analog.reverse_status, rgb.rgb_cycle_value, analog.reverse_cycle);
    rgb_light_control_buffer(&rgb_lights_register_buffer, rotary.rotary_button_status, analog.reverse_status); 
    
    payload_buffer(rgb_lights_register_buffer, analog.vertical_raw_conversion, analog.horizontal_raw_conversion, rotary.rotary_total_value, peri_config_toggle);
        for(int zv = 0; zv < 4; zv++){

        switch(zv){

        case 0:
            printf("Transmitter Switch Case 0.\n");

    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_0 address
    printf("\n Sending: 0b%08x.\n", payload_zero);
    success = my_nrf.send_packet(&payload_zero, sizeof(payload_zero));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, payload_zero);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        case 1:
            printf("Transmitter Switch Case 1.\n");

    // send to receiver's DATA_PIPE_1 address
    my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_1 address
    printf("\n Sending: 0b%08x.\n", payload_one.vertical_buffer);
    printf("\n Sending: 0b%08x.\n", payload_one.horizontal_buffer);
    success = my_nrf.send_packet(&payload_one, sizeof(payload_one));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
        printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x & 0b%04x.\n",time_reply - time_sent, payload_one.vertical_buffer, payload_one.horizontal_buffer);
      
    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);
                
        break;
        case 2:
                printf("Transmitter Switch Case 2.\n");

    // send to receiver's DATA_PIPE_2 address
    my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_2 address
    printf("\n Sending: 0b%04x.\n", peri_config_toggle);
    success = my_nrf.send_packet(&payload_two, sizeof(payload_two));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, peri_config_toggle);

    }  else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        case 3:
        printf("Transmitter Switch Case 3.\n");

    // send to receiver's DATA_PIPE_3 address
    my_nrf.tx_destination((uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_3 address
    printf("\n Sending: 0b%08x.\n", payload_three);
    success = my_nrf.send_packet(&payload_three, sizeof(payload_three));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, payload_three);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        default:
            printf("Transmitter Switch Default.\n");
        break;
        }
    }
    sleep_ms(500);

  
  if(rotary.rotary_value <= 101){
    sprintf(top_string, "R Encoder Value");
    sprintf(bottom_string, "Current: %i", (rotary.rotary_total * 5));
    write_display(&lcd_pins, top_string, bottom_string, 0, 0);
  }else{
    sprintf(top_string, "Maximum Value.");
    }

  (pico_adc.adc0_mapped_value <= 127) ? gpio_put(lcd_pins.back_light, 0) : photoresistor_status_toggle(&back_time, &pico_adc, &lcd_pins);

  sleep_ms(100);

  if(button_status.rotary_button_status == true){
    button_status.rotary_button_status = false;
  }
}
// tight_loop_contents();
}
