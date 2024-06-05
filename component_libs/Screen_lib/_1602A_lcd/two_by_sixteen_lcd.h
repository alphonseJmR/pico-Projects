#ifndef TWO_X_SIXTEEN_LCD_H
#define TWO_X_SIXTEEN_LCD_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "./Periphereals/shift_registers.h"
#include "./Periphereals/rand_funcs.h"

#define stage_inat 0x00
#define stage_intl 0x02
#define stage_indp 0x04
#define stage_comm 0x08
#define stage_char 0x10

#define comm_start 0x40
#define comm_end 0x70

#define e_toggle 0x20
#define com_sel = 040
#define backlight_toggle 0x80

#define com_com_e 0xA0
#define com_com_f 0x80
#define com_char_e 0xE0
#define com_char_f 0xC0


//  Pre-set to register so as to pull high/low the required pins for the lcd to accept data.
//  Command is user-configed.  RS, E, Backlight - 0x40, 0x20, 0x10.
void lcd_register_prep(uint8_t command, register_pins *pins){

    uint8_t command_transform;

    printf("LCD Prep Input: %02x.\n", (command & 0xF0));
    command_transform = uint_flip((command & 0xF0));
    printf("LCD Prep Output: %02x.\n", (command_transform & 0x0F));

        gpio_put(pins->register_one_enable, 1);

    for(int j = 8; j >= 0; j--){
            command_transform = (j < 6) ? (command_transform ^= (command_transform & 0x04)) : command_transform;
                ((command_transform >> j) & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_ms(2);
                clk_pulse(pins);
            }
            latch_data(pins);
            clk_pulse(pins);

        gpio_put(pins->register_one_enable, 0);
}

void byte_shifting(uint8_t command, uint8_t byte_input, register_pins *pins){

  printf("Bit Shifter.\n");

    uint8_t upper_bits;
    uint8_t lower_bits;
    uint8_t send_byte;

    uint8_t command_flip;
    uint8_t command_par;

    command_flip = uint_flip(command);

    command_par = (command & 0xFF);
    printf("Input: %02x.\n", command_par);
    printf("Byte Input: %02x.\n", byte_input);
    bool com_t;

    (command_par == 0xE0) ? printf("Character Command.\n") : printf("Com Command.\n");
    com_t = (command_par == 0xA0) ? true : false;

    //  Call lcd_display_prep first, as this sets the lcd ready for the data input, whether command or char.
    //  A loop of two, to first shift the upper four bits of the data, then the bottom four bits.
 
    for(int i = 0; i < 2; i++){

      send_byte = 0x00;
      
        lcd_register_prep(com_com_e, pins);
          gpio_put(pins->register_one_enable, 1);

      //  printf("Pre Command.\n");
        //  We don't want to send both upper and lower bits twice, so we'll use the iterations to send the sections.
        if(i == 0){
          printf("\nFirst Shift / nibble.\n");
            //  set the variable upper_bits to the output expression.
            lower_bits += (command_flip & 0x0F);
            upper_bits += (byte_input & 0xF0);
            send_byte += ((upper_bits & 0xF0) | (lower_bits & 0x0F));
            print_binary(send_byte);
            printf("Shifting Upper Bits: %02x.\n", upper_bits);
            for(int j = 8; j >= 0; j--){
                    //  per iteration, check if upper_bits is a 1 or 0, and put the gpio_pin in the appropriate position.
            lower_bits = (j < 6) ? (lower_bits ^= (lower_bits & 0x04)) : lower_bits;
                (send_byte >> j & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_ms(10);
                clk_pulse(pins);
            }
            latch_data(pins);
            clk_pulse(pins);
            (com_t) ? printf("Send COMMAND finish.\n") : printf("Send CHARACTER finish.\n");
           // (com_t) ? lcd_register_prep(com_com_f, pins) : lcd_register_prep(com_char_f, pins);
            
            printf("\n\n");
        }
        
        if(i == 1){
          printf("Second Shift / nibble.\n");
            lower_bits += ((command_flip & 0x0F) >> 4);
            upper_bits += ((byte_input << 4)& 0xF0);
            send_byte += ((upper_bits & 0xF0) | (lower_bits & 0x0F));
            print_binary(send_byte);
            printf("Shifting Lower Bits: %02x.\n", lower_bits);
            for(int j = 8; j >= 0; j--){
            lower_bits = (j < 6) ? (lower_bits ^= (lower_bits & 0x04)) : lower_bits;
                (send_byte >> j & 0x01) ? gpio_put(pins->register_one_data, 1) : gpio_put(pins->register_one_data, 0);
                  sleep_ms(10);
                clk_pulse(pins);
            }  
            latch_data(pins);
            clk_pulse(pins);
        (com_t) ? printf("Send COMMAND finish.\n") : printf("Send CHARACTER finish.\n");
       // (com_t) ? lcd_register_prep(com_com_f, pins) : lcd_register_prep(com_char_f, pins);
        
          printf("\n\n");
      }

      gpio_put(pins->register_one_enable, 0);
    }
}

void lcd_initialize(register_pins *pins){

  printf("Register -> LCD Initialization.\n\n");

  byte_shifting(com_com_e, 0x02, pins);
    sleep_ms(50);
    printf("Shifted command: 0x02.\n");

  byte_shifting(com_com_e, 0x28, pins);
    sleep_ms(20);
    printf("Shifted command: 0x28.\n");

  byte_shifting(com_com_e, 0x0C, pins);
    sleep_ms(5);
    printf("Shifted command: 0x08.\n");

  byte_shifting(com_com_e, 0x01, pins);
    sleep_ms(5);
    printf("Shifted command: 0x01.\n");

  printf("Register -> LCD Initialized.\n");
}

void lcd_single_line_out(register_pins *pins, uint8_t comm, const char *display){

  //  printf("\n\nlcd display array size: %u.\n\n", sizeof(display));
  printf("Single line lcd out func ->\n");
    for(int i = 0; i < strlen(display); i++){
      
        //  printf("Outputting: %u.\n", display[i]);
          printf("\n");
            byte_shifting(comm, display[i], pins);
              sleep_us(500);

    }
}

void lcd_screen_write(register_pins *pins, uint8_t com, const char *line_one, const char *line_two){

  printf(" screen write func\n");
  
  //  Clear LCD display.
  byte_shifting(com_com_e, 0x01, pins);
  sleep_ms(20);

  //  Print first line to LCD display.
  lcd_single_line_out(pins, com_char_e, line_one);
   sleep_ms(20);

  //  Bring LCD display to second line.
  byte_shifting(com_com_e, 0x80, pins);
   sleep_ms(20);

  byte_shifting(com_com_e, 0x40, pins);
  sleep_ms(20);

  //  Print second line to LCD display.
  lcd_single_line_out(pins, com_char_e, line_two);
    sleep_ms(20);

    //  Bring LCD display to second line.
  byte_shifting(com_com_e, 0x80, pins);
   sleep_ms(20);

  byte_shifting(com_com_e, 0x40, pins);
  sleep_ms(20);
}



#endif