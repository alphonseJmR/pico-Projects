#ifndef SSD1306_I2C_DRIVER_H
#define SSD1306_I2C_DRIVER_H

#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306_commands.h"
#include "font_8x8.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define test_i2c i2c0
#define ssd_height 64
#define ssd_width 128
#define ssd1306_i2c_addr 0x3C
#define ssd1306_i2c_addr2 0x3D

#define max_char_width 18
#define max_drv 250

// a is the smaller number
#define get_percentage(a, b) (double)((a/b)*100)

//  hard coded variable to use as shadow buffer.
uint8_t temp_buf[1024];
uint8_t kept_empty_buffer[1024];
uint8_t nema17_screen_buffer[1024];

//  These are for rf_controller_setup
//  s1 - rotary encoder
//  s2 - nrf screen
//  s3 - undetermined
uint8_t s1_temp_buf[1024];
uint8_t s2_temp_buf[1024];
uint8_t s3_temp_buf[1024];


typedef struct screen_i2c_config_s {

  i2c_inst_t *inst;
  uint8_t scl;
  uint8_t sda;
  uint32_t b_rate;
  uint8_t dev_id;

}ssd_i2c;


typedef struct screen_buffer_s {

  uint8_t line1_buf[17];
  uint8_t line2_buf[17];
  uint8_t line3_buf[17];
  uint8_t line4_buf[17];

}s_buffer;


ssd_i2c test_screen = {
  
  .inst = i2c0,
  .scl = 17,
  .sda = 16,
  .b_rate = 50000

};


typedef struct ssd_render_area_s {

    uint8_t s_col;
    uint8_t e_col;
    uint8_t s_page;
    uint8_t e_page;

    uint16_t bufflen;

}ssd_area_render;

// this is the frame we'll use for our setup, no need to define another
ssd_area_render frame = {
    .s_col = 0,
    .e_col = ssd_width - 1,
    .s_page = 0,
    .e_page = ssd_height - 1
};


void calc_ra_buflen(ssd_area_render *ara){
    ara->bufflen = ((ara->e_col - ara->s_col + 1) * (ara->e_page - ara->s_page + 1) / 8) + 1;
    printf("Calculated Buffer Size: %i.\n", ara->bufflen);
}


//  helper function to shorten calc_ra_buflen function.
void set_frame_size(){
    calc_ra_buflen(&frame);
}

// sends command to addr in struct setup
void send_ssd_command(ssd_i2c *cfg, uint8_t cmd){
  uint8_t com_buf[2] = {0x80, cmd};
//  printf("Writing command to dev Id: 0x%02x.\n", cfg->dev_id);
    i2c_write_blocking(cfg->inst, cfg->dev_id, com_buf, 2, false);
}


void send_cmd_list(ssd_i2c *cfg, uint8_t buff[], uint8_t buflen){
    for(uint8_t a = 0; a < buflen; a++){
        send_ssd_command(cfg, buff[a]);
    }
}


void clear_shadow_buffer(uint8_t buffer_sel){
  switch(buffer_sel){
    case 1:
        for(uint16_t a = 0; a < 1024; a++){
            s1_temp_buf[a] = 0x00;
        }
    break;

    case 2:
        for(uint16_t a = 0; a < 1024; a++){
            s2_temp_buf[a] = 0x00;
        }
    break;

    case 3:
        for(uint16_t a = 0; a < 1024; a++){
            s3_temp_buf[a] = 0x00;
        }
    break;

    default:
    break;
  }
}


//  for now, temp_buf will be hard set as 1024.
void send_buff(ssd_i2c *cfg, uint8_t buff[], uint16_t bufflen, uint8_t buffer_sel){

    uint8_t i2c_addr_buf = 0x00;
    
  //  printf("Writing buffer to dev Id: 0x%02x.\n", cfg->dev_id);

  switch(buffer_sel){
    case 1:
        s1_temp_buf[0] = 0x40;
        memcpy(s1_temp_buf + 1, buff, bufflen);
        i2c_write_blocking(cfg->inst, cfg->dev_id, s1_temp_buf, bufflen + 1, false);
        clear_shadow_buffer(1);
    break;

    case 2:
        s2_temp_buf[0] = 0x40;
        memcpy(s2_temp_buf + 1, buff, bufflen);
        i2c_write_blocking(cfg->inst, cfg->dev_id, s2_temp_buf, bufflen + 1, false);
        clear_shadow_buffer(2);
    break;

    case 3:
        s3_temp_buf[0] = 0x40;
        memcpy(s3_temp_buf + 1, buff, bufflen);
        i2c_write_blocking(cfg->inst, cfg->dev_id, s3_temp_buf, bufflen + 1, false);
        clear_shadow_buffer(3);
    break;

    default:
    break;
  }
}


// these are the commands to initialize the screen upon startup
void ssd_screen_init(ssd_i2c *cfg){
    uint8_t init_cmds[25] = {
        set_disp,
        set_mem_mode,
        set_disp_start_line,
        set_seg_remap | 0x01,
        set_mux_ratio,
        ssd_height - 1,
        set_com_out_dir | 0x08,
        set_disp_offset,
        0x00,
        set_com_pin_cfg,
        0x02,
        set_disp_clk_div,
        0x80,
        set_precharge,
        0xF1,
        set_vcom_desel,
        0x30,
        set_contrast,
        0xFF,
        set_entire_on,
        set_norm_disp,
        set_charge_pump,
        0x14,
        set_scroll | 0x00,
        set_disp | 0x01
    };
    send_cmd_list(cfg, init_cmds, 25);
}


void ssd_scroll(ssd_i2c *cfg, bool status){
    uint8_t cmd_lst[8] = {
        set_hori_scroll | 0x00,
        0x00,
        0x00,
        0x00,
        num_pages - 1,
        0x00,
        0xFF,
        set_scroll | (status ? 0x01 : 0)
    };

    send_cmd_list(cfg, cmd_lst, 8);
}


void render(ssd_i2c *cfg, uint8_t *buff, ssd_area_render *area, uint8_t buf_sel){
    uint8_t cmds[6] = {
        set_col_addr,
        area->s_col,
        area->e_col,
        set_page_addr,
        area->s_page,
        area->e_page
    };

    send_cmd_list(cfg, cmds, 6);
    send_buff(cfg, buff, area->bufflen, buf_sel);
}


void set_pixel(ssd_i2c *cfg, uint8_t *buff, uint8_t x, uint8_t y, bool status){
    bool within_screen = true;
    const uint8_t bytes_per_row = ssd_width;
        
    if( ((x >= 0 && x <= ssd_width) && (y >= 0 && y <= ssd_height)) ){
        within_screen = true;
    }else{
        within_screen = false;
    }
    
    if(within_screen){
     uint8_t byte_idx = ((y / 8) * bytes_per_row + x);
     uint8_t byte = buff[byte_idx];

        if(status){
            byte |= 1 << (y % 8);
        }else{
            byte &= ~(1 << (y % 8));
        }
        buff[byte_idx] = byte;

    }else{
        printf("X || Y not within screen size.\n");
    }

}


void draw_line(ssd_i2c *cfg, uint8_t *buf, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool status){
    int8_t dx = abs(x1 - x0);
    int8_t sx = x0 < x1 ? 1 : -1;
    int8_t dy = -abs(y1 - y0);
    int8_t sy = y0 - y1 ? 1 : -1;
    int8_t err = dx + dy;
    int8_t e2;

    while(true){
        set_pixel(cfg, buf, x0, y0, true);
        if(x0 == x1 && y0 == y1){
            break;
        }
        e2 = 2 * err;

        if(e2 >= dy){
            err += dy;
            x0 += sx;
        }

        if(e2 <= dx){
            err += dx;
            y0 += sy;
        }
    }
}


int8_t get_font_index(uint8_t charr){
    int8_t char_buf = 0x00;

    if(charr >= 0x20 && charr <= 0x7F){
        char_buf = (charr - 0x20);
        return char_buf;
    }else{
        printf("Non acceptable character entry.\n");
        //  return the space char place, which is 0.
        return 0x00;
    }

}


// writes a char to buffer
void write_char(ssd_i2c *cfg, uint8_t *buff, int16_t x, int16_t y, uint8_t charr, bool clr_prev){
    if(x > ssd_width - 8 || y > ssd_height - 8){
        return;
    }
    y = y/8;
 //   charr = toupper(charr);
    int idx = get_font_index(charr);
    int fb_idx = y * 128 + x;

  if(clr_prev){
    for(uint8_t a = 0; a < 8; a++){
      buff[fb_idx++] = (uint8_t)font[idx][a];
    }
  }else{
    for(uint8_t a = 0; a < 8; a++){
      buff[fb_idx++] |= (uint8_t)font[idx][a];
    }
  }
}


void write_string(ssd_i2c *cfg, int16_t x, int16_t y, char *str, bool clr_prev){
    if(x > ssd_width - 8 || y > ssd_height - 8){
        return;
    }
    while(*str){
        write_char(cfg, temp_buf, x, y, *str++, clr_prev);
        x+=8;
    }
}


// this is write_string but with using a array as in input arg.
void write_general_string(ssd_i2c *cfg, uint8_t *screen_buff, int16_t x, int16_t y, char *str, bool clr_prev){
    if(x > ssd_width - 8 || y > ssd_height - 8){
        return;
    }
    while(*str){
        write_char(cfg, screen_buff, x, y, *str++, clr_prev);
        x+=8;
    }
}


void render_general_string(ssd_i2c *cfg, uint8_t *buff, int16_t x, int16_t y, ssd_area_render *box, char *str, uint8_t buf_sel){
    write_general_string(cfg, buff, x, y, str, false);
    i2c_init(cfg->inst, cfg->b_rate);
    render(cfg, buff, box, buf_sel);
    i2c_deinit(cfg->inst);
}


/*
void write_temp_string(ssd_i2c *cfg, int16_t x, int16_t y, char *str){
    if(x > ssd_width - 8 || y > ssd_height - 8){
        return;
    }
    while(*str){
        write_char(cfg, nema17_screen_buffer, x, y, *str++, false);
        x+=8;
    }
}
*/

// This is after editing the temp_buf
void send_to_screen(ssd_i2c *cfg, uint8_t *buffer, uint8_t buf_sel){
    i2c_init(cfg->inst, cfg->b_rate);
        render(cfg, buffer, &frame, buf_sel);
    i2c_deinit(cfg->inst);
}


void clear_gram(ssd_i2c *cfg, uint8_t buf_sel){
    clear_shadow_buffer(buf_sel);
    send_to_screen(cfg, temp_buf, buf_sel);
}

/*
void write_string_to_screen(ssd_i2c *cfg, uint16_t x, uint16_t y, char *str, uint8_t buf_sel){
        write_string(cfg, x, y, str, false);
        send_to_screen(cfg, temp_buf, buf_sel);

}
*/

//  this should write to screen
void write_general_string_to_screen(ssd_i2c *cfg, uint16_t x, uint16_t y, char *str, uint8_t buf_sel, uint8_t *boofer){
  write_general_string(cfg, boofer, x, y, str, false);
  send_to_screen(cfg, boofer, buf_sel);
}

/*
//  Basic sprintf buffer to output "value: %i"
//  uhh.. learn how to input arg string into sprintf
void update_screen_buffers(s_buffer *lines, double val1, double val2, double val3, double val4){
  sprintf(lines->line1_buf, "Value 1: %.2f", val1);
  sprintf(lines->line2_buf, "Value 2: %.2f", val2);
  sprintf(lines->line3_buf, "Value 3: %.2f", val3);
  sprintf(lines->line4_buf, "Value 4: %.2f", val4);
}


void update_screen_rotary_buffers(s_buffer *lines, double val1, double val2, double val3, double val4){
  sprintf(lines->line1_buf, "Encoder 1: %.2f", val1);
  sprintf(lines->line2_buf, "Encoder 2: %.2f", val2);
  sprintf(lines->line3_buf, "Encoder 3: %.2f", val3);
  sprintf(lines->line4_buf, "PWM Duty: %.2f", val4);
}


// Show motor and drv8825 temps
// 3rd & 4th lines display if fans are active.
void update_screen_temp_buffers(s_buffer *lines, double m_temp, double drv_temp, bool f1_status, bool f2_status){

  sprintf(lines->line1_buf, "M Temp:   %.1f", m_temp);
  sprintf(lines->line2_buf, "D Temp:   %.1f", drv_temp);
  sprintf(lines->line3_buf, "DRV Max:  %.1f%c", get_percentage(drv_temp, max_drv), '%');
  sprintf(lines->line4_buf, "Fn1: %c  Fn2: %c", (f1_status ? 'A' : 'S'), (f2_status ? 'A' : 'S'));

}
*/
/*
void write_temp_screen_string(ssd_i2c *screen, s_buffer *line, double m_temp, double drv_temp, bool f1, bool f2, uint8_t buf_sel){
  //  clear_temp_gram(screen, temp_buf);
    update_screen_temp_buffers(line, m_temp, drv_temp, f1, f2);
  write_temp_string(screen, 0, 0, line->line1_buf);
  write_temp_string(screen, 0, 10, line->line2_buf);
  write_temp_string(screen, 0, 20, line->line3_buf);
  write_temp_string(screen, 0, 30, line->line4_buf);
  send_to_screen(screen, nema17_screen_buffer, buf_sel);

}
*/

void grid_design(ssd_i2c *cfg, uint8_t *temp, uint8_t buf_sel){
    
    switch(buf_sel){
        case 1:
            for(uint16_t a = 0; a < 1024 - 1; a++){
                s1_temp_buf[a] = (0x55 | (a % 8));
            }
        break;

        case 2:
            for(uint16_t a = 0; a < 1024 - 1; a++){
                s2_temp_buf[a] = (0x55 | (a % 8));
            }
        break;

        case 3:
            for(uint16_t a = 0; a < 1024 - 1; a++){
                s3_temp_buf[a] = (0x55 | (a % 8));
            }
        break;

        default:
            printf("Unknown Screen Entred.\n");
    }
    
    send_to_screen(cfg, temp, buf_sel);
}

/*
void setup_ssd1306(ssd_i2c *cfg, uint8_t buf_sel){
    i2c_init(cfg->inst, cfg->b_rate);
    gpio_set_function(cfg->sda, GPIO_FUNC_I2C);
    gpio_set_function(cfg->scl, GPIO_FUNC_I2C);
    gpio_pull_up(cfg->sda);
    gpio_pull_up(cfg->scl);

    set_frame_size();
    ssd_screen_init(cfg);
    
    clear_shadow_buffer(buf_sel);
    switch(buf_sel){
      case 1:
        draw_line(cfg, temp_buf, 40, 32, 80, 32, true);
        write_string(cfg, 24, 0, "SSD1306", true);
        write_string(cfg, 24, 10, "Rotary Encoder", true);
        write_string(cfg, 24, 20, "Value Display", true);
        write_string(cfg, 24, 30, "  Screen  ", true);
        render(cfg, temp_buf, &frame, buf_sel);
        clear_shadow_buffer(buf_sel);
        i2c_deinit(cfg->inst);
      break;

      case 2:
        draw_line(cfg, nema17_screen_buffer, 40, 32, 80, 32, true);
        write_string(cfg, 24, 0, "SSD1306", true);
        write_string(cfg, 24, 10, "Nema17 Motor", true);
        write_string(cfg, 24, 20, " And DRV8825  ", true);
        write_string(cfg, 24, 30, "   Monitor   ", true);
        render(cfg, nema17_screen_buffer, &frame, buf_sel);
        clear_shadow_buffer(buf_sel);
        i2c_deinit(cfg->inst);
      break;

      default:
        printf("No default selected for setup_sd1306 yet.\n");
      break;
    }

}
*/

void setup_rf_controller_ssd1306(ssd_i2c *cfg, uint8_t buf_sel){
    i2c_init(cfg->inst, cfg->b_rate);
    gpio_set_function(cfg->sda, GPIO_FUNC_I2C);
    gpio_set_function(cfg->scl, GPIO_FUNC_I2C);
    gpio_pull_up(cfg->sda);
    gpio_pull_up(cfg->scl);

    set_frame_size();
    ssd_screen_init(cfg);
    
    clear_shadow_buffer(buf_sel);
    switch(buf_sel){
      case 1:
        draw_line(cfg, s1_temp_buf, 40, 32, 80, 32, true);
        write_general_string(cfg, s1_temp_buf, 24, 0, "SSD1306", true);
        write_general_string(cfg, s1_temp_buf, 24, 10, "Rotary Encoder", true);
        write_general_string(cfg, s1_temp_buf, 24, 20, "Value Display", true);
        write_general_string(cfg, s1_temp_buf, 24, 30, "  Screen  ", true);
        render(cfg, s1_temp_buf, &frame, buf_sel);
            sleep_ms(200);
        clear_shadow_buffer(buf_sel);
            grid_design(cfg, s1_temp_buf, 1);
        i2c_deinit(cfg->inst);
      break;

      case 2:
        draw_line(cfg, s2_temp_buf, 40, 32, 80, 32, true);
        write_general_string(cfg, s2_temp_buf, 24, 0, "SSD1306", true);
        write_general_string(cfg, s2_temp_buf, 24, 10, "NRF24L01", true);
        write_general_string(cfg, s2_temp_buf, 24, 20, " Configuration  ", true);
        write_general_string(cfg, s2_temp_buf, 24, 30, "   Display   ", true);
        render(cfg, s2_temp_buf, &frame, buf_sel);
        clear_shadow_buffer(buf_sel);
          //  grid_design(cfg, s2_temp_buf, 2);
        i2c_deinit(cfg->inst);
      break;

      case 3:
        draw_line(cfg, s3_temp_buf, 40, 32, 80, 32, true);
        write_general_string(cfg, s3_temp_buf, 24, 0, "SSD1306", true);
        write_general_string(cfg, s3_temp_buf, 24, 10, "Figure Out", true);
        write_general_string(cfg, s3_temp_buf, 24, 20, " Use For This  ", true);
        write_general_string(cfg, s3_temp_buf, 24, 30, "   Display   ", true);
        render(cfg, s3_temp_buf, &frame, buf_sel);
        clear_shadow_buffer(buf_sel);
        i2c_deinit(cfg->inst);
      break;

      default:
        printf("No default selected for setup_sd1306 yet.\n");
      break;
    }

}



#endif