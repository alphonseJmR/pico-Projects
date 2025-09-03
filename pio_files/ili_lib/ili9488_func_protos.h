#ifndef ILI9488_FUNCTION_PROTOTYPE_S_H
#define ILI9488_FUNCTION_PROTOTYPE_S_H

#include <stdint.h>
#include <stdlib.h>
#include "ili_user_config.h"


uint16_t get_rand(uint16_t max);
void calc_box_outline(ili_box_line *ol, uint16_t x, uint16_t y, uint16_t xl, uint16_t yl, uint8_t lw);
uint8_t flip(uint8_t input);
void set_pix_color(ili_pix_color *pc, uint32_t rgb);
void set_rand_pix_color(ili_pix_color *pc);
void set_reset(ili_pio *p, bool state);
void set_wait(ili_pio *p, bool state);
void set_rs_pin(ili_pio *p, bool state);
void set_csx(ili_pio *p, bool state);
ili_pix_pos set_pixel_pos(uint16_t x, uint16_t y);
void ili_send(ili_pio *p, uint32_t data, bool d_type);
void ili_command(ili_pio *p, uint8_t data);
void ili_send_command(ili_pio *p, uint8_t data);
void ili_param(ili_pio *p, uint8_t data);
void ili_comparm(ili_pio *p, uint8_t command, uint8_t param);
void ili_comparm_x(ili_pio *p, uint8_t command, uint8_t *params, uint16_t p_size);
void ili_comparm_dual(ili_pio *p, uint8_t command, uint8_t p1, uint8_t p2);
void ili_comparm_4(ili_pio *p, uint8_t command, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);
void ili_set_column(ili_pio *p, ili_pix_pos *ppos);
void ili_set_row(ili_pio *p, ili_pix_pos *ppos);
void ili_draw_pixel_c(ili_pio *p, ili_pix_color *pc);
void ili_draw_x_pixel(ili_pio *p, ili_pix_color *pc, uint32_t numpix);
void ili_set_pixel(ili_pio *p, ili_pix_color *pc, ili_pix_pos *ppos_x);
void ili_single_draw_pixel(ili_pio *p, ili_pix_color *pc, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
void ili_set_box_corners(ili_pio *p, uint16_t cs, uint16_t ce, uint16_t rs, uint16_t re);
void ili_draw_line(ili_pio *p, ili_pix_color *pc, uint16_t x, uint16_t y, uint16_t line_l, uint16_t line_w, bool direction);
void create_color_box(ili_pio *p, ili_pix_color *pc, uint16_t bw_s, uint16_t bh_s, uint16_t width, uint16_t height);
void ili_draw_outline_box(ili_pio *p, ili_pix_color *pc, ili_box_line *line);
void ili_fill_outline(ili_pio *p, ili_pix_color *lc, ili_pix_color *bc, ili_box_line *line);
void fill_x_rand_pixels(ili_pio *p, uint num_pix);
void fill_rand_box_pixels(ili_pio *p, ili_rbvs *rb, uint num_pix);
void setup_rand_box(ili_rbvs *rb);
void ili_draw_rand_box(ili_pio *p);
void ili_fill_random_box(ili_pio *p);
void ili_draw_bitmap(ili_pio *p, ili_pix_color *pc, const char *bmap, const char *bmap1);
void ili_draw_full_map(ili_pio *p, const char *left_half, const char *right_half);
void ili_draw_char(ili_pio *p, ili_pix_color *pc, uint16_t x, uint16_t y, char ch);
void ili_draw_string(ili_pio *p, ili_pix_color *pc, uint16_t x, uint16_t y, char *stri, uint16_t s_size);
void ili_fill_background(ili_pio *p, ili_pix_color *bgc);


#endif