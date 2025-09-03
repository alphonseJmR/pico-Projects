#ifndef ILI9488_PRE_SET_COLORS_H
#define ILI9488_PRE_SET_COLORS_H

// #include "ili_user_config.h"
#include "image_array.c"
#include "test_parse.c"
#include "window_parse.c"

//  The main pixel color struct.
typedef struct ili9488_pixel_color_s {

  uint8_t red;
  uint8_t green;
  uint8_t blue;
  
}ili_pix_color;

enum pixel_color {
    black = 0,
    red,
    green,
    blue,
    cyan,
    turquoise,
    spring_green,
    orange,
    magenta,
    yellow,
    violet,
    ocean,
    raspberry,
    white
};


ili_pix_color c_openSauce_orange = {
	.red = 0xEB,
	.green = 0x97,
	.blue = 0x22
	};

ili_pix_color c_black = {
    .red = 0x00,
    .green = 0x00,
    .blue = 0x00
};

ili_pix_color c_red = {
    .red = 0xFF,
    .green = 0x00,
    .blue = 0x00
};

ili_pix_color c_green = {
    .red = 0x00,
    .green = 0xFF,
    .blue = 0x00
};

ili_pix_color c_blue = {
    .red = 0x00,
    .green = 0x00,
    .blue = 0xFF
};

ili_pix_color c_cyan = {
    .red = 0x00,
    .green = 0xFF,
    .blue = 0xFF
};

ili_pix_color c_turquoise = {
    .red = 0x00,
    .green = 0xFF,
    .blue = 0x7d
};

ili_pix_color c_spring_green = {
    .red = 0x7D,
    .green = 0xFF,
    .blue = 0x00
};

ili_pix_color c_orange = {
    .red = 0xFF,
    .green = 0x7D,
    .blue = 0x00
};

ili_pix_color c_magenta = {
    .red = 0xFF,
    .green = 0x00,
    .blue = 0xFF
};

ili_pix_color c_yellow = {
    .red = 0xFF,
    .green = 0xFF,
    .blue = 0x00
};

ili_pix_color c_violet = {
    .red = 0x7D,
    .green = 0x00,
    .blue = 0xFF
};

ili_pix_color c_ocean = {
    .red = 0x00,
    .green = 0x80,
    .blue = 0x80
};

ili_pix_color c_raspberry = {
    .red = 0xFF,
    .green = 0x00,
    .blue = 0x7D
};

ili_pix_color c_white = {
    .red = 0xFF,
    .green = 0xFF,
    .blue = 0xFF
};

#endif
