/**
 * -------------------------------------------------------------------------------------+
 * @desc        LCD FONT 6x8
 * -------------------------------------------------------------------------------------+
 * @source      https://github.com/basti79/LCD-fonts/blob/master/6x8_vertikal_LSB_1.h
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @date        08.12.2020
 * @update      08.12.2022
 * @file        font6x8.h
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      
 * -------------------------------------------------------------------------------------+
 * @descr       LCD pixel fonts
 * -------------------------------------------------------------------------------------+
 * @usage       Display characters
 */

#ifndef __FONT6x8_H__
#define __FONT6x8_H__

  // includes
  #include <stdint.h>

  // Characters definition
  // -----------------------------------
  // number of columns for chars
  #define CHARS_COLS_LENGTH  6

  // @author basti79
  // @source https://github.com/basti79/LCD-fonts/blob/master/6x8_vertikal_LSB_1.h
  static const uint8_t FONTS[][CHARS_COLS_LENGTH] = {
    {0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20
    {0x00,0x00,0x06,0x5F,0x06,0x00},	// 0x21
    {0x00,0x07,0x03,0x00,0x07,0x03},	// 0x22
    {0x00,0x24,0x7E,0x24,0x7E,0x24},	// 0x23
    {0x00,0x24,0x2B,0x6A,0x12,0x00},	// 0x24
    {0x00,0x63,0x13,0x08,0x64,0x63},	// 0x25
    {0x00,0x36,0x49,0x56,0x20,0x50},	// 0x26
    {0x00,0x00,0x07,0x03,0x00,0x00},	// 0x27
    {0x00,0x00,0x3E,0x41,0x00,0x00},	// 0x28
    {0x00,0x00,0x41,0x3E,0x00,0x00},	// 0x29
    {0x00,0x08,0x3E,0x1C,0x3E,0x08},	// 0x2A
    {0x00,0x08,0x08,0x3E,0x08,0x08},	// 0x2B
    {0x00,0x00,0xE0,0x60,0x00,0x00},	// 0x2C
    {0x00,0x08,0x08,0x08,0x08,0x08},	// 0x2D
    {0x00,0x00,0x60,0x60,0x00,0x00},	// 0x2E
    {0x00,0x20,0x10,0x08,0x04,0x02},	// 0x2F
    {0x00,0x3E,0x51,0x49,0x45,0x3E},	// 0x30
    {0x00,0x00,0x42,0x7F,0x40,0x00},	// 0x31
    {0x00,0x62,0x51,0x49,0x49,0x46},	// 0x32
    {0x00,0x22,0x49,0x49,0x49,0x36},	// 0x33
    {0x00,0x18,0x14,0x12,0x7F,0x10},	// 0x34
    {0x00,0x2F,0x49,0x49,0x49,0x31},	// 0x35
    {0x00,0x3C,0x4A,0x49,0x49,0x30},	// 0x36
    {0x00,0x01,0x71,0x09,0x05,0x03},	// 0x37
    {0x00,0x36,0x49,0x49,0x49,0x36},	// 0x38
    {0x00,0x06,0x49,0x49,0x29,0x1E},	// 0x39
    {0x00,0x00,0x6C,0x6C,0x00,0x00},	// 0x3A
    {0x00,0x00,0xEC,0x6C,0x00,0x00},	// 0x3B
    {0x00,0x08,0x14,0x22,0x41,0x00},	// 0x3C
    {0x00,0x24,0x24,0x24,0x24,0x24},	// 0x3D
    {0x00,0x00,0x41,0x22,0x14,0x08},	// 0x3E
    {0x00,0x02,0x01,0x59,0x09,0x06},	// 0x3F
    {0x00,0x3E,0x41,0x5D,0x55,0x1E},	// 0x40
    {0x00,0x7E,0x11,0x11,0x11,0x7E},	// 0x41
    {0x00,0x7F,0x49,0x49,0x49,0x36},	// 0x42
    {0x00,0x3E,0x41,0x41,0x41,0x22},	// 0x43
    {0x00,0x7F,0x41,0x41,0x41,0x3E},	// 0x44
    {0x00,0x7F,0x49,0x49,0x49,0x41},	// 0x45
    {0x00,0x7F,0x09,0x09,0x09,0x01},	// 0x46
    {0x00,0x3E,0x41,0x49,0x49,0x7A},	// 0x47
    {0x00,0x7F,0x08,0x08,0x08,0x7F},	// 0x48
    {0x00,0x00,0x41,0x7F,0x41,0x00},	// 0x49
    {0x00,0x30,0x40,0x40,0x40,0x3F},	// 0x4A
    {0x00,0x7F,0x08,0x14,0x22,0x41},	// 0x4B
    {0x00,0x7F,0x40,0x40,0x40,0x40},	// 0x4C
    {0x00,0x7F,0x02,0x04,0x02,0x7F},	// 0x4D
    {0x00,0x7F,0x02,0x04,0x08,0x7F},	// 0x4E
    {0x00,0x3E,0x41,0x41,0x41,0x3E},	// 0x4F
    {0x00,0x7F,0x09,0x09,0x09,0x06},	// 0x50
    {0x00,0x3E,0x41,0x51,0x21,0x5E},	// 0x51
    {0x00,0x7F,0x09,0x09,0x19,0x66},	// 0x52
    {0x00,0x26,0x49,0x49,0x49,0x32},	// 0x53
    {0x00,0x01,0x01,0x7F,0x01,0x01},	// 0x54
    {0x00,0x3F,0x40,0x40,0x40,0x3F},	// 0x55
    {0x00,0x1F,0x20,0x40,0x20,0x1F},	// 0x56
    {0x00,0x3F,0x40,0x3C,0x40,0x3F},	// 0x57
    {0x00,0x63,0x14,0x08,0x14,0x63},	// 0x58
    {0x00,0x07,0x08,0x70,0x08,0x07},	// 0x59
    {0x00,0x71,0x49,0x45,0x43,0x00},	// 0x5A
    {0x00,0x00,0x7F,0x41,0x41,0x00},	// 0x5B
    {0x00,0x02,0x04,0x08,0x10,0x20},	// 0x5C
    {0x00,0x00,0x41,0x41,0x7F,0x00},	// 0x5D
    {0x00,0x04,0x02,0x01,0x02,0x04},	// 0x5E
    {0x80,0x80,0x80,0x80,0x80,0x80},	// 0x5F
    {0x00,0x00,0x03,0x07,0x00,0x00},	// 0x60
    {0x00,0x20,0x54,0x54,0x54,0x78},	// 0x61
    {0x00,0x7F,0x44,0x44,0x44,0x38},	// 0x62
    {0x00,0x38,0x44,0x44,0x44,0x28},	// 0x63
    {0x00,0x38,0x44,0x44,0x44,0x7F},	// 0x64
    {0x00,0x38,0x54,0x54,0x54,0x08},	// 0x65
    {0x00,0x08,0x7E,0x09,0x09,0x00},	// 0x66
    {0x00,0x18,0xA4,0xA4,0xA4,0x7C},	// 0x67
    {0x00,0x7F,0x04,0x04,0x78,0x00},	// 0x68
    {0x00,0x00,0x00,0x7D,0x40,0x00},	// 0x69
    {0x00,0x40,0x80,0x84,0x7D,0x00},	// 0x6A
    {0x00,0x7F,0x10,0x28,0x44,0x00},	// 0x6B
    {0x00,0x00,0x00,0x7F,0x40,0x00},	// 0x6C
    {0x00,0x7C,0x04,0x18,0x04,0x78},	// 0x6D
    {0x00,0x7C,0x04,0x04,0x78,0x00},	// 0x6E
    {0x00,0x38,0x44,0x44,0x44,0x38},	// 0x6F
    {0x00,0xFC,0x44,0x44,0x44,0x38},	// 0x70
    {0x00,0x38,0x44,0x44,0x44,0xFC},	// 0x71
    {0x00,0x44,0x78,0x44,0x04,0x08},	// 0x72
    {0x00,0x08,0x54,0x54,0x54,0x20},	// 0x73
    {0x00,0x04,0x3E,0x44,0x24,0x00},	// 0x74
    {0x00,0x3C,0x40,0x20,0x7C,0x00},	// 0x75
    {0x00,0x1C,0x20,0x40,0x20,0x1C},	// 0x76
    {0x00,0x3C,0x60,0x30,0x60,0x3C},	// 0x77
    {0x00,0x6C,0x10,0x10,0x6C,0x00},	// 0x78
    {0x00,0x9C,0xA0,0x60,0x3C,0x00},	// 0x79
    {0x00,0x64,0x54,0x54,0x4C,0x00},	// 0x7A
    {0x00,0x08,0x3E,0x41,0x41,0x00},	// 0x7B
    {0x00,0x00,0x00,0x77,0x00,0x00},	// 0x7C
    {0x00,0x00,0x41,0x41,0x3E,0x08},	// 0x7D
    {0x00,0x02,0x01,0x02,0x01,0x00},	// 0x7E
    {0x00,0x3C,0x26,0x23,0x26,0x3C},	// 0x7F
  };

#endif
