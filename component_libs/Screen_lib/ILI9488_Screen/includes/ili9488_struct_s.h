#ifndef ILI9488_STRUCT_S_H
#define ILI9488_STRUCT_S_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../../Periphereals/resources/pico_pin_enum.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"


#define bit uint8_t
#define ebit uint8_t
#define sbit uint16_t
#define len size_t
#define baud uint32_t
#define tbit uint32_t


typedef struct spi_packet_s {
    
    spi_inst_t *instance;
    ebit tx_buf[1];
    ebit rx_buf[1];
    baud rate;
    ebit length;
    ebit spi_func_status;

}spi_packet_s;


typedef struct ili_font_s {

    ebit f_width;
    ebit f_height;

}i_font;


typedef struct ili9488_window_var_s {

    sbit dw_X;
    sbit dw_Y;
    sbit dw_Width;
    sbit dw_Height;

}ili9488_window_var_s;


typedef struct ili_string_s {
    sbit x_start;
    sbit y_start;
    ebit *string_ptr;

}i_string;


typedef struct pixel_map_s {

    tbit x_start;
    tbit y_start;
    tbit map_width;
    tbit map_height;
    const sbit *pixel_map_ptr;

}pix_map;


typedef struct ili_limit_s {

    sbit x_start;
    sbit y_start;
    sbit x_end;
    sbit y_end;

}limits;


typedef struct ili_operation_var_s {

    tbit ili_width;
    tbit ili_height;
    tbit ili_foreground_color;
    tbit ili_background_color;  
	
}ili_operation_var_s;


typedef struct spi_pin_manager_s {

    uint8_t miso;
    uint8_t mosi;
    uint8_t csn;
    uint8_t sck;
    uint8_t dc_rs;
    uint8_t reset;

}spi_pins;

typedef struct spi_buffer_function_t {
  spi_packet_s* (*spi_buffer)(spi_packet_s *temp, uint8_t data);
}spib;

typedef struct init_var_s {
    
  struct ili_operation_var_s *my_op;
  struct spi_packet_s *data_pack;
  struct ili9488_window_var_s *my_window;
  struct spi_pin_manager_s *my_pins;

}init_var_s;

typedef struct ili9488_init_function_s {

  func_ack (*pos_g_ctrl)(spi_packet_s *pack, uint8_t com, const uint8_t *gam_data);
  func_ack (*neg_g_ctrl)(spi_packet_s *pack,  uint8_t com, const uint8_t *data);
  func_ack (*pwr_ctrl_o)(spi_packet_s *pack,  uint8_t com, const uint8_t *data);
  func_ack (*pwr_ctrl_t)(spi_packet_s *pack, uint8_t com, const uint8_t *data);
  func_ack (*vcom_ctrl)(spi_packet_s *pack, uint8_t com, const uint8_t *data);
  func_ack (*mem_acc_ctrl)(spi_packet_s *pack, uint8_t com, uint8_t data);
  func_ack (*pix_intr_format)(spi_packet_s *pack, uint8_t com, uint8_t data);
  func_ack (*intr_mode_ctrl)(spi_packet_s *pack, uint8_t com, uint8_t data);
  func_ack (*frame_rate_ctrl)(spi_packet_s *pack, uint8_t com, uint8_t data);
  func_ack (*display_inversion)(spi_packet_s *pack, uint8_t com, uint8_t data);
  func_ack (*display_func_ctrl)(spi_packet_s *pack, uint8_t com, const uint8_t *data);
  func_ack (*sleep_out)(spi_packet_s *pack, uint8_t com);
  func_ack (*display_on)(spi_packet_s *pack, uint8_t com);

}ili9488_funcs;

#endif