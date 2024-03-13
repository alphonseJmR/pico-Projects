#ifndef ILI_ALL_FUNC_PROTO_H
#define ILI_ALL_FUNC_PROTO_H

#include <stdio.h>
#include <stdint.h>
#include "ili9488_screen_commands.h"
#include "ili9488_error_management.h"
#include "ili9488_power_control.h"
#include "hardware/spi.h"

#define fun uint8_t
#define txb uint8_t
#define rxb uint8_t
#define len size_t
#define baud uint32_t

#define get_0b_to_8b(x)   ((fun)((x & 0xFFFFFFFF) & 0x000000FF))
#define get_8b_to_16b(x)  ((fun)((x & 0xFFFFFFFF) & 0x0000FF00))
#define get_16b_to_24b(x) ((fun)((x & 0xFFFFFFFF) & 0x00FF0000))
#define get_24b_to_32b(x) ((fun)((x & 0xFFFFFFFF) & 0xFF000000))

#define ILI_LCD_WIDTH 320
#define ILI_LCD_HEIGHT 480
#define lcd_data_cache_size ILI_LCD_WIDTH
#define cord uint16_t
#define color_in uint16_t
#define ili_coord int16_t
#define fud uint16_t
#define crud uint32_t

#define max_iterations (int)(ILI_LCD_WIDTH * ILI_LCD_HEIGHT)

//  Large character set.  Keep collapsed.
const fun charset10x14[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCC,
	0xFF, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0,
	0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0x60, 0x1E, 0x70, 0x3F, 0x30, 0x33, 0x30, 0xFF, 0xFC,
	0xFF, 0xFC, 0x33, 0x30, 0x33, 0xF0, 0x39, 0xE0, 0x18, 0xC0,
	0x60, 0x00, 0xF0, 0x0C, 0xF0, 0x3C, 0x60, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x18, 0xF0, 0x3C, 0xC0, 0x3C, 0x00, 0x18,
	0x3C, 0xF0, 0x7F, 0xF8, 0xC3, 0x1C, 0xC7, 0x8C, 0xCF, 0xCC,
	0xDC, 0xEC, 0x78, 0x78, 0x30, 0x30, 0x00, 0xFC, 0x00, 0xCC,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xEC, 0x00,
	0xF8, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x3F, 0xF0, 0x78, 0x78,
	0x60, 0x18, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0x60, 0x18,
	0x78, 0x78, 0x3F, 0xF0, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x60, 0x0E, 0xE0, 0x07, 0xC0, 0x03, 0x80, 0x3F, 0xF8,
	0x3F, 0xF8, 0x03, 0x80, 0x07, 0xC0, 0x0E, 0xE0, 0x0C, 0x60,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x3F, 0xF0,
	0x3F, 0xF0, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x44, 0x00, 0xEC, 0x00, 0xF8, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x18, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x00, 0xF0, 0x00, 0xC0, 0x00, 0x00, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0xFC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xFC, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x70, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x0C, 0x70, 0x1C, 0xE0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC,
	0xC1, 0xCC, 0xC3, 0x8C, 0xE7, 0x0C, 0x7E, 0x0C, 0x3C, 0x0C,
	0x30, 0x30, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x03, 0xC0, 0x07, 0xC0, 0x0E, 0xC0, 0x1C, 0xC0, 0x38, 0xC0,
	0x70, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xC0, 0x00, 0xC0,
	0xFC, 0x30, 0xFC, 0x38, 0xCC, 0x1C, 0xCC, 0x0C, 0xCC, 0x0C,
	0xCC, 0x0C, 0xCC, 0x0C, 0xCE, 0x1C, 0xC7, 0xF8, 0xC3, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE3, 0x1C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x9C, 0x71, 0xF8, 0x30, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0xFC,
	0xC7, 0xFC, 0xCE, 0x00, 0xDC, 0x00, 0xF8, 0x00, 0xF0, 0x00,
	0x3C, 0xF0, 0x7F, 0xF8, 0xE7, 0x9C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3C, 0x00, 0x7E, 0x00, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x1C,
	0xC3, 0x38, 0xC3, 0x70, 0xE7, 0xE0, 0x7F, 0xC0, 0x3F, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x3C, 0xF0,
	0x3C, 0xF0, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x44, 0x3C, 0xEC,
	0x3C, 0xF8, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0,
	0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x00, 0x00, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x38, 0x70,
	0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x70, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0xC1, 0xEC,
	0xC3, 0xEC, 0xC3, 0x00, 0xE6, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x30, 0xF0, 0x71, 0xF8, 0xE3, 0x9C, 0xC3, 0x0C, 0xC3, 0xFC,
	0xC3, 0xFC, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x3F, 0xFC, 0x7F, 0xFC, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xC0, 0x7F, 0xFC, 0x3F, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x30, 0x30,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x73, 0xF8, 0x33, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x00, 0x38, 0xC0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0x07, 0x80, 0x07, 0x80, 0x0F, 0xC0,
	0x1C, 0xE0, 0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x70, 0x00, 0x38, 0x00, 0x1F, 0x00,
	0x1F, 0x00, 0x38, 0x00, 0x70, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0x1C, 0x00, 0x0E, 0x00, 0x07, 0x00,
	0x03, 0x80, 0x01, 0xC0, 0x00, 0xE0, 0xFF, 0xFC, 0xFF, 0xFC,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xE7, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0xCC,
	0xC0, 0xEC, 0xC0, 0x7C, 0xE0, 0x38, 0x7F, 0xFC, 0x3F, 0xEC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x80, 0xC3, 0x80,
	0xC3, 0xC0, 0xC3, 0xC0, 0xE7, 0x70, 0x7E, 0x3C, 0x3C, 0x1C,
	0x3C, 0x18, 0x7E, 0x1C, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x9C, 0xE1, 0xF8, 0x60, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xFF, 0xC0, 0xFF, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0xFF, 0xE0, 0xFF, 0xC0,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x3C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xF0, 0x3C, 0xF8, 0x7C, 0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80,
	0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0, 0xF8, 0x7C, 0xF0, 0x3C,
	0xFC, 0x00, 0xFE, 0x00, 0x07, 0x00, 0x03, 0x80, 0x01, 0xFC,
	0x01, 0xFC, 0x03, 0x80, 0x07, 0x00, 0xFE, 0x00, 0xFC, 0x00,
	0xC0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xDC, 0x0C, 0xF8, 0x0C, 0xF0, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x30, 0x00, 0x30,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x00, 0xE0, 0x00,
	0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C, 0x00,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x00, 0x70, 0x00,
	0x38, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x06, 0x78, 0x0E, 0xFC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xCC, 0x07, 0xFC, 0x03, 0xF8,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x9C, 0x01, 0xF8, 0x00, 0xF0,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0x30,
	0x00, 0xF0, 0x01, 0xF8, 0x03, 0x9C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C, 0xFF, 0xFC, 0xFF, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0xDC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xDC, 0x07, 0xD8, 0x03, 0x90,
	0x00, 0x00, 0x03, 0x00, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x00,
	0xE3, 0x00, 0x70, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xDC, 0x0F, 0xF8, 0x07, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x80, 0x01, 0xFC, 0x00, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xFC,
	0x1B, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x1C, 0xCF, 0xF8, 0xCF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xE0, 0x01, 0xE0,
	0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0E, 0x00, 0x07, 0x00, 0x03, 0xC0,
	0x03, 0xC0, 0x07, 0x00, 0x0E, 0x00, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x00, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0xFC, 0x03, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0xF8, 0x03, 0xF0,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00,
	0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x03, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xFC, 0x0E, 0x78, 0x06, 0x30,
	0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0xFF, 0xF0, 0xFF, 0xF8,
	0x0C, 0x1C, 0x0C, 0x1C, 0x0C, 0x38, 0x0C, 0x30, 0x00, 0x00,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0F, 0xC0, 0x0F, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x0F, 0xE0, 0x0F, 0xC0,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0xF0, 0x01, 0xE0,
	0x01, 0xE0, 0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C,
	0x0C, 0x00, 0x0E, 0x00, 0x07, 0x0C, 0x03, 0x9C, 0x01, 0xF8,
	0x01, 0xF0, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00, 0x0C, 0x00,
	0x0C, 0x0C, 0x0C, 0x1C, 0x0C, 0x3C, 0x0C, 0x7C, 0x0C, 0xEC,
	0x0D, 0xCC, 0x0F, 0x8C, 0x0F, 0x0C, 0x0E, 0x0C, 0x0C, 0x0C,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x3F, 0xF0, 0x7C, 0xF8,
	0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00,
	0x03, 0x0C, 0x03, 0x0C, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x0C,
	0xC3, 0x0C, 0xC0, 0x0C, 0xE0, 0x0C, 0x70, 0x0C, 0x30, 0x0C,
	0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C,
	0x7C, 0xF8, 0x3F, 0xF0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC
};

color_in pixel_cache[lcd_data_cache_size*3];


//  SPI low level packet.  
typedef struct spi_packet_s {
    
    spi_inst_t *instance;
    txb tx_buf;
    rxb rx_buf;
    baud rate;
    fun length;
    fun spi_func_status;

}spi_packet_s;


//  SPI data buffer parts
typedef struct spi_part_s {

    fun Data_Id;
    fun Word_Count_LSB;
    fun Word_Count_MSB;
    fun Ecc;
    fun Data_Zero;
    fun Data_One;
    fun Data_Two;
    fun Data_Three;
    fun Data_Four;
    fun CRC_LSB;
    fun CRC_MSB;

}spi_part_s;


//  Font parameters.
typedef struct ili_font_s {
    fun f_width;
    fun f_height;

}i_font;


// Window creator parameters.
typedef struct ili9488_window_var_s {

    uint16_t dw_X;
    uint16_t dw_Y;
    uint16_t dw_Width;
    uint16_t dw_Height;

}ili9488_window_var_s;


typedef struct function_coordinate_s {

    uint32_t x_start;
    uint32_t y_start;
    uint32_t x_end;
    uint32_t y_end;
    uint32_t radius;

}fun_coo;


typedef struct ili_string_s {
    uint32_t x_start;
    uint32_t y_start;
    fun *string_ptr;

}i_string;


typedef struct pixel_map_s {

    uint32_t x_start;
    uint32_t y_start;
    uint32_t map_width;
    uint32_t map_height;
    const color_in *pixel_map_ptr;

}pix_map;


typedef struct ili_limit_s {

    ili_coord x_start;
    ili_coord y_start;
    ili_coord x_end;
    ili_coord y_end;

}limits;


typedef struct ili_operation_var_s {

    uint32_t ili_width;
    uint32_t ili_height;
    uint32_t ili_foreground_color;
    uint32_t ili_background_color;
	
}ili_operation_var_s;


typedef struct spi_pin_manager_s {

    uint miso;
    uint mosi;
    uint csn;
    uint sck;

    uint dc_rs;
    uint led_bl;

}spi_pins;


struct ili_font_s my_font = {

  .f_width = 10,
  .f_height = 14

};


ili_operation_var_s my_op = {

.ili_width = ILI_LCD_WIDTH,
.ili_height = ILI_LCD_HEIGHT,
.ili_foreground_color = 0xADD8E6u,
.ili_background_color = 0x006400u

};


spi_packet_s data_packet = {
  .instance = NULL,
  .tx_buf = 0,
  .rx_buf = 0,
  .length = 0,
  .rate = ONE_MBS,
  .spi_func_status = 0

};


spi_part_s my_parts = {

  .Data_Id = 0,
  .Data_Zero = 0,
  .Data_One = 0,
  .Data_Two = 0,
  .Data_Three = 0,
  .Data_Four = 0,
  .Ecc = 0,
  .CRC_LSB = 0,
  .CRC_MSB = 0

};


ili9488_window_var_s my_window = {

.dw_X = 0,
.dw_Y = 0,
.dw_Width = 0,
.dw_Height= 0

};


spi_pins my_pins = {

  .miso = GPIO_FOUR,
  .csn = GPIO_FIVE,
  .sck = GPIO_TWO,
  .mosi = GPIO_THREE,
  .dc_rs = GPIO_ZERO,
  .led_bl = GPIO_FIFTEEN

  };


//  Function Prototypes from pin_management.h
static func_ack pin_validate(spi_pins *pins);
spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins);
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins);
func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins);
void csn_put_high(fun csn);
void csn_put_low(fun csn);
void dcrs_put_high(fun ce);
void dcrs_put_low(fun ce);


//  Function Prototypes from spi_management.h
func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate);
func_ack deinitialize_spi_management(spi_inst_t *instance);
fun spi_length(spi_packet_s *inst, fun input);
fun spi_lpa_command_write(spi_packet_s *inst, fun data_t, fun command, fun num_param, fun p_one, fun p_two, fun p_three, fun p_four, fun ecc);
fun spi_spa_command_write(spi_packet_s *inst, fun data_t, fun command, fud p_one, fun ecc);
fun spi_lpa_data_write(spi_packet_s *inst, fun data_t, fun command, fud data_one, fud data_three, fun ecc);
fun spi_spa_data_write(spi_packet_s *inst, fun data_t, fun command, fud data, fun ecc);
fun spi_read_data(spi_packet_s *inst);
spi_part_s data_break(spi_part_s *parts, uint16_t data_in);
spi_part_s color_to_data(spi_part_s *parts, uint32_t color);
fun data_header(fun data_t);
/*
func_ack spi_managed_data_write(spi_pins *pins, spi_packet_s *pack, spi_part_s *part, fun command);
func_ack managed_data_spi(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, fun command);
func_ack managed_command_spi(spi_packet_s *packet, spi_part_s *parts, fun command, fun *param, fun size);
spi_packet_s packet_buffer(spi_packet_s *packet, spi_part_s *part, int buf_inc, bool l_s);
func_ack spi_managed_command_write(spi_packet_s *packet, spi_part_s *part, fun command, fun *param, fun size);
*/

//  Function Prototype from func_def.h
func_ack ili_delay(uint32_t wait_value);
func_ack ili_write_ram_prep(spi_packet_s *packet, fun data_t, fun command, fun param);
func_ack ili_write_ram(spi_packet_s *packet, fun data_t, fun command, uint32_t color);
func_ack ili_register_write(spi_packet_s *packet, fun data_t, fun command, fud data, crud l_data);
func_ack ili_check_coords(fun_coo *coords);
func_ack ili_set_display_dir(spi_packet_s *pack, fun data_t, bool direction);
func_ack ili_set_window(ili9488_window_var_s *window, fun data_t, spi_packet_s *pack, spi_part_s *parts);
func_ack ili_set_foreground_color(ili9488_window_var_s *window, fun data_t, spi_packet_s *pack, uint32_t color_i);
func_ack ili_set_cursor_pos(cord x, cord y, spi_part_s *parts, fun data_t, spi_packet_s *pack);
func_ack ili_fill(uint32_t color, fun data_t, spi_packet_s *pack);
func_ack ili_draw_pixel(uint32_t x, uint32_t y, spi_part_s *parts, fun data_t, spi_packet_s *pack);
func_ack ili_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, spi_part_s *parts, fun data_t, spi_packet_s *pack);
func_ack ili_draw_prep(fud x, fud y, fud width, fud height, ili9488_window_var_s *window, spi_part_s *parts, fun data_t, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_char(uint32_t x, uint32_t y, fun character, spi_part_s *parts, fun data_t, spi_packet_s *pack);
func_ack ili_draw_string(uint32_t x, uint32_t y, fun *string_ptr, spi_part_s *parts, fun data_t, spi_packet_s *pack);


func_ack ili_delay(uint32_t wait_value){

  printf("\nIli Delay.\n");

uint32_t i;

  for(i = 0; i < wait_value; i++){
    for(i = 0; i < 100000; i++){

    }
  }

  return general_ack;
}

fun spi_read_data(spi_packet_s *inst){

    printf("Attempting to read ILI9488 Device ID.\n");
    fun *dev_id;
    fun device;
    device = 0;

    csn_put_high(my_pins.csn);
    dcrs_put_low(my_pins.dc_rs);
    spi_read_blocking(inst->instance, 0, (dev_id), 2);
    csn_put_low(my_pins.csn);

    device = *dev_id;

    (device > 0) ? printf("Device ID: 0x%04x.\n", device) : printf("Error Reading from ILI9488.\n");

    return (device > 0) ? device : 0x0000;

}


static func_ack pin_validate(spi_pins *pins) {

  printf("Validating pin selection.\n\n");

  printf("Checking MISO pin: %i.\n\n", pins->miso);
  printf("Checking MOSI pin: %i.\n\n", pins->mosi);
  printf("Checking CSN pin: %i.\n\n", pins->csn);
  printf("Checking SCK pin: %i.\n\n", pins->sck);

    //  Validate four main SPI pins.
        //  Valid Pin setup: mosi - 0x01, miso - 0x02, csn - 0x04, sck - 0x08
    fun valid_pins = 0x00;

    if((pins->miso == spi_rx_zz) || (pins->miso == spi_rx_zo) || (pins->miso == spi_rx_oz) || (pins->miso == spi_rx_oo) || (pins->miso == spi_rx_zt) ){

        valid_pins += 0x01;

    }
    if((pins->mosi == spi_tx_zz) || (pins->mosi == spi_tx_zo) || (pins->mosi == spi_tx_oz) || (pins->mosi == spi_tx_oo) || (pins->mosi == spi_tx_zt) ){

      valid_pins += 0x02;

    }
    if((pins->csn == spi_csn_zz) || (pins->csn ==  spi_csn_zo) || (pins->csn ==  spi_csn_oz) || (pins->csn == spi_csn_oo) || (pins->csn ==  spi_csn_zt) ){

      valid_pins += 0x04;

    }
    if((pins->sck == spi_sck_zz) || (pins->sck == spi_sck_zo) || (pins->sck == spi_sck_oz) || (pins->sck == spi_sck_oo) || (pins->sck == spi_sck_zt) ){

      valid_pins += 0x08;

    }

  printf("Current Valid Pins: 0x%02x.\n\n", valid_pins);

  func_ack status = ((valid_pins & 0x0F)) ? pins_ack : func_error;
  printf("Pin Validation Status: %i.\n\n", status);
  
  return status;
}


spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins){

  printf("Configuring SPI Instance.\n");
  printf("Checking User Set MISO Pin: %i.\n", pins->miso);

  if(pins->miso == 0){
    instance->instance = spi0;
  }
  if(pins->miso == 4){
    instance->instance = spi0;
  }
  if(pins->miso == 8){
    instance->instance = spi1;
  }
  if(pins->miso == 12){
    instance->instance = spi1;
  }
  if(pins->miso == 16){
    instance->instance = spi0;
  }

  printf("Current SPI Instance: SPI%i.\n", (instance->instance == spi0) ? 0 : 1);

return *instance;
}


//  Pin Manager Configure
    //  Mosi  - Master Out Slave In
    //  Miso  - Master In Slave Out
    //  Sck   - Clock Signal
    //  CSN   - Chip Select Not
    //  Dc_Rs - Data/Command Register Select
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins) {

  printf("Configuring Pins for Use.\n");

  func_ack status = pin_validate(pins);

  if(status == 2){

  configure_instance(packet, pins);

    // set GPIO function as SPI for SCK, MOSI & MISO & CSN
    gpio_set_function(pins->sck, GPIO_FUNC_SPI);
    gpio_set_function(pins->mosi, GPIO_FUNC_SPI);
    gpio_set_function(pins->miso, GPIO_FUNC_SPI);
    gpio_set_function(pins->csn, GPIO_FUNC_SPI);

    // initialise CE & CSN
  //  gpio_init(pins->csn);
    gpio_init(pins->dc_rs);

    // set direction for CE & CSN
  //  gpio_set_dir(pins->csn, GPIO_OUT);
    gpio_set_dir(pins->dc_rs, GPIO_OUT);
  
  }else{

    printf("Pins not valid.\n");

    return 0;
  }


  return status;
};


func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins){

    printf("Initializing user input pins for SPI.\n");

    func_ack fun_status;
    func_ack configure_status;
    func_ack exit_status;
    fud rate_running;

    printf("\nUser Defined Pins:\n\n\tMosi: %i\n\tMiso: %i\n\tSck: %i\n\tCSN: %i\n\tDC-RS: %i\n\n", pins->mosi, pins->miso, pins->sck, pins->csn, pins->dc_rs);

    configure_status = pin_manager_configure(inst, pins);
    printf("Configured: %s.\n", ((configure_status != 0) ? "Yes" : "No") );

    printf("Initialize SPI Protocol Parameters.\n");

    rate_running = spi_init(inst->instance, inst->rate);

    printf("Baudrate: 0x%08x.\n", rate_running);

    fun_status = general_ack;

    exit_status = ((configure_status == pins_ack) && (fun_status == general_ack)) ? pins_init_ack : func_error;

        return exit_status;

}


void csn_put_high(fun csn) {

  gpio_put(csn, HIGH);

  return;
}


void csn_put_low(fun csn) {

  gpio_put(csn, LOW);

  return;
}


void dcrs_put_high(fun ds) {

  printf("Putting Pin: %i.\n", ds);

  gpio_put(ds, HIGH);

  return;
}


void dcrs_put_low(fun ds) {

  gpio_put(ds, LOW);

  return;
}


ebit spi_length(ebit input){

  ebit sized;
  sized = 0x00;

//  printf("Checking length.\n");

  sized = (sizeof(input) / sizeof(ebit));

//  printf("Length: %i.\n", sized);

  return sized;
}


spi_part_s color_to_data(spi_part_s *parts, uint32_t color){


  parts->Data_Zero = get_0b_to_8b(color);
  parts->Data_One = get_8b_to_16b(color);
  parts->Data_Two = get_16b_to_24b(color);
  parts->Data_Three = get_24b_to_32b(color);

  return *parts;
}


func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate){

//  printf("\nInitialize SPI Management.\n");

      // initialise SPI instance at the specified baudrate (Hz).
  spi_init(instance, baudrate);

    return general_ack;
}


func_ack deinitialize_spi_management(spi_inst_t *instance){

//  printf("\nDeinitialize SPI Management.\n");
    
    // Deinitialize SPI instance.  
  spi_deinit(instance);

    return general_ack;
}


fun data_header(uint8_t data_type){

  fun header;
  header = 0x00;

  header += ((ili_VC) & (data_type));

//  printf("Current Data Type: 0x%02x.\n", header);

  return header;
}

fun spi_lpa_command_write(spi_packet_s *inst, fun data_t, fun command, fun num_param, fun p_one, fun p_two, fun p_three, fun p_four, fun ecc){

  fun dt;
  fun *comm_ptr;
  fun *po_ptr;
  fun *pt_ptr;
  fun *pth_ptr;
  fun *pf_ptr;
  fun *ecc_ptr;
  fun *dt_ptr;

  fun return_bytes;
  fun final_bytes;

  *comm_ptr = command;
  *po_ptr = p_one;
  *pt_ptr = p_two;
  *pth_ptr = p_three;
  *pf_ptr = p_four;
  *ecc_ptr = ecc;

  dt = 0;

  printf("\n\nUpdated SPI LPA Command Write Function.\n");
  data_break(&my_parts, num_param);

  initialize_spi_management(inst->instance, inst->rate);

  csn_put_low(my_pins.csn);
  dcrs_put_low(my_pins.dc_rs);

  dt = data_header(data_t);
  *dt_ptr = dt;

  inst->length = spi_length(inst, dt);
  return_bytes = spi_write_blocking(inst->instance, dt_ptr, inst->length);
  final_bytes += return_bytes;

  inst->length = spi_length(inst, command);
  return_bytes = spi_write_blocking(inst->instance, comm_ptr, inst->length);
  final_bytes += return_bytes;

  for(int i = 0; i < 2; i++){

    if(i == 0){

    inst->length = spi_length(inst, my_parts.Data_Zero);
    return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Zero, inst->length);
    final_bytes += return_bytes;

    }else if(i == 1){

    inst->length = spi_length(inst, my_parts.Data_One);
    return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_One, inst->length);
    final_bytes += return_bytes;

    }
  }

  inst->length = spi_length(inst, ecc);
  return_bytes = spi_write_blocking(inst->instance, ecc_ptr, inst->length);
  final_bytes += return_bytes;

    if(num_param != 0){

     for(int i = 0; i < num_param -1; i++){

      if(i == 0){
           
        inst->length = spi_length(inst, p_one);
        return_bytes = spi_write_blocking(inst->instance, po_ptr, inst->length);
        final_bytes += return_bytes;

      }else if(i == 1){

        inst->length = spi_length(inst, p_two);
        return_bytes = spi_write_blocking(inst->instance, pt_ptr, inst->length);
        final_bytes += return_bytes;

      }else if(i == 2){

        inst->length = spi_length(inst, p_three);
        return_bytes = spi_write_blocking(inst->instance, pth_ptr, inst->length);
        final_bytes += return_bytes;

      }else if(i == 3){

        inst->length = spi_length(inst, p_four);
        return_bytes = spi_write_blocking(inst->instance, pf_ptr, inst->length);
        final_bytes += return_bytes;

      }

      }

    }

  printf("Final Bytes Transmitted: %i.\n", final_bytes);

  deinitialize_spi_management(inst->instance);

  csn_put_high(my_pins.csn);

  return final_bytes;

}


fun spi_spa_command_write(spi_packet_s *inst, fun data_t, fun command, fud p_one, fun ecc){

  fun dt;
  fun *comm_ptr;
  fun *ecc_ptr;
  fun *dt_ptr;

  fun return_bytes;
  fun final_bytes;

  dt = 0;
  *comm_ptr = command;
  *ecc_ptr = ecc;

  data_break(&my_parts, p_one);

  printf("\n\nUpdated SPI SPA Command Write Function.\n");

  dt = data_header(data_t);
  *dt_ptr = dt;
  
  initialize_spi_management(inst->instance, inst->rate);

  csn_put_high(my_pins.csn);
  dcrs_put_low(my_pins.dc_rs);

    inst->length = spi_length(inst, dt);
  return_bytes = spi_write_blocking(inst->instance, dt_ptr, inst->length);
  final_bytes += return_bytes;

  inst->length = spi_length(inst, command);
  return_bytes = spi_write_blocking(inst->instance, comm_ptr, inst->length);
  final_bytes += return_bytes;

  if(p_one != 0){

  inst->length = spi_length(inst, p_one);
  return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Zero, inst->length);
  final_bytes += return_bytes;

  inst->length = spi_length(inst, p_one);
  return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_One, inst->length);
  final_bytes += return_bytes;

  }

  inst->length = spi_length(inst, ecc);
  return_bytes = spi_write_blocking(inst->instance, ecc_ptr, inst->length);
  final_bytes += return_bytes;
  printf("Final Bytes Transmitted: %i.\n", final_bytes);

  deinitialize_spi_management(inst->instance);

    csn_put_high(my_pins.csn);

  return final_bytes;

}


fun spi_lpa_data_write(spi_packet_s *inst, fun data_t, fun command, fud data_one, fud data_three, fun ecc){

  fun dt;
  fun *command_ptr;
  fun *ecc_ptr;
  fun *dt_ptr;

  dt = 0;
  fun return_bytes;
  fun final_bytes;
  fun data_length;

  data_break(&my_parts, data_one);
  data_break(&my_parts, data_three);

  *command_ptr = (command != 0) ? command : 0;
  *ecc_ptr = ecc;

  data_length = 0x00;

  data_length += (my_parts.Data_Zero != 0) ? 0x01 : 0;
  data_length += (my_parts.Data_One != 0) ? 0x01 : 0;
  data_length += (my_parts.Data_Two != 0) ? 0x01 : 0;
  data_length += (my_parts.Data_Four != 0) ? 0x01 : 0;

  dt = data_header(data_t);
  *dt_ptr = dt;

  printf("\n\nUpdated SPI LPA Data Write Function.\n");

  initialize_spi_management(inst->instance, inst->rate);

  csn_put_low(my_pins.csn);
  dcrs_put_high(my_pins.dc_rs);

    inst->length = spi_length(inst, dt);
  return_bytes = spi_write_blocking(inst->instance, dt_ptr, inst->length);
  final_bytes += return_bytes;

  inst->length = spi_length(inst, command);
  return_bytes = spi_write_blocking(inst->instance, command_ptr, inst->length);
  final_bytes += return_bytes;

  for(int i = 0; i < 2; i++){

    if(i == 0){

    inst->length = spi_length(inst, my_parts.Data_Zero);
    return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Zero, inst->length);
    final_bytes += return_bytes;

    }else if(i == 1){

    inst->length = spi_length(inst, my_parts.Data_One);
    return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_One, inst->length);
    final_bytes += return_bytes;

    }
  }

  inst->length = spi_length(inst, ecc);
  return_bytes = spi_write_blocking(inst->instance, ecc_ptr, inst->length);
  final_bytes += return_bytes;

    if(data_length != 0){

     for(int i = 0; i < (data_length - 1); i++){

      if(i == 0){
           
        inst->length = spi_length(inst, my_parts.Data_Zero);
        return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Zero, inst->length);
        final_bytes += return_bytes;

      }else if(i == 1){

        inst->length = spi_length(inst, my_parts.Data_One);
        return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_One, inst->length);
        final_bytes += return_bytes;

      }else if(i == 2){

        inst->length = spi_length(inst, my_parts.Data_Two);
        return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Two, inst->length);
        final_bytes += return_bytes;

      }else if(i == 3){

        inst->length = spi_length(inst, my_parts.Data_Three);
        return_bytes = spi_write_blocking(inst->instance, &my_parts.Data_Three, inst->length);
        final_bytes += return_bytes;

      }

      }

    }


  printf("Final Bytes Transmitted: %i.\n", final_bytes);

  deinitialize_spi_management(inst->instance);

  csn_put_high(my_pins.csn);

  return final_bytes;

}


fun spi_spa_data_write(spi_packet_s *inst, fun data_t, fun command, fud data, fun ecc){

  fun dt;
  fun *coms_ptr;
  fun *data_break_zero;
  fun *data_break_one;
  fun *ec_ptr;
  fun *dt_ptr;

  fun return_bytes;
  fun final_bytes;

  data_break(&my_parts, data);

  dt = 0;
  *coms_ptr = command;
  *data_break_zero = my_parts.Data_Zero;
  *data_break_one = my_parts.Data_One;
  *ec_ptr = ecc;

  printf("Updated SPI Data SPA Write Function.\n");

  dt = data_header(data_t);
  *dt_ptr = dt;

  initialize_spi_management(inst->instance, inst->rate);

  csn_put_low(my_pins.csn);
  dcrs_put_high(my_pins.dc_rs);

  inst->length = spi_length(inst, dt);
  return_bytes = spi_write_blocking(inst->instance, dt_ptr, inst->length);
  final_bytes += return_bytes;

  inst->length = spi_length(inst, command);
  return_bytes = spi_write_blocking(inst->instance, coms_ptr, inst->length);
  final_bytes += return_bytes;

  if(data != 0){

    for(int i = 0; i < 2; i++){

      if(i == 0){

    inst->length = spi_length(inst, my_parts.Data_Zero);
    return_bytes = spi_write_blocking(inst->instance, data_break_zero, inst->length);
    final_bytes += return_bytes;

      }else if(i == 1){


    inst->length = spi_length(inst, my_parts.Data_One);
    return_bytes = spi_write_blocking(inst->instance, data_break_one, inst->length);
    final_bytes += return_bytes;

      }
    }

  }

  inst->length = spi_length(inst, ecc);
  return_bytes = spi_write_blocking(inst->instance, ec_ptr, inst->length);
  final_bytes += return_bytes;
  printf("Final Bytes Transmitted: %i.\n", final_bytes);

  deinitialize_spi_management(inst->instance);

  csn_put_high(my_pins.csn);

  return final_bytes;

}


spi_part_s data_break(spi_part_s *parts, uint16_t data_in){

//  printf("Breaking data: 0x%04x.\n", data_in);

  parts->Data_Zero = 0x00;
  parts->Data_One = 0x00;
  parts->Data_Two = 0x00;
  parts->Data_Three = 0x00;

  parts->Data_Zero = get_0b_to_8b(data_in);
  parts->Data_One = get_8b_to_16b(data_in);
  parts->Data_Two = get_16b_to_24b(data_in);
  parts->Data_Three = get_24b_to_32b(data_in);

  return *parts;
}


func_ack ili_write_ram_prep(spi_packet_s *packet, fun data_t, fun command, fun param){

// printf("RAM Prep.\n");

  fun byte_return;

    byte_return = spi_spa_command_write(packet, data_t, command, param, 0);
    
  //  spi_managed_command_write(packet, part, Memory_Write, 0, 0);

    return (byte_return > 0) ? ili_ack : func_error;
}


func_ack ili_write_ram(spi_packet_s *packet, fun data_t, fun command, uint32_t color){

    fun stat;
    fun packet_size;

  //  printf("Writing RAM.\n");

    color_to_data(&my_parts, color);
     packet_size += (my_parts.Data_One != 0) ? 0x01 : 0;
     packet_size += (my_parts.Data_Two != 0) ? 0x01 : 0;
     packet_size += (my_parts.Data_Three != 0) ? 0x01 : 0;
     packet_size += (my_parts.Data_Four != 0) ? 0x01 : 0;

    stat += spi_lpa_command_write(packet, data_t, command, packet_size, my_parts.Data_Zero, my_parts.Data_One, my_parts.Data_Two, my_parts.Data_Three, 0);

    return (stat > 0) ? ili_ack : func_error;
}


func_ack ili_register_write(spi_packet_s *packet, fun data_t, fun command, fud data, crud l_data){

  printf("Ili Register Writing.\n\n");

    fun spa_bytes;
    fun lpa_bytes;

    fun pack_length;
    fun final_byte;

    fud d_in_one;
    fud d_in_two;

    spa_bytes = 0x00;
    lpa_bytes = 0x00;

    final_byte = 0x00;

    if((data > 0) && l_data == 0){

      spa_bytes += spi_spa_data_write(packet, data_t, command, data, 0);
      final_byte += spa_bytes;


    }else if((data == 0) && (l_data > 0)){

      data_break(&my_parts, (l_data & 0x0000FFFF));
      data_break(&my_parts, (l_data & 0xFFFF0000));

      d_in_one += (my_parts.Data_Zero & my_parts.Data_One);
      d_in_two += (my_parts.Data_Two & my_parts.Data_Three);

    lpa_bytes = spi_lpa_data_write(packet, data_t, command, d_in_one, d_in_two, 0);
    final_byte += lpa_bytes;

    }

    final_byte = ((spa_bytes > 0) || (lpa_bytes > 0)) ? register_written : func_error;
    printf("Final Bytes Written: %i.\n", final_byte);

    return final_byte;
}


func_ack ili_check_coords(fun_coo *coords){

    uint32_t par;

    if(coords->x_start >= ILI_LCD_WIDTH) {
        coords->x_start = ILI_LCD_WIDTH - 1;
    }

    if(coords->x_end >= ILI_LCD_WIDTH){
        coords->x_end = ILI_LCD_WIDTH - 1;
    }

    if(coords->y_start >= ILI_LCD_WIDTH) {
        coords->y_start = ILI_LCD_WIDTH - 1;
    }

    if(coords->y_end >= ILI_LCD_WIDTH){
        coords->y_end = ILI_LCD_WIDTH - 1;
    }

    if(coords->x_start > coords->x_end) {
        par = coords->x_start;
        coords->x_start = coords->x_end;
        coords->x_end = par;
    }

    if(coords->y_start > coords->y_end){
        par = coords->y_start;
        coords->y_start = coords->y_end;
        coords->y_end = par;
    }

    return general_ack;
}


func_ack ili_set_display_dir(spi_packet_s *pack, fun data_t, bool direction){
    
    fun stat;
    
    fud dir_control;
    dir_control = 0x00;

    if(direction){
        dir_control = 0xE8;
    }else {
        dir_control = 0x48;
    }

    stat = ili_register_write(pack, data_t, Memory_Access_Control, dir_control, 0);

    // Should return register_written, 9.
    return (stat == 9) ? ili_ack : func_error;

}


func_ack ili_set_window(ili9488_window_var_s *window, fun data_t, spi_packet_s *pack, spi_part_s *parts){

  printf("ILI Setting Window.\n\n");

    fun stat;
    fun e_stat;
    fud col_s, col_e, row_s, row_e;
    fud d_out_one;
    fud d_out_two;

    col_s = window->dw_X;
    col_e = (window->dw_Width + (window->dw_X - 1));

    row_s = window->dw_Y;
    row_e = (window->dw_Height + (window->dw_Y - 1));

    parts->Data_Zero = get_8b_to_16b(col_s);
    parts->Data_One = get_0b_to_8b(col_s);
    parts->Data_Two = get_8b_to_16b(col_e);
    parts->Data_Three = get_0b_to_8b(col_e);

    d_out_one = (parts->Data_Zero & parts->Data_One);
    d_out_two = (parts->Data_Two & parts->Data_Three);

    stat += spi_lpa_data_write(pack, data_t, Column_Address_Set, d_out_one, d_out_two, 0);
    stat += spi_spa_data_write(pack, data_t, NOP, 0, 0);

    d_out_one = 0;
    d_out_two = 0;

    parts->Data_Zero = get_8b_to_16b(row_s);
    parts->Data_One = get_0b_to_8b(row_s);
    parts->Data_Two = get_8b_to_16b(row_e);
    parts->Data_Three = get_0b_to_8b(row_e);

    d_out_one = (parts->Data_Zero & parts->Data_One);
    d_out_two = (parts->Data_Two & parts->Data_Three);

    stat += spi_lpa_data_write(pack, data_t, Page_Address_Set, d_out_one, d_out_two, 0);
    stat += spi_spa_data_write(pack, data_t, NOP, 0, 0);

    e_stat = (stat > 0) ? spi_ack : func_error;

    return e_stat;
}


func_ack ili_set_foreground_color(ili9488_window_var_s *window, fun data_t, spi_packet_s *pack, uint32_t color_i){

  printf("Setting ILI Foreground Color.\n\n");
    sleep_ms(500);

    int j;
    fud stat;

    uint16_t pixels_written;
    uint16_t biggles;
    biggles = sizeof(pixel_cache)/sizeof(uint16_t);
    printf("Size of pixel array: %i.\n", biggles);

	for (int i = 0; i < biggles; ) {

		pixel_cache[i++] = color_i >> 16;
		pixel_cache[i++] = color_i >> 8;
		pixel_cache[i++] = color_i & 0xFF;

	}

  j = 0;
  pixels_written = 0;

  stat += ili_write_ram_prep(pack, data_t, Memory_Access_Control, 0);
    if(stat == 3){
 for(int k = 0; k < biggles; k++){

  pixels_written += ili_write_ram(pack, data_t, Memory_Write, pixel_cache[k]);
  printf("\n\nWriting Pixel Cache: %i.\n", pixel_cache[k]);

  }
    }

    return (pixels_written > 0) ? ili_ack : func_error;

}


func_ack ili_set_cursor_pos(cord x, cord y, spi_part_s *parts, fun data_t, spi_packet_s *pack){
	/* Set Horizontal Address Start Position */

    fun stat;
    fun e_stat;
    

	parts->Data_Zero = get_8b_to_16b(x);
	parts->Data_One = get_0b_to_8b(x);
	parts->Data_Two = get_8b_to_16b(x);
	parts->Data_Three = get_0b_to_8b(x);
  stat += ili_register_write(pack, data_t, Column_Address_Set, (parts->Data_Zero & parts->Data_One), 0);
	stat += ili_register_write(pack, data_t, NOP, 0, 0);

	/* Set Horizontal Address End Position */
	parts->Data_Zero = get_8b_to_16b(y);
	parts->Data_One = get_0b_to_8b(y);
	parts->Data_Two = get_8b_to_16b(y);
	parts->Data_Three = get_0b_to_8b(y);
	stat += ili_register_write(pack, data_t, Page_Address_Set, (parts->Data_Zero & parts->Data_One), 0);
	stat += ili_register_write(pack, data_t, NOP, 0, 0);

    e_stat = (stat > 0) ? ili_ack : func_error;

    return e_stat;
}


func_ack ili_fill(uint32_t color, fun data_t, spi_packet_s *pack){

    fun stat;
    fun f_stat;
    int current_iteration;
    current_iteration = 0;

    stat = 0;
    f_stat = 0;

    uint32_t dw;
    data_break(&my_parts, (color & 0x0000FFFF));
    data_break(&my_parts, (color & 0xFFFF0000));

    stat += spi_spa_command_write(&data_packet, data_t, Sleep_OUT, 0, 0);

    stat += ili_set_cursor_pos(0, 0, &my_parts, data_t, pack);
    f_stat += stat;
    stat += ili_write_ram_prep(pack, data_t, Memory_Write, 0);
    f_stat += stat;

    for(dw = max_iterations; dw > 0; dw--){
      for(int i = 0; i < 4; i++){

        if(i == 0){

          stat += ili_write_ram(pack, data_t, Memory_Write_Continue, my_parts.Data_Zero);
          f_stat += stat;

        }else if(i == 1){

          stat += ili_write_ram(pack, data_t, Memory_Write_Continue, my_parts.Data_One);
          f_stat += stat;

        }else if(i == 2){

          stat += ili_write_ram(pack, data_t, Memory_Write_Continue, my_parts.Data_Two);
          f_stat += stat;
        }else if(i == 3){

          stat += ili_write_ram(pack, data_t, Memory_Write_Continue, my_parts.Data_Three);
          f_stat += stat;

        }
      }
    
      current_iteration++;
      printf("\n\nCurrent Iteration: %i.\n\tOut Of: %i\n\n", current_iteration, max_iterations);
    }

    stat += spi_spa_command_write(&data_packet, data_t, Sleep_IN, 0, 0);
    f_stat += stat;

    return (f_stat > 0) ? ili_ack : func_error;
}


func_ack ili_draw_pixel(uint32_t x, uint32_t y, spi_part_s *parts, fun data_t, spi_packet_s *pack){

    fun stat;
    fun f_stat;
    uint32_t i;

    stat = 0;
    f_stat = 0;

    if((x >= ILI_LCD_WIDTH) || (y >= ILI_LCD_HEIGHT)){
        return 1;
    }

     stat += spi_spa_command_write(&data_packet, data_t, Sleep_OUT, 0, 0);

  stat = ili_set_cursor_pos(x, y, parts, data_t, pack);
  f_stat += stat;
  stat = ili_write_ram_prep(pack, data_t, Memory_Write, 0);
  f_stat += stat;

    while(pixel_cache[i] != '\n'){

    stat = ili_write_ram(pack, data_t, Memory_Write, pixel_cache[i++]);
    f_stat += stat;

    }

  stat += spi_spa_command_write(&data_packet, data_t, Sleep_OUT, 0, 0);
  f_stat += stat;

    return (f_stat > 0) ? ili_ack : func_error;

}


func_ack ili_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, spi_part_s *parts, fun data_t, spi_packet_s *pack){

    fun stat;
    fun drawn_stat;

    stat = 0;
    drawn_stat = 0;

    int d_x, d_y;
    int x_inc, y_inc, cumulation;
    int x, y;

    x = x1;
    y = y1;
    d_x = (x2 - x1);
    d_y = (y2 - y1);
    x_inc = (d_x > 0) ? 1 : -1;
    y_inc = (d_y > 0) ? 1 : -1;
    d_x = abs(x2 - x1);
    d_y = abs(y2 - y1);

  stat = ili_draw_pixel(x, y, parts, data_t, pack);
  drawn_stat += stat;

    if(d_x > d_y){
        cumulation = (d_x >> 1);

        for(int i = 1; i <= d_x; i++){

            x += x_inc;
            cumulation += d_y;

            if(cumulation >= d_x) {

                cumulation -= d_x;
                y += y_inc;

            }

    stat = ili_draw_pixel(x, y, parts, data_t, pack);
    drawn_stat += stat;

        }
    }  else {

            cumulation = (d_y >> 1);

            for(int i = 1; i <= d_y; i++){

                y += y_inc;
                cumulation += d_x;

                if(cumulation >= d_y){

                    cumulation -= d_y;
                    x += x_inc;

            }

  stat = ili_draw_pixel(x, y, parts, data_t, pack);
  drawn_stat += stat;
        }
    }

    return (drawn_stat > 0) ? ili_ack : func_error;
}


func_ack ili_draw_prep(fud x, fud y, fud width, fud height, ili9488_window_var_s *window, spi_part_s *parts, fun data_t, spi_pins *pins, spi_packet_s *pack){

    fun stat;
    fun f_stat;

    window->dw_X = x;
    window->dw_Y = y;
    window->dw_Width = width;
    window->dw_Height = height;

    stat = ili_set_window(window, data_t, pack, parts);
    f_stat += stat;

    stat = ili_set_cursor_pos(x, y, parts, data_t, pack);
    f_stat += stat;

    stat = ili_write_ram_prep(pack, data_t, Memory_Write, 0);
    f_stat += stat;
    stat = ili_write_ram_prep(pack, data_t, NOP, 0);
    f_stat += stat;

    return (f_stat > 0) ? ili_ack : func_error;

}


func_ack ili_draw_char(uint32_t x, uint32_t y, fun character, spi_part_s *parts, fun data_t, spi_packet_s *pack){

    fun stat;
    fun f_stat;

    stat = 0;
    f_stat = 0;

    uint32_t row, col;
    uint32_t o_set, o_set1, o_set2;

    o_set = ((uint32_t)character - 0x20) * 20;

    for(col = 0; col < 10; col++){
        o_set1 = o_set + col * 2;
        o_set2 = o_set1 + 1;

        for(row = 0; row < 8; row++){
            if((charset10x14[o_set1] >> (7 - row)) & 0x01){
                
              stat = ili_draw_pixel((x + col), (y + row + 8), parts, data_t, pack);
              f_stat += stat;
            }

        for (row = 0; row < 6; row++){
            if((charset10x14[o_set2] >> (7 - row) & 0x01)){
                
              stat = ili_draw_pixel((x + col), (y + row + 8), parts, data_t, pack);
              f_stat += stat;
        }
      }
    }
  }
    return (f_stat > 0) ? ili_ack : func_error;
}


func_ack ili_draw_string(uint32_t x, uint32_t y, fun *string_ptr, spi_part_s *parts, fun data_t, spi_packet_s *pack){

    fun stat;
    fud f_stat;
    int i;

    const char *input[100];
    *input = string_ptr;

    uint32_t x_org = x;

    while(*input[i] != 0){

        if(*input[i] == '\n'){

            y += my_font.f_height + 2;
            x = x_org;

        }else {

          stat = ili_draw_char(x, y, *input[i], parts, data_t, pack);
            x += my_font.f_width + 2;
            f_stat += stat;
        }
    input[i++];

    }
    return (f_stat > 0) ? ili_ack : func_error;
}


typedef struct ili_init_var_s {
    
  struct ili_operation_var_s *my_op;
  struct spi_packet_s *data_packet;
  struct spi_part_s *my_parts;
  struct ili9488_window_var_s *my_window;
	struct spi_pin_manager_s *my_pins;

}ili_init_var_s;


ili_init_var_s my_init = {

  .my_op = &my_op,
  .data_packet = &data_packet,
  .my_parts = &my_parts,
  .my_window = &my_window,
  .my_pins = &my_pins

};


func_ack ili_initialize(ili_init_var_s *init);


func_ack ili_initialize(ili_init_var_s *init){

  printf("\nILI Initialize:\n");

    fun stat;
    fun e_stat;
    uint16_t color;
    fun *p_ptr;

    printf("Send: Soft Reset.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S ,SOFT_RESET, 0, 0);
    ili_delay(200);

    printf("Send: Sleep Out.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S, Sleep_OUT, 0, 0);
    ili_delay(200);

   // *p_ptr = 0x48;
    printf("Send: Memory Access Control.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSW_OS, Memory_Access_Control, 0x48, 0);
   // stat += managed_command_spi(*(&init->data_packet), *(&init->my_parts),Memory_Access_Control, (p_ptr), 0);
    ili_delay(200);

    // *p_ptr = 0x04;
    printf("Send: CABC Control Nine.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSW_OS, CABC_Control_Nine, 0x04, 0);
    // stat += managed_command_spi(*(&init->data_packet), *(&init->my_parts), CABC_Control_Nine, p_ptr, 0);
    ili_delay(200);

    // *p_ptr = 0x05;
    printf("Send: Interface Pixel Format.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSW_OS, Interface_Pixel_Format, 0x05, 0);
    // stat += managed_command_spi(*(&init->data_packet), *(&init->my_parts), Interface_Pixel_Format, p_ptr, 0);
    ili_delay(200);

    printf("Send: Normal Display Mode On.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S, Normal_Display_Mode_ON, 0, 0);
    // stat += managed_command_spi(*(&init->data_packet), *(&init->my_parts), Normal_Display_Mode_ON, 0, 0);
    ili_delay(200);

    printf("Send: Display On.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S, Display_ON, 0, 0);
    // stat += managed_command_spi(*(&init->data_packet), *(&init->my_parts), Display_ON, 0, 0);
    ili_delay(200);

    printf("Set: Display Dir>>Normal.\n");
    stat += ili_set_display_dir(*(&init->data_packet), SPa_DCSWN_S, true);
    ili_delay(200);

    printf("Set: Foreground Color:DarkGreen?\n");
    stat += ili_set_foreground_color(*(&init->my_window), SPa_DCSWN_S, *(&init->data_packet), (init->my_op->ili_foreground_color));
    ili_delay(200);

    printf("Fill ILI Screen.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S, All_Pixel_ON, 0, 0);
    ili_delay(200);
    
    printf("Set: Cursor Position.\n");
    stat += ili_set_cursor_pos(0, 0, *(&init->my_parts), SPa_DCSWN_S, *(&init->data_packet));
    ili_delay(200);


      printf("Sleep IN.\n");
    stat += spi_spa_command_write((init->data_packet), SPa_DCSWN_S, Sleep_IN, 0, 0);
    ili_delay(200);
    printf("Current Status Returns: %i.\n\n", stat);

    e_stat = (stat == 39) ? ili_initialized : func_error;
    printf("Exiting with status: %i.\n", e_stat);

    return e_stat;
}


//  Commented out section of old code, in case of reuse / need.


/*
func_ack spi_managed_command_write(spi_packet_s *packet, spi_part_s *part, fun command, fun *param, fun size){

  printf("SPI Managed Command Write.\n");

  fun bytes;
  fun f_stat;
  fun stat;

  bytes = spi_write_blocking(packet->instance, &command, 0, 0);
  printf("Current number of bytes: %i.\n", bytes);

  f_stat = 0x00;

  if(param != 0){

  packet->spi_func_status += spi_write_blocking(packet->instance, param, 0, 0);
  f_stat += packet->spi_func_status;

  }
    if(size > 0){
  
    for(int i = 0; i < size; i++){

      if(i == 0){

        packet->spi_func_status += spi_write_blocking(packet->instance, &part->Data_Zero, 0, 0);
        f_stat += packet->spi_func_status;

      }
      if(i == 1){

        packet->spi_func_status += spi_write_blocking(packet->instance, &part->Data_One, 0, 0);
        f_stat += packet->spi_func_status;

      }
      if(i == 2){

        packet->spi_func_status += spi_write_blocking(packet->instance, &part->Data_Two, 0, 0);
        f_stat += packet->spi_func_status;

      }
      if(i == 3){

        packet->spi_func_status += spi_write_blocking(packet->instance, &part->Data_Three, 0, 0);
        f_stat += packet->spi_func_status;

      }
      if(i == 4){

        packet->spi_func_status += spi_write_blocking(packet->instance, &part->Data_Four, 0, 0);
        f_stat += packet->spi_func_status;

      }


  }

  } 

  packet->spi_func_status += spi_write_blocking(packet->instance, part->Ecc, 0, 0);
  f_stat += packet->spi_func_status;


  stat = (bytes > 0) ? spi_ack : func_error;

  return stat;

}

*/

/*
func_ack managed_data_spi(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, fun command){

  fun fun_stat;

    packet->spi_func_status = 0;

    //  Initialize communication.
  packet->spi_func_status += initialize_spi_management(packet->instance, packet->rate);
        sleep_us(3);

    //  Initialize data transfer.
    packet->spi_func_status += spi_managed_data_write(pins, packet, part, command);
        sleep_us(3);

    //  Deinitialize communication.
    packet->spi_func_status += deinitialize_spi_management(packet->instance);
        sleep_us(3);

    fun_stat = (packet->spi_func_status == 0x09) ? spi_ack : func_error;

    return fun_stat;
}


func_ack managed_command_spi(spi_packet_s *packet, spi_part_s *parts, fun command, fun *param, fun size){
  
  fun fun_stat;
  packet->spi_func_status = 0;

  packet->spi_func_status += initialize_spi_management(packet->instance, packet->rate);
    ili_delay(100);

  packet->spi_func_status += spi_managed_command_write(packet, parts, command, param, size);
    ili_delay(100);

  packet->spi_func_status += deinitialize_spi_management(packet->instance);
    ili_delay(100);

  fun_stat = (packet->spi_func_status == 0x09) ? spi_ack : func_error;
  
  return fun_stat;
}
*/


/*
func_ack spi_managed_data_write(spi_pins *pins, spi_packet_s *pack, spi_part_s *part, fun command) {

  printf("SPI Managed Write.\n\n");

  txb packet_buf;
  spi_length(pack);
  fun bytes;
  fun packet_length;
  int i;
  bool lp_a;
  fun *com_ptr;
  *com_ptr = command;

  printf("Current Command: 0x%02x.\n", command);

  packet_length = (spi_check_data_size(part));
  lp_a = (packet_length > 2) ? false : true;

  if(lp_a == false){

    spi_write_blocking(pack->instance, com_ptr, 0, 0);
    for(i = 0; i < 0xFF; i++);

    csn_put_high(pins->csn);
    for(int i = 0; i <= packet_length; i++){
    }

  }else if(lp_a == true){

    part->data_packet = packet_buffer(pack, part, i, lp_a);

      ili_delay(100);
    bytes = spi_write_blocking(pack->instance, &pack->tx_buf, &pack->rx_buf, pack->length);
  for(i = 0; i < 0x5F; i++);

    }

  // check that bytes written/read match bytes in tx_buffer & rx_buffer
  func_ack status = (bytes == pack->length) ? spi_ack : func_error;
//  printf("Status: %i.\n", status);

  return status;
}

spi_packet_s packet_buffer(spi_packet_s *packet, spi_part_s *part, int buf_inc, bool l_s){

  if(l_s == true){

      switch(buf_inc){

    case 0:
      packet->tx_buf = part->Data_Id;
    break;

    case 1:
      packet->tx_buf = part->Data_Zero;
    break;

    case 2:
      packet->tx_buf = part->Data_One;
    break;

    case 3:
      packet->tx_buf = part->Ecc;
    break;
    
    default:
      printf("Loop Out of range.\n");
      break;
    }
  }else {

      if(part->Data_Two == 0 && buf_inc == 5){
        buf_inc++;
      }
      if(part->Data_Three == 0 && buf_inc == 6){
        buf_inc++;
      }
      if(part->Data_Four == 0 && buf_inc == 7){
        buf_inc++;
      }

          switch(buf_inc){

    case 0:
      packet->tx_buf = part->Data_Id;
    break;

    case 1:
      packet->tx_buf = part->Word_Count_LSB;
    break;

    case 2:
      packet->tx_buf = part->Word_Count_MSB;
    break;

    case 3:
      packet->tx_buf = part->Data_One;
    break;

    case 4:
      packet->tx_buf = part->Data_Two;
    break;

    case 5:
      packet->tx_buf = part->Data_Three;
    break;

    case 6:
      packet->tx_buf = part->Data_Four;
    break;

    case 7:
      packet->tx_buf = part->CRC_LSB;
    break;

    case 8:
      packet->tx_buf = part->CRC_MSB;
    break;
    
    default:
      printf("Loop Out of range.\n");
      break;
    }

  }
  return *packet;
}

*/

#endif