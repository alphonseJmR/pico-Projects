#ifndef ILI9488_USER_HIGH_LEVEL_CONFIG_S_H
#define ILI9488_USER_HIGH_LEVEL_CONFIG_S_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hardware/pio.h"
#include "ili9488_char_set.h"
#include "ili_pre-set_colors.h"

#define landscape

 typedef struct pio_smp_s {

  uint8_t shift;
  uint8_t in;
  uint8_t set_one;
  uint8_t set_two;
  uint8_t clk;

}pio_smp;

typedef struct pio_pad_in_s {

	volatile uint8_t num_one;
	volatile uint8_t num_two;
	volatile uint8_t num_three;
	volatile uint8_t num_four;

}pio_pad;

/*
#define ebit uint8_t
#define sbit uint16_t
#define len size_t
#define tbit uint32_t
#define get_0b_to_8b(x)   ((ebit)((x & 0xFFFFFFFF) & 0x000000FF))
#define get_8b_to_16b(x)  ((ebit)((x & 0xFFFFFFFF) & 0x0000FF00))
#define get_16b_to_24b(x) ((ebit)((x & 0xFFFFFFFF) & 0x00FF0000))
#define get_24b_to_32b(x) ((ebit)((x & 0xFFFFFFFF) & 0xFF000000))
*/

#ifdef portrait

#define ILI_LCD_WIDTH 320
#define ILI_LCD_HEIGHT 480
#define max_iili_char_width 22

#endif

#ifdef landscape

#define ILI_LCD_WIDTH 480
#define ILI_LCD_HEIGHT 320
#define max_ili_char_width 43

#endif

#define lcd_data_cache_size ILI_LCD_WIDTH
#define max_iterations (int)(ILI_LCD_WIDTH * ILI_LCD_HEIGHT)

#define not_eq1(a,b) (bool)( (a != b))
#define not_eq2(a,b,c) (bool)( (a != b) && (a != c))
#define not_eq3(a,b,c,d) (bool)( (a != b) && (a != c) && (a != d))



//    CSX && Reset are going to be CPU allocated.

typedef struct ili9488_pio_pins {

    uint8_t data;
    uint8_t clock;
    uint8_t dc_rs;
    uint8_t csx;
    uint8_t reset;
    uint8_t wait;
    uint8_t reg_s;

}ili_lcd_pins;



typedef struct ili9488_pio_define_s {

    PIO c_pio;
    uint8_t c_sm;
    uint8_t c_offset;
    bool c_addable;

    PIO r_pio;
    uint8_t r_sm;
    uint8_t r_offset;
    bool r_addable;

}ili_pio_s;

//  This contains the (xsu, xsl, xeu, xel, ysu, ysl, yeu, yel)
typedef struct ili_pixel_mem_addr {

  // X Start: Start Upper
  uint8_t xssu;
  // X Start: Start Lower
  uint8_t xssl;
  // X End: End Upper
  uint8_t xesu;
  // X End: End Lower
  uint8_t xesl;

  // Y Start: Start Upper
  uint8_t yssu;
  // Y Start: Start Lower
  uint8_t yssl;
  // Y End: End Upper
  uint8_t yesu;
  // Y End: End Lower
  uint8_t yesl;

}ili_pix_pos;


//  This struct contains the previous two.
typedef struct ili_pio_config_s {

  ili_lcd_pins s_pins;
  ili_pio_s s_pio;

}ili_pio;


typedef struct ili9488_box_outline_coord_s {

  uint16_t left_x;
  uint16_t left_y;
  uint16_t left_length;

  uint16_t top_x;
  uint16_t top_y;
  uint16_t top_length;

  uint16_t right_x;
  uint16_t right_y;
  uint16_t right_length;

  uint16_t bottom_x;
  uint16_t bottom_y;
  uint16_t bottom_length;

  uint16_t line_width;
  uint16_t x_offset;
  uint16_t y_offset;

  uint16_t abs_box_length;

  // Used when putting text in outline.
  uint16_t s_len;
  //  Adjusted len for spacing.
  uint16_t s_len_adj;

}ili_box_line;


typedef struct ili_rand_box_variable_s {

  uint box_size;
  uint16_t x_start;
  uint16_t x_length;
  uint16_t y_start;
  uint16_t y_height;
  uint8_t line_width;
  uint8_t box_r;
  uint8_t box_g;
  uint8_t box_b;
  //  Max pixels calculated by (((x_length * y_height) - (line_width * 2) / 3)
  uint16_t max_pixels;

}ili_rbvs;


typedef struct known_param_sizes {

  size_t pgd;
  size_t ngd;
  size_t pco;
  size_t pct;
  size_t vcc;
  size_t dfc;

}param_sizes;

param_sizes knowns;

 uint8_t init_comms[13] = {

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

 uint8_t pos_gamma_dat[15] = {

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

 uint8_t display_func_ctrl[3] = {
  0x02,
  0x02,
  0x3B
};


#endif
