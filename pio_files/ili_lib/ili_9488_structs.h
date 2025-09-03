#ifndef ILI9488_STRUCT_S_H
#define ILI9488_STRUCT_S_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VER
// #define HOR

//  Just for drawing a single slanted line
typedef struct ili_bresen_line {
    #ifdef VER
        //  PTP = points to plot, [0]x [1]y
        uint16_t ptp[320][2];
        //  CPTP = calculated points to plot, xs upper, xs lower, etc
        uint8_t cptp[320][8];
        uint8_t pix_color[320][3];
        uint16_t line_length;
    #endif
    #ifdef HOR
        uint16_t ptp[480][2];
        uint8_t cptp[480][8];
        uint8_t pix_color[320][3];
        uint16_t line_length;
    #endif

    int16_t dx;
    int16_t dy;
    int16_t slope;
    int16_t yi;
    int16_t xi;
    int16_t D;
    int16_t y;
    int16_t x;
    int16_t line_pos;



}bresen;


typedef struct ili_draw_pyramid {
    
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;

    uint16_t x2;
    uint16_t x3;
    uint16_t y2;
    uint16_t y3;

    uint16_t x4;
    uint16_t x5;
    uint16_t y4;
    uint16_t y5;

}ili_pyr;


#endif