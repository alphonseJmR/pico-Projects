#ifndef ILI9488_SCREEN_FUNCTION_S_H
#define ILI9488_SCREEN_FUNCTION_S_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "ili9488_func_protos.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ili_9488_structs.h"
#include "ili_pre-set_colors.h"

// #define portrait
#define landscape

#define u16 uint16_t
#define i16 int16_t

#define rgb(a,b,c)      (uint32_t)( ((a & 0xFF) << 16) | ((b & 0xFF) << 8) | (c & 0xFF) )
#define sep_a(a)        (uint8_t)( (a & 0x00FF0000) >> 16)
#define sep_b(a)        (uint8_t)( (a & 0x0000FF00) >> 8)
#define sep_c(a)        (uint8_t)( (a & 0x000000FF) )

#define comb(a,b)       (u16)( ((a & 0xFF) << 8) | (b & 0xFF))
#define upper(a)        (uint8_t) ( (a & 0xFF00) >> 8)
#define lower(a)        (uint8_t) ( a & 0x00FF)

#define box_xe(a,b) (u16)((a - 1) + b)
#define box_ye(a,b) (u16)((a - 1) + b)

//  Max Pixel value
#define pvm 255

//  Max ILI Width
#define ILI_WIDTH 320

//  Max ILI Height
#define ILI_HEIGHT 480


ili_pix_color set_color(uint8_t color_sel){
    switch(color_sel){
        case 0:
            return c_black;
        case 1:
            return c_green;
        case 2:
            return c_blue;
        case 3:
            return c_cyan;
        case 4:
            return c_turquoise;
        case 5:
            return c_spring_green;
        case 6:
            return c_orange;
        case 7:
            return c_yellow;
        case 8:
            return c_ocean;
        case 9:
            return c_white;
        
        default:
            printf("Error in color selection.  Max choice is 9.\n");
            return c_black;
    }
}

//  Get a random number from an array of random numbers, returns a u16
//  Entry is a u16 to occupy bigger values
u16 get_rand(u16 max){

  u16 rand_buff[10];
    for(uint8_t a = 0; a < 10; a++){
      rand_buff[a] = (rand() % max);
    }
    return (rand_buff[ ((rand() % 10))]);
}

//  This function "calculates" the edges of a boxes outline.
void calc_box_outline(ili_box_line *ol, u16 x, u16 y, u16 xl, u16 yl, uint8_t lw){
    
    ol->left_x = x;
    ol->left_y = y;
    ol->left_length = yl;
    ol->top_x = (x + lw);
    ol->top_y = (y + yl);
    ol->top_length = (xl);
    ol->right_x = (x + lw) + (xl - lw);
    ol->right_y = (y + lw);
    ol->right_length = (yl - lw);
    ol->bottom_x = x + lw;
    ol->bottom_y = y;
    ol->bottom_length = (xl - lw);
    ol->line_width = lw;

}


//  This short function takes in a "24" bit value, and converts it to a pix color struct
void set_pix_color(ili_pix_color *pc, uint32_t rgb){

    uint8_t r = sep_a(rgb);
    uint8_t g = sep_b(rgb);
    uint8_t b = sep_c(rgb);

    pc->red = ((r << 2) & 0xFF);
    pc->green = ((g << 2) & 0xFF);
    pc->blue = ((b << 2) & 0xFF);

}


//  This function set's a pix struct to rand vals (0-254)
void set_rand_pix_color(ili_pix_color *pc){

    pc->red = ((get_rand(pvm) << 2) & 0x00FF);
    pc->green = ((get_rand(pvm) << 2) & 0x00FF);
    pc->blue = ((get_rand(pvm) << 2) & 0x00FF);

}


//  This function is capable of setting reset high&low, but initial reset stays high.
void set_reset(ili_pio *p, bool state){
    gpio_put(p->s_pins.reset, state);
}


void set_wait(ili_pio *p, bool state){
//    printf("Putting Pin %i in %i state.\n", p->s_pins.wait, state);
    gpio_put(p->s_pins.wait, state);
}


//  Previous sleep function was 30us
void set_rs_pin(ili_pio *p, bool state){
    gpio_put(p->s_pins.reg_s, state);
        sleep_us(25);
}


//  CPU usage setting the csx pin state transitions.
//  Previous sleep function was 20us
void set_csx(ili_pio *p, bool state){
    gpio_put(p->s_pins.csx, state);
        sleep_us(15);
}


//  This sets a struct with the pixel location values of a single pixel.  Can be used in an arr of pixel positions
//  Per datasheet of ili9488, column start must be == or less than column end. row follows rules
ili_pix_pos set_pixel_pos(u16 x, u16 y){
  ili_pix_pos buf;
  //    Set the pixel column
    buf.xssu = upper(x);
    buf.xssl = lower(x);
    buf.xesu = upper(x);
    buf.xesl = lower(x);

  //    Set the pixel row
    buf.yssu = upper(y);
    buf.yssl = lower(y);
    buf.yesu = upper(y);
    buf.yesl = lower(y);

  return buf;
}


//  Lowest ili9488 function send
//  d_type bool: true == parameter, false == command
void ili_send(ili_pio *p, uint32_t data, bool d_type){
    set_rs_pin(p, d_type);
        pio_sm_put_blocking(p->s_pio.c_pio, p->s_pio.c_sm, data);
    set_wait(p, true);
    //  Working sleep min so far, 1us?
        sleep_us(1);
    set_wait(p, false);

}


// Helper function to abstract up from ili_send
void ili_command(ili_pio *p, uint8_t data){
    ili_send(p, data, false);
}


void ili_send_command(ili_pio *p, uint8_t data){
    set_csx(p, 0);
        ili_command(p, data);
    set_csx(p, 1);
}


// Helper function to abstract up from ili_send
void ili_param(ili_pio *p, uint8_t data){
    ili_send(p, data, true);
}


// Helper to send commands with single parameters.
void ili_comparm(ili_pio *p, uint8_t command, uint8_t param){

    set_csx(p, 0);
        ili_command(p, command);
        ili_param(p, param);
    set_csx(p, 1);

}


//  Helper function that takes command plus n params in an uint8_t array.
void ili_comparm_x(ili_pio *p, uint8_t command, uint8_t *params, u16 p_size){

    set_csx(p, 0);
      ili_command(p, command);
        for(u16 a = 0; a < p_size - 1; a++){
            printf("Param %i: 0x%02x.\n", a, params[a]);
            ili_param(p, params[a]);
        }
    set_csx(p, 1);
        
}


void ili_comparm_dual(ili_pio *p, uint8_t command, uint8_t p1, uint8_t p2){
    
    set_csx(p, 0);
        ili_command(p, command);
        ili_param(p, p1);
        ili_param(p, p2);
    set_csx(p, 1);
        
}


//  Main function to send 4 parameter commands
void ili_comparm_4(ili_pio *p, uint8_t command, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4){
    
    set_csx(p, 0);
        ili_command(p, command);
        ili_param(p, p1);
        ili_param(p, p2);
        ili_param(p, p3);
        ili_param(p, p4);
    set_csx(p, 1);
        
}

void ili_simple_col(ili_pio *p, uint16_t x){
    ili_comparm_4(p, 0x2A, ((x & 0xFF00) >> 8), (x & 0x00FF), ((x & 0xFF00) >> 8 ), (x & 0x00FF));
}

void ili_simple_row(ili_pio *p, uint16_t y){
    ili_comparm_4(p, 0x2B, ((y & 0xFF00) >> 8), (y & 0x00FF), ((y & 0xFF00) >> 8 ), (y & 0x00FF));
}

//  Sets position to single pixel point
void ili_simple_position(ili_pio *p, uint16_t x, uint16_t y){
    ili_simple_col(p, x);
    ili_simple_row(p, y);
}



//  We'll use this to send the bresen line
void ili_set_col(ili_pio *p, uint8_t xsu, uint8_t xsl, uint8_t xeu, uint8_t xel){
    ili_command(p, 0x2A);
        ili_param(p, xsu);
        ili_param(p, xsl);
        ili_param(p, xeu);
        ili_param(p, xel);
}


//  This is for bresen line as well
void ili_row_set(ili_pio *p, uint8_t ysu, uint8_t ysl, uint8_t yeu, uint8_t yel){
    ili_command(p, 0x2B);
        ili_param(p, ysu);
        ili_param(p, ysl);
        ili_param(p, yeu);
        ili_param(p, yel);
}


void ili_pixel_set(ili_pio *p, bresen *line, uint pos){

    ili_set_col(p, line->cptp[pos][0], line->cptp[pos][1], line->cptp[pos][2], line->cptp[pos][3]);
    ili_row_set(p, line->cptp[pos][4], line->cptp[pos][5], line->cptp[pos][6], line->cptp[pos][7]);
    
}


//  Helper function to shorten code, set column data from ili_pix_pos
void ili_set_column(ili_pio *p, ili_pix_pos *ppos){
    
        ili_command(p, 0x2A);
        ili_param(p, ppos->xssu);
        ili_param(p, ppos->xssl);
        ili_param(p, ppos->xesu);
        ili_param(p, ppos->xesl);
        
}


//  Helper function to shorten code, set row data from ili_pix_pos
void ili_set_row(ili_pio *p, ili_pix_pos *ppos){
    
        ili_command(p, 0x2B);
        ili_param(p, ppos->yssu);
        ili_param(p, ppos->yssl);
        ili_param(p, ppos->yesu);
        ili_param(p, ppos->yesl);
        
}


void ili_set_p_color(ili_pio *p, bresen *line, u16 pos){
    ili_command(p, 0x2C);
        ili_param(p, line->pix_color[pos][0]);
        ili_param(p, line->pix_color[pos][1]);
        ili_param(p, line->pix_color[pos][2]);
}


//  Sets COL & ROW Start Pos
void ili_set_pixel_pos(ili_pio *p, ili_pix_pos *t_pps){
      ili_set_column(p, t_pps);
      ili_set_row(p, t_pps);
}


//  Write the single pixel color to pre set memory location.
//  Send a single (0x2C, per color write) command and color value parameters.
void ili_draw_pixel_c(ili_pio *p, ili_pix_color *pc){

    ili_command(p, 0x2C);
            ili_param(p, pc->red);
            ili_param(p, pc->green);
            ili_param(p, pc->blue);

}


void draw_generic_image(ili_pio *p, const uint32_t *image, uint32_t im_size, uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye){
    
    uint32_t pixel = 0x00000000;
    uint16_t y_cord = ys;
    uint16_t x_cord = xs;
    
    for(uint16_t a = ys; a < (ys + ye); a++){
    printf("Current Y: %i.\n", a);
    for(uint16_t b = xs; b < xe; b++){
        printf("Current Pixel Color: 0x%06x.\n", image[pixel]);
        ili_simple_position(p, b, a);
        set_csx(p, 0);
            ili_command(p, 0x2C);
                ili_param(p, ((image[pixel] & 0xFF0000) >> 16));
                ili_param(p, ((image[pixel] & 0x00FF00) >> 8));
                ili_param(p, (image[pixel] & 0x0000FF));
        set_csx(p, 1);
        pixel++;
    }
    }
}



//  Pixel array for now, is import[].  image_pos should increment by three.
void ili_draw_generic_pixels(ili_pio *p, const pixel_cord *image, uint32_t nm_px, uint16_t xs, uint16_t ys, uint16_t w, uint16_t h) {

    uint32_t pixel_cnt = 0x0000;


    for(uint32_t a = 0; a < nm_px; a++){
        ili_simple_position(p, ((image[a].x_cord - w) - 37), (h - image[a].y_cord) + 225);
    
        set_csx(p, 0);
      ili_command(p, 0x2C);
        ili_param(p, c_black.red);
        ili_param(p, c_black.green);
        ili_param(p, c_black.blue);
        set_csx(p, 1);
    }
}



void ili_draw_window_pane(ili_pio *p, const pixel_cord *image, uint16_t nmpx){
    for(uint16_t a = 0; a < nmpx; a++){
        ili_simple_position(p, (image[a].x_cord) + 7, (image[a].y_cord) + 50);

          set_csx(p, 0);
        ili_command(p, 0x2C);
        ili_param(p, c_black.red);
        ili_param(p, c_black.green);
        ili_param(p, c_black.blue);
          set_csx(p, 1);
    }
}
                

//  Write X amount of pixels to preset contigious memory location
//  This writes single color across the chosen memory
//  Used after ili_set_box_corners
void ili_draw_x_pixel(ili_pio *p, ili_pix_color *pc, uint32_t numpix){

    set_csx(p, 0);
    //  Start memory write
    ili_command(p, 0x2C);
    //  For numpix, continue writing pixels
        for(uint32_t a = 0; a < numpix; a++){
            ili_param(p, pc->red);
            ili_param(p, pc->green);
            ili_param(p, pc->blue);
        }
    set_csx(p, 1);
}



//  Function draws a pixel colored pixel at X Y location, applied by ili_pix_pos.
void ili_pixel_draw(ili_pio *p, ili_pix_color *pc, ili_pix_pos *ppos){
    set_csx(p, 0);
      ili_set_pixel_pos(p, ppos);
      ili_draw_pixel_c(p, pc);
    set_csx(p, 1);
}



//  Set the xs, xe, ys, ye coords to screen memory.  
void ili_set_box_corners(ili_pio *p, u16 cs, u16 ce, u16 rs, u16 re){
    
    uint8_t a, b, c, d = 0;

    //  Get Column Data
    a = upper(cs);
    b = lower(cs);
    c = upper(ce);
    d = lower(ce);
    //  Send column data
    ili_comparm_4(p, 0x2A, a, b, c, d);

    //  Get Row Data
    a = upper(rs);
    b = lower(rs);
    c = upper(re);
    d = lower(re);
    //  Send row data
    ili_comparm_4(p, 0x2B, a, b, c, d);

}


//  Function draws a line on the screen, vertical or horizontal only.
//  Direction: true for vertical, false for horizontal
//  Removed sleep_us during Refactor: reinstate if needed
void ili_draw_line(ili_pio *p, ili_pix_color *pc, u16 x, u16 y, u16 line_l, u16 line_w, bool direction){

    u16 line_size = (line_l * line_w);

    if(direction){
        ili_set_box_corners(p, x, box_xe(x, line_w), y, box_ye(y, line_l));
        ili_draw_x_pixel(p, pc, line_size);
    }else{
        ili_set_box_corners(p, x, box_xe(x, line_l), y, box_ye(y, line_w));
        ili_draw_x_pixel(p, pc, line_size);
    }

}


void plot(u16 x, u16 y){
    printf("Plotting Point: X:%i - Y:%i\n", x, y);
}


void ili_bresen_coords(bresen *line, u16 pos){
    
        printf("Coord Position: %i.\n", pos);
    u16 x_val = line->ptp[pos][0];
        printf("X Coord: %i\n", x_val);
    u16 y_val = line->ptp[pos][1];
        printf("Y Coord: %i\n", y_val);

    line->cptp[pos][0] = upper(x_val);
    line->cptp[pos][1] = lower(x_val);

    line->cptp[pos][2] = upper(x_val);
    line->cptp[pos][3] = lower(x_val);

    line->cptp[pos][4] = upper(y_val);
    line->cptp[pos][5] = lower(y_val);

    line->cptp[pos][6] = upper(y_val);
    line->cptp[pos][7] = lower(y_val);
}


void ili_calc_bresen_neg(bresen *line, u16 x0, u16 y0, u16 x1, u16 y1){
   printf("Currently in Bresen NEG.\n");
   
    i16 dx = x1 - x0;
    i16 dy = y1 - y0;
    i16 yi = 1;
      if(dy < 0){
        yi = -1;
        dy = -dy;
      }
    i16 D = ((2 * dy) - dx);
    i16 y = y0;
    
    int line_pos = 0;

    for(i16 x = x0; x <= x1; x++){
        plot(x, y);
        line->ptp[line_pos][0] = x;
        line->ptp[line_pos][1] = y;
      if(D > 0){
        y = y + yi;
        D = D + (2 * (dy - dx));
      }
      D = D + (2 * dy);
      ili_bresen_coords(line, line_pos);
      line_pos++;
    }
    line->line_length = line_pos;
}

\
//  This function sets the bresen structs points to plot into an array.
//  Until we add complete solution, line can go left -> right
void ili_calc_bresen_pos(bresen *line, u16 x0, u16 y0, u16 x1, u16 y1){
        printf("Currently in Bresen POS.\n");
  
    i16 dx = x1 - x0;
    i16 dy = y1 - y0;
    i16 xi = 1;
      if(dx < 0){
        xi = -1;
        dx = -dx;
      }
    i16 D = ((2 * dx) - dy);
    i16 x = x0;

    int line_pos = 0;

    for(i16 y = y0; y <= y1; y++){
        plot(x, y);
        line->ptp[line_pos][0] = x;
        line->ptp[line_pos][1] = y;
        if(D > 0){
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        D = (D + (2 * dx));
        ili_bresen_coords(line, line_pos);
        line_pos++;
    }
    line->line_length = line_pos;
}


//  Using u16 for inputs, as for the lcd screen, there are no negative memory locations.
bool calculate_slope(bresen *line){
    line->slope = (line->dy / line->dx);
        printf("Slope of line: %i.\n", line->slope);
    return (line->slope >= 1) ? true : false;
}


void ili_calc_bresen(bresen *line, u16 x0, u16 y0, u16 x1, u16 y1){
        printf("Currently in Bresen.\n");
    
    line->dx = x1 - x0;
    line->dy = y1 - y0;
    line->yi = 1;

    //  if true, slope greater than one.  flip flop x & y)
    if(calculate_slope(line)){
      line->D = ((2 * line->dx) - line->dy);
      line->y = y0;
        if(line->dy < 0){
          line->yi = -1;
        }else{
          line->yi = 1;
        }
    for(line->y = x0; line->y <= x1; line->y++){
      plot(line->x, line->y);
        line->ptp[line->line_pos][0] = line->y;
        line->ptp[line->line_pos][1] = line->x;
      if(line->D > 0){
        line->y = line->y + line->yi;
        line->D = line->D - (2 * line->dx);
      }
        line->D = (line->D + (2 * line->dy));
            ili_bresen_coords(line, line->line_pos);
            line->line_pos++;
            }
            line->line_length = line->line_pos;
    }else{
    line->D = ((2 * line->dy) - line->dx);
    line->x = x0;
    line->line_pos = 0;

    for(line->x = x0; line->x <= x1; line->x++){
        plot(line->x, line->y);
        line->ptp[line->line_pos][0] = line->x;
        line->ptp[line->line_pos][1] = line->y;
        if(line->D > 0){
            line->y = line->y + line->yi;
            line->D = line->D - (2 * line->dx);
        }
        line->D = (line->D + (2 * line->dy));
        ili_bresen_coords(line, line->line_pos);
        line->line_pos++;
    }

    }
    line->line_length = line->line_pos;
}


//  This might be the full solution for the bresenline?
void ili_calc_bresen_line(bresen *line, u16 x0, u16 y0, u16 x1, u16 y1){
      printf("Calculating For Bresen POS or NEG\n");
        if(abs(y1 - y0) < abs(x1 - x0)){
            printf("Bresen NEG\n");
        if(x0 > x1){
            ili_calc_bresen_neg(line, x1, y1, x0, y0);
        }else{
            ili_calc_bresen_neg(line, x0, y0, x1, y1);
        }

    }else{
            printf("Bresen POS\n");
        if(y0 > y1){
            ili_calc_bresen_pos(line, x1, y1, x0, y0);
        }else{
            ili_calc_bresen_pos(line, x0, y0, x1, y1);
        }

    }
}


void ili_bresen_single_pix_color(bresen *line, uint32_t color){
    uint8_t r, g, b = 0;
    r = sep_a(color);
    g = sep_b(color);
    b = sep_c(color);

    for(uint t = 0; t < line->line_length; t++){
         //   printf("R: 0x%02xG:0x%02xB:0x%02x\n", r, g, b);
        line->pix_color[t][0] = r;
        line->pix_color[t][1] = g;
        line->pix_color[t][2] = b;
    }
}


//  Draw a single bresen line of a single color
void ili_draw_bresen_line(ili_pio *p, bresen *line, u16 x0, u16 y0, u16 x1, u16 y1, uint32_t color){
    ili_calc_bresen(line, x0, y0, x1, y1);
    // ili_calc_bresen_line(line, x0, y0, x1, y1);
    ili_bresen_single_pix_color(line, color);

    set_csx(p, 0);
      for(u16 a = 0; a < line->line_length; a++){
        
            ili_pixel_set(p, line, a);
            ili_set_p_color(p, line, a);
        
      }
    set_csx(p, 1);
    
}


//  Add line thickness?
void ili_draw_pyramid(ili_pio *p, ili_pyr *mid, uint32_t color){
    bresen l1;
    bresen l2;
    bresen l3;

    //  Begin calculating all the lines.
    ili_calc_bresen_line(&l1, mid->x0, mid->y0, mid->x1, mid->y1);
    ili_bresen_single_pix_color(&l1, color);
    ili_calc_bresen_line(&l2, mid->x2, mid->y2, mid->x3, mid->y3);
    ili_bresen_single_pix_color(&l2, color);
    ili_calc_bresen_line(&l3, mid->x4, mid->y4, mid->x5, mid->y5);
    ili_bresen_single_pix_color(&l3, color);

    printf("Line 1 Length: %i.\n", l1.line_length);
    printf("Line 2 Length: %i.\n", l2.line_length);
    printf("Line 3 Length: %i.\n", l3.line_length);

        for(u16 a = 0; a < l1.line_length; a++){
            set_csx(p, 0);
            ili_pixel_set(p, &l1, a);
            ili_set_p_color(p, &l1, a);
            set_csx(p, 1);
        }

        for(u16 b = 0; b < l2.line_length; b++){
            set_csx(p, 0);
            ili_pixel_set(p, &l2, b);
            ili_set_p_color(p, &l2, b);
            set_csx(p, 1);
        }
        
        for(u16 c = 0; c < l3.line_length; c++){
            set_csx(p, 0);
            ili_pixel_set(p, &l3, c);
            ili_set_p_color(p, &l3, c);
            set_csx(p, 1);
        }
    

}


//  Sets the xs, xe, ys, ye coords into memory, then 
//  Removed sleep function during Refactor
void create_color_box(ili_pio *p, ili_pix_color *pc, u16 bw_s, u16 bh_s, u16 width, u16 height){

    uint b_size = (height * width);

    ili_set_box_corners(p, bw_s, box_xe(bw_s, width), bh_s, box_ye(bh_s, height));
    ili_draw_x_pixel(p, pc, b_size);

}


//  Draw the outline of a chosen box size.
void ili_draw_outline_box(ili_pio *p, ili_pix_color *pc, ili_box_line *line){

    //  Draw Left Line
    ili_draw_line(p, pc, line->left_x, line->left_y, line->left_length, line->line_width, true);
    sleep_us(5);

    //  Draw Top Line
    ili_draw_line(p, pc, (line->left_x + line->line_width), (line->top_y - line->line_width), (line->top_length - line->line_width), line->line_width, false);
    sleep_us(5);

    //  Draw Right Line
    ili_draw_line(p, pc, (line->right_x - line->line_width), line->right_y, line->right_length, line->line_width, true);
    sleep_us(5);

    //  Draw Bottom Line
    ili_draw_line(p, pc, line->bottom_x, line->bottom_y, line->bottom_length, line->line_width, false);
    sleep_us(5);

}


//  Fills the outline of a preset box outline
void ili_fill_outline(ili_pio *p, ili_pix_color *lc, ili_pix_color *bc, ili_box_line *line){

        u16 box_xs, box_ys, box_xe, box_ye = 0x0000;

        box_xs = (line->left_x + line->line_width);
        box_xe = (line->top_length - (line->line_width * 2));
        box_ys = (line->bottom_y + line->line_width);
        box_ye = (line->left_length - (line->line_width * 2));

    ili_draw_outline_box(p, lc, line);
    create_color_box(p, bc, box_xs, box_ys, box_xe, box_ye);

}


/*
// Function sets random pixels with random a color
void fill_x_rand_pixels(ili_pio *p, uint num_pix){

    u16 rand_x, rand_y = 0;
    ili_pix_color rand_pix;
    u16 pixel_pos[num_pix][4]; 
    set_rand_pix_color(&rand_pix);


    for(uint a = 0; a < num_pix; a++){
        rand_x = get_rand(320);
        rand_y = get_rand(480);
        rand_buff_x[a] = set_pixel_pos(rand_x, rand_x);
        rand_buff_y[a] = set_pixel_pos(rand_y, rand_y);
    }

    for(uint b = 0; b < num_pix; b++){
        ili_single_draw_pixel(p, &rand_pix, &rand_buff_x[b], &rand_buff_y[b]);
    }

}


//  Fill a box with a random color?
void fill_rand_box_pixels(ili_pio *p, ili_rbvs *rb, uint num_pix){

    u16 x_pos, y_pos = 0x0000;
    ili_pix_color rand;
    ili_pix_pos buff_x[num_pix];
    ili_pix_pos buff_y[num_pix];

    set_rand_pix_color(&rand);

    for(uint a = 0; a < num_pix; a++){
        x_pos = (get_rand(rb->x_length) + rb->x_start);
        y_pos = (get_rand(rb->y_height) + rb->y_start);
        buff_x[a] = set_pixel_pos(x_pos, x_pos);
        buff_y[a] = set_pixel_pos(y_pos, y_pos);
    }
    for(uint b = 0; b < num_pix; b++){
        ili_single_draw_pixel(p, &rand, &buff_x[b], &buff_y[b]);
    }
}


//  Setup the variables for a the memory location of a random box corners
void setup_rand_box(ili_rbvs *rb){
    printf("\nSetting Up Random Box.\n\n");

    //  This is our "liasion variable"
    u16 new_random = 0;
    uint8_t padding = 0;
    u16 max_box_length, max_box_height = 0x0000;
    //  Max pixel color is just 255, ((0-255) << 2) for r, g, and b.

    // We'll start with a random line width ((0-4) + 1).  +1 to ensure at least a pixel border.
    rb->line_width = (get_rand(4) + 1);
            printf("Box Border Width: %i.\n", rb->line_width);
    padding = (rb->line_width * 2);
            printf("Padded Border Width: %i.\n\n", padding);

    //  Next we get an x start.  We'll subtract a line width of three 
    new_random = (get_rand(ILI_WIDTH - padding));
            printf("X Start random pre Position: %i.\n", new_random);
    //  We want a max X start where we can still have padding
        if(new_random > (ILI_WIDTH - padding)){
            rb->x_start = ((ILI_WIDTH) - padding);
        }else{
            rb->x_start = new_random;
        }
            printf("\tX Start Position: %i.\n", rb->x_start);

    new_random = (get_rand(ILI_HEIGHT - padding));
            printf("Y Start random pre Position: %i.\n", new_random);
    //  We also want a max Y start where we can still have padding.
        if(new_random > (ILI_HEIGHT - padding)){
            rb->y_start = (ILI_HEIGHT - padding);
        }else{
            rb->y_start = new_random;
        }
            printf("\tY Start Position: %i.\n", rb->y_start);


    max_box_length = (ILI_WIDTH - (rb->x_start + padding));
    max_box_height = (ILI_HEIGHT - (rb->y_start + padding));
        if(max_box_length < 2 || max_box_height < 2){
            setup_rand_box(rb);
        }else{
            printf("\nMax Box Length: %i.\n", max_box_length);
            printf("Max Box Height: %i.\n", max_box_height);

    new_random = get_rand(max_box_length);
            printf("X Border random pre Length: %i.\n", new_random);
        while(new_random == 0){
            new_random = get_rand(max_box_length);
        }
        if(new_random < (padding + 5)){
            if( (padding + 5) <= (max_box_length)){
                new_random = padding + 5;
            }else{
                new_random = (max_box_length - 1);
            }
        }
    rb->x_length = new_random;
            printf("\tX Border Length: %i.\n", rb->x_length);

    new_random = get_rand(max_box_height);
            printf("Y Border random pre Height: %i.\n", new_random);
        while(new_random == 0){
            new_random = get_rand(max_box_height);
        }
        if(new_random < (padding + 5)){
            if( (padding + 5) <= max_box_height){
                new_random = padding + 5;
            }else{
                new_random = (max_box_height - 1);
            }
        }
    rb->y_height = new_random;
        
            printf("\tY Border Height: %i.\n", rb->y_height);

    rb->box_size = (rb->x_length * rb->y_height);
            printf("Our Random Box Size: %i.\n", rb->box_size);

    if(rb->box_size == 0){
        //  Empty Box Size, restart function.
            printf("\n\nEmpty Box Size, Restart Function.\n\n");
        setup_rand_box(rb);
    }else{
        rb->max_pixels = (get_rand(rb->box_size) / 3);
            printf("\n Random Box Variable Ready.\n\n");
    }
}
}


//  Draw a random box on screen
void ili_draw_rand_box(ili_pio *p){

    ili_rbvs new_box;
    ili_box_line rand_box;
    ili_pix_color border_outline;
    ili_pix_color pix_1;
    ili_pix_color pix_2;
    ili_pix_color pix_3;

    set_rand_pix_color(&pix_1);
    set_rand_pix_color(&pix_2);
    set_rand_pix_color(&pix_3);

    set_rand_pix_color(&border_outline);

    setup_rand_box(&new_box);

    calc_box_outline(&rand_box, new_box.x_start, new_box.y_start, new_box.x_length, new_box.y_height, new_box.line_width);
    ili_draw_outline_box(p, &border_outline, &rand_box);
    
    fill_rand_box_pixels(p, &new_box, new_box.max_pixels);
    fill_rand_box_pixels(p, &new_box, new_box.max_pixels);
    fill_rand_box_pixels(p, &new_box, new_box.max_pixels);

}


//  Fill the random box with a random color
void ili_fill_random_box(ili_pio *p){

    ili_rbvs new_rand;
    ili_box_line new_outline;
    ili_pix_color border_c;
    ili_pix_color infill_c;

    set_rand_pix_color(&border_c);
    set_rand_pix_color(&infill_c);
    setup_rand_box(&new_rand);
    calc_box_outline(&new_outline, new_rand.x_start, new_rand.y_start, new_rand.x_length, new_rand.y_height, new_rand.line_width);
    ili_fill_outline(p, &infill_c, &border_c, &new_outline);

}

*/


//  Return uint size of new array containing
uint ili_buffer_240x320(ili_pio *p, const char *bmp1){
    u16 c, d = 0;
    uint total_buffer_size = 0;
      for(uint a = 0; a < 76800; a++){
        if( (bmp1[a] & 0x01) ){
            total_buffer_size++;
        }
      }    
    return total_buffer_size;
}


void ili_print_480x320(ili_pio *p, ili_pix_color *pc, const char *bmp1, const char *bmp2){
    
    uint total_buffer_size = 0;
    uint fill_set = 0;
    uint fs1, fs2 = 0;
    uint fill_size = 0;
    uint c, d = 0;


    fs1 = ili_buffer_240x320(p, bmp1);
//        printf("BMP1 Size: %i.\n", fs1);
    fs2 = ili_buffer_240x320(p, bmp2);
//        printf("BMP2 Size: %i.\n", fs2);
    fill_size = (fs1 + fs2);
//        printf("\n\nTotal BMP Size: %i.\n", fill_size);

    ili_pix_pos q_fill[fill_size + 2];

        total_buffer_size = 0;
    for(c = 0; c < 320; c++){
      for(d = 0; d < 240; d++){
        if( (bmp1[total_buffer_size] & 0x01) ){
            q_fill[fill_set] = set_pixel_pos(c, d);
//                printf("Pixel At:\n\tX: %i\n\tY:%i\n", c, d);
            fill_set++;
        }
        if( (bmp2[total_buffer_size] & 0x01) ){
            q_fill[fill_set] = set_pixel_pos(c, (d + 240));
//                printf("Pixel At:\n\tX: %i\n\tY:%i\n", c, (d + 240));
                fill_set++;
            }
        total_buffer_size++;
      }
    }
    for(uint a = 0; a < fill_size; a++){

        ili_pixel_draw(p, pc, &q_fill[a]);
    }
}


//  Print Buffer of color into a serialized memory location of a remapped bitmap of left half of bitmap
//  Return struct, or just modify data and print?
//  Double x to be fluid for right half of logo
void ili_draw_bitmap(ili_pio *p, ili_pix_color *pc, const char *bmap, const char *bmap1){
        printf("Drawing Bitmap\n\n");
            ili_print_480x320(p, pc, bmap, bmap1);
        printf("Bitmap Drawn");
        sleep_ms(1000);
}


uint8_t ili_get_text_size(char ch){
    u16 char_offset = 0x0000;
    u16 offset, offset_1 = 0x0000;
    u16 row, col = 0x0000;
    uint8_t text_pix = 0;

        char_offset = ((ch - 0x20) * 20);
//    printf("Current Char: %c at offset 0x%04x.\n", ch, char_offset);
    for(col = 0; col < 10; col++){
        //First offset is the 0th index of our original char_offset
        offset = (char_offset + (col * 2));
        //Second offset is the offset, plus 1, as we're working with two bytes per row.
        offset_1 = offset + 1;
        for(row = 0; row < 8; row++){
            if( ( ((charset10x14_test[offset]) << (7 - row) & 0x80 )) ){
                text_pix++;
            }
        }
        for(row = 7; row > 1; row--){
            if( ( ((charset10x14_test[offset_1]) << (7 - row ) & 0x80 )) ){
                text_pix++;
            }
        }
    }
    return text_pix;
}


void ili_draw_char(ili_pio *p, ili_pix_color *pc, u16 x, u16 y, char ch){
    printf("Drawing Character.\n");
    uint8_t pix_size = 0;
    pix_size = ili_get_text_size(ch);

    u16 char_offset = 0x0000;
    u16 offset, offset_1 = 0x0000;
    u16 row, col = 0x0000;
    uint text_ocp = 0;
    ili_pix_pos logo[pix_size];

    // This is our original array position to offset at..
    char_offset = ((ch - 0x20) * 20);
//    printf("Current Char: %c at offset 0x%04x.\n", ch, char_offset);
    for(col = 0; col < 10; col++){
        //First offset is the 0th index of our original char_offset
        offset = (char_offset + (col * 2));
        //Second offset is the offset, plus 1, as we're working with two bytes per row.
        offset_1 = offset + 1;
        for(row = 0; row < 8; row++){
            if( ( ((charset10x14_test[offset]) << (7 - row) & 0x80 )) ){
                logo[text_ocp] = set_pixel_pos((col + x), (row + y + 8));
                text_ocp++;
            }
        }
        for(row = 7; row > 1; row--){
            if( ( ((charset10x14_test[offset_1]) << (7 - row ) & 0x80 )) ){
                logo[text_ocp] = set_pixel_pos((col + x), (y + row));
                text_ocp++;
            }
        }
    }
    
    for(uint pix = 0; pix < text_ocp; pix++){
        ili_pixel_draw(p, pc, &logo[pix]);
    }
}


void ili_draw_string(ili_pio *p, ili_pix_color *pc, u16 x, u16 y, char *stri, u16 s_size){
        printf("Drawing String.\n");
    u16 text_pos_x = x;

#ifdef portrait
    if(s_size < 20){
        text_pos_x += 30;
    }
#endif
#ifdef landscape
    if(s_size < 27){
            text_pos_x += 75;
        }
#endif
    

    for(u16 a = 0; a < (s_size - 1); a++){
        ili_draw_char(p, pc, text_pos_x, y, stri[a]);
        text_pos_x += 11;
    }
    printf("String Drawn.\n");
}


void ili_fill_background(ili_pio *p, ili_pix_color *bgc){
    printf("Start Background Fill.\n");
        create_color_box(p, bgc, 1, 1, 319, 479);
    printf("End Background Fill.\n");

}


// this is going to be to cover text, at ~y-150 to ~y-180
void draw_image_pane(ili_pio *p, const pixel_cord *window, uint16_t nmpx, uint16_t ys, uint16_t y_height){
    create_color_box(p, &c_openSauce_orange, 6, ys, 470, y_height);

   ili_draw_window_pane(p, window, nmpx);
    
}


#endif