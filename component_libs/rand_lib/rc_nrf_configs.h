#ifndef RC_CONFIGS_H
#define RC_CONFIGS_H

#include <stdio.h>

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




#endif
