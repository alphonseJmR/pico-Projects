#ifndef ILI_ALL_FUNC_PROTO_H
#define ILI_ALL_FUNC_PROTO_H

#include <stdio.h>
#include "ili9488_screen_commands.h"
#include "ili9488_error_management.h"
#include "ili9488_power_control.h"
#include "hardware/spi.h"

#define txb uint8_t
#define rxb uint8_t
#define fun uint8_t
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
#define fun uint8_t
//  Large character set.  Keep collapsed.
const uint8_t charset10x14[] = {
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
    len length;
    uint8_t spi_func_status;

}spi_packet_s;


//  SPI data buffer parts
typedef struct spi_part_s {

    uint8_t Data_Id;
    uint8_t Word_Count_LSB;
    uint8_t Word_Count_MSB;
    uint8_t Ecc;
    uint8_t Data_Zero;
    uint8_t Data_One;
    uint8_t Data_Two;
    uint8_t Data_Three;
    uint8_t Data_Four;
    uint8_t CRC_LSB;
    uint8_t CRC_MSB;

}spi_part_s;


//  Font parameters.
typedef struct ili_font_s {
    uint8_t f_width;
    uint8_t f_height;

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
    uint8_t *string_ptr;

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
  .rate = 0,
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
  .sck = GPIO_SIX,
  .mosi = GPIO_SEVEN,
  .dc_rs = GPIO_TWO,
  .led_bl = GPIO_THREE

  };


//  Function Prototypes from pin_management.h
static func_ack pin_validate(spi_pins *pins);
spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins);
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins);
func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins);
void csn_put_high(uint8_t csn);
void csn_put_low(uint8_t csn);
void dcrs_put_high(uint8_t ce);
void dcrs_put_low(uint8_t ce);
func_ack selector(bool csn, bool dc_rs, bool enable, spi_pins *pins);


//  Function Prototypes from spi_management.h
spi_packet_s spi_length(spi_packet_s *inst);
spi_part_s color_to_data(spi_part_s *parts, uint32_t color);
func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate);
func_ack deinitialize_spi_management(spi_inst_t *instance);
func_ack spi_managed_data_write(spi_pins *pins, spi_packet_s *pack, spi_part_s *part, uint8_t *command);
func_ack spi_managed_command_write(spi_packet_s *packet, spi_part_s *part, uint8_t *command, uint8_t *param, uint8_t size);
uint8_t spi_check_data_size(spi_part_s *parts);
spi_part_s data_break(spi_part_s *parts, uint16_t data_in);
func_ack managed_data_spi(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint8_t *command);
func_ack managed_command_spi(spi_packet_s *packet, spi_part_s *parts, fun *command, fun *param, fun size);
spi_packet_s packet_buffer(spi_packet_s *packet, spi_part_s *part, int buf_inc, bool l_s);


//  Function Prototype from func_def.h
func_ack ili_delay(uint32_t wait_value);
func_ack ili_write_ram_prep(spi_pins *pins, spi_part_s *part, spi_packet_s *packet);
func_ack ili_write_ram(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint32_t color);
func_ack ili_register_write(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint8_t *command);
func_ack ili_check_coords(fun_coo *coords);
func_ack ili_set_display_dir(spi_pins *pins, spi_packet_s *pack, spi_part_s *parts, bool direction);
func_ack ili_set_window(ili9488_window_var_s *window, spi_packet_s *pack, spi_part_s *parts);
func_ack ili_set_foreground_color(ili9488_window_var_s *window, spi_packet_s *pack, spi_part_s *parts, uint32_t color_i);
func_ack ili_set_cursor_pos(cord x, cord y, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_fill(uint16_t color, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_pixel(uint32_t x, uint32_t y, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_prep(uint32_t x, uint32_t y, uint32_t width, uint32_t height, ili9488_window_var_s *window, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_char(uint32_t x, uint32_t y, uint8_t character, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);
func_ack ili_draw_string(uint32_t x, uint32_t y, uint8_t *string_ptr, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack);


func_ack ili_delay(uint32_t wait_value){

  printf("\nIli Delay.\n");

uint32_t i;

  for(i = 0; i < wait_value; i++){
    for(i = 0; i < 100000; i++){

    }
  }

  return general_ack;
}


static func_ack pin_validate(spi_pins *pins) {

  printf("Validating pin selection.\n");

    //  Validate four main SPI pins.

  typedef struct validate_pin_s
  {
    uint spi_pin;
    uint check_one;
    uint check_two;
    uint check_three;
    uint check_four;

  } validate_pin_t;

  validate_pin_t spi_pins[5] = {

    (validate_pin_t) { .spi_pin = pins->mosi, .check_one = spi_tx_zz, .check_two = spi_tx_zo, .check_three = spi_tx_oo, .check_four = spi_tx_zt},
    (validate_pin_t) { .spi_pin = pins->miso, .check_one = spi_rx_zz, .check_two = spi_rx_zo, .check_three = spi_rx_oo, .check_four = spi_rx_zt},
    (validate_pin_t) { .spi_pin = pins->csn, .check_one = spi_csn_zz, .check_two = spi_csn_zo, .check_three = spi_csn_oo, .check_four = spi_csn_zt},
    (validate_pin_t) { .spi_pin = pins->sck, .check_one = spi_sck_zz, .check_two = spi_sck_zo, .check_three = spi_sck_oo, .check_four = spi_sck_zt}

  };

    //  Valid Pin setup: mosi - 0x01, miso - 0x02, csn - 0x04, sck - 0x08
  uint8_t valid_pins = 0;

    for(int i = 0; i < 4; i++){

        if(i == 0){

            if(spi_pins[0].spi_pin == (spi_pins[0].check_one || spi_pins[0].check_two || spi_pins[0].check_three || spi_pins[0].check_four)){
                
                valid_pins += 0x01;

            }

        }
        if(i == 1){

            if(spi_pins[1].spi_pin == (spi_pins[1].check_one || spi_pins[1].check_two || spi_pins[1].check_three || spi_pins[1].check_four)){
                
                valid_pins += 0x02;

            }

        }
        if(i == 2){

            if(spi_pins[2].spi_pin == (spi_pins[2].check_one || spi_pins[2].check_two || spi_pins[2].check_three || spi_pins[2].check_four)){
                
                valid_pins += 0x04;

            }

        }
        if(i == 3){

            if(spi_pins[1].spi_pin == (spi_pins[4].check_one || spi_pins[4].check_two || spi_pins[3].check_three || spi_pins[3].check_four)){
                
                valid_pins += 0x08;

            }

        }

    }

  printf("Current Valid Pins: %i.\n", valid_pins);

  func_ack status = ((valid_pins & 0x0F) == true) ? pins_ack : func_error;
  
  return status;
}


spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins){

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

  printf("Current SPI Instance: SPI%i.\n", ((pins->miso % 4 == 0) ? 0 : 1) );

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

  configure_instance(packet, pins);

  if (status == 2)
  {
    // set GPIO function as SPI for SCK, MOSI & MISO & CSN
    gpio_set_function(pins->sck, GPIO_FUNC_SPI);
    gpio_set_function(pins->mosi, GPIO_FUNC_SPI);
    gpio_set_function(pins->miso, GPIO_FUNC_SPI);

    // initialise CE & CSN
    gpio_init(pins->csn);
    gpio_init(pins->dc_rs);

    // set direction for CE & CSN
    gpio_set_dir(pins->csn, GPIO_OUT);
    gpio_set_dir(pins->dc_rs, GPIO_OUT);
  }
  
  return status;
};


func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins){

    printf("Initializing user input pins for SPI.\n");

    func_ack fun_status;
    func_ack configure_status;
    func_ack exit_status;

    printf("\nUser Defined Pins:\n\tMosi: %i\n\tMosi: %i\n\tSck: %i\n\tCSN: %i\n\tDC-RS: %i\n\n", pins->mosi, pins->miso, pins->sck, pins->csn, pins->dc_rs);

    configure_status = pin_manager_configure(inst, pins);
    printf("Configured: %s.\n", ((configure_status != 0) ? "Yes" : "No") );

    fun_status = general_ack;

    exit_status = ((configure_status == pins_ack) && (fun_status == general_ack)) ? pins_init_ack : func_error;

        return exit_status;

}


void csn_put_high(uint8_t csn) {

  gpio_put(csn, HIGH);

  return;
}


void csn_put_low(uint8_t csn) {

  gpio_put(csn, LOW);

  return;
}


void dcrs_put_high(uint8_t ce) {

  gpio_put(ce, HIGH);

  return;
}


void dcrs_put_low(uint8_t ce) {

  gpio_put(ce, LOW);

  return;
}


func_ack selector(bool csn, bool dc_rs, bool enable, spi_pins *pins){

    if(csn == true){

        if(enable == true){

            csn_put_high(pins->csn);

        }else{
            csn_put_low(pins->csn);
        }

    }
    if(dc_rs == true){

        if(enable == true){

            dcrs_put_high(pins->dc_rs);

        }else{

            dcrs_put_low(pins->dc_rs);

        }

    }

    return general_ack;

}


spi_packet_s spi_length(spi_packet_s *inst){

  printf("Checking length.\n");

  inst->length = (sizeof(inst->tx_buf) / sizeof(txb));

  printf("Length: %i.\n", inst->length);

  return *inst;
}


spi_part_s color_to_data(spi_part_s *parts, uint32_t color){


  parts->Data_Zero = get_0b_to_8b(color);
  parts->Data_One = get_8b_to_16b(color);
  parts->Data_Two = get_16b_to_24b(color);
  parts->Data_Three = get_24b_to_32b(color);

  return *parts;
}


func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate){

  printf("Initialize SPI Management.\n");

      // initialise SPI instance at the specified baudrate (Hz).
  spi_init(instance, baudrate);

    return general_ack;
}


func_ack deinitialize_spi_management(spi_inst_t *instance){

  printf("Deinitialize SPI Management.\n");
    
    // Deinitialize SPI instance.  
  spi_deinit(instance);

    return general_ack;
}


func_ack spi_managed_data_write(spi_pins *pins, spi_packet_s *pack, spi_part_s *part, uint8_t *command) {

//  printf("Length: %u.\n", len);
//  printf("Tx Buffer: %02x.\n", tx_buffer);
  txb packet_buf;
  spi_length(pack);
  uint8_t bytes;
  uint8_t packet_length;
  int i;
  bool lp_a;

  packet_length = (spi_check_data_size(part));
  lp_a = (packet_length > 2) ? false : true;

    spi_write_read_blocking(pack->instance, command, 0, 0);
    for(i = 0; i < 0xFF; i++);

    csn_put_high(pins->csn);
    for(int i = 0; i <= packet_length; i++){

  data_packet = packet_buffer(pack, part, i, lp_a);
      ili_delay(100);
    bytes = spi_write_read_blocking(pack->instance, &pack->tx_buf, &pack->rx_buf, pack->length);
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


func_ack spi_managed_command_write(spi_packet_s *packet, spi_part_s *part, uint8_t *command, uint8_t *param, uint8_t size){

  uint8_t bytes;
  fun stat;

  bytes = spi_write_read_blocking(packet->instance, command, 0, 0);

  if(param != 0){

  packet->spi_func_status += spi_write_read_blocking(packet->instance, param, 0, 0);
  
  }
    if(size > 0){
  
    for(int i = 0; i < size; i++){

      if(i == 0){

        packet->spi_func_status += spi_write_read_blocking(packet->instance, &part->Data_Zero, 0, 0);

      }
      if(i == 1){

        packet->spi_func_status += spi_write_read_blocking(packet->instance, &part->Data_One, 0, 0);

      }
      if(i == 2){

        packet->spi_func_status += spi_write_read_blocking(packet->instance, &part->Data_Two, 0, 0);

      }
      if(i == 3){

        packet->spi_func_status += spi_write_read_blocking(packet->instance, &part->Data_Three, 0, 0);

      }
      if(i == 4){

        packet->spi_func_status += spi_write_read_blocking(packet->instance, &part->Data_Four, 0, 0);

      }


  }

  } 

  stat = (bytes > 0) ? spi_ack : func_error;

  return stat;

}


uint8_t spi_check_data_size(spi_part_s *parts){

  uint8_t packet_size;
  //  packet_size is a minimum of two data, data_zero and data_one.
  packet_size = 0x02;

  if(parts->Data_Two > 0){

    packet_size += 0x01;
  }
  if(parts->Data_Three > 0){

    packet_size += 0x01;
  }
  if(parts->Data_Zero > 0){

    packet_size += 0x01;
  }

  return packet_size;
}


spi_part_s data_break(spi_part_s *parts, uint16_t data_in){

  parts->Data_Zero = get_0b_to_8b(data_in);
  parts->Data_One = get_8b_to_16b(data_in);

  return *parts;
}


func_ack managed_data_spi(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint8_t *command){

  uint8_t fun_stat;

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


func_ack managed_command_spi(spi_packet_s *packet, spi_part_s *parts, fun *command, fun *param, fun size){
  
  uint8_t fun_stat;
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


func_ack ili_write_ram_prep(spi_pins *pins, spi_part_s *part, spi_packet_s *packet){

    csn_put_low(pins->csn);
    
    spi_managed_command_write(packet, part, (fun)Memory_Write, 0, 0);

    return spi_ack;
}


func_ack ili_write_ram(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint32_t color){

    fun stat;
    fun d_size;

    csn_put_high(pins->csn);

    color_to_data(part, color);
    d_size = spi_check_data_size(part);
    stat += managed_command_spi(packet, part, 0, 0, d_size);

    //  Stat should return spi_ack.

    return stat;
}


func_ack ili_register_write(spi_pins *pins, spi_packet_s *packet, spi_part_s *part, uint8_t *command){

    uint8_t c_status;
    uint8_t fun_stat;
    uint8_t pack_size;
    volatile uint32_t i;
    bool l_pa;

    csn_put_low(pins->csn);
        pack_size = spi_check_data_size(part);
    c_status += managed_data_spi(pins, packet, part, command);
    for(i = 0; i < 0xFF; i++);

    if(pack_size > 0){

       pack_size = spi_check_data_size(part);
       l_pa = (pack_size > 2) ? false : true;

        csn_put_high(pins->csn);
       c_status += managed_data_spi(pins, packet, part, command);
        for(i = 0; i < 0x5F; i++);
    }

    fun_stat = ((c_status == 1) || (c_status == 2)) ? register_written : func_error;

    return fun_stat;
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


func_ack ili_set_display_dir(spi_pins *pins, spi_packet_s *pack, spi_part_s *parts, bool direction){
    
    fun stat;
    

    if(direction){
        parts->Data_Zero = 0xE8;
    }else {
        parts->Data_Zero = 0x48;
    }

    stat = ili_register_write(pins, pack, parts, (fun)Memory_Access_Control);

    // Should return register_written, 9.
    return stat;

}


func_ack ili_set_window(ili9488_window_var_s *window, spi_packet_s *pack, spi_part_s *parts){

    fun stat;
    fun e_stat;
    uint16_t col_s, col_e, row_s, row_e;
    uint8_t count;

    count = spi_check_data_size(parts);

    col_s = window->dw_X;
    col_e = (window->dw_Width + (window->dw_X - 1));

    row_s = window->dw_Y;
    row_e = (window->dw_Height + (window->dw_Y - 1));

    parts->Data_Zero = get_8b_to_16b(col_s);
    parts->Data_One = get_0b_to_8b(col_s);
    parts->Data_Two = get_8b_to_16b(col_e);
    parts->Data_Three = get_0b_to_8b(col_e);

    stat += managed_command_spi(pack, parts, (fun)Column_Address_Set, 0, count);
    stat += managed_command_spi(pack, parts, (fun)NOP, 0, 0);

    parts->Data_Zero = get_8b_to_16b(row_s);
    parts->Data_One = get_0b_to_8b(row_s);
    parts->Data_Two = get_8b_to_16b(row_e);
    parts->Data_Three = get_0b_to_8b(row_e);

    stat += managed_command_spi(pack, parts, (fun)Page_Address_Set, 0, count);
    stat += managed_command_spi(pack, parts, (fun)NOP, 0, 0);

    e_stat = (stat == 0x04) ? spi_ack : func_error;

    return e_stat;
}


func_ack ili_set_foreground_color(ili9488_window_var_s *window, spi_packet_s *pack, spi_part_s *parts, uint32_t color_i){

    fun stat;

	for (int i = 0; i < (sizeof(pixel_cache)/sizeof(uint16_t)); ) {

		pixel_cache[i++] = color_i >> 16;
		pixel_cache[i++] = color_i >> 8;
		pixel_cache[i++] = color_i & 0xFF;

	}

    stat = (pixel_cache[0] != 0) ? general_ack : func_error;

    return stat;

}


func_ack ili_set_cursor_pos(cord x, cord y, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){
	/* Set Horizontal Address Start Position */

    fun stat;
    fun e_stat;
    uint8_t count;

    count = spi_check_data_size(parts);

	parts->Data_Zero = get_8b_to_16b(x);
	parts->Data_One = get_0b_to_8b(x);
	parts->Data_Two = get_8b_to_16b(x);
	parts->Data_Three = get_0b_to_8b(x);
  stat += ili_register_write(pins, pack, parts, (fun)Column_Address_Set);
	stat += ili_register_write(pins, pack, parts, (fun)NOP);

	/* Set Horizontal Address End Position */
	parts->Data_Zero = get_8b_to_16b(y);
	parts->Data_One = get_0b_to_8b(y);
	parts->Data_Two = get_8b_to_16b(y);
	parts->Data_Three = get_0b_to_8b(y);
	stat += ili_register_write(pins, pack, parts, (fun)Page_Address_Set);
	stat += ili_register_write(pins, pack, parts, (fun)NOP);

    e_stat = (stat == 36) ? general_ack : func_error;

    return e_stat;
}


func_ack ili_fill(uint16_t color, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;

    uint32_t dw;

    stat += ili_set_cursor_pos(0, 0, parts, pins, pack);
    stat += ili_write_ram_prep(pins, parts, pack);

    for(dw = (ILI_LCD_WIDTH * ILI_LCD_HEIGHT); dw > 0; dw--){

     stat += ili_write_ram(pins, pack, parts, color);

    }

}


func_ack ili_draw_pixel(uint32_t x, uint32_t y, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;
    uint32_t i;

    if((x >= ILI_LCD_WIDTH) || (y >= ILI_LCD_HEIGHT)){
        return 1;
    }

    ili_set_cursor_pos(x, y, parts, pins, pack);

    ili_write_ram_prep(pins, parts, pack);

    while(pixel_cache[i] != '\n'){

    ili_write_ram(pins, pack, parts, pixel_cache[i++]);
    
    }

    return general_ack;

}


func_ack ili_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;

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

    ili_draw_pixel(x, y, parts, pins, pack);

    if(d_x > d_y){
        cumulation = (d_x >> 1);

        for(int i = 1; i <= d_x; i++){

            x += x_inc;
            cumulation += d_y;

            if(cumulation >= d_x) {

                cumulation -= d_x;
                y += y_inc;

            }

    ili_draw_pixel(x, y, parts, pins, pack);

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

    ili_draw_pixel(x, y, parts, pins, pack);
        }
    }

    return general_ack;
}


func_ack ili_draw_prep(uint32_t x, uint32_t y, uint32_t width, uint32_t height, ili9488_window_var_s *window, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;

    window->dw_X = x;
    window->dw_Y = y;
    window->dw_Width = width;
    window->dw_Height = height;

    ili_set_window(window, pack, parts);

    ili_set_cursor_pos(x, y, parts, pins, pack);

    ili_write_ram_prep(pins, parts, pack);

    return general_ack;

}


func_ack ili_draw_char(uint32_t x, uint32_t y, uint8_t character, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;

    uint32_t row, col;
    uint32_t o_set, o_set1, o_set2;

    o_set = ((uint32_t)character - 0x20) * 20;

    for(col = 0; col < 10; col++){
        o_set1 = o_set + col * 2;
        o_set2 = o_set1 + 1;

        for(row = 0; row < 8; row++){
            if((charset10x14[o_set1] >> (7 - row)) & 0x01){
                
                ili_draw_pixel((x + col), (y + row + 8), parts, pins, pack);

            }

        for (row = 0; row < 6; row++){
            if((charset10x14[o_set2] >> (7 - row) & 0x01)){
                
                ili_draw_pixel((x + col), (y + row + 8), parts, pins, pack);

        }
      }
    }
  }
}


func_ack ili_draw_string(uint32_t x, uint32_t y, uint8_t *string_ptr, spi_part_s *parts, spi_pins *pins, spi_packet_s *pack){

    fun stat;

    uint32_t x_org = x;

    while(*string_ptr != 0){

        if(*string_ptr == '\n'){
            y += my_font.f_height + 2;
            x = x_org;
        }else {
            ili_draw_char(x, y, *string_ptr, parts, pins, pack);
            x += my_font.f_width + 2;
        }
    string_ptr++;

    }
    return general_ack;
}


typedef struct ili_init_var_s {
    
  struct ili_operation_var_s my_op;
  struct spi_packet_s data_packet;
  struct spi_part_s my_parts;
  struct ili9488_window_var_s my_window;
	struct spi_pin_manager_s my_pins;

}ili_init_var_s;

ili_init_var_s my_init;


func_ack ili_initialize(ili_init_var_s *init);


func_ack ili_initialize(ili_init_var_s *init){

  printf("\nILI Initialize:\n");

    fun stat;
    fun e_stat;
    uint16_t color;
    printf("Send: Soft Reset.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), SOFT_RESET, 0, 0);
    ili_delay(200);

    printf("Send: Sleep Out.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), Sleep_OUT, 0, 0);
    ili_delay(200);

    printf("Send: Memory Access Control.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts),Memory_Access_Control, 0x48, 0);
    ili_delay(200);

    printf("Send: CABC Control Nine.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), CABC_Control_Nine, 0x04, 0);
    ili_delay(200);

    printf("Send: Interface Pixel Format.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), Interface_Pixel_Format, 0x05, 0);
    ili_delay(200);

    printf("Send: Normal Display Mode On.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), Normal_Display_Mode_ON, 0, 0);
    ili_delay(200);

    printf("Send: Display On.\n");
    stat += managed_command_spi(&(init->my_op), &(init->my_parts), Display_ON, 0, 0);
    ili_delay(200);

    printf("Set: Display Dir>>Normal.\n");
    stat += ili_set_display_dir(&(init->my_op), &(init->my_op), &(init->my_parts), true);
    ili_delay(200);

    printf("Set: Foreground Color:DarkGreen?\n");
    stat += ili_set_foreground_color(&(init->my_window), &(init->my_op), &(init->my_parts), &(init->my_op.ili_foreground_color));
    ili_delay(200);

    printf("Set: Cursor Position.\n");
    stat += ili_set_cursor_pos(0, 0, (&my_parts), (&my_pins), (&data_packet));
    ili_delay(200);

    e_stat = (stat == 24) ? ili_initialized : func_error;
    printf("Exiting with status: %i.\n", e_stat);

    return e_stat;
}


#endif