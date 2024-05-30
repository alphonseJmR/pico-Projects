#ifndef RC_CONFIGS_H
#define RC_CONFIGS_H

#include <stdio.h>

typedef struct led_config_s {

    uint8_t front_white;
    uint8_t front_blue;
    uint8_t front_red;
    uint8_t front_green;
    uint8_t front_purple;
    uint8_t front_yellow;

    uint8_t rear_white;
    uint8_t rear_blue;
    uint8_t rear_red;
    uint8_t rear_green;
    uint8_t rear_purple;
    uint8_t rear_yellow;

}led_s;

typedef struct payload_data_s {

  //    ready_load is the current line 45, payloads_t, rewritten.
    struct ready_load {
      uint8_t load_zero;
      struct load_one_dual {
        uint16_t vertical_buffer;
        uint16_t horizontal_buffer;
      }load_one_dual;
      uint8_t load_two;
      uint16_t load_three;
    }ready_load;
  
    struct payload_buffer {
      uint8_t load_zero_buffer;
    struct load_one_buffer {
      uint16_t vertical_analog_buffer;
      uint16_t horizontal_analog_buffer;
    }load_one_buffer;
      uint8_t load_two_buffer;
      uint16_t load_three_buffer;
    }payload_buffer;

}payload_data;

/*
adc_port_values my_adc = {
    .adc0_pin = NULL,
    .adc1_pin = NULL,
    .adc2_pin = NULL,

    .adc0_min_in_map_value = 0,     //  ADC0 Port will route the vertical channel of the joystick into a mapped value we can use.
    .adc0_max_in_map_value = 4096,  //  4096 is the max resolution we can expect.
    .adc1_min_in_map_value = 0,
    .adc1_max_in_map_value = 4096,
    .adc2_min_in_map_value = NULL,
    .adc2_max_in_map_value = NULL,

    .adc0_min_out_map_value = 0,
    .adc0_max_out_map_value = 1024,
    .adc1_min_out_map_value = 700,  //  ADC1 Port routes the horizontal channel of the joystick into a value we can use.
    .adc1_max_out_map_value = 1700, //  700 && 1700 are values that currently used to test the amount of motion in the servo it produces to check wheel bearing 
    .adc2_min_out_map_value = NULL,
    .adc2_max_out_map_value = NULL,
    
    .adc0_mapped_value = 0,
    .adc1_mapped_value = 0,
    .adc2_mapped_value = 0,
    .adc0_mapped_float = 0,
    .adc1_mapped_float = 0,
    .adc2_mapped_float = 0
};
*/


led_s my_leds = {
    .front_white = 0x70,
    .front_blue = 0x10,
    .front_red = 0x40,
    .front_green = 0x20,
    .front_purple = 0x50,
    .front_yellow = 0x60,

    .rear_white = 0x07,
    .rear_blue = 0x01,
    .rear_red = 0x04,
    .rear_green = 0x02,
    .rear_purple = 0x05,
    .rear_yellow = 0x06

};

uint8_t set_rgb_color(uint8_t choice, led_s *configs, bool posi){

    if(posi){
      switch(choice){
        case 0:
          return configs->front_white;
        case 1:
          return configs->front_green;
        case 2:
          return configs->front_blue;
        case 3:
          return configs->front_yellow;
        case 4:
          return configs->front_purple;
        default:
          printf("No choice selected.\n");
          return 0;
      }
    }else{
        switch(choice){
        case 0:
          return configs->rear_white;
        case 1:
          return configs->rear_green;
        case 2:
          return configs->rear_blue;
        case 3:
          return configs->rear_yellow;
        case 4:
          return configs->rear_purple;
        default:
          printf("No choice selected.\n");
          return 0;
      }
    }

}


void payload_buffer(payload_data *data, adc_port_values *mapped, rotary_encoder_struct *cur_val, uint8_t rgb_colors){

data->payload_buffer.load_zero_buffer = cur_val->rotary_total;

data->payload_buffer.load_one_buffer.vertical_analog_buffer = mapped->adc0_mapped_value;
data->payload_buffer.load_one_buffer.horizontal_analog_buffer = mapped->adc1_mapped_value;





}



#endif
