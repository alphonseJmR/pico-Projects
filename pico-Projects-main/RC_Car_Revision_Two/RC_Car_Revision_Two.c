#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Poly_Tools.h"


registered_pins my_regis = {

  .DATA_LINE_ONE = GPIO_FIVE,
  .LATCH_LINE_ONE = GPIO_THREE,
  .ENABLE_LINE_ONE = GPIO_FOUR,
  .DATA_LINE_TWO = UNDEFINED,
  .LATCH_LINE_TWO = UNDEFINED,
  .ENABLE_LINE_TWO = UNDEFINED,
  .REGISTERS_CLK_LINE = GPIO_TWO

};

payload_data my_loads = {

  .ready_load.load_zero = 0,
  .ready_load.load_one_dual.vertical_buffer = 0,
  .ready_load.load_one_dual.horizontal_buffer = 0,
  .ready_load.load_two = 0,
  .ready_load.load_three = 0,

  .payload_buffer.load_zero_buffer = 0,
  .payload_buffer.load_one_buffer.vertical_analog_buffer = 0,
  .payload_buffer.load_one_buffer.horizontal_analog_buffer = 0,
  .payload_buffer.load_two_buffer = 0,
  .payload_buffer.load_three_buffer = 0

};

pin_manager_t my_pins = { 

    .sck = GPIO_THIRTEEN,
    .mosi = GPIO_TWELVE, 
    .miso = GPIO_ELEVEN, 
    .csn = GPIO_TEN, 
    .ce = GPIO_NINE

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

int main () {
  
  int pin_init;
  pin_init = 0;

  int qprint, qprint_o;
  qprint, qprint_o = 0;


   stdio_init_all();

  printf("RC Car Program Initialising.");

  sleep_ms(7000);
  sleep_ms(7000);

  pin_init += register_pin_initialisation(&my_regis);

  (pin_init == 2) ? printf("\n\n////\tRegister Pins Init Success.\t////\n") : printf("\n\n////\tRegister Pins Init Failure.\t////\n");

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

  // * set addresses for DATA_PIPE_0 - DATA_PIPE_3.
  // * These are addresses the transmitter will send its packets to.
  
  my_nrf.rx_destination(DATA_PIPE_0, (uint8_t[]){0x37,0x37,0x37,0x37,0x37});
  my_nrf.rx_destination(DATA_PIPE_1, (uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_2, (uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_3, (uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

  // set to RX Mode
  my_nrf.receiver_mode();

  // data pipe number a packet was received on
  uint8_t pipe_number = 0;

  //  register_595_output(&my_regis, 0x15, false, 0xFF);


while(1){

  printf("\n\n////\tStart of main's while loop.\t////\n\n");

 snhc595_test(&my_regis, testing_values);

  sleep_ms(150);

  if(my_nrf.is_packet(&pipe_number)){
    printf("Pipeline: #%u\n", &pipe_number);
    switch (pipe_number)
    {

      case DATA_PIPE_0:
        // read payload
        my_nrf.read_packet(&my_loads.payload_buffer.load_zero_buffer, sizeof(my_loads.payload_buffer.load_zero_buffer));
        my_loads.ready_load.load_zero = my_loads.payload_buffer.load_zero_buffer;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", my_loads.ready_load.load_zero, pipe_number);
      break;

      case DATA_PIPE_1:
        // read payload
        my_nrf.read_packet(&my_loads.payload_buffer.load_one_buffer, sizeof(my_loads.payload_buffer.load_one_buffer));
        my_loads.ready_load.load_one_dual.vertical_buffer = my_loads.payload_buffer.load_one_buffer.vertical_analog_buffer;
        my_loads.ready_load.load_one_dual.horizontal_buffer = my_loads.payload_buffer.load_one_buffer.horizontal_analog_buffer;

        qprint = my_loads.ready_load.load_one_dual.vertical_buffer;
        qprint_o = my_loads.ready_load.load_one_dual.horizontal_buffer;
        printf("\nPacket received:- Payload (1: 0x%04x, 2: 0x%08x) on data pipe (%d)\n", qprint, qprint_o, pipe_number);
      break;
        
      case DATA_PIPE_2:
        // read payload
        my_nrf.read_packet(&my_loads.payload_buffer.load_two_buffer, sizeof(my_loads.payload_buffer.load_two_buffer));
        my_loads.ready_load.load_two = my_loads.payload_buffer.load_two_buffer;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", my_loads.ready_load.load_two, pipe_number);
      break;
        
      case DATA_PIPE_3:
        // read payload
        my_nrf.read_packet(&my_loads.payload_buffer.load_three_buffer, sizeof(my_loads.payload_buffer.load_three_buffer));
        my_loads.ready_load.load_three = my_loads.payload_buffer.load_three_buffer;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", my_loads.ready_load.load_three, pipe_number);
      break;
        
      case DATA_PIPE_4:
      break;
        
      case DATA_PIPE_5:
      break;
        
      default:
      break;
        }

} 
else {

  printf("No Data Received.\n\n");
  
}

  printf("\n\n");
  printf("//Payload   Zero   BUFFER//: %u.\n", my_loads.payload_buffer.load_zero_buffer);
  printf("//Payload  One V   BUFFER//: %u.\n", my_loads.payload_buffer.load_one_buffer.vertical_analog_buffer);
  printf("//Payload  One H   BUFFER//: %u.\n", my_loads.payload_buffer.load_one_buffer.horizontal_analog_buffer);
  printf("//Payload   Two    BUFFER//: %u.\n", my_loads.payload_buffer.load_two_buffer);
  printf("//Payload  Three   BUFFER//: %u.\n\n", my_loads.payload_buffer.load_three_buffer);

printf("\n\n");
  sleep_ms(500);
  
  printf("//Payload   Zero   ACTIVE//: %u.\n", my_loads.ready_load.load_zero);
  printf("//Payload  One V   ACTIVE//: %u.\n", my_loads.ready_load.load_one_dual.vertical_buffer);
  printf("//Payload  One H   ACTIVE//: %u.\n", my_loads.ready_load.load_one_dual.horizontal_buffer);
  printf("//Payload   Two    ACTIVE//: %u.\n", my_loads.ready_load.load_two);
  printf("//Payload  Three   ACTIVE//: %u.\n\n", my_loads.ready_load.load_three);

printf("\n\n");
  sleep_ms(500);
}

tight_loop_contents();
}

