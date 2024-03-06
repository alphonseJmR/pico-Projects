#ifndef ILI9488_FUNCTIONS_H
#define ILI9488_FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>

/**
 * Input parameters when initializing ili9325 driver.
 */
struct ili9325_opt_t{
	uint32_t ul_width;          //!< lcd width in pixel
	uint32_t ul_height;         //!< lcd height in pixel
	uint32_t foreground_color;  //!< lcd foreground color
	uint32_t background_color;  //!< lcd background color
};

/**
 * Font structure
 */
struct ili9325_font{
	/* Font width in pixels. */
	uint8_t width;
	/* Font height in pixels. */
	uint8_t height;
};

/**
 * Display direction option
 */
enum ili9325_display_direction{
	LANDSCAPE  = 0,
	PORTRAIT   = 1
};

/**
 * Shift direction option
 */
enum ili9325_shift_direction{
	H_INCREASE  = 0,
	H_DECREASE  = 1
};

/**
 * Scan direction option
 */
enum ili9325_scan_direction{
	V_INCREASE  = 0,
	V_DEREASE   = 1
};

uint32_t ili9325_init(struct ili9325_opt_t *p_opt);
void ili9325_display_on(void);
void ili9325_display_off(void);
void ili9325_set_foreground_color(ili9325_color_t ul_color);
void ili9325_fill(ili9325_color_t ul_color);
void ili9325_set_window(uint32_t ul_x, uint32_t ul_y,
		uint32_t ul_width, uint32_t ul_height);
void ili9325_set_cursor_position(uint16_t us_x, uint16_t us_y);
void ili9325_scroll (int32_t ul_lines);
void ili9325_enable_scroll (void);
void ili9325_disable_scroll(void);
void ili9325_set_display_direction(enum ili9325_display_direction e_dd,
		enum ili9325_shift_direction e_shd, enum ili9325_scan_direction e_scd);
uint32_t ili9325_draw_pixel(uint32_t ul_x, uint32_t ul_y);
ili9325_color_t ili9325_get_pixel(uint32_t ul_x, uint32_t ul_y);
void ili9325_draw_line(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2);
void ili9325_draw_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2);
void ili9325_draw_filled_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2);
uint32_t ili9325_draw_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r);
uint32_t ili9325_draw_filled_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r);
void ili9325_draw_prepare(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		    uint32_t ul_height);
void ili9325_draw_string(uint32_t ul_x, uint32_t ul_y, const uint8_t *p_str);
void ili9325_draw_pixmap(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		uint32_t ul_height, const ili9325_color_t *p_ul_pixmap);
void ili9325_set_top_left_limit(ili9325_coord_t x, ili9325_coord_t y);
void ili9325_set_bottom_right_limit(ili9325_coord_t x, ili9325_coord_t y);
void ili9325_set_limits(ili9325_coord_t start_x, ili9325_coord_t start_y,
		ili9325_coord_t end_x, ili9325_coord_t end_y);
ili9325_color_t ili9325_read_gram(void);
void ili9325_write_gram(ili9325_color_t color);
void ili9325_copy_pixels_to_screen(const ili9325_color_t *pixels, uint32_t count);
void ili9325_copy_raw_pixel_24bits_to_screen(const uint8_t *raw_pixels, uint32_t count);
void ili9325_duplicate_pixel(const ili9325_color_t color, uint32_t count);
void ili9325_copy_pixels_from_screen(ili9325_color_t *pixels, uint32_t count);
void ili9325_set_orientation(uint8_t flags);




#endif