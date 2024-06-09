#ifndef _2004A_4BIT_WIRE_H
#define _2004A_4BIT_WIRE_H
//  Via a 595 shift register

#include <stdio.h>
#include "../../Periphereals/resources/pico_pin_enum.h"
#include "../../Periphereals/shift_registers.h"
#include "hardware/gpio.h"
#include "2004a_error_management.h"
#include "_2004A_commands.h"

#define low_b 0x40
#define med_low_b 0x50
#define medium_b 0x10
#define med_high_b 0x30
#define high_b 0x20

#define ebit uint8_t

#define lcd_rs_pin 8
#define lcd_e_pin 9

typedef struct lcd_line_data
{

  char line_one[20];
  char line_two[20];
  char line_three[20];
  char line_four[20];

} lcd_lines;

lcd_lines my_lcd_lines = {

  .line_one = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  .line_two = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  .line_three = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  .line_four = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

};

//  Function prototypes
uint8_t test_flip(uint8_t in_byte);
uint8_t uint_flip(uint8_t input_byte);
uint8_t half_byte_flip(uint8_t byte);
void print_binary(uint16_t byte_input);
func_akk setup_my_registers(register_pins *pins);
void clock_pulse(register_pins *clock_line);
void latch_register(register_pins *latch, bool which);
void register_nibble(register_pins *pins, ebit input_data);
void register_byte(register_pins *pins, ebit data_byte);
func_akk pico_com_to_lcd(register_pins *pins, ebit command);
func_akk pico_char_to_lcd(register_pins *pins, ebit character);
func_akk pico_to_clear_lcd(register_pins *pins);
func_akk pico_to_lcd_line(register_pins *pins, char *data, ebit line_pos);
func_akk pico_lcd_initialise(register_pins *pins);
func_akk pico_to_write_screen(register_pins *pins, char *l_one, char *l_two, char *l_three, char *l_four);
func_akk pico_to_default_screen(register_pins *pins, lcd_lines *line, uint16_t in_one, uint16_t in_two, uint16_t in_three, uint8_t colors);
void pico_to_return_lcd_home(register_pins *pins);
func_akk pico_to_fill_screen_with_char(register_pins *pins, ebit special_filler);
void rs_low(uint8_t pin);
void rs_high(uint8_t pin);
void e_pulse(void);


func_akk setup_my_registers(register_pins *pins){
  ebit status;
  status = 0;

  printf("Beginning Initialisation of GPIO --> Register Pins.\n");

  if (pins->register_one_data != UNDEFINED)
  {
      printf("Register One Data Pin : %i\n", pins->register_one_data);
    gpio_init(pins->register_one_data);
    gpio_set_dir(pins->register_one_data, GPIO_OUT);
    gpio_pull_down(pins->register_one_data);
  }
  else
  {
    printf("No Pin Set For Reg One DATA.\n");
  }
  status++;

  if (pins->register_one_latch != UNDEFINED)
  {
      printf("Register One Latch Pin : %i\n", pins->register_one_latch);
    gpio_init(pins->register_one_latch);
    gpio_set_dir(pins->register_one_latch, GPIO_OUT);
    gpio_pull_down(pins->register_one_latch);
  }
  else
  {
    printf("No Pin Set For Reg One LATCH.\n");
  }
  status++;

  if (pins->register_one_enable != UNDEFINED)
  {
      printf("Register One Enable Pin : %i\n", pins->register_one_enable);
    gpio_init(pins->register_one_enable);
    gpio_set_dir(pins->register_one_enable, GPIO_OUT);
    gpio_pull_down(pins->register_one_enable);
  }
  else
  {
    printf("No Pin Set For Reg One DATA.\n");
  }
  status++;

  if (pins->register_two_data != UNDEFINED)
  {
      printf("Register Two Data Pin : %i\n", pins->register_two_data);
    gpio_init(pins->register_two_data);
    gpio_set_dir(pins->register_two_data, GPIO_OUT);
    gpio_pull_down(pins->register_two_data);
  }
  else
  {
    printf("No Pin Set For Reg TWO DATA.\n");
  }
  status++;

  if (pins->register_two_latch != UNDEFINED)
  {
      printf("Register Two Latch Pin : %i\n", pins->register_two_latch);
    gpio_init(pins->register_two_latch);
    gpio_set_dir(pins->register_two_latch, GPIO_OUT);
    gpio_pull_down(pins->register_two_latch);
  }
  else
  {
    printf("No Pin Set For Reg One LATCH.\n");
  }
  status++;

  if (pins->register_two_enable != UNDEFINED)
  {
      printf("Register Two Enable Pin : %i\n", pins->register_two_enable);
    gpio_init(pins->register_two_enable);
    gpio_set_dir(pins->register_two_enable, GPIO_OUT);
    gpio_pull_down(pins->register_two_enable);
  }
  else
  {
    printf("No Pin Set For Reg One ENABLE.\n");
  }
  status++;

  if (pins->register_clk_line != UNDEFINED)
  {
    printf("Register Clock Line Pin : %i\n", pins->register_clk_line);
    gpio_init(pins->register_clk_line);
    gpio_set_dir(pins->register_clk_line, GPIO_OUT);
    gpio_pull_down(pins->register_clk_line);
  }
  else
  {
    printf("No Pin Set For Reg CLK.\n");
  }
  status++;

    printf("Quick Setup LCD: RS Pin : %i\n", lcd_rs_pin);
      gpio_init(lcd_rs_pin);
    gpio_set_dir(lcd_rs_pin, GPIO_OUT);
    gpio_pull_down(lcd_rs_pin);

    printf("Quick Setup LCD: E Pin : %i\n", lcd_e_pin);
      gpio_init(lcd_e_pin);
    gpio_set_dir(lcd_e_pin, GPIO_OUT);
    gpio_pull_down(lcd_e_pin);

    
  return ((status == 7) ? pin_init : err);
}


void clock_pulse(register_pins *clock_line){

  gpio_put(clock_line->register_clk_line, 1);
    sleep_us(5);
  gpio_put(clock_line->register_clk_line, 0);

}


void latch_register(register_pins *latch, bool which){

  if(!which){
    gpio_put(latch->register_one_latch, 1);
      sleep_us(5);
    gpio_put(latch->register_one_latch, 0);
      sleep_us(5);
  }else {
    gpio_put(latch->register_two_latch, 1);
      sleep_us(5);
    gpio_put(latch->register_two_latch, 0);
      sleep_us(5);
  }

}


void register_nibble(register_pins *pins, ebit input_data)
{

    gpio_put(pins->register_one_enable, 1);

        for (int i = 0; i < 8; i++)
      {
          gpio_put(pins->register_clk_line, 1);

          gpio_put(pins->register_one_data, (((input_data >> i) & 0x01)));
          (i != 7) ? printf("%i", ((input_data >> i) & 0x01)) : printf("%i\n", ((input_data >> i) & 0x01));
          
              sleep_us(5);
            gpio_put(pins->register_clk_line, 0);
            
          sleep_us(10);
      }
        latch_register(pins, false);
        clock_pulse(pins);
        gpio_put(pins->register_one_enable, 0);

}


void register_byte(register_pins *pins, ebit data_byte)
{

  ebit LSB, MSB;
  LSB, MSB= 0;
    printf("\n//// Input Byte: 0x%02x. ////\n", data_byte);
  
  MSB = ((data_byte >> 4) & 0x0F);
  LSB = (data_byte & 0x0F);
  
  printf("////  MSB: 0x%02x ////\n////  LSB: 0x%02x ////\n", MSB, LSB);

    gpio_put(lcd_e_pin, 0);
      sleep_us(10);
    register_nibble(pins, (MSB & 0x0F));
      sleep_us(10);
    gpio_put(lcd_e_pin, 1);
      sleep_us(10);

    gpio_put(lcd_e_pin, 0);
      sleep_us(10);
    register_nibble(pins, (LSB & 0x0F));
      sleep_us(10);
    gpio_put(lcd_e_pin, 1);
       sleep_us(5);

}


func_akk pico_com_to_lcd(register_pins *pins, ebit command)
{

//  printf("mcu->lcd: com\n");
  rs_low(lcd_rs_pin);
    sleep_us(50);
  register_byte(pins, command);

  return device_command_success;
}


func_akk pico_char_to_lcd(register_pins *pins, ebit character)
{
  
 // printf("mcu->lcd: char\n");
  rs_high(lcd_rs_pin);
    sleep_us(50);
    printf("Char: %c.\n", character);
  register_byte(pins, character);

  return device_data_success;
}


void pico_char_line_to_lcd(register_pins *pins, char *in){

  pico_to_return_lcd_home(pins);
    sleep_us(50);
  
  rs_high(lcd_rs_pin);
    sleep_us(50);
  for(int i = 0; i < 20; i++){

    register_byte(pins, in[i]);
      sleep_us(5);

  }

}

// Returns 3
func_akk pico_to_clear_lcd(register_pins *pins){

  ebit status;
  status = 0x00;

  status = pico_com_to_lcd(pins, LCD_CLEARDISPLAY);
    sleep_ms(5);

  return (status == 3) ? device_command_success : err;

}


void pico_to_return_lcd_home(register_pins *pins){

  printf("\n//\tReturning Home\t//\n");
    pico_com_to_lcd(pins, LCD_RETURNHOME);
  sleep_ms(5);
}


func_akk pico_to_fill_screen_with_char(register_pins *pins, ebit special_filler){

  ebit func_status;
  func_status = 0;

  func_status = pico_to_clear_lcd(pins);
    sleep_us(50);
  pico_to_return_lcd_home(pins);
    sleep_us(50);


  if(func_status == 3){
    func_status = 0;
    for(int i = 0; i < 80; i++){
      pico_char_to_lcd(pins, special_filler);
      func_status++;
    }
  }else {
    func_status = err;
  }

  return (func_status == 80) ? device_data_success : err;

}


//  Send command to set the line && position of the LCD cursor.
func_akk pico_set_lcd_cursor(register_pins *pins, ebit line, ebit position)
{

  printf("\n///\tSETTING LCD CURSOR POSITION\t///\n");
  printf("//\tTo Line: %i\n//\tPosition: %i\n", line, position);
  
  ebit s;
  s = 0;

  ebit output_position;
  output_position = 0;

  ebit offset_lines[] = {0x00, 0x40, 0x14, 0x54};
  output_position = (LCD_SETDDRAMADDR + ( offset_lines[line] + position ));
  printf("Output Line Position: 0x%02x.\n", output_position);
  s += pico_com_to_lcd(pins, output_position);
    sleep_us(500);

  return (s > 0) ? device_command_success : err;
}


//  Write character data to selected line.
func_akk pico_to_lcd_line(register_pins *pins, char *data, ebit line_pos)
{

  pico_set_lcd_cursor(pins, line_pos, 0);

  printf("\n//\t\tPICO_TO_LCD_LINE\t\t//\n");
  ebit status;
  status = 0;

  for (int i = 0; i < 20; i++)
  {
    printf("Current Data: 0x%02x.\n", data[i]);
    pico_char_to_lcd(pins, data[i]);
      sleep_us(5);
    status += 1;
  }

  printf("//STATUS_VALUE:// %i.\n", status);
  return (status > 0) ? device_data_success : err;
}


//  Send initialisation commands to the lcd.
func_akk pico_lcd_initialise(register_pins *pins){
  ebit status;
  status = 0;

  printf("\n//LCD INITIALISE//\n");

  pico_com_to_lcd(pins, (0x02 | 0x80));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_FUNCTIONSET | LCD_2LINE)));
    sleep_ms(20);
  pico_com_to_lcd(pins, (0x08 | 0x04));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_DISPLAYON | LCD_CURSORON)));
    sleep_ms(20);
  pico_com_to_lcd(pins, (0x04 | 0x02));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_ENTRYMODESET | LCD_ENTRYLEFT)));
  sleep_ms(20);
  pico_com_to_lcd(pins, (0x01));
    status += 1;
    printf("Return Home?\n");

  return (status == 4) ? device_command_success : err;
}


//  Write data to the full screen, by lines.
func_akk pico_to_write_screen(register_pins *pins, char *l_one, char *l_two, char *l_three, char *l_four)
{
  ebit status;
  status = 0;

  status += pico_to_lcd_line(pins, l_one, 0);
    sleep_us(5);
  
  status += pico_to_lcd_line(pins, l_two, 1);
    sleep_us(5);

  status += pico_to_lcd_line(pins, l_three, 2);
    sleep_us(5);

  status += pico_to_lcd_line(pins, l_four, 3);
    sleep_us(5);

  return (status == 12) ? device_data_success : err;
}


func_akk pico_to_default_screen(register_pins *pins, lcd_lines *line, uint16_t in_one, uint16_t in_two, uint16_t in_three, uint8_t colors)
{
  uint8_t status;
  status = 0;

  sprintf(line->line_one, "__RC Cont: Active__");
  status += 1;
  sprintf(line->line_two, "Ver: %i Hor: %i", in_one, in_two);
  status += 1;
  sprintf(line->line_three, "VRoom Amount: %04x", in_three);
  status += 1;
  sprintf(line->line_four, "FL: %02x RL: %02x ", (colors & 0xF0), (colors & 0x0F));
  status += 1;
  printf("Status: %i.\n", status);

    pico_to_return_lcd_home(pins);
      sleep_us(50);

  //  pico_to_clear_lcd(pins);
      sleep_us(10);


  status += pico_to_write_screen(pins, line->line_one, line->line_two, line->line_three, line->line_four);
  printf("Status: %i.\n", status);
  printf("\n\t\t////  ////  ////  ////  ////  ////  ////  ////\n");
  printf("..\t\t\tLCD Screen Estimated Display\t\t\t..\n");
  printf("\t\tRC Controller: Active\n");
  printf("\t\tVer: %04x| Hor: %04x\n", in_one, in_two);
  printf("\t\tVRoom Amount:P %04x\n", in_three);
  printf("\t\tHead: %02xRear: %02x\n", (colors & 0xF0), (colors & 0x0F));
printf("\n");
  printf("\t\t////  ////  ////  ////  ////  ////  ////  ////\n");

  return (status == 8) ? device_data_success : err;
}


void print_binary(uint16_t byte_input) {
  
    printf("\n");
      
      for (int i = 8; i > 0; i--) {
        
        if ((byte_input >> i) & 0x01) {
          printf("1");
        }else {
          printf("0");
        }
      }
    printf("\n");
}

//  Flip a uint8_t value.  example in: 0xD7 out: 0x7D
uint8_t uint_flip(uint8_t input_byte){
  uint8_t flipped_val;
  flipped_val = 0x00;

  flipped_val += (0x01 & (input_byte >> 7)) ? (0x08) : (0x00);
  flipped_val += (0x01 & (input_byte >> 6)) ? (0x04) : (0x00);
  flipped_val += (0x01 & (input_byte >> 5)) ? (0x02) : (0x00);
  flipped_val += (0x01 & (input_byte >> 4)) ? (0x01) : (0x00);

  flipped_val += (0x01 & (input_byte >> 3)) ? (0x80) : (0x00);
  flipped_val += (0x01 & (input_byte >> 2)) ? (0x40) : (0x00);
  flipped_val += (0x01 & (input_byte >> 1)) ? (0x20) : (0x00);
  flipped_val += (0x01 & (input_byte >> 0)) ? (0x10) : (0x00);

  return flipped_val;
}

uint8_t test_flip(uint8_t in_byte){
  uint8_t flipped_val;
  flipped_val = 0x00;

  flipped_val += (0x01 & (in_byte >> 7)) ? (0x10) : (0x00);
  flipped_val += (0x01 & (in_byte >> 6)) ? (0x20) : (0x00);
  flipped_val += (0x01 & (in_byte >> 5)) ? (0x40) : (0x00);
  flipped_val += (0x01 & (in_byte >> 4)) ? (0x80) : (0x00);

  flipped_val += (0x01 & (in_byte >> 3)) ? (0x01) : (0x00);
  flipped_val += (0x01 & (in_byte >> 2)) ? (0x02) : (0x00);
  flipped_val += (0x01 & (in_byte >> 1)) ? (0x04) : (0x00);
  flipped_val += (0x01 & (in_byte >> 0)) ? (0x08) : (0x00);

  return flipped_val;

}


uint8_t half_byte_flip(uint8_t byte){

uint8_t half_byte;
half_byte = 0x00;

  half_byte += (byte & 0x08) ? (0x01) : (0x00);
  half_byte += (byte & 0x04) ? (0x02) : (0x00);
  half_byte += (byte & 0x02) ? (0x04) : (0x00);
  half_byte += (byte & 0x01) ? (0x08) : (0x00);

  return half_byte;

}


void rs_low(uint8_t pin){

  gpio_put(pin, 0);

}

void rs_high(uint8_t pin){

  gpio_put(pin, 1);

}

void e_pulse(void){
  gpio_put(lcd_e_pin, 1);
    sleep_us(50);
  gpio_put(lcd_e_pin, 0);
}

#endif
