#ifndef PICO_ADC_PORT_INPUT_S_H
#define PICO_ADC_PORT_INPUT_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico_pin_enum.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

typedef struct {
  uint adc0_pin;
  uint adc1_pin;
  uint adc2_pin;
  uint16_t adc0_raw_read;
  uint16_t adc1_raw_read;
  uint16_t adc2_raw_read;
  uint16_t adc0_min_in_map_value;
  uint16_t adc0_max_in_map_value;
  uint16_t adc1_min_in_map_value;
  uint16_t adc1_max_in_map_value;
  uint16_t adc2_min_in_map_value;
  uint16_t adc2_max_in_map_value;
  uint16_t adc0_min_out_map_value;
  uint16_t adc0_max_out_map_value;
  uint16_t adc1_min_out_map_value;
  uint16_t adc1_max_out_map_value;
  uint16_t adc2_min_out_map_value;
  uint16_t adc2_max_out_map_value;
  uint16_t adc0_mapped_value;
  uint16_t adc1_mapped_value;
  uint16_t adc2_mapped_value;
  float adc0_mapped_float;
  float adc1_mapped_float;
  float adc2_mapped_float;

} adc_port_values;
adc_port_values pico_adc;


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