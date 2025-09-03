#ifndef RADIO_CONTROLLER_VERSION_4_STRUCTURES_S_H
#define RADIO_CONTROLLER_VERSION_4_STRUCTURES_S_H

#include <stdint.h>
#include <stdbool.h>
#include "hi_lvl_resources/project_struct_s.h"
#include "hardware/pio.h"


 typedef struct cdhc_dma_s {

  int8_t channel;
  bool r_inc;
  bool w_inc;
  uint8_t transfer_size;

} demas;

//  We won't add read or write increment as I need to see what I'll be using in general purpose dma_s
typedef struct gp_dma_s {

  int8_t chan;
  uint8_t tran_size;
  //  read address
  volatile void *r_addr;

}gp_dma;


typedef struct cdhc_pin_s {

  uint8_t lat;
  uint8_t clk;
  uint8_t inh;
  uint8_t data;

}cdhc_pins;


typedef struct cdhc_pio_s {

  PIO port;
  uint8_t sm;
  uint8_t offset;
  bool can_add;

}cdhc_pio;


typedef struct pio_cdhc_ {

  cdhc_pins Pins;
  cdhc_pio Pio;

}pio_cdhc;


extern pio_rot rotary_data;


//  Our PIO pins, 3 set pins, 1 in pin.
typedef struct mcp_3008_adc_s {

  PIO port;
  uint8_t sm;
  uint8_t offset;
  uint8_t clk_sm;
  uint8_t clk_off;
  
  uint8_t d_out;
  uint8_t clock;
//  CS bar (cs!)
  uint8_t cs_n;
  uint8_t d_in;
  
  uint16_t converted[8];
  uint32_t raw_input[8];
}mcp_s;

#endif