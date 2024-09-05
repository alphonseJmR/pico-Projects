#ifndef ILI9488_FUNCTION_PROTO_S_H
#define ILI9488_FUNCTION_PROTO_S_H

#include "ili9488_struct_s.h"
#include "../resources/ili9488_error_management.h"


//  Function Prototypes for PIN usage.
static func_ack pin_validate(spi_pins *pins);
spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins);
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins);
func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins);
func_ack gpio_setup(spi_pins *pins);

//  Functions for hardware pins, CSn, DC/RS, RESET
//  and dual call functions between the pins.
void csn_put_high(bit csn);
void csn_put_low(bit csn);
void dcrs_put_high(bit ce);
void dcrs_put_low(bit ce);
void reset_put_high(bit rs);
void reset_put_low(bit rs);
void reset_pulse(bit rs);
void com_start(spi_pins *pins);
void data_start(spi_pins *pins);
void data_end(spi_pins *pins);

// func_ack internal_write_function(init_var_s *ili, uint8_t input);
func_ack internal_write_function(init_var_s *ili, uint8_t input, bool bCbD);
spi_packet_s* spi_buffer(spi_packet_s *temp, uint8_t data);
func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate);
func_ack deinitialize_spi_management(spi_inst_t *instance);
ebit spi_length(ebit input);
// ebit spi_lpa_command_write(spi_packet_s *inst, ebit command, ebit param);
func_ack spi_spa_command_write(spi_packet_s *inst, ebit command, ebit param);
func_ack spi_spa_data_write(spi_packet_s *inst, ebit command, tbit data);
ebit spi_read_data(spi_packet_s *inst);
func_ack ili_delay(uint32_t wait_value);

func_ack ili_check_coords(limits *coords);
func_ack ili_set_init_window(ili9488_window_var_s *window, spi_packet_s *pack);
func_ack ili_set_user_window(spi_packet_s *pack, sbit xs, sbit xe, sbit ys, sbit ye);
// func_ack ili_set_foreground_color(ili9488_window_var_s *window, spi_packet_s *pack, uint32_t color_i);
func_ack ili_set_background_color(spi_packet_s *pack, uint32_t color_i);
func_ack ili_fill_color(spi_packet_s *pack, ebit ba_pixels[][3], sbit x, sbit y);
func_ack ili_set_cursor_pos(spi_packet_s *pack, sbit x, sbit y);
func_ack ili_fill(spi_packet_s *pack, ebit pixels[][3]);

func_ack ili_draw_pixel(spi_packet_s *pack, tbit x, tbit y, tbit color);
func_ack ili_draw_line(spi_packet_s *pack, tbit color, sbit x1, sbit y1, sbit x2, sbit y2);
func_ack ili_draw_prep(ili9488_window_var_s *window, spi_packet_s *pack, sbit x, sbit y, sbit width, sbit height);
func_ack ili_draw_char(spi_packet_s *pack, sbit x, sbit y, char character, tbit color);
func_ack ili_draw_string(spi_packet_s *pack, tbit x, tbit y, char *string_ptr, tbit color);

//  ILI9488 initialization function prototypes.
func_ack ili_pos_gam_ctrl(spi_packet_s *pack, uint8_t com, const uint8_t *gam_data);
func_ack ili_neg_gam_ctrl(spi_packet_s *pack, uint8_t com, const uint8_t *data);
func_ack ili_pwr_ctrl_o(spi_packet_s *pack, uint8_t com, const uint8_t *data);
func_ack ili_pwr_ctrl_t(spi_packet_s *pack, uint8_t com, const uint8_t *data);
func_ack ili_vcom_ctrl(spi_packet_s *pack, uint8_t com, const uint8_t *data);
func_ack ili_mem_acc_ctrl(spi_packet_s *pack, uint8_t com, uint8_t data);
func_ack ili_pix_intr_format(spi_packet_s *pack, uint8_t com, uint8_t data);
func_ack ili_intr_mode_ctrl(spi_packet_s *pack, uint8_t com, uint8_t data);
func_ack ili_frame_rate_ctrl(spi_packet_s *pack, uint8_t com, uint8_t data);
func_ack ili_dspy_inver(spi_packet_s *pack, uint8_t com, uint8_t data);
func_ack ili_dspy_func_ctrl(spi_packet_s *pack, uint8_t com, const uint8_t *data);
func_ack ili_s_out(spi_packet_s *pack, uint8_t com);
func_ack ili_dspy_on(spi_packet_s *pack, uint8_t com);
func_ack ili_initialize(init_var_s *init, ili9488_funcs *ili);

#endif