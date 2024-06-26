#ifndef RC_CONFIGS_H
#define RC_CONFIGS_H

#include <stdio.h>
#include <stdint.h>


typedef struct payload_data_s {

  //    ready_load is the current line 45, payloads_t, rewritten.
  struct ready_load {
        uint8_t load_zero;

        struct load_vert_data {
          uint8_t vert_upper;
          uint8_t vert_lower;
        }load_vert_data;

        struct load_hori_data {
          uint8_t hori_upper;
          uint8_t hori_lower;
        }load_hori_data;

        uint8_t load_two;
        uint16_t load_three;
  }ready_load;
  
  struct payload_buffer {
        uint8_t load_zero_buffer;

      struct load_vert_buffer {
        uint8_t vert_upper_buf;
        uint8_t vert_lower_buf;
      }load_vert_buffer;

      struct load_hori_buffer {
        uint8_t hori_upper_buf;
        uint8_t hori_lower_buf;
      }load_hori_buffer;

        uint8_t load_two_buffer;
        uint16_t load_three_buffer;
  }payload_buffer;

}payload_data;

typedef struct split_payload_parity_s {

uint16_t v_par_one;
uint16_t v_par_two;
uint16_t v_par_three;

uint16_t h_par_one;
uint16_t h_par_two;
uint16_t h_par_three;

}load_parity;

void buffer_payload(payload_data *data_buffer, uint8_t load_zero_input, uint16_t load_two_input, uint16_t load_three_input){

  
  data_buffer->payload_buffer.load_zero_buffer = load_zero_input;
  
  data_buffer->ready_load.load_vert_data.vert_upper = ((load_two_input & 0xFF00) >> 8);
  data_buffer->ready_load.load_vert_data.vert_lower = (load_two_input & 0x00FF);
  
  data_buffer->ready_load.load_hori_data.hori_upper = ((load_three_input & 0xFF00) >> 8);
  data_buffer->ready_load.load_hori_data.hori_lower = (load_three_input & 0x00FF);
  
  data_buffer->payload_buffer.load_three_buffer = 0xCD;

  printf("////\tBuffer Values:\t////\n");
  printf("Load Zero Output: %i\n", load_zero_input);
  printf("Load One Output: %i\n", load_two_input);
  printf("Load Two Output: %i\n", load_three_input);
  printf("Load Three Output: %i\n", 0xCD);

}


typedef struct rc_pwm_port_s {

const uint8_t pwm_driveit;
const uint8_t pwm_backit;
const uint8_t pwm_turnit;

}rc_pwms;



#endif
