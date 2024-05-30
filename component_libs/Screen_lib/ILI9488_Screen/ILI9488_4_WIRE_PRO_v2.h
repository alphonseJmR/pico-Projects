#ifndef ILI9488_4_WIRE_PRO_H
#define ILI9488_4_WIRE_PRO_H

#include <stdio.h>
#include <stdint.h>
#include "ili9488_screen_commands.h"
#include "ili9488_error_management.h"
#include "ili9488_power_control.h"
#include "./peri_header.h"
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

const ebit charset10x14[] = {
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

typedef struct spi_packet_s {
    
    spi_inst_t *instance;
    ebit tx_buf;
    ebit rx_buf;
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

    uint miso;
    uint mosi;
    uint csn;
    uint sck;

    uint dc_rs;
    uint reset;
    uint led_bl;

}spi_pins;


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
  .tx_buf = 0,
  .rx_buf = 0,
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

  .miso = GPIO_FOUR,
  .mosi = GPIO_THREE,
  .csn = GPIO_FIVE,
  .sck = GPIO_TWO,
  .dc_rs = GPIO_ZERO,
  .reset = GPIO_ONE,
  .led_bl = GPIO_FIFTEEN

  };



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
func_ack ili_fill_color(spi_packet_s *pack, ebit ba_pixels[][3], sbit x, sbit y, tbit color);
func_ack ili_set_cursor_pos(spi_packet_s *pack, sbit x, sbit y);
func_ack ili_fill(spi_packet_s *pack, ebit pixels[][3]);

func_ack ili_draw_pixel(spi_packet_s *pack, tbit x, tbit y, tbit color);
func_ack ili_draw_line(spi_packet_s *pack, tbit color, sbit x1, sbit y1, sbit x2, sbit y2);
func_ack ili_draw_prep(ili9488_window_var_s *window, spi_packet_s *pack, sbit x, sbit y, sbit width, sbit height);
func_ack ili_draw_char(spi_packet_s *pack, sbit x, sbit y, char character, tbit color);
func_ack ili_draw_string(spi_packet_s *pack, tbit x, tbit y, char *string_ptr, tbit color);

//  ILI9488 initialization function prototypes.
func_ack ili_pos_gam_ctrl(spi_packet_s *pack);
func_ack ili_neg_gam_ctrl(spi_packet_s *pack);
func_ack ili_pwr_ctrl_o(spi_packet_s *pack);
func_ack ili_pwr_ctrl_t(spi_packet_s *pack);
func_ack ili_vcom_ctrl(spi_packet_s *pack);
func_ack ili_mem_acc_ctrl(spi_packet_s *pack);
func_ack ili_pix_intr_format(spi_packet_s *pack);
func_ack ili_intr_mode_ctrl(spi_packet_s *pack);
func_ack ili_frame_rate_ctrl(spi_packet_s *pack);
func_ack ili_dspy_inver(spi_packet_s *pack);
func_ack ili_dspy_func_ctrl(spi_packet_s *pack);
func_ack ili_s_out(spi_packet_s *pack);
func_ack ili_dspy_on(spi_packet_s *pack);


// Write len bytes directly from src to the SPI, and discard any data received back
int __not_in_flash_func(spi_write_blocked)(spi_inst_t *spi, const uint8_t *src, size_t len, bool d_cx) {
    uint i;
    for (i = 0; i < len; ++i) {
        while (!spi_is_writable(spi))
            tight_loop_contents();
          if(i < len - 1){
        spi_get_hw(spi)->dr = (uint32_t)src[i];
          }else{
            gpio_put(my_pins.dc_rs, d_cx);
            spi_get_hw(spi)->dr = (uint32_t)src[i];
          }
    }
    // Drain RX FIFO, then wait for shifting to finish (which may be *after*
    // TX FIFO drains), then drain RX FIFO again
    while (spi_is_readable(spi))
        (void)spi_get_hw(spi)->dr;
    while (spi_get_hw(spi)->sr & SPI_SSPSR_BSY_BITS)
        tight_loop_contents();
    while (spi_is_readable(spi))
        (void)spi_get_hw(spi)->dr;

    // Don't leave overrun flag set
    spi_get_hw(spi)->icr = SPI_SSPICR_RORIC_BITS;

    return (int)len;
}


static func_ack pin_validate(spi_pins *pins) {

  printf("Validating pin selection.\n\n");

  printf("Checking MISO pin: %i.\n\n", pins->miso);
  printf("Checking MOSI pin: %i.\n\n", pins->mosi);
  printf("Checking CSN pin: %i.\n\n", pins->csn);
  printf("Checking SCK pin: %i.\n\n", pins->sck);

    //  Validate four main SPI pins.
        //  Valid Pin setup: mosi - 0x01, miso - 0x02, csn - 0x04, sck - 0x08
    ebit valid_pins = 0x00;

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


func_ack gpio_setup(spi_pins *pins){

    ebit pin_stat;
    pin_stat = 0;

    printf("Setting Up User Defined Pins.\n");

    // set GPIO function as SPI for SCK, MOSI & MISO & CSN
    gpio_set_function(pins->sck, GPIO_FUNC_SPI);
        pin_stat++;
    gpio_set_function(pins->mosi, GPIO_FUNC_SPI);
        pin_stat++;
    gpio_set_function(pins->miso, GPIO_FUNC_SPI);
        pin_stat++;
    gpio_set_function(pins->csn, GPIO_FUNC_SPI);
        pin_stat++;

    // initialise CE & CSN
  //  gpio_init(pins->csn);
    gpio_init(pins->dc_rs);
        pin_stat++;
    gpio_init(pins->reset);
        pin_stat++;

    // set direction for CE & CSN
  //  gpio_set_dir(pins->csn, GPIO_OUT);
    gpio_set_dir(pins->dc_rs, GPIO_OUT);
        pin_stat++;
    gpio_set_dir(pins->reset, GPIO_OUT);
        pin_stat++;
    gpio_set_dir(pins->miso, GPIO_IN);
        pin_stat++;

    return (pin_stat == 9) ? pins_ack : func_error;

}


//  Pin Manager Configure
    //  Mosi  - Master Out Slave In
    //  Miso  - Master In Slave Out
    //  Sck   - Clock Signal
    //  CSN   - Chip Select Not
    //  Dc_Rs - Data/Command Register Select
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins) {

  printf("Configuring Pins for Use.\n");

  ebit status;
    status = 0;

  status += pin_validate(pins);

  if(status == 2){

  configure_instance(packet, pins);
    status += gpio_setup(pins);

  }else{

    printf("Pins not valid.\n");

    return func_error;
  }

  return (status == 4) ? pins_init_ack : func_error;
};


func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins){

    printf("Initializing user input pins for SPI.\n");

    func_ack status;
    func_ack configure_status;
    func_ack exit_status;
    sbit rate_running;

    printf("\nUser Defined Pins:\n\n\tMosi: %i\n\tMiso: %i\n\tSck: %i\n\tCSN: %i\n\tDC-RS: %i\n\n", pins->mosi, pins->miso, pins->sck, pins->csn, pins->dc_rs);

    configure_status = pin_manager_configure(inst, pins);
    printf("Configured: %s.\n", ((configure_status != 0) ? "Yes" : "No") );

    printf("Initialize SPI Protocol Parameters.\n");

    rate_running = spi_init(inst->instance, inst->rate);

    printf("Baudrate: %i.\n", rate_running);

    status = general_ack;

    exit_status = ((configure_status == pins_init_ack) && (status == general_ack)) ? pins_init_ack : func_error;

        return exit_status;

}

//  Quick call functions for putting pins, CSN, DC/RS, and Reset pins HIGH/LOW.

void csn_put_high(bit csn) {

  gpio_put(csn, HIGH);
    sleep_ms(5);

  return;
}

void csn_put_low(bit csn) {

  gpio_put(csn, LOW);
    sleep_ms(5);

  return;
}

void dcrs_put_high(bit ds) {

//  printf("Putting Pin: %i.\n", ds);

  gpio_put(ds, HIGH);
    sleep_ms(5);

  return;
}

void dcrs_put_low(bit ds) {

  gpio_put(ds, LOW);
    sleep_ms(5);

  return;
}

void reset_put_high(bit rs){

    gpio_put(rs, HIGH);
      sleep_ms(5);

    return;
}

void reset_put_low(bit rs){

    gpio_put(rs, LOW);
    sleep_ms(5);

    return;

}

void reset_pulse(bit rs){

  reset_put_high(rs);
    sleep_us(2);
  reset_put_low(rs);
}

void com_start(spi_pins *pins){
  
  dcrs_put_low(pins->dc_rs);
    sleep_ms(5);
  csn_put_low(pins->csn);  // Low for command.
    sleep_ms(5);
  
}


void data_start(spi_pins *pins){

  dcrs_put_high(pins->dc_rs);
    sleep_ms(5);
}

void data_end(spi_pins *pins){

  dcrs_put_low(pins->dc_rs);
    sleep_ms(5);
  csn_put_high(pins->csn);
    sleep_ms(5);

}


ebit spi_length(ebit input){

  ebit sized;
  sized = 0x00;

//  printf("Checking length.\n");

  sized = (sizeof(input) / sizeof(ebit));

//  printf("Length: %i.\n", sized);

  return sized;
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

//  Passes NO PARAMS.
//  Return SPI ACK or error.
func_ack spi_command_write(spi_packet_s *inst, ebit command){
printf("Setting IDLE Mode.\n");

  ebit status;
  ebit com_, dat_;
  status = 0;

  dcrs_put_low(my_pins.dc_rs);
  csn_put_low(my_pins.csn);
 
  printf("Success.\n");
  com_ = Sleep_OUT;
  printf("Success.\n");
  COM = &com_;
  printf("Success.\n");
  inst->length = spi_length(Sleep_OUT);
  status += spi_write_blocking(inst->instance, COM, inst->length);
  printf("Success.\n");

  inst->length = spi_length(command);
  printf("Success.\n");
  com_ = command;
  printf("Success.\n");
  COM = &com_;
  printf("Success.\n");
  status += spi_write_blocking(inst->instance, COM, inst->length);
  printf("Success.\n");

  com_ = Sleep_IN;
  printf("Success.\n");
  COM = &com_;
  printf("Success.\n");
  inst->length = spi_length(Sleep_IN);
  status += spi_write_blocking(inst->instance, COM, inst->length);
  printf("Success.\n");

  dcrs_put_high(my_pins.dc_rs);
  csn_put_high(my_pins.csn);
  printf("Success.\n");

  return (status == 3) ? spi_ack : func_error;
}


//  Use to send command with (0 || 1) parameters.
//  Return SPI ACK or error.
func_ack spi_spa_command_write(spi_packet_s *inst, ebit command, ebit param){

    ebit status;
    ebit f_status;
    ebit com_, dat_;
    status = 0;

  printf("\n\nUpdated SPI SPA Command Write Function.\n");
  initialize_spi_management(inst->instance, inst->rate);

    com_start(&my_pins);

  com_ = command;
  com = &com_;;
    inst->length = spi_length(command);
  status = spi_write_blocking(inst->instance, com, inst->length);
  f_status += status;

    data_start(&my_pins);

  dat_ = param;
  dat = & dat_;
  inst->length = spi_length(param);
  status = spi_write_blocking(inst->instance, dat, inst->length);
  f_status += status;

    data_end(&my_pins);


  deinitialize_spi_management(inst->instance);

  return (f_status % 2 == 0) ? spi_ack : func_error;

}


//  Use to send command with up to 4 parameters.
//  Return SPI ACK or error.
func_ack spi_lpa_command_write(spi_packet_s *inst, ebit command, tbit long_param){

  initialize_spi_management(inst->instance, ONE_MBS);

  printf("Updated SPI LPA Command Write.\n");

    ebit data_one, data_two, data_three, data_four;
    ebit status;
    ebit f_status;
    bool final;
    ebit com_, dat_;

    status, f_status = 0;

    if(long_param > 0x1FFFFFF){
      printf("Param is 32bit.\n");
      // 32 Bit Data.

    data_one = (get_24b_to_32b(long_param));
    data_two = (get_16b_to_24b(long_param));
    data_three = (get_8b_to_16b(long_param));
    data_four = (get_0b_to_8b(long_param));
    d_one = &data_one;
    d_two = &data_two;
    d_three = &data_three;
    d_four = &data_four;

    }else if((long_param > 0x1FFFF) && (long_param <= 0xFFFFFF)) {
      printf("Param is 24bit.\n");
      //24 Bit Data.

    data_one = (get_24b_to_32b(long_param));
    data_two = (get_16b_to_24b(long_param));
    data_three = (get_8b_to_16b(long_param));
    data_four = 0;
    d_one = &data_one;
    d_two = &data_two;
    d_three = &data_three;
    d_four = &data_four;

    }else {
      printf("Param is 16bit.\n");
      // 16 Bit Data.

    data_one = (get_24b_to_32b(long_param));
    data_two = (get_16b_to_24b(long_param));
    data_three = 0;
    data_four = 0;
    d_one = &data_one;
    d_two = &data_two;
    d_three = &data_three;
    d_four = &data_four;

    }
  
    com_start(&my_pins);
    printf("COM Starting.\n");
  com_ = command;
  com = &com_;;
  inst->length = spi_length(command);
  status = spi_write_blocking(inst->instance, com, inst->length);
  f_status += status;
  status = 0;

    printf("F_status: %i.\n", f_status);

    data_start(&my_pins);

    printf("Sending MSB: 0x%02x.\n", d_one);

  inst->length = spi_length(*d_one);
  status = spi_write_blocking(inst->instance, d_one, inst->length);
  f_status += status;
  status = 0;
  printf("F_status: %i.\n", f_status);

    data_end(&my_pins);

    data_start(&my_pins);

    printf("Sending Next Bit or LSB: 0x%02x.\n", d_two);

  inst->length = spi_length(*d_two);
  status = spi_write_blocking(inst->instance, d_two, inst->length);
  f_status += status;
  status = 0;
  printf("F_status: %i.\n", f_status);

    data_end(&my_pins);

  if(*d_three != 0){

    data_start(&my_pins);

  printf("Sending Next Bit or LSB: 0x%02x.\n", d_three);

  inst->length = spi_length(*d_three);
  status = spi_write_blocking(inst->instance, d_three, inst->length);
  status = 0;
  f_status += status;
  printf("F_status: %i.\n", f_status);

    data_end(&my_pins);

  }else if (*d_four != 0){
    
        data_start(&my_pins);

  printf("Sending Next Bit or LSB: 0x%02x.\n", d_four);

  inst->length = spi_length(*d_four);
  status = spi_write_blocking(inst->instance, d_four, inst->length);
  f_status += status;
  printf("F_status: %i.\n", f_status);

    data_end(&my_pins);
  }

  com_ = NOP;
  com = &com_;;
  inst->length = spi_length(*com);
  status = spi_write_blocking(inst->instance, com, inst->length);
  f_status += status;
  printf("F_status: %i.\n", f_status);

  printf("Final Status: %i.\n", f_status);

  if(f_status % 2 == 0){
    final = true;
  }else if(f_status % 3 == 0){
    final = true;
  }else if(f_status % 4 == 0){
    final = true;
  }else if(f_status % 5 == 0) {
    final = true;
  }else {
    final = false;
  }

  deinitialize_spi_management(inst->instance);

  return (final) ? spi_ack : func_error;

}


//  Return SPI ACK or error.
func_ack spi_spa_data_write(spi_packet_s *inst, ebit command, tbit data){


    ebit data_one;
    ebit data_two;
    ebit data_three;
    ebit data_four;
    ebit status;
    ebit f_status;

    status, f_status = 0;

    if(data > 0x1FFFFFF){
      // 32 Bit Data.

    data_one = (get_24b_to_32b(data));
    data_two = (get_16b_to_24b(data));
    data_three = (get_8b_to_16b(data));
    data_four = (get_0b_to_8b(data));

    }else if(data > 0x1FFFF && data < 0x1FFFFFF) {
      //24 Bit Data.

    data_one = (get_16b_to_24b(data));
    data_two = (get_8b_to_16b(data));
    data_three = (get_0b_to_8b(data));
    data_four = 0;

    }else {

    data_one = (get_8b_to_16b(data));
    data_two = (get_0b_to_8b(data));
    data_three = 0;
    data_four = 0;

    }
  
  printf("\n\nUpdated SPI LPA Data Write Function.\n");

  initialize_spi_management(inst->instance, inst->rate);

    com_start(&my_pins);

    inst->length = spi_length(command);
  status = spi_write_blocking(inst->instance, &command, inst->length);
  f_status += status;

    data_start(&my_pins);

  inst->length = spi_length(data_one);
  status = spi_write_blocking(inst->instance, &data_one, inst->length);
  f_status += status;

    data_end(&my_pins);

        data_start(&my_pins);

  inst->length = spi_length(data_two);
  status = spi_write_blocking(inst->instance, &data_two, inst->length);
  f_status += status;

    data_end(&my_pins);

        data_start(&my_pins);

  inst->length = spi_length(data_three);
  status = spi_write_blocking(inst->instance, &data_three, inst->length);
  f_status += status;

    data_end(&my_pins);

        data_start(&my_pins);

  inst->length = spi_length(data_four);
  status = spi_write_blocking(inst->instance, &data_four, inst->length);
  f_status += status;

    data_end(&my_pins);


  printf("Final Status: %i.\n", f_status);

  deinitialize_spi_management(inst->instance);


  return (f_status % 2 == 0) ? spi_ack : func_error;

}


func_ack ili_check_coords(limits *coords){

  printf("Validating coordinates are within range.\n");

    uint32_t par;
    ebit cord_status;
    cord_status = 0;

    if(coords->x_start >= ILI_LCD_WIDTH) {
        coords->x_start = ILI_LCD_WIDTH - 1;
    }
    cord_status += 0x01; 

    if(coords->x_end >= ILI_LCD_WIDTH){
        coords->x_end = ILI_LCD_WIDTH - 1;
    }
    cord_status += 0x02;

    if(coords->y_start >= ILI_LCD_WIDTH) {
        coords->y_start = ILI_LCD_WIDTH - 1;
    }
    cord_status += 0x04;

    if(coords->y_end >= ILI_LCD_WIDTH){
        coords->y_end = ILI_LCD_WIDTH - 1;
    }
    cord_status += 0x08;

    if(coords->x_start > coords->x_end) {
        par = coords->x_start;
        coords->x_start = coords->x_end;
        coords->x_end = par;
    }
    cord_status += 0x10; 

    if(coords->y_start > coords->y_end){
        par = coords->y_start;
        coords->y_start = coords->y_end;
        coords->y_end = par;
    }
    cord_status += 0x20;

    return (cord_status == 0x3F) ? coords_ack : func_error;
}


//  return ILI ACK or error.
func_ack ili_set_init_window(ili9488_window_var_s *window, spi_packet_s *pack){

    ebit cord_check;
    ebit com_, dat_;

    cord_check = 0;
    cord_check = ili_check_coords(&my_limits);
    printf("Coord Check Returned: %i.\n", cord_check);
      sleep_ms(700);
    sbit xs_val, xe_val, ys_val, ye_val;
    xs_val, xe_val, ys_val, ye_val = 0;

  printf("ILI Setting Window.\n\n");

    ebit status;
    sbit col_s, col_e, row_s, row_e;

    col_s = window->dw_X;
    printf("Col_s: %i.\n", col_s);
    col_e = (window->dw_Width + (window->dw_X - 1));
    printf("Col_e: %i.\n", col_e);
    row_s = window->dw_Y;
    printf("Row_s: %i.\n", row_s);
    row_e = (window->dw_Height + (window->dw_Y - 1));
    printf("Row_e: %i.\n", row_e);

    csn_put_low(my_pins.csn);
    dcrs_put_low(my_pins.dc_rs);

    printf("Sending Command: Column Address Start/End.\n");
    com_ = Sleep_OUT;
    COMM = &com_;
    status += spi_write_blocking(pack->instance, COMM, 1);

    com_ = Column_Address_Set;
    COMM = &com_;
    printf("\n\nValue of COM pointer: 0x%02x.\n\n", *COMM);
    status += spi_write_blocking(pack->instance, COMM, 1);

    dcrs_put_high(my_pins.dc_rs);
    dat_ = col_s;
    DATA = &dat_;
    printf("\n\nValue of DATA pointer : 0x%02x.\n\n", *DATA);
    status += spi_write_blocking(pack->instance, DATA, 1);
    dat_ = col_e;
    DATA = &dat_;
    printf("\n\nValue of DATA pointer : 0x%02x.\n\n", *DATA);
    status += spi_write_blocking(pack->instance, DATA, 1);

    dcrs_put_low(my_pins.dc_rs); 
    status += spi_spa_command_write(pack, NOP, 1);

    initialize_spi_management(pack->instance, ONE_MBS);

    printf("Sending Command: Page Address Start.\n");
    com_ = Sleep_OUT;
    COMM = &com_;
    status += spi_write_blocking(pack->instance, COMM, 1);
    
    com_ = Page_Address_Set;
    COMM = &com_;
    printf("\n\nValue of COM pointer (Page Addr Start): 0x%02x.\n\n", *COMM);
    status += spi_write_blocking(pack->instance, COMM, 1);
     

    dcrs_put_high(my_pins.dc_rs);
    dat_ = row_s;
    DATA = &dat_;
    status += spi_write_blocking(pack->instance, DATA, 1);
    dat_ = row_e;
    DATA = &dat_;
    status += spi_write_blocking(pack->instance, DATA, 1);

    dcrs_put_low(my_pins.dc_rs);
    com_ = NOP;
    COMM = &com_;
    status += spi_write_blocking(pack->instance, COMM, 1);

    printf("NOP Sent.\n");

    com_ = Sleep_IN;
    COMM = &com_;
    status += spi_write_blocking(pack->instance, COMM, 1);

    csn_put_high(my_pins.csn);

    return (status == 10) ? ili_ack : func_error;

}


//  Returns SPI_ACK or error.
func_ack ili_set_user_window(spi_packet_s *pack, sbit xs, sbit xe, sbit ys, sbit ye){

    ebit cord_check;
    ebit com_, dat_;
    ebit status;
    sbit col_s, col_e, row_s, row_e;
    cord_check = 0;
    cord_check = ili_check_coords(&my_limits);
    printf("Coord Check Returned: %i.\n", cord_check);
    sbit xs_val, xe_val, ys_val, ye_val;
    xs_val, xe_val, ys_val, ye_val = 0;


  printf("ILI Setting Window.\n\n");

    col_s = xs;
    printf("Col_s: %i.\n", col_s);
    col_e = xe;
    printf("Col_e: %i.\n", col_e);
    row_s = ys;
    printf("Row_s: %i.\n", row_s);
    row_e = ye;
    printf("Row_e: %i.\n", row_e);

    csn_put_low(my_pins.csn);
    dcrs_put_high(my_pins.dc_rs);

    printf("Sending Command: Column Address Start/End.\n");

    com_ = Column_Address_Set;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);
    status = 0;

    dcrs_put_low(my_pins.dc_rs);
    dat_ = col_s;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    status = 0;
    dat_ = col_e;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    status = 0;

    dcrs_put_high(my_pins.dc_rs); 
    printf("Sending Command: Page Address Start.\n");
    com_ = Page_Address_Set;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);
    status = 0;

    dcrs_put_low(my_pins.dc_rs);
    dat_ = row_s;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    status = 0;
    dat_ = row_e;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    status = 0;

    dcrs_put_high(my_pins.dc_rs);
    com_ = NOP;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 0);
    status = 0;

    com_ = Sleep_IN;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);
    status = 0;

    csn_put_high(my_pins.csn);


    return (status == 8) ? spi_ack : func_error;

}

/*
//  Return GENERAL ACK or error.
func_ack ili_set_foreground_color(ili9488_window_var_s *window, spi_packet_s *pack, uint32_t color_i){

  if(color_i == 0){
    color_i = 0xFFFFFF;
  }

  printf("Setting ILI Foreground Color.\n\n");
    sleep_ms(500);

    ebit status;
    status = 0;

    printf("Size of pixel array: 0x%04x.\n", max_iterations);

	for (int i = 0; i < max_iterations; ) {

		pixel_cache[i][0] = (( color_i >> 16) & 0xFC);  // Previous Value was 16
    //        status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
		pixel_cache[i][1] = (( color_i >> 8) & 0xFC);    // Previous Value was 8
    //        status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
		pixel_cache[i][2] = ( color_i & 0xFC);  // Previous Value was 0xFF
     //       status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
	}
    
    return (pixel_cache[1] != 0) ? general_ack : func_error;

}
*/

//  Return GENERAL ACK or error.
func_ack ili_set_background_color(spi_packet_s *pack, uint32_t color_i){

  if(color_i == 0){
    color_i = 0xFFFFFF;
  }

  printf("Setting ILI Background Color.\n\n");
  printf("Chosen color: 0x%06x.\n\n", color_i);
    sleep_ms(500);

    ebit status;
    status = 0;
    printf("Size of pixel array: %i.\n", (max_iterations / 3));

	for (int i = 0; i < (max_iterations / 3); i++) {

		pixel_cache[i][0] = (( color_i >> 16) & 0xFC);  // Previous Value was 16
    //        status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
		pixel_cache[i][1] = (( color_i >> 8) & 0xFC);    // Previous Value was 8
    //        status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
		pixel_cache[i][2] = ( color_i & 0xFC);  // Previous Value was 0xFF
     //       status += spi_spa_data_write(pack, Memory_Write_Continue, pixel_cache[i]);
	}
    
    return (pixel_cache[1][0] != 0x00) ? general_ack : func_error;

}


//  Return ILI_ACK or error.
func_ack ili_set_cursor_pos(spi_packet_s *pack, sbit x, sbit y){
	/* Set Horizontal Address Start Position */

    ebit status;
    ebit f_status;

    ebit com_, dat_;

    ebit x_s, x_e, y_s, y_e;
    x_s, x_e, y_s, y_e = 0;

    x_s = ((x >> 8) & 0xFF);
    x_e = (x & 0xFF);
    y_s = ((y >> 8) & 0xFF);
    y_e = (y & 0xFF);

    com_start(&my_pins);
    com_ = Column_Address_Set;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);
    
    dcrs_put_high(my_pins.dc_rs);
    dat_ = x_s;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);

    dat_ = x_e;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    dcrs_put_low(my_pins.dc_rs);


    com_start(&my_pins);
    com_ = Page_Address_Set;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);

    dcrs_put_high(my_pins.dc_rs);
    dat_ = y_s;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);

    dat_ = y_e;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    dcrs_put_low(my_pins.dc_rs);

    return (status == 5) ? ili_ack : func_error;

}


//  Return ILI ACK or error.
func_ack ili_fill(spi_packet_s *pack, ebit pixels[][3]){

    ebit status;
    ebit f_status;
    sbit r, g, b;
    ebit com_, dat_;

    printf("Iterations: %i.\n", (max_iterations / 3));

    status, f_status = 0;
    r, g, b = 0;

    csn_put_low(my_pins.csn);
    dcrs_put_high(my_pins.dc_rs);

    com_ = Memory_Write;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1); // returns 1
    f_status += status;
    status = 0;

    dcrs_put_low(my_pins.dc_rs);

    for( int i = (max_iterations / 3); i > 0; i--){

      r = (pixels[i][0]);
      g = (pixels[i][1]);
      b = (pixels[i][2]);

    dat_ = r;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1);
    f_status += status;
    status = 0;

    dat_ = g;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1); // returns 1
    f_status += status;
    status = 0;

    dat_ = b;
    DAT = &dat_;
    status += spi_write_blocking(pack->instance, DAT, 1); // returns 1
    f_status += status;
    status = 0;

        if(i > ((max_iterations / 3) - 150)){
      printf("\n\nCurrent Iteration: %i.\n\tOut Of: %i\n\n", i, (max_iterations / 3));
        }
    }

    data_end(&my_pins);

    return (f_status == 4) ? ili_ack : func_error;

}


func_ack ili_fill_color(spi_packet_s *pack, ebit ba_pixels[][3], sbit x, sbit y, tbit color){

  printf("Filling ILI screen color.\n");

  ebit status, f_status;
  status, f_status = 0;

  printf("Pixel Cache Size: %i.\n", (max_iterations / 3));

  status += ili_set_user_window(pack, 0, x, 0, y);
  f_status = status;
  status = 0;

  status += ili_fill(pack, ba_pixels);
  f_status = status;
  status = 0;

  return (f_status == 4) ? ili_ack : func_error;

}


//  Returns ILI ACK or error.
func_ack ili_draw_pixel(spi_packet_s *pack, tbit x, tbit y, tbit color){

  printf("\nDrawing Pixel.\n");

    ebit status, f_status, i;
    status, f_status, i = 0;
    ebit r, g, b;
    ebit com_, dat_;
    r, g, b = 0;

    r = ((color >> 16) & 0xFC);
    g = ((color >> 8) & 0xFC);
    b = (color & 0xFC);


    if((x >= ILI_LCD_WIDTH) || (y >= ILI_LCD_HEIGHT)){
        return func_error;
    }

    com_start(&my_pins);
    com_ = Sleep_OUT;
    COM = &com_;
    status += spi_write_blocking(pack->instance, COM, 1);
    f_status += status;
    status = 0;

    status = ili_set_cursor_pos(pack, x, y);  // Should return 3.
    f_status += status;
    status = 0;

    com_start(&my_pins);

    com_ = Memory_Write;
    COM = &com_;
    status = spi_write_blocking(pack->instance, COM, 1);
    f_status += status;
    status = 0;

    data_start(&my_pins);

    dat_ = r;
    DAT = &dat_;
    pack->length = spi_length(r);
    status += spi_write_blocking(pack->instance, DAT, pack->length);
    f_status += status;
    status = 0;

    dat_ = g;
    DAT = &dat_;
    pack->length = spi_length(g);
    status += spi_write_blocking(pack->instance, DAT, pack->length);
    f_status += status;
    status = 0;

    dat_ = b;
    DAT = &dat_;
    pack->length = spi_length(b);
    status += spi_write_blocking(pack->instance, DAT, pack->length);
    f_status += status;
    status = 0;

    return ((f_status <= 6) || (f_status >= 9) ) ? ili_ack : func_error;

}


// Return ILI ACK or error.
func_ack ili_draw_line(spi_packet_s *pack, tbit color, sbit x1, sbit y1, sbit x2, sbit y2){

  printf("\nDrawing Line.\n\n");

    ebit status;
    tbit f_status;
    status, f_status = 0;

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

    status = ili_draw_pixel(pack, x, y, color);
    f_status += status;
    status = 0;

    if(d_x > d_y){
        cumulation = (d_x >> 1);

        for(int i = 1; i <= d_x; i++){

            x += x_inc;
            cumulation += d_y;

            if(cumulation >= d_x) {

                cumulation -= d_x;
                y += y_inc;

            }

    status = ili_draw_pixel(pack, x, y, color);
    f_status += status;

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

    status = ili_draw_pixel(pack, x, y, color);
    f_status += status;
        }
    }
        printf("Total F_STATUS: %i.\n", f_status);
          sleep_ms(500);

    return (f_status > 0) ? ili_ack : func_error;
}


//  Return ILI ACK or error.
func_ack ili_draw_char(spi_packet_s *pack, sbit x, sbit y, char character, tbit color){

  printf("Drawing Character.\n\n");

    ebit status, f_status;
    status, f_status = 0;

    sbit row, col;
    sbit o_set, o_set1, o_set2;

    o_set = (character - 0x20) * 20;

    for(col = 0; col < 10; col++){

        o_set1 = o_set + col * 2;
        o_set2 = o_set1 + 1;

        for(row = 0; row < 8; row++){
            if((charset10x14[o_set1] >> (7 - row)) & 0x01){
                
              status = ili_draw_pixel(pack, (x + col), (y + row + 8), color);
              f_status += status;
              status = 0;
            }

        for (row = 0; row < 6; row++){
            if((charset10x14[o_set2] >> (7 - row) & 0x01)){
                
              status = ili_draw_pixel(pack, (x + col), (y + row + 8), color);
              f_status += status;
              status = 0;
        }
      }
    }
  }

    printf("Final Write Character: %i.\n", (f_status / 3));
      sleep_ms(500);
    return (f_status > 0) ? ili_ack : func_error;
}


func_ack ili_draw_string(spi_packet_s *pack, tbit x, tbit y, char *string_ptr, tbit color){

    printf("Drawing String.\n");
    ebit status, f_status, i;

    const char *input[100];
    *input = string_ptr;

    sbit x_org = x;

    for(int i = 0; i < 100; i++){

      printf("Draw Loop.\n");

        if(*input[i] == 0){

          printf("Array is empty?\n");

            y += my_font.f_height + 2;
            x = x_org;

        }else {

          status = ili_draw_char(pack, x, y, *input[i], color);
          printf("Drawing Char.\n");
            x += my_font.f_width + 2;
            f_status += status;

        }
    input[i];

    }
    printf("Final Status: %i.\n", f_status);
      sleep_ms(500);
    return (f_status == ((sizeof(input)/sizeof(char)) * 3)) ? ili_ack : func_error;
}


func_ack ili_delay(uint32_t wait_value){

  printf("\nIli Delay.\n");

    tbit i;

  for(i = 0; i < wait_value; i++){
    for(i = 0; i < 100000; i++){

    }
  }

  return general_ack;
}


ebit spi_read_data(spi_packet_s *inst){

    printf("Attempting to read ILI9488 Device ID.\n");
    ebit *dev_id;
    ebit device;
    device = 0;
    ebit status;

    status = spi_spa_command_write(inst, Read_ID_FOUR, 0);

    dcrs_put_high(my_pins.dc_rs);
    csn_put_high(my_pins.csn);
    
    spi_read_blocking(inst->instance, 0, (dev_id), 2);

    csn_put_low(my_pins.csn);
    dcrs_put_low(my_pins.dc_rs);

    device = *dev_id;

    (device > 0) ? printf("Device ID: 0x%04x.\n", device) : printf("Error Reading from ILI9488.\n");

    return (device > 0) ? device : 0x0000;

}

func_ack ili_pos_gam_ctrl(spi_packet_s *pack){

    printf("Setting Positive Gamma Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xE0;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);

    write_dat = 0x00;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x03;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x09;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x08;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x16;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0A;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x3F;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x78;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x4C;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x09;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0A;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x08;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x16;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x1A;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0F;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);

    return (f_status == 16) ? ili_ack : func_error;
}

func_ack ili_neg_gam_ctrl(spi_packet_s *pack){

    printf("Setting Negative Gamma Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xE1;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x00;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x16;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x19;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x03;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0F;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x05;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x32;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x45;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x46;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x04;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0E;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0D;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x35;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x37;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x0F;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);

    return (f_status == 16) ? ili_ack : func_error;
}

func_ack ili_pwr_ctrl_o(spi_packet_s *pack){

  printf("Setting Power Control One.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xC0;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x17;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

        write_dat = 0x15;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 3) ? ili_ack : func_error;

}

func_ack ili_pwr_ctrl_t(spi_packet_s *pack){

    printf("Setting Power Control Two.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xC1;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x41;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_vcom_ctrl(spi_packet_s *pack){

    printf("Setting VCOM Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xC5;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x00;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x12;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x80;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 4) ? ili_ack : func_error;
}

func_ack ili_mem_acc_ctrl(spi_packet_s *pack){

    printf("Setting Memory Access Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0x36;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x48;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_pix_intr_format(spi_packet_s *pack){

    printf("Setting Pixel Interface Format.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0x3A;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x66;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_intr_mode_ctrl(spi_packet_s *pack){

    printf("Setting Interface Mode Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xB0;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x00;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_frame_rate_ctrl(spi_packet_s *pack){

    printf("Setting Frame Rate Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xB1;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0xA0;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_dspy_inver(spi_packet_s *pack){

    printf("Setting Display Inversion.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xB4;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x02;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 2) ? ili_ack : func_error;
}

func_ack ili_dspy_func_ctrl(spi_packet_s *pack){

    printf("Setting VCOM Control.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0xB6;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_start(&my_pins);
    write_dat = 0x02;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x02;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    write_dat = 0x3B;
    dat = &write_dat;
    pack->length = spi_length(write_dat);
    status = spi_write_blocking(pack->instance, dat, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 4) ? ili_ack : func_error;
}

func_ack ili_s_out(spi_packet_s *pack){

    printf("Setting Sleep OUT.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0x11;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

      sleep_ms(150);
    data_end(&my_pins);
    return (f_status == 1) ? ili_ack : func_error;
}

func_ack ili_dspy_on(spi_packet_s *pack){

    printf("Setting Display On.\n");

    ebit status, f_status;
    status, f_status = 0;
    ebit write_com, write_dat;
    write_com, write_dat = 0;

    com_start(&my_pins);
    write_com = 0x29;
    com = &write_com;
    pack->length = spi_length(write_com);
    status = spi_write_blocking(pack->instance, com, pack->length);
    f_status += status;

    data_end(&my_pins);
    return (f_status == 1) ? ili_ack : func_error;
}


typedef struct ili_init_var_s {
    
  struct ili_operation_var_s *my_op;
  struct spi_packet_s *data_packet;
  struct ili9488_window_var_s *my_window;
struct spi_pin_manager_s *my_pins;

}ili_init_var_s;


ili_init_var_s my_init = {

  .my_op = &my_op,
  .data_packet = &data_packet,
  .my_window = &my_window,
  .my_pins = &my_pins

};


func_ack ili_initialize(ili_init_var_s *init);


func_ack ili_initialize(ili_init_var_s *init){

  printf("\nILI Initialize:\n");

    ebit status, f_status;
    status, f_status = 0;

    reset_pulse(init->my_pins->reset);
      sleep_ms(150);

    //  Start SPI communication.
    initialize_spi_management((init->data_packet->instance), ONE_MBS);
      sleep_ms(100);

    status = ili_pos_gam_ctrl(init->data_packet);
    f_status += status;
    status = ili_neg_gam_ctrl(init->data_packet);
    f_status += status;
    status = ili_pwr_ctrl_o(init->data_packet);
    f_status += status;
    status = ili_pwr_ctrl_t(init->data_packet);
    f_status += status;
    status = ili_vcom_ctrl(init->data_packet);
    f_status += status;
    status = ili_mem_acc_ctrl(init->data_packet);
    f_status += status;
    status = ili_pix_intr_format(init->data_packet);
    f_status += status;
    status = ili_intr_mode_ctrl(init->data_packet);
    f_status += status;
    status = ili_frame_rate_ctrl(init->data_packet);
    f_status += status;
    status = ili_dspy_inver(init->data_packet);
    f_status += status;
    status = ili_dspy_func_ctrl(init->data_packet);
    f_status += status;
    status = ili_s_out(init->data_packet);
    f_status += status;
    status = ili_dspy_on(init->data_packet);
    f_status += status;

    deinitialize_spi_management((init->data_packet->instance));

    printf("Exiting with status: %i.\n", f_status);

    return (f_status == 39) ? ili_initialized : func_error;
    
}

/*
func_ack ili_set_display_dir(spi_packet_s *pack, bool direction){
    
    ebit status;
    
    sbit dir_control;
    dir_control = 0x00;

    if(direction){
        dir_control = 0xE8;
    }else {
        dir_control = 0x48;
    }

    status = spi_spa_command_write(pack, Memory_Write, dir_control);
    status = spi_spa_command_write(pack, NOP, 0);

    // Returns spi_acki, 1.
    return (status == spi_ack) ? ili_ack : func_error;

}
*/

#endif