#ifndef RANDOM_FUNCTIONS_H
#define RANDOM_FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

uint8_t uint_flip(uint8_t input_byte){
  uint8_t flipped_val;
  flipped_val = 0x00;

  flipped_val += (0x01 & (input_byte >> 7)) ? (0x01 << 0) : (0x00 << 0);
  flipped_val += (0x01 & (input_byte >> 6)) ? (0x01 << 1) : (0x00 << 1);
  flipped_val += (0x01 & (input_byte >> 5)) ? (0x01 << 2) : (0x00 << 2);
  flipped_val += (0x01 & (input_byte >> 4)) ? (0x01 << 3) : (0x00 << 3);

  flipped_val += (0x01 & (input_byte >> 3)) ? (0x01 << 7) : (0x00 << 7);
  flipped_val += (0x01 & (input_byte >> 2)) ? (0x01 << 6) : (0x00 << 6);
  flipped_val += (0x01 & (input_byte >> 1)) ? (0x01 << 5) : (0x00 << 5);
  flipped_val += (0x01 & (input_byte >> 0)) ? (0x01 << 4) : (0x00 << 4);

  return flipped_val;
}

int input_response(uint16_t x) {
 
  int r;
  r = ((x * 3) > 768) ? 16 : 8;
  return r;

}

uint16_t uints_mesh(uint8_t input_one, uint8_t input_two){

  uint16_t output;
  output = 0x0000;

  output += ((input_one >> 0) & 0x01) ? 0x0100 : 0x0000;
  output += ((input_one >> 1) & 0x01) ? 0x0200 : 0x0000;
  output += ((input_one >> 2) & 0x01) ? 0x0400 : 0x0000;
  output += ((input_one >> 3) & 0x01) ? 0x0800 : 0x0000;
  output += ((input_one >> 4) & 0x01) ? 0x1000 : 0x0000;
  output += ((input_one >> 5) & 0x01) ? 0x2000 : 0x0000;
  output += ((input_one >> 6) & 0x01) ? 0x4000 : 0x0000;
  output += ((input_one >> 7) & 0x01) ? 0x8000 : 0x0000;

  output += ((input_two >> 0) & 0x01) ? 0x0001 : 0x0000;
  output += ((input_two >> 1) & 0x01) ? 0x0002 : 0x0000;
  output += ((input_two >> 2) & 0x01) ? 0x0004 : 0x0000;
  output += ((input_two >> 3) & 0x01) ? 0x0008 : 0x0000;
  output += ((input_two >> 4) & 0x01) ? 0x0010 : 0x0000;
  output += ((input_two >> 5) & 0x01) ? 0x0020 : 0x0000;
  output += ((input_two >> 6) & 0x01) ? 0x0040 : 0x0000;
  output += ((input_two >> 7) & 0x01) ? 0x0080 : 0x0000;


  return output;


}

//  Use to console print the binary form of the inputted variable.  Able to accept UINT, UINT8_T, UINT16_T, UINT32_T data.
//  Example print_binary(0, 0, 2056, 0);  Would print the binary form of the uint16_t value 2056.
void print_binary(uint16_t byte_input) {
  
    printf("\n");
    int returned;

    if(byte_input > 0){
      if(input_response(byte_input) == 8){

        for (int i = 7; i >= 0; i--) {
          if ((byte_input >> i) & 0x01) {
              printf("1");
          }else {
              printf("0");
            }
          }
      }else{
        for (int i = 15; i >= 0; i--) {
          if ((byte_input >> i) & 0x01) {
              printf("1");
          }else {
              printf("0");
            }
          }
      }
    }
    printf("\n");
    }

#endif