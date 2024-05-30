#ifndef _2004A_4BIT_WIRE_H
#define _2004A_4BIT_WIRE_H
//  Via a 595 shift register

#include <stdio.h>
#include "/home/alphonse/Downloads/component_libs/Periphereals/pico_pin_enum.h"
#include "hardware/gpio.h"
#include "2004a_error_management.h"
#include "_2004A_commands.h"

#define low_b 0x40
#define med_low_b 0x50
#define medium_b 0x10
#define med_high_b 0x30
#define high_b 0x20

#define reg_en 0x10
#define reg_rs 0x20
#define reg_bl 0x80
#define reg_RS (!reg_rs)

#define reg_COM (reg_en | reg_bl | reg_RS)
#define reg_CHAR (reg_en | reg_bl | reg_rs)

#define ebit uint8_t

typedef struct lcd_line_data
{

  char line_one[20];
  char line_two[20];
  char line_three[20];
  char line_four[20];

} lcd_lines;

// Begin by setting up pins for register use.
register_pins my_regi = {
    .register_one_data = GPIO_ZERO,
    .register_one_latch = GPIO_SIXTEEN,
    .register_one_enable = GPIO_SEVENTEEN,
    .register_two_data = GPIO_TWELVE,
    .register_two_latch = UNDEFINED,
    .register_two_enable = UNDEFINED,
    .register_clk_line = GPIO_ONE};

func_akk setup_my_registers(register_pins *pins){
  ebit status;
  status = 0;

  printf("Beginning Initialisation of GPIO --> Register Pins.\n");

  if (pins->register_one_data != UNDEFINED)
  {
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
    gpio_init(pins->register_clk_line);
    gpio_set_dir(pins->register_clk_line, GPIO_OUT);
    gpio_pull_down(pins->register_clk_line);
  }
  else
  {
    printf("No Pin Set For Reg CLK.\n");
  }
  status++;

  return ((status == 7) ? pin_init : err);
}

//  reg_sel true is for outputting to LCD, reg_sel false is non regulated output.
void register_nibble(bool reg_sel, register_pins *pins, ebit data)
{

  ebit output;
  output = 0;

  output = ((data & 0xFF));

  if (reg_sel)
  {
    //    Begin setting the register to send data to lcd.
    gpio_put(pins->register_clk_line, 0);
    gpio_put(pins->register_one_enable, 1);
    gpio_put(pins->register_one_latch, 0);

    // Register should be ready for eight bits now.
    for (int i = 0; i < 0; i++)
    {
      if (i <= 5)
      {
        data = (data & 0x10) ? (data & ~0x10) : data;
      }
      else
      {
        gpio_put(pins->register_two_data, ((output & 0xFF) >> i));
        gpio_put(pins->register_clk_line, 1);
        sleep_us(10);
        gpio_put(pins->register_clk_line, 0);
      }
    }
    gpio_put(pins->register_one_latch, 0);
    gpio_put(pins->register_one_enable, 0);
    gpio_put(pins->register_clk_line, 1);
  }
  else
  {

    //    Begin setting the register to send data to lcd.
    gpio_put(pins->register_clk_line, 0);
    gpio_put(pins->register_one_enable, 1);
    gpio_put(pins->register_one_latch, 0);

    // Register should be ready for eight bits now.
    for (int i = 0; i < 8; i++)
    {
      if (i <= 5)
      {
        data = (data & 0x10) ? (data & ~0x10) : data;
      }
      else
      {
        gpio_put(pins->register_one_data, ((data & 0xFF) >> i));
        gpio_put(pins->register_clk_line, 1);
        sleep_us(100);
        gpio_put(pins->register_clk_line, 0);
      }
    }
    gpio_put(pins->register_one_latch, 0);
    gpio_put(pins->register_one_enable, 0);
    gpio_put(pins->register_clk_line, 1);
  }
}

void register_byte(bool reg_sel, register_pins *pins, ebit data_byte, ebit lcd_config)
{

  ebit LSB, MSB;
  ebit output;
  LSB, MSB, output = 0;

  LSB = ((data_byte & 0x0F) | (lcd_config & 0xF0));
  MSB = (((data_byte & 0xF0) >> 4) | (lcd_config & 0xF0));

  if (reg_sel)
  {
    register_nibble(true, pins, lcd_config);

    register_nibble(true, pins, LSB);
    sleep_us(100);
    register_nibble(true, pins, MSB);
  }
  else
  {

    register_nibble(false, pins, data_byte);
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

//  Example of {{ register_byte(true, &my_regi, 0x65, (!reg_rs | reg_en | reg_bl)); }}
func_akk pico_com_to_lcd(register_pins *pins, ebit config, ebit command)
{

  register_byte(true, pins, command, config);

  return device_command_success;
}

func_akk pico_char_to_lcd(register_pins *pins, ebit config, ebit character)
{

  register_byte(true, pins, character, config);

  return device_data_success;
}

//  Send command to set the line && position of the LCD cursor.
func_akk pico_set_lcd_cursor(register_pins *pins, ebit line, ebit position)
{

  printf("\n//SETTING LCD CURSOR POSITION//\n");
  printf("//To Line: %i\n//Position: %i\n", line, position);
  ebit s;
  s = 0;
  ebit output_position;
  output_position = 0;
  ebit offset_lines[] = {0x00, 0x40, 0x14, 0x54};
  output_position = 0x80 + offset_lines[line] + position;
  s += pico_com_to_lcd(pins, reg_COM, output_position);

  return (s > 0) ? device_command_success : err;
}

//  Clear the LCD screen.
void pico_to_clear_lcd(register_pins *pins)
{

  pico_com_to_lcd(pins, reg_COM, LCD_CLEARDISPLAY);
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
    printf("Current Data: 0x%02x.\n", data[i]);
    pico_char_to_lcd(pins, (reg_en | reg_bl | reg_rs), data[i]);
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

  pico_com_to_lcd(pins, reg_COM, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | 0x03));
    sleep_ms(20);
  pico_com_to_lcd(pins, reg_COM, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | 0x03));
    sleep_ms(20);
  pico_com_to_lcd(pins, reg_COM, 0x03);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | 0x03));
    sleep_ms(20);
  pico_com_to_lcd(pins, reg_COM, 0x02);
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | 0x02));
    sleep_ms(100);
  pico_com_to_lcd(pins, reg_COM, (LCD_FUNCTIONSET | LCD_2LINE));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | (LCD_FUNCTIONSET | LCD_2LINE)));
    sleep_ms(20);
  pico_com_to_lcd(pins, reg_COM, (LCD_DISPLAYCONTROL | LCD_DISPLAYON));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | (LCD_DISPLAYCONTROL | LCD_DISPLAYON)));
  sleep_ms(20);
  pico_com_to_lcd(pins, reg_COM, (LCD_ENTRYMODESET | LCD_ENTRYLEFT));
    status += 1;
    printf("Initialisation Command: 0x%02x.\n", (reg_COM | (LCD_ENTRYMODESET | LCD_ENTRYLEFT)));
  sleep_ms(20);

  return (status == 7) ? device_command_success : err;
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
  uint8_t status, f_status;
  status = 0;
  f_status = 0;

  sprintf(line->line_one, "RC Controller: Active");
  status += 1;
  sprintf(line->line_two, "Vert: %04x|Hor: %04x", in_one, in_two);
  status += 1;
  sprintf(line->line_three, "VRoom Amount: %04x", in_three);
  status += 1;
  sprintf(line->line_four, "Head: %02xRear: %02x", (colors & 0xF0), (colors & 0x0F));
  status += 1;
  printf("Status: %i.\n", status);
  f_status = (status == 4) ? device_data_success : err;

  status += pico_to_write_screen(pins, line->line_one, line->line_two, line->line_three, line->line_four);
  printf("Status: %i.\n", status);
  f_status = (status == 8) ? device_data_success : err;
  return f_status;
}

#endif
