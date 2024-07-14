#ifndef PICO_ADC_PORT_INPUT_S_H
#define PICO_ADC_PORT_INPUT_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico_pin_enum.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "../../rand_lib/rc_nrf_configs.h"

#define adc_Clk 48000000
#define Adc_Freq 10000

/*

void dma_setup(adc_dma *my_dmas, input_types *types, payload_data *data){

  types->dma_type_chan = dma_claim_unused_channel(true);
  my_dmas->adc1_chan = dma_claim_unused_channel(true);
  my_dmas->adc2_chan = dma_claim_unused_channel(true);

  volatile void *load_ptr = &data->ready_load.load_zero;
  volatile void *read_ptr = &types->button_value;

  dma_channel_config c1 = dma_channel_get_default_config(types->dma_type_chan);
  dma_channel_config c2 = dma_channel_get_default_config(my_dmas->adc1_chan);
  dma_channel_config c3 = dma_channel_get_default_config(my_dmas->adc2_chan);

  channel_config_set_transfer_data_size(&c1, DMA_SIZE_8);
  channel_config_set_read_increment(&c1, false);
  channel_config_set_write_increment(&c1, true);
  channel_config_set_dreq(&c1, 0x3b);
  dma_channel_configure(types->dma_type_chan, &c1,
   load_ptr,
   read_ptr,
   1,
   false);

  channel_config_set_transfer_data_size(&c2, DMA_SIZE_16);
  channel_config_set_read_increment(&c2, false);
  channel_config_set_write_increment(&c2, true);
  channel_config_set_dreq(&c2, DREQ_ADC);
  dma_channel_configure(my_dmas->adc1_chan, &c2, &my_dmas->adc1_dma_stored, &adc_hw->fifo, 1, false);
  channel_config_set_chain_to(&c2, my_dmas->adc0_chan);

  channel_config_set_transfer_data_size(&c3, DMA_SIZE_16);
  channel_config_set_read_increment(&c3, false);
  channel_config_set_write_increment(&c3, true);
  channel_config_set_dreq(&c3, DREQ_ADC);
  dma_channel_configure(my_dmas->adc2_chan, &c3, &my_dmas->adc2_dma_stored, &adc_hw->fifo, 1, false);
  channel_config_set_chain_to(&c3, my_dmas->adc2_chan);
}

//  rnd_rbin is a bit mask of 0 - 0x1f for adc ports.  port 0 - 0x01, port1 0x02, port2 0x04.. etc
//  adc_clk set to 48 Mhz (48000000)
void setup_adc_free_run(adc_port_values *adc, uint64_t adc_clk, uint64_t freq, uint8_t rnd_rbin){

    if(adc->adc0_pin != UNDEFINED){
  adc_gpio_init(adc->adc0_pin);
    }
    if(adc->adc1_pin != UNDEFINED){
  adc_gpio_init(adc->adc1_pin);
    }
    if(adc->adc2_pin != UNDEFINED){
  adc_gpio_init(adc->adc2_pin);
    }

  adc_init();

  adc_set_clkdiv((adc_clk/freq));

  adc_select_input(0);

  adc_set_round_robin(rnd_rbin);

  adc_fifo_setup(true, true, 1, false, true);

  adc_run(true);

}
*/


// ===== Functions Below ========

/*
    long impliments the variable to handle large values, can be changed if precision is necessary or easy.
    Parameters:     --long type implied--
    x:          Input for both analog.vertical & analog.horizontal.
    in_min:     Minimum value that an ADC can input. i.e {in_min = 0 || in_min = -1024}
    in_max:     Maximum value that an ADC can input. i.e {in_max = 1024 || in_max = 4096}
    out_min:    Minimum value to be set by MAP function. i.e {out_min = 0 || out_min = -1024}
    out_max:    Maximum value to be set by MAP function. i.e {out_max = 255 || out_max = 4096}

    EXAMPLE:
    x = 3600    in_min = 0      in_max = 4096       out_min = 200       out_max = 600

    (3600-0)*(600-200)/(4096-0)+200

    ===
    
    551.5625 the .5625 can be dropped if desired.
*/
float map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (float)((x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min);
}


//  Adc_pin_call function takes in a struct and based on the set values, can call all three ADC ports, and map their respective values.
void adc_pin_call(adc_port_values *config) {
  /*
    Select inputs according to ADC enabled pins.  Perform ADC call on inputs 0, 1, or 2. 
    Select which input to read data from (0 | 1 | 2).
  */
  if(config->adc0_pin != UNDEFINED) {
    adc_select_input(0);
    config->adc0_raw_read = adc_read();
    printf("\n\n\tRaw value of ADC0: %d.\n", config->adc0_raw_read);
    }
  if(config->adc1_pin != UNDEFINED) {
    adc_select_input(1);
    config->adc1_raw_read = adc_read();
    printf("\t\tRaw value of ADC1: %d.\n", config->adc1_raw_read);
    }
  if(config->adc2_pin != UNDEFINED) {
    adc_select_input(2);
    config->adc2_raw_read = adc_read();
    printf("\t\t\tRaw value of ADC2: %d.\n\n", config->adc2_raw_read);
    }
    
    //  Convert raw analog input data of vertical & horizontal using MAP function.
    //  Store in respectively named analog.()_raw_conversion variable.
    if(config->adc0_raw_read > 0){
    config->adc0_mapped_value = map(config->adc0_raw_read, config->adc0_min_in_map_value, config->adc0_max_in_map_value, config->adc0_min_out_map_value, config->adc0_max_out_map_value);
      config->adc0_mapped_float = (float)map((float)config->adc0_raw_read, (float)config->adc0_min_in_map_value, (float)config->adc0_max_in_map_value, (float)config->adc0_min_out_map_value, (float)config->adc0_max_out_map_value);
    } else {
      printf("\tADC0 Port Has No Value.\n");
    }
    if(config->adc1_raw_read > 0){
    config->adc1_mapped_value = map(config->adc1_raw_read, config->adc1_min_in_map_value, config->adc1_max_in_map_value, config->adc1_min_out_map_value, config->adc1_max_out_map_value);
      config->adc1_mapped_float = (float)map((float)config->adc1_raw_read, (float)config->adc1_min_in_map_value, (float)config->adc1_max_in_map_value, (float)config->adc1_min_out_map_value, (float)config->adc1_max_out_map_value);
    }  else {
      printf("\tADC1 Port Has No Value.\n");
    }
    if(config->adc2_raw_read > 0){
    config->adc2_mapped_value = map(config->adc2_raw_read, config->adc2_min_in_map_value, config->adc2_max_in_map_value, config->adc2_min_out_map_value, config->adc2_max_out_map_value);
      config->adc2_mapped_float = (float)map((float)config->adc2_raw_read, (float)config->adc2_min_in_map_value, (float)config->adc2_max_in_map_value, (float)config->adc2_min_out_map_value, (float)config->adc2_max_out_map_value);
    }  else {
      printf("\tADC2 Port Has No Value.\n");
    }

    
};

void adc_pin_setup(adc_port_values *config) {

  printf("\nADC Pin Initialization.\n");

  if(config->adc0_pin != UNDEFINED){
  adc_gpio_init(config->adc0_pin);
  printf("ADC0 Pin Initialized.\n");
  printf("ADC0 PIN: %d.\n\n", config->adc0_pin);
  }else {
    printf("ADC0 Not Initialized.\n");
  }

  if(config->adc1_pin != UNDEFINED){
  adc_gpio_init(config->adc1_pin);
  printf("ADC1 Pin Initialized.\n");
  printf("ADC1 PIN: %d.\n\n", config->adc1_pin);
  }else {
    printf("ADC1 Not Initialized.\n");
  }

  if(config->adc2_pin != UNDEFINED){
  adc_gpio_init(config->adc2_pin);
  printf("ADC2 Pin Initialized.\n");
  printf("ADC2 PIN: %d.\n\n", config->adc2_pin);
  }else {
    printf("ADC2 Not Initialized.\n");
  }

}



#endif
