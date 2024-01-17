#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf24_driver.h"
#include "peri_header.h"
#include "dc_motors.h"
#include "rc_nrf_configs.h"
#include "stdlib.h"
#include "pico/time.h"

#define RCC_max 0xFFFF

//  R_cycle_c - Recevier Cycle Count:
uint16_t R_cycle_c;

  motor_flags_status cfl= {

  .vdti = true,
  .vdtn = false,
  .vdtp = false,
  .hdti = true,
  .hdtl = false,
  .hdtr = false

};

servo_motor_config_t servo_configuration = {

  .servo_one_pin = GPIO_FIFTEEN,
  .servo_initial_position = 1500,
  .servo_inital_positioning = true,
  .mcu_clock = 125000000,
  .so_clkdiv = 64,
  .so_wrap = 39062,
  .servo_two_pin = UNDEFINED,
  .servo_three_pin = UNDEFINED,
  .servo_four_pin = UNDEFINED

};

pwm_settings_t pwm_header_config = {

  .pwm_pin_one = GPIO_SIXTEEN,
  .pwm_pin_two = GPIO_SEVENTEEN,
  .pwm_pin_three = UNDEFINED,
  .pwm_pin_four = UNDEFINED,
  .pwm_pin_five = UNDEFINED,
  .pwm_pin_six = UNDEFINED,
  .wrap_one_init = 5000,
  .duty_cycle_one_init = .5

};

register_pins reg_pins = {

  .register_one_data = GPIO_ONE,
  .register_one_latch = GPIO_SEVEN,
  .register_one_enable = GPIO_EIGHT,
  .register_two_data = UNDEFINED,
  .register_two_latch = UNDEFINED,
  .register_two_enable = UNDEFINED,
  .register_clk_line = GPIO_NINE

};

payload_active_s active_payload = {
  .pay_zero = 0x00,
  .pay_one_buffer_s.vertical_active = 0x00,
  .pay_one_buffer_s.horizontal_active = 0x00,
  .pay_two = 0x00,
  .pay_three = 0x00
};

payload_recieved_buffer_t payload_recieved = {
  .payload_zero_r = 0x00,
  .payload_one_buffer_r.vertical_buffer = 0x00,
  .payload_one_buffer_r.horizontal_buffer = 0x00,
  .payload_two_r = 0x00,
  .payload_three_r = 0x00
};


int main () {

   stdio_init_all();

   sleep_ms(7000);
  sleep_ms(7000);

  servo_initialization(&servo_configuration);
  initial_pwm_pin_init(&pwm_header_config);
  shift_register_pin_init(&reg_pins);
  set_servo_initial_position(&servo_configuration);

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
    .retr_delay = ARD_750US 
  
};

  // SPI baudrate
  uint32_t my_baudrate = 5000000;

  // provides access to driver functions
  nrf_client_t my_nrf;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise((int)NULL)) 
  my_nrf.initialise(&my_config);

  /**
   * set addresses for DATA_PIPE_0 - DATA_PIPE_3.
   * These are addresses the transmitter will send its packets to.
   */
  my_nrf.rx_destination(DATA_PIPE_0, (uint8_t[]){0x37,0x37,0x37,0x37,0x37});
  my_nrf.rx_destination(DATA_PIPE_1, (uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_2, (uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_3, (uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

  // set to RX Mode
  my_nrf.receiver_mode();

  // data pipe number a packet was received on
  uint8_t pipe_number = 0;
  rc_light_register_initialize(&u_vars);
  initialize_print_debug(&my_config, &my_pins, my_baudrate);

  R_cycle_c = 0x0000;
while(1){



  printf("\n\nLoop beginning.\n\n\n\tNRF Receiving.\n");
  if(R_cycle_c < (RCC_max - 0x01)){
  printf("\nCycle: %u.\n", R_cycle_c);
  }else{
    printf("Recycling.\n");
    R_cycle_c = 1;
  }

  sleep_ms(150);
  
  if(my_nrf.is_packet(&pipe_number)){
    printf("Pipeline: #%u\n", &pipe_number);
    switch (pipe_number)
    {

      case DATA_PIPE_0:
        // read payload
        my_nrf.read_packet(&payload_recieved.payload_zero_r, sizeof(payload_recieved.payload_zero_r));
        active_payload.pay_zero = payload_recieved.payload_zero_r;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_zero_r, pipe_number);
      break;

      case DATA_PIPE_1:
        // read payload
        my_nrf.read_packet(&payload_recieved.payload_one_buffer_r, sizeof(payload_recieved.payload_one_buffer_r));
        active_payload.pay_one_buffer_s.vertical_active = payload_recieved.payload_one_buffer_r.vertical_buffer;
        active_payload.pay_one_buffer_s.horizontal_active = payload_recieved.payload_one_buffer_r.horizontal_buffer;

        printf("\nPacket received:- Payload (1: 0x%04x, 2: 0x%08x) on data pipe (%d)\n", payload_recieved.payload_one_buffer_r.vertical_buffer, payload_recieved.payload_one_buffer_r.horizontal_buffer, pipe_number);
      break;
        
      case DATA_PIPE_2:
        // read payload
        my_nrf.read_packet(&payload_recieved.payload_two_r, sizeof(payload_recieved.payload_two_r));
        active_payload.pay_two = payload_recieved.payload_two_r;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_two_r, pipe_number);
      break;
        
      case DATA_PIPE_3:
        // read payload
        my_nrf.read_packet(&payload_recieved.payload_three_r, sizeof(payload_recieved.payload_three_r));
        active_payload.pay_three = payload_recieved.payload_three_r;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_three_r, pipe_number);
      break;
        
      case DATA_PIPE_4:
      break;
        
      case DATA_PIPE_5:
      break;
        
      default:
      break;
        }

  u_vars.register_value_zero = payload_recieved.payload_two_r;
  printf("\n\nPayload zero: %u.\n", payload_recieved.payload_zero_r);
  printf("Payload One V: %u.\n", payload_recieved.payload_one_buffer_r.vertical_buffer);
  printf("Payload One H: %u.\n", payload_recieved.payload_one_buffer_r.horizontal_buffer);
  printf("Payload Two: %u.\n", payload_recieved.payload_two_r);
  printf("Payload Three: %u.\n\n", payload_recieved.payload_three_r);

  sleep_ms(500);
  
  printf("Payload Zero  ACTIVE: %u.\n", active_payload.pay_zero);
  printf("Payload One V ACTIVE: %u.\n", active_payload.pay_one_buffer_s.vertical_active);
  printf("Payload One H ACTIVE: %u.\n", active_payload.pay_one_buffer_s.horizontal_active);
  printf("Payload Two   ACTIVE: %u.\n", active_payload.pay_zero);
  printf("Payload Three ACTIVE: %u.\n\n", active_payload.pay_three);

} 
else {

  printf("No Data Received.\n\n");
  
}

  serial_register_output(&reg_pins, &u_vars, 0, -1);
//  sleep_ms(500);

  u_vars.register_value_one = 0x07;
  u_vars.register_value_two = 0xEB;
  u_vars.register_value_three = 0x5A;
  u_vars.register_value_four = 0x7D;

  print_binary(u_vars.register_value_one, u_vars.register_value_two);
  print_binary(u_vars.register_value_three, u_vars.register_value_four);

  motor_activation(&pwm_header_config, &motor_flags, &active_payload);
  
//  sleep_ms(500);
R_cycle_c += 0x01;
}

tight_loop_contents();
}

