#ifndef LCD_SHIFT_REGISTER_CONTROLLER_H
#define LCD_SHIFT_REGISTER_CONTROLLER_H

#include <stdio.h>
#include "./Periphereals/resources/pico_pin_enum.h"
#include "project_struct_s.h"
#include "struct_definition_s.h"
#include "./Screen_lib/_1602A_lcd/_1602a_command_s.h"
#include "hardware/gpio.h"

#define register_parts
#define _2x16_LCD
static const uint8_t ddram_line1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static const uint8_t ddram_line2[] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
lcd_lines pattern_one = {

    .line_one = {'L', 'a', 's', 'e', 'r', ' ', 'P', 'a', 't', 't', 'e', 'r', 'n', ':', ' ', ' '},
    .line_two = {'B', 'a', 'r', 'r', 'e', 'l', ' ', 'R', 'o', 't', 'a', 't', 'i', 'o', 'n', ' '}

};

lcd_lines pattern_two = {

    .line_one = {'L', 'a', 's', 'e', 'r', ' ', 'P', 'a', 't', 't', 'e', 'r', 'n', ':', ' ', ' '},
    .line_two = {'D', 'u', 'a', 'l', 'l', 'y', ' ', 'R', 'o', 't', 'a', 't', 'i', 'o', 'n', ' '}

};

lcd_lines pattern_three = {

    .line_one = {'L', 'a', 's', 'e', 'r', ' ', 'P', 'a', 't', 't', 'e', 'r', 'n', ':', ' ', ' '},
    .line_two = {'D', 'e', 's', 't', 'r', 'u', 'c', 't', 'i', 'o', 'n', ' ', 'B', 'e', 'a', 'm'}

};

lcd_lines intro_screen = {

    .line_one = {' ', 'A', 'l', 'p', 'h', 'o', 'n', 's', 'e', ' ', 'T', 'u', 'r', 'r', 'e', 't'},
    .line_two = {' ', ' ', 'V', 'e', 'r', 's', 'i', 'o', 'n', ' ', 'O', 'n', 'e', ' ', ' ', ' '}

};

//  4/8 Wire will determine how we send the lcd data.
#define _4_wire_
// #define _8_wire_


uint8_t uint_flip(uint8_t input_byte){
  uint8_t flipped_val = 0x00;

  for(uint8_t a = 0; a < 8; a++){
    if(a < 4){
      flipped_val += ((input_byte & (0x01 << a)) << 4);
    }else if(a < 8){
      flipped_val += ((input_byte & (0x01 << a)) >> 4);
    }else{
      printf("Out of bound flipped for loop.\n");
    }
  }

  return flipped_val;
}



uint8_t flip_it(uint8_t byte){
  uint8_t buff = 0x00;

  buff += (byte & 0x80) ? 0x10 : 0x00;
  buff += (byte & 0x40) ? 0x20 : 0x00;
  buff += (byte & 0x20) ? 0x40 : 0x00;
  buff += (byte & 0x10) ? 0x80 : 0x00;
  buff += (byte & 0x08) ? 0x01 : 0x00;
  buff += (byte & 0x04) ? 0x02 : 0x00;
  buff += (byte & 0x02) ? 0x04 : 0x00;
  buff += (byte & 0x01) ? 0x08 : 0x00;

  return buff;
}

void print_binary(uint16_t byte_input) {
  
  //  printf("\n");
      
      for (uint8_t i = 8; i > 0; i--) {
        
        if ((byte_input >> (i - 1) & 0x01)) {
          printf("1");
        }else {
          printf("0");
        }
      }
    printf("\n");
}

#ifdef register_parts


void setup_my_registers(register_lcd *pinn){
  
        printf("Beginning Initialisation of GPIO --> Register LCD Pins.\n");

  if (pinn->lcd_reg_data != UNDEFINED)
  {
        printf("Register Data Pin : %i\n", pinn->lcd_reg_data);
    gpio_init(pinn->lcd_reg_data);
    gpio_set_dir(pinn->lcd_reg_data, GPIO_OUT);
    gpio_pull_down(pinn->lcd_reg_data);
  }
  else
  {
    printf("No pin set for lcd register data.\n");
  }

  if (pinn->lcd_reg_latch != UNDEFINED)
  {
        printf("Register Latch Pin : %i\n", pinn->lcd_reg_latch);
    gpio_init(pinn->lcd_reg_latch);
    gpio_set_dir(pinn->lcd_reg_latch, GPIO_OUT);
    gpio_pull_down(pinn->lcd_reg_latch);
  }
  else
  {
    printf("No pin set for lcd register latch.\n");
  }

  if (pinn->lcd_reg_enable != UNDEFINED)
  {
        printf("Register Enable Pin : %i\n", pinn->lcd_reg_enable);
    gpio_init(pinn->lcd_reg_enable);
    gpio_set_dir(pinn->lcd_reg_enable, GPIO_OUT);
    gpio_pull_down(pinn->lcd_reg_enable);
  }
  else
  {
    printf("No pin set for lcd register enable.\n");
  }

  if (pinn->lcd_reg_clk != UNDEFINED)
  {
        printf("Register Clock Pin : %i\n", pinn->lcd_reg_clk);
    gpio_init(pinn->lcd_reg_clk);
    gpio_set_dir(pinn->lcd_reg_clk, GPIO_OUT);
    gpio_pull_down(pinn->lcd_reg_clk);
  }
  else
  {
    printf("No pin set for lcd register clock.\n");
  }

  if(pinn->lcd_rs != UNDEFINED){

        printf("LCD RS Pin: %i\n", pinn->lcd_rs);
    gpio_init(pinn->lcd_rs);
    gpio_set_dir(pinn->lcd_rs, GPIO_OUT);
    gpio_pull_down(pinn->lcd_rs);
  }else{
    printf("No pin set for LCD RS.\n");
  }

  if(pinn->lcd_e != UNDEFINED){
        printf("LCD E Pin: %i\n", pinn->lcd_e);
    gpio_init(pinn->lcd_e);
    gpio_set_dir(pinn->lcd_e, GPIO_OUT);
    gpio_pull_down(pinn->lcd_e);
  }else{
    printf("No pin set for LCD E.\n");
  }
    
}

void send_clock_pulse(register_lcd *set){

  gpio_put(set->lcd_reg_clk, 1);
    sleep_us(5);
  gpio_put(set->lcd_reg_clk, 0);

}

void send_latch_pulse(register_lcd *set){

    gpio_put(set->lcd_reg_latch, 1);
      sleep_us(5);
    gpio_put(set->lcd_reg_latch, 0);
      sleep_us(5);
  
}

void set_clock_high(register_lcd *set){
    gpio_put(set->lcd_reg_clk, 1);
}

void set_clock_low(register_lcd *set){
    gpio_put(set->lcd_reg_clk, 0);
}

void set_enable_high(register_lcd *set){
    gpio_put(set->lcd_reg_enable, 1);
      sleep_ms(1);
}

void set_enable_low(register_lcd *set){
    gpio_put(set->lcd_reg_enable, 0);
      sleep_ms(1);
}

void set_register(register_lcd *set, uint8_t place, uint8_t data){
    gpio_put(set->lcd_reg_data, (((data >> place) & 0x01)));

}

void set_lcd_rs(register_lcd *set, bool set_val){
    gpio_put(set->lcd_rs, set_val);
      sleep_ms(5);
}

void set_lcd_e(register_lcd *set, bool set_val){
    gpio_put(set->lcd_e, set_val);
        sleep_ms(5);
}

void send_nibble(register_lcd *set, uint8_t input)
{
  // printf("Input: 0x%02x.\n", input);
  set_enable_high(set);

      for (uint8_t a = 0; a < 8; a++){
        set_clock_low(set);
        set_register(set, a, input);
        //  printf("Bit: %i.\n", ((input >> a) & 0x01));
          //  sleep_us(5);       
        set_clock_high(set);
            sleep_us(5);
    }
      send_latch_pulse(set);
      send_clock_pulse(set);
      set_enable_low(set);

    sleep_ms(2);
}



void send_byte(register_lcd *set, uint8_t d_byte)
{
  #ifdef _4_wire_
//  With 4 wire communication we need to send upper and lower portion of input byte (0xF0 || 0x0F)
  uint8_t LSB, MSB;

  LSB, MSB = 0;
  MSB = ((d_byte >> 4) & 0x0F);
  LSB = (d_byte & 0x0F);

  // MSB = (d_byte & 0xF0);
  // LSB = ((d_byte << 4) & 0xF0);

    set_lcd_e(set, true);
    send_nibble(set, (MSB & 0x0F));
     set_lcd_e(set, false);
      sleep_us(2);

     set_lcd_e(set, true);
    send_nibble(set, (LSB & 0x0F));
    set_lcd_e(set, false);
      
  #endif

  #ifdef _8_wire_

    set_lcd_e(set, false);
    send_nibble(set, d_byte);
    set_lcd_e(set, true);
    //  sleep_us(2);

  #endif
}

void send_lcd_command(register_lcd *set, uint8_t command){
//  printf("\tLCD Command.\n");
    set_lcd_rs(set, false);
        sleep_us(100);
    send_byte(set, command);
}

void send_lcd_character(register_lcd *set, uint8_t character){
//  printf("\tLCD Character.\n");
      sleep_us(100);
    send_byte(set, character);
      sleep_ms(20);
}


#endif

#ifdef _2x16_LCD


//  Position is set from ddram_line(1,2).
//  Valid values for both (1,2) are 0-15
void set_lcd_cursor(register_lcd *set, uint8_t line, uint8_t pos){

    uint8_t set_position = 0;
      switch(line){
        case 1:
            set_position = (LCD_SET_DDRAM_ADDR | ddram_line1[pos]);
        break;

        case 2:
            set_position = (LCD_SET_DDRAM_ADDR | ddram_line2[pos]);
        break;

        default:
            printf("Error with line selection.  Setting to home.\n");
            set_position = (LCD_SET_DDRAM_ADDR | ddram_line1[0]);
        break;
      }
    send_lcd_command(set, set_position);
        sleep_us(300);
}

void set_lcd_screen_clear(register_lcd *set){
    send_lcd_command(set, LCD_CLEAR_DISPLAY);
        sleep_ms(5);
}

void set_lcd_home(register_lcd *set){
    send_lcd_command(set, LCD_RETURN_HOME);
        sleep_ms(5);
}

void write_lcd_line(register_lcd *set, char *str){
  set_lcd_rs(set, true);
    for(uint8_t a = 0; a < 16; a++){
        send_lcd_character(set, str[a]);
            sleep_ms(5);
    }

}

void set_screen_write(register_lcd *set, char *top_str, char *bottom_str){

    set_lcd_cursor(set, 1, 0);
        write_lcd_line(set, top_str);
            sleep_ms(2);
    set_lcd_cursor(set, 2, 0);
        write_lcd_line(set, bottom_str);
            sleep_ms(2);
}

//  This writes data while not in laser pattern menu.
//  turret_angle can be set based on last servo position.
void write_data(register_lcd *set, uint16_t turret_angle, uint8_t turn){

    char l_one[16], l_two[16];
    sprintf(l_one, "FireDeg 0x%04x  ", turret_angle);
      switch(turn){

        case 0:
          sprintf(l_two, "__Turret Idle__");
        break;
        
        case 1:
          sprintf(l_two, "Swiveling Right");
        break;

        case 2:
          sprintf(l_two, "Swiveling Left_");  
        break;
        
        default:
        printf("Error In Turning Value.\n");
        break;
      }

    set_screen_write(set, l_one, l_two);
}

//  The selector menu will input the pattern choice.
void write_pattern_selection(register_lcd *set, lcd_lines *pattern_choice){
    set_screen_write(set, pattern_choice->line_one, pattern_choice->line_two);
}

void write_intro_screen(register_lcd *set, lcd_lines *intro){
    set_screen_write(set, intro->line_one, intro->line_two);
}

void set_lcd_initialisation(register_lcd *set){
    printf("\nInitialise LCD via Register.\n");

            // Initialization sequence
    set_lcd_rs(set, false);
      send_byte(set, 0x02);
    printf("\n\n\n\tRS Pin LOW:  Command Initialize.\n\n\n");
        sleep_ms(20); // Wait for more than 15ms
        
    send_byte(set, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(10); // Wait for more than 4.1ms

    send_byte(set, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(5); // Wait for more than 100us

    send_byte(set, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(20);

    send_lcd_command(set, (LCD_FUNCTION_SET | LCD_2LINE));
        printf("Initialisation Command: 0x%02x.\n", ((LCD_FUNCTION_SET | LCD_2LINE)));
            sleep_ms(20);

    send_lcd_command(set, (LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_ON));
        printf("Initialisation Command: 0x%02x.\n", ((LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_BLINK_ON)));
            sleep_ms(20);

    send_lcd_command(set, (LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT ));
        printf("Initialisation Command: 0x%02x.\n", ((LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT)));
            sleep_ms(20);

    set_lcd_cursor(set, 1, 0);
        printf("Setting LCD at home position\n");
            sleep_ms(20);

}

#endif


#endif