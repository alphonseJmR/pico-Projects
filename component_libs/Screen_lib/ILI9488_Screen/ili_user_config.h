#ifndef ILI9488_USER_HIGH_LEVEL_CONFIG_S_H
#define ILI9488_USER_HIGH_LEVEL_CONFIG_S_H

#include <stdio.h>
#include <stdint.h>
#include "resources/ili9488_screen_commands.h"
#include "resources/ili9488_error_management.h"
#include "resources/ili9488_power_control.h"
#include "includes/ili9488_char_set.h"
#include "includes/ili9488_struct_s.h"
#include "hardware/spi.h"

#define bit uint
#define ebit uint8_t
#define sbit uint16_t
#define len size_t
#define baud uint32_t
#define tbit uint32_t

#define get_0b_to_8b(x)   ((ebit)((x & 0xFFFFFFFF) & 0x000000FF))
#define get_8b_to_16b(x)  ((ebit)((x & 0xFFFFFFFF) & 0x0000FF00))
#define get_16b_to_24b(x) ((ebit)((x & 0xFFFFFFFF) & 0x00FF0000))
#define get_24b_to_32b(x) ((ebit)((x & 0xFFFFFFFF) & 0xFF000000))

#define ILI_LCD_WIDTH 320
#define ILI_LCD_HEIGHT 480
#define lcd_data_cache_size ILI_LCD_WIDTH
#define max_iterations (int)(ILI_LCD_WIDTH * ILI_LCD_HEIGHT)

ebit pixel_cache[(max_iterations / 3)][3];
ebit *com;
ebit *COM;
ebit *COMM;
ebit *DAT;
ebit *DATA;
ebit *dat;
ebit *d_one;
ebit *d_two;
ebit *d_three;
ebit *d_four;

const uint8_t init_comms[13] = {

  0xE0,   //  Positive Gamma Control
  0xE1,   //  Negative Gamma Control
  0xC0,   //  Power Control O
  0xC1,   //  Power Control T
  0xC5,   //  VCOM Control
  0x36,   //  Mem Acc Control
  0x3A,   //  Pix Intr Format
  0xB0,   //  Inter Mode Control
  0xB1,   //  Frame Rate Control
  0xB4,   //  Display Inversion
  0xB6,   //  Display Function Control
  0x11,   //  Sleep Out
  0x29,   //  Display On

};

const uint8_t pos_gamma_dat[15] = {

  0x00,
  0x03,
  0x09,
  0x08,
  0x16,
  0x0A,
  0x3F,
  0x78,
  0x4C,
  0x09,
  0x0A,
  0x08,
  0x16,
  0x1A,
  0x0F

};

uint8_t neg_gamma_dat[15] = {

  0x00,
  0x16,
  0x19,
  0x03,
  0x0F,
  0x05,
  0x32,
  0x45,
  0x46,
  0x04,
  0x0E,
  0x0D,
  0x35,
  0x37,
  0x0F

};

uint8_t pwr_con_o[2] = {
  0x17,
  0x15
};

uint8_t pwr_con_t[1] = {
  0x41
};

uint8_t vcom_ctrl[3] = {
  0x00,
  0x12,
  0x80
};

uint8_t display_func_ctrl[3] =
{
  0x02,
  0x02,
  0x3B
};


struct ili_font_s my_font = {

  .f_width = 10,
  .f_height = 14

};


ili_operation_var_s my_op = {

.ili_width = ILI_LCD_WIDTH,
.ili_height = ILI_LCD_HEIGHT,
.ili_foreground_color = 0x006600,
.ili_background_color = 0x0039E6

};


spi_packet_s data_packet = {
  .instance = NULL,
  .tx_buf[0] = 0,
  .rx_buf[0] = 0,
  .rate = ONE_MBS,
  .length = 0,
  .spi_func_status = 0

};

limits my_limits = {

  .x_start = 0,
  .y_start = 0,
  .x_end = ILI_LCD_WIDTH,
  .y_end = ILI_LCD_HEIGHT

};



ili9488_window_var_s my_window = {

.dw_X = 20,
.dw_Y = 20,
.dw_Width = ILI_LCD_WIDTH,
.dw_Height= ILI_LCD_HEIGHT

};


spi_pins my_pins = {

  .miso = GPIO_TWELVE,
  .mosi = GPIO_ELEVEN,
  .csn = GPIO_THIRTEEN,
  .sck = GPIO_TEN,
  .dc_rs = GPIO_FOURTEEN,
  .reset = GPIO_FIFTEEN

};

init_var_s my_init = {

  .my_op = &my_op,
  .data_packet = &data_packet,
  .my_window = &my_window,
  .my_pins = &my_pins,
  .spi_buffer = spi_buffer

};

struct spi_packet_s spi_buffer(uint8_t data){

  spi_packet_s temp_pack =
  {
    .instance = my_init.data_packet,
    .tx_buf[0] = data,
    .rx_buf[0] = 0,
    .rate = ONE_MBS,
    .length = 0,
    .spi_func_status = 0

  };

  return temp_pack;

}


#endif