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

#define lcd_rs 8
#define lcd_e 9

#define lcd_A 10
#define lcd_B 7
#define lcd_C 19
#define lcd_D 20


typedef struct lcd_line_data
{

  char line_one[20];
  char line_two[20];
  char line_three[20];
  char line_four[20];

} lcd_lines;

// Begin by setting up pins for register use.
register_pins my_regi = {
    .register_one_data = GPIO_TWELVE,
    .register_one_latch = GPIO_SIXTEEN,
    .register_one_enable = GPIO_SEVENTEEN,
    .register_two_data = GPIO_ZERO,
    .register_two_latch = UNDEFINED,
    .register_two_enable = UNDEFINED,
    .register_clk_line = GPIO_ONE
    
    };

//  Function prototypes

uint8_t uint_flip(uint8_t input_byte);
void print_binary(uint16_t byte_input);
func_akk setup_my_registers(register_pins *pins);
void clock_pulse(register_pins *clock_line);
void latch_register(register_pins *latch, bool which);
void register_nibble(register_pins *pins, ebit input_data);
void register_byte(bool reg_sel, register_pins *pins, ebit data_byte, bool Chomar);
uint8_t char_to_uint_t(char *inputs, uint8_t *output);
func_akk pico_com_to_lcd(register_pins *pins, ebit command);
func_akk pico_char_to_lcd(register_pins *pins, ebit character);
func_akk pico_to_clear_lcd(register_pins *pins);
func_akk pico_to_lcd_line(register_pins *pins, ebit *data, ebit line_pos);
func_akk pico_lcd_initialise(register_pins *pins);
func_akk pico_to_write_screen(register_pins *pins, char *l_one, char *l_two, char *l_three, char *l_four);
func_akk pico_to_default_screen(register_pins *pins, lcd_lines *line, uint16_t in_one, uint16_t in_two, uint16_t in_three, uint8_t colors);
func_akk pico_to_fill_screen_with_char(register_pins *pins, ebit special_filler);


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

    printf("Quick Setup LCD: RS Pin : %i\n", lcd_rs);
      gpio_init(lcd_rs);
    gpio_set_dir(lcd_rs, GPIO_OUT);
    gpio_pull_down(lcd_rs);

    printf("Quick Setup LCD: E Pin : %i\n", lcd_e);
      gpio_init(lcd_e);
    gpio_set_dir(lcd_e, GPIO_OUT);
    gpio_pull_down(lcd_e);

    
  return ((status == 7) ? pin_init : err);
}


void clock_pulse(register_pins *clock_line){

  gpio_put(clock_line->register_clk_line, 1);
    sleep_us(50);
  gpio_put(clock_line->register_clk_line, 0);

}


void latch_register(register_pins *latch, bool which){

  if(!which){
    gpio_put(latch->register_one_latch, 1);
      sleep_us(50);
    gpio_put(latch->register_one_latch, 0);
      sleep_us(50);
  }else {
    gpio_put(latch->register_two_latch, 1);
      sleep_us(50);
    gpio_put(latch->register_two_latch, 0);
      sleep_us(50);
  }


}


//  reg_sel false is for outputting to LCD, reg_sel true is non regulated output.
//  Chomar is the COM/CHAR command bitspace.
void register_nibble(register_pins *pins, ebit input_data)
{

  // ebit test_flipp;
  // test_flipp = 0x00;

    gpio_put(pins->register_one_enable, 1);
    input_data = (input_data | 0x30);
  //  print_binary(input_data);

    // test_flipp += uint_flip(input_data);
    // print_binary(test_flipp);
        for (int i = 0; i < 8; i++)
      {
          gpio_put(pins->register_one_data, (((input_data >> i) & 0x01) ? 1 : 0) );
            clock_pulse(pins);
          sleep_us(75);
      }
        latch_register(pins, false);
        clock_pulse(pins);
        gpio_put(pins->register_one_enable, 0);

}


void register_byte(bool reg_sel, register_pins *pins, ebit data_byte, bool Chomar)
{

  ebit LSB, MSB;
  ebit output;
  LSB, MSB, output = 0;

  LSB = (data_byte & 0x0F);
  MSB = ((data_byte & 0xF0) >> 4);

  if(reg_sel)
  {
  if(!Chomar){
    //  Send LSB of input data first, for command RS go LOW, e go high, send reg, e go low, rs go high.
    //  This is the COMMAND conditional.
    gpio_put(lcd_rs, 0);
      sleep_us(5);
    gpio_put(lcd_e, 1);
      sleep_us(5);
    register_nibble(pins, LSB);
      sleep_us(5);
    gpio_put(lcd_e, 0);
      sleep_us(10);
    //  Send MSB of input data.  Format follows from LSB.
    gpio_put(lcd_e, 1);
      sleep_us(5);
    register_nibble(pins, MSB);
      sleep_us(5);
    gpio_put(lcd_e, 0);
       sleep_us(5);
    gpio_put(lcd_rs, 1);
      sleep_us(10);

  }else {

    //  Send LSB of input data first, for character RS go HIGH, e go high, send reg, e go low, rs go LOW.
    //  This is the CHARACTER conditional.
    gpio_put(lcd_rs, 1);
      sleep_us(5);
    gpio_put(lcd_e, 1);
      sleep_us(5);
    register_nibble(pins, LSB);
      sleep_us(5);
    gpio_put(lcd_e, 0);
      sleep_us(10);
    //  Send MSB of input data.  Format follows from LSB.
    gpio_put(lcd_rs, 1);
    sleep_us(5);
    gpio_put(lcd_e, 1);
      sleep_us(5);
    register_nibble(pins, MSB);
      sleep_us(5);
    gpio_put(lcd_e, 0);
       sleep_us(5);
    gpio_put(lcd_rs, 0);
      sleep_us(10);

  }
  }
  else
  {
    register_nibble(pins, data_byte);
  }
}


uint8_t char_to_uint_t(char *inputs, uint8_t *output)
{
  for (int i = 0; i < 20; i++)
  {
    output[i] = 0x00;
  }
  for (int j = 0; j < 20; j++)
  {
    output[j] = (uint8_t)inputs[j];
  }
}



func_akk pico_com_to_lcd(register_pins *pins, ebit command)
{
//  printf("mcu->lcd: com\n");

  register_byte(true, pins, command, false);

  return device_command_success;
}


func_akk pico_char_to_lcd(register_pins *pins, ebit character)
{
 // printf("mcu->lcd: char\n");

  register_byte(true, pins, character, true);

  return device_data_success;
}


func_akk pico_to_clear_lcd(register_pins *pins){

  ebit status;
  status = 0x00;

  status = pico_com_to_lcd(pins, LCD_CLEARDISPLAY);
    sleep_ms(5);

  return (status == 3) ? device_command_success : err;

}


func_akk pico_to_fill_screen_with_char(register_pins *pins, ebit special_filler){

  ebit func_status;
  func_status = 0;

  func_status = pico_to_clear_lcd(pins);

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
//  One or Zero for line, as there's only two "memory" lines on the lcd
//  Position will be + 20 of the first or second memory lines.
//  There will be a "test" input position
func_akk pico_set_lcd_cursor(register_pins *pins, ebit line, ebit position)
{

  printf("\n//SETTING LCD CURSOR POSITION//\n");
  printf("//To Line: %i\n//Position: %i\n", line, position);
  
  ebit s;
  s = 0;

  ebit output_position;
  output_position = 0;
  ebit test_position;
  test_position = 15;

  ebit offset_lines[] = {0x00, 0x40, 0x14, 0x54};
 // ebit offset_position[] = {(0x00 + 20), (0x40 + 20), (0x00 + test_position)};
  output_position = (LCD_SETDDRAMADDR | ( offset_lines[line] + position ));
  printf("Output Line Position: 0x%02x.\n", output_position);

  // set dd_ramaddr 0x80

  s += pico_com_to_lcd(pins, output_position);

  return (s > 0) ? device_command_success : err;
}


//  Write character data to selected line.
func_akk pico_to_lcd_line(register_pins *pins, ebit *data, ebit line_pos)
{

  pico_set_lcd_cursor(pins, line_pos, 0);

  printf("\n//PICO_TO_LCD_LINE//\n");
  ebit status;
  status = 0;

  for (int i = 0; i < 20; i++)
  {
  //  printf("Current Data: 0x%02x.\n", data[i]);
    pico_char_to_lcd(pins, data[i]);
      sleep_ms(1);
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



  register_nibble(pins, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (0x03));
    sleep_ms(20);
  register_nibble(pins, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (0x03));
    sleep_ms(20);
  register_nibble(pins, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (0x03));
    sleep_ms(20);
  register_nibble(pins, 0x02);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (0x02));
    sleep_ms(100);

  pico_com_to_lcd(pins, (LCD_FUNCTIONSET | LCD_2LINE));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_FUNCTIONSET | LCD_2LINE)));
    sleep_ms(20);
  pico_com_to_lcd(pins, (LCD_DISPLAYCONTROL | LCD_DISPLAYON));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_DISPLAYCONTROL | LCD_DISPLAYON)));

  pico_to_clear_lcd(pins);
  sleep_ms(20);
  pico_com_to_lcd(pins, (LCD_ENTRYMODESET | LCD_ENTRYLEFT));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", ((LCD_ENTRYMODESET | LCD_ENTRYLEFT)));
  sleep_ms(20);

  return (status == 8) ? device_command_success : err;
}


//  Write data to the full screen, by lines.
func_akk pico_to_write_screen(register_pins *pins, char *l_one, char *l_two, char *l_three, char *l_four)
{

  ebit q_one[20], q_two[20], q_three[20], q_four[20];
  ebit status;
  status = 0;

  for (int j = 0; j < 20; j++)
  {
    q_one[j] = 0x00;
    q_two[j] = 0x00;
    q_three[j] = 0x00;
    q_four[j] = 0x00;
  }

  char_to_uint_t(l_one, q_one);
  char_to_uint_t(l_two, q_two);
  char_to_uint_t(l_three, q_three);
  char_to_uint_t(l_four, q_four);

  pico_to_clear_lcd(pins);
  status += 1;

  pico_to_lcd_line(pins, q_one, 0);
  status += 1;

  pico_to_lcd_line(pins, q_two, 1);
  status += 1;

  pico_to_lcd_line(pins, q_three, 2);
  status += 1;

  pico_to_lcd_line(pins, q_four, 3);
  status += 1;

  return (status >= 5) ? device_data_success : err;
}


func_akk pico_to_default_screen(register_pins *pins, lcd_lines *line, uint16_t in_one, uint16_t in_two, uint16_t in_three, uint8_t colors)
{
  uint8_t status;
  status = 0;

  sprintf(line->line_one, "RC Controller: Active");
  status += 1;
  sprintf(line->line_two, "Ver: %04x| Hor: %04x", in_one, in_two);
  status += 1;
  sprintf(line->line_three, "VRoom Amount: %04x", in_three);
  status += 1;
  sprintf(line->line_four, "Head: %02xRear: %02x", (colors & 0xF0), (colors & 0x0F));
  status += 1;
  printf("Status: %i.\n", status);


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

  uint8_t uint_flip(uint8_t input_byte){
  uint8_t flipped_val;
  flipped_val = 0x00;

/*
  flipped_val += (0x01 & (input_byte >> 7)) ? (0x01 << 0) : (0x00 << 0);
  flipped_val += (0x01 & (input_byte >> 6)) ? (0x01 << 1) : (0x00 << 1);
  flipped_val += (0x01 & (input_byte >> 5)) ? (0x01 << 2) : (0x00 << 2);
  flipped_val += (0x01 & (input_byte >> 4)) ? (0x01 << 3) : (0x00 << 3);

  flipped_val += (0x01 & (input_byte >> 3)) ? (0x01 << 7) : (0x00 << 7);
  flipped_val += (0x01 & (input_byte >> 2)) ? (0x01 << 6) : (0x00 << 6);
  flipped_val += (0x01 & (input_byte >> 1)) ? (0x01 << 5) : (0x00 << 5);
  flipped_val += (0x01 & (input_byte >> 0)) ? (0x01 << 4) : (0x00 << 4);
 */ 
  for(int i = 0; i < 8; i++){

    flipped_val += (0x01 & (input_byte >> i)) ? (0x01 << ~i) : (0x00 << ~i);

  }

  return flipped_val;
}

#endif
