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

register_pins reg_pins = {

  .register_one_data = GPIO_ONE,
  .register_one_latch = GPIO_SEVEN,
  .register_one_enable = GPIO_EIGHT,
  .register_clk_line = GPIO_NINE

};


int main () {

   stdio_init_all();

   sleep_ms(7000);
  sleep_ms(7000);

  servo_initialization(&servo_configuration);
  initial_pwm_pin_init(&pwm_header_config);
  shift_register_pin_init(&reg_pins);
  set_servo_initial_position(&servo_configuration);

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

while(1){


  if(my_nrf.is_packet(&pipe_number)){
  switch (pipe_number)
  {
    case DATA_PIPE_0:
      // read payload
      my_nrf.read_packet(&payload_recieved.payload_zero_r, sizeof(payload_recieved.payload_zero_r));
      // receiving a one byte uint8_t payload on DATA_PIPE_0.
      printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_zero_r, pipe_number);
    break;

    case DATA_PIPE_1:
      // read payload
      my_nrf.read_packet(&payload_recieved.payload_one_buffer_r, sizeof(payload_recieved.payload_one_buffer_r));
      // receiving a eight byte struct payload on DATA_PIPE_1.
      printf("\nPacket received:- Payload (1: 0x%04x, 2: 0x%08x) on data pipe (%d)\n", payload_recieved.payload_one_buffer_r.vertical_buffer, payload_recieved.payload_one_buffer_r.horizontal_buffer, pipe_number);
    break;
        
    case DATA_PIPE_2:
      // read payload
      my_nrf.read_packet(&payload_recieved.payload_two_r, sizeof(payload_recieved.payload_two_r));      
      // receiving a one byte string payload on DATA_PIPE_2.
      printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_two_r, pipe_number);
    break;
        
    case DATA_PIPE_3:
      // read payload
      my_nrf.read_packet(&payload_recieved.payload_three_r, sizeof(payload_recieved.payload_three_r));
      // receiving a two byte string payload on DATA_PIPE_2.
      printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_recieved.payload_three_r, pipe_number);
    break;
        
    case DATA_PIPE_4:
    break;
        
    case DATA_PIPE_5:
    break;
        
    default:
    break;
      }
}  

  u_vars.register_value_zero = payload_recieved.payload_two_r;

    rc_light_register_initialize(&u_vars);
    serial_register_output(&reg_pins, &u_vars, 0, NULL);
  motor_activation(&pwm_header_config, &motor_flags, &pwm_header_config);
  sleep_ms(500);
}

tight_loop_contents();
}

