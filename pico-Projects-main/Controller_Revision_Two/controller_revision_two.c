#include <stdio.h>
#include "stdlib.h"
#include "Poly_Tools.h"
#include "nrf24_driver.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define ebit uint8_t
uint8_t char_val;

uint8_t function_status_checker;
uint16_t rot_buf;
int rgb_cycle;
ebit initialise_sequence;

lcd_lines my_lines = {
  .line_one = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
  .line_two = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
  .line_three = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
  .line_four = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', }

};

register_pins reg_pins = {

  .register_one_data = GPIO_TWELVE,
  .register_one_latch = GPIO_SEVENTEEN,
  .register_one_enable = GPIO_SIXTEEN,
  .register_two_data = GPIO_ZERO,
  .register_clk_line = GPIO_ONE

};

button_types enabled_buttons = {

  .rotary_button_clk = GPIO_THIRTEEN,
  .rotary_button_dt = GPIO_FIFTEEN,
  .rotary_button_button = GPIO_EIGHTEEN,
  .analog_button_one = GPIO_FOURTEEN,
  .analog_button_two = UNDEFINED,
  .button_one = UNDEFINED,
  .button_two = UNDEFINED,
  .button_three = UNDEFINED

};

payload_data my_loads = { 

  .ready_load = {
    .load_zero = 0,
    .load_one_dual = {
    .vertical_buffer = 0,
    .horizontal_buffer = 0
  },
    .load_two = 0,
    .load_three = 0
  },

  .payload_buffer = {
    .load_zero_buffer = 0,
    .load_one_buffer = {
      .vertical_analog_buffer = 0,
      .horizontal_analog_buffer = 0
    },
    .load_two_buffer = 0,
    .load_three_buffer = 0
  }

};

adc_port_values my_adc = {
    .adc0_pin = GPIO_TWENTY_SIX,
    .adc1_pin = GPIO_TWENTY_SEVEN,
    .adc2_pin = GPIO_TWENTY_EIGHT,

    .adc0_min_in_map_value = 0,       //  ADC0 Port will be the battery voltage detection.
    .adc0_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc1_min_in_map_value = 0,       //  ADC1 Port will route the vertical channel of the joystick into a mapped value we can use.
    .adc1_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc2_min_in_map_value = 0,       //  ADC2 Port will route the horizontal channel of the joystick into a mapped value we can use.
    .adc2_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc0_min_out_map_value = 0,      //  Usually a minimum of zero, but for say a servo, keeping it center may require non zero.  Also, the pico can't do negatives?
    .adc0_max_out_map_value = 10,     //  For the Battery voltage, we can try an arbirtraily low number, like 10.

    .adc1_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc1_max_out_map_value = 1024,   //  For joystick input lets set the cap at 1024

    .adc2_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc2_max_out_map_value = 1024,      //  For joystick input lets set the cap at 1024.
    
    .adc0_mapped_value = 0,
    .adc1_mapped_value = 0,
    .adc2_mapped_value = 0,
    .adc0_mapped_float = 0,
    .adc1_mapped_float = 0,
    .adc2_mapped_float = 0
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
    .power = RF_PWR_NEG_12DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US 
  };

int main() {
  initialise_sequence = 0;
  function_status_checker = 0x00;
  rot_buf = 0x0000;

  char_val = 0x65;

  stdio_init_all();
//  Sleep arbitrary amount of seconds to allow user to connect to serial monitor.
      sleep_ms(7000);
      sleep_ms(7000);

//  Initialise ADC PORT's.
      adc_init();
      adc_pin_setup(&my_adc);

//  Initialize pin setup for rotary encoder.
      rotary_encoder_init(&enabled_buttons);
      button_interrupt_init(&enabled_buttons);

//  Initialize pin setup for cd74hc595e(s).
    initialise_sequence += setup_my_registers(&my_regi);

//  Initialize LCD for control interface.
     initialise_sequence += pico_lcd_initialise(&my_regi);


  // SPI baudrate
  uint32_t my_baudrate = 5000000;

  nrf_client_t my_nrf;

  // initialise my_nrf
  initialise_sequence += nrf_driver_create_client(&my_nrf);

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

  // initialize_print_debug(&my_config, &my_pins, my_baudrate);
  printf("\n\n\nInitialisation Sequence Ended With Value: %i.\n\n", initialise_sequence);
    sleep_ms(2500);

  (initialise_sequence == 4) ? printf("\n\n//START PROGRAM//\n\n") : printf("\n\n//ABORT PROGRAM//\n\tSequence BEQ:\n\n", initialise_sequence);
  
while(1) {

  printf("\n");
  rot_buf = 0;
  adc_pin_call(&my_adc);
  rot_buf = rotary.rotary_total;
  sleep_ms(500);

  pico_to_default_screen(&my_regi, &my_lines, my_adc.adc1_mapped_value, my_adc.adc2_mapped_value, rot_buf, 0x36);
  //  Begin Transmitting RF data.

  printf("\n\n\n\t\t////////\t\t////////\n\n\n\n");
  printf("\t\t\t\tFilling Screen with %i.\n", char_val);
  sleep_ms(2000);
    function_status_checker = pico_to_fill_screen_with_char(&my_regi, char_val);
  printf("\nShould be finished.\n");
  sleep_ms(2000);
/*
  for(int zv = 0; zv < 1; zv++)
  {
    
      printf("Pipeline 0.\n");

    // send to receiver's DATA_PIPE_0 address
      my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});
      printf("Step 1.\n");
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
      printf("Step 2.\n");
    // send packet to receiver's DATA_PIPE_0 address
      printf("\n Sending: 0b%04x.\n", my_loads.payload_buffer.load_zero_buffer);
      success = my_nrf.send_packet(&my_loads.payload_buffer.load_zero_buffer, sizeof((my_loads.payload_buffer.load_zero_buffer)));
      printf("Step 3.\n");
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
      
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, (my_loads.payload_buffer.load_zero_buffer));

    } 
    else
    {
      printf("\nPacket not sent:- Receiver not available.\n");
    }


      printf("Pipeline 1.\n");
    // send to receiver's DATA_PIPE_1 address
      my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_1 address
      printf("\n Sending: 0b%04x.\n", (my_loads.payload_buffer.load_one_buffer.vertical_analog_buffer));
      printf("\n Sending: 0b%04x.\n", my_loads.payload_buffer.load_one_buffer.horizontal_analog_buffer);
      success = my_nrf.send_packet(&my_loads.payload_buffer.load_one_buffer, sizeof(my_loads.payload_buffer.load_one_buffer));
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
    if (success)
    {
        printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x & 0b%04x.\n",time_reply - time_sent, my_loads.payload_buffer.load_one_buffer.vertical_analog_buffer, my_loads.payload_buffer.load_one_buffer.horizontal_analog_buffer);
    }
    else
    {
      printf("\nPacket not sent:- Receiver not available.\n");
    }
          
      printf("Pipeline 2.\n");
    // send to receiver's DATA_PIPE_2 address
      my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_2 address
      printf("\n Sending: 0b%04x.\n", my_loads.payload_buffer.load_two_buffer);
      success = my_nrf.send_packet(&my_loads.payload_buffer.load_two_buffer, sizeof(my_loads.payload_buffer.load_two_buffer));
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, my_loads.payload_buffer.load_two_buffer);
    }
    else
    {
      printf("\nPacket not sent:- Receiver not available.\n");
    }

      printf("Pipeline Three.\n");
    // send to receiver's DATA_PIPE_3 address
      my_nrf.tx_destination((uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});
    // time packet was sent
      time_sent = to_us_since_boot(get_absolute_time()); // time sent
    // send packet to receiver's DATA_PIPE_3 address
      printf("\n Sending: 0b%08x.\n", my_loads.payload_buffer.load_three_buffer);
      success = my_nrf.send_packet(&my_loads.payload_buffer.load_three_buffer, sizeof(my_loads.payload_buffer.load_three_buffer));
    // time auto-acknowledge was received
      time_reply = to_us_since_boot(get_absolute_time()); // response time
    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, my_loads.payload_buffer.load_three_buffer);
    }
    else
    {
      printf("\nPacket not sent:- Receiver not available.\n");
    }

    }    
  

*/
  sleep_ms(500);
    printf("Next Iteration?\n");
    
}
tight_loop_contents();
}
