/*as data: 8bits

  pi_out_data[0, 1, 2, 3, 4, 5, 6, 7]
  pi_out_data_buffer[];
  for(int a = 0; a < 8; a++) {
    pi_out_data
  }
*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

  struct nRF_transmitter {
    int baudrate;      // Max 10MHZ, use 2.5mhz
    int data_size;               // Max is 8
    bool data_transmit;       //  CS pin
    char data_buffer_array[];      // [0, 0, 0, 0, 0, 0, 0, 0]
    char data_array[];           //  [0, 0, 0, 0, 0, 0, 0, 0]
    
  }nrf_tx;

  int main(void) {
    stdio_init_all();

    pin_manager_t my_pins = {
        .sck = 14,
        .mosi = 11,
        .miso = 12,
        .csn = 13,
        .ce = 15;
    };

    nrf_manager_t my_config = {

        


    }
  }