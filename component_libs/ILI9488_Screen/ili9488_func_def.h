#ifndef ILI9488_FUNCTION_DEFINITIONS_H
#define ILI9488_FUNCTION_DEFINITIONS_H

#include <stdio.h>
#include "ILI9488_FUNCTION.h"
#include "ili9488_error_management.h"
#include "ILI9488_PACKET_BUILDER.h"
#include "ili9488_pin_management.h"
#include "spi_management.h"

#define lcd_data_cache_size ILI_LCD_WIDTH
#define cord uint16_t

#define fun uint8_t

color_in pixel_cache[lcd_data_cache_size*3];


func_ack ili_write_ram_prep(spi_pins *pins, spi_packet_parts_s *part, spi_packet_s *packet){

    csn_put_low(pins->csn);
    
    spi_managed_command_write(packet, part, Memory_Write, 0, 0);

    return spi_ack;
}

func_ack ili_write_ram(spi_pins *pins, spi_packet_s *packet, spi_packet_parts_s *part, uint32_t color){

    fun stat;
    fun d_size;

    csn_put_high(pins->csn);

    color_to_data(part, color);
    d_size = spi_check_data_size(part);
    stat += managed_command_spi(packet, part, 0, 0, d_size);

    return stat;
}


func_ack ili_register_write(spi_pins *pins, spi_packet_s *packet, spi_packet_parts_s *part, uint8_t command, uint16_t data, uint32_t size){

    uint8_t c_status;
    uint8_t fun_stat;
    uint8_t pack_size;
    volatile uint32_t i;
    bool l_pa;

    csn_put_low(pins->csn);
        pack_size = spi_check_data_size(part);
    c_status += managed_command_spi(packet, part, command, 0, pack_size);
    for(i = 0; i < 0xFF; i++);

    if(size > 0){

       pack_size = spi_check_data_size(part);
       l_pa = (pack_size > 2) ? false : true;

        csn_put_high(pins->csn);
       c_status += managed_data_spi(packet, part);
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


func_ack ili_set_display_dir(spi_packet_s *pack, bool direction){
    
    fun stat;
    twob value;

    if(direction){
        value = 0xE8;
    }else {
        value = 0x48;
    }

    stat = managed_command_spi(pack, Memory_Access_Control, value);

    return stat;

}


func_ack ili_set_window(win_vars *window, spi_packet_s *pack, spi_packet_parts_s *parts){

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

    stat += managed_command_spi(pack, parts, Column_Address_Set, 0, count);
    stat += managed_command_spi(pack, parts, NOP, 0, 0);

    parts->Data_Zero = get_8b_to_16b(row_s);
    parts->Data_One = get_0b_to_8b(row_s);
    parts->Data_Two = get_8b_to_16b(row_e);
    parts->Data_Three = get_0b_to_8b(row_e);

    stat += managed_command_spi(pack, parts, Page_Address_Set, 0, count);
    stat += managed_command_spi(pack, parts, NOP, 0, 0);

    e_stat = (stat == 0x04) ? spi_ack : func_error;

    return e_stat;
}


func_ack ili_set_foreground_color(win_vars *window, spi_packet_s *pack, spi_packet_parts_s *parts, uint32_t color_i){

    fun stat;

	for (int i = 0; i < pixel_cache; ) {

		pixel_cache[i++] = color_i >> 16;
		pixel_cache[i++] = color_i >> 8;
		pixel_cache[i++] = color_i & 0xFF;

	}

    stat = (pixel_cache[0] != 0) ? general_ack : func_error;

    return stat;

}


func_ack ili_fill(){

}


func_ack ili_set_cursor_pos(cord x, cord y, spi_packet_parts_s *parts, spi_pins *pins, spi_packet_s *pack){
	/* Set Horizontal Address Start Position */

    fun stat;
    fun e_stat;
    uint8_t count;

    count = spi_check_data_size(parts);

	parts->Data_Zero = get_8b_to_16b(x);
	buf[1] = get_0b_to_8b(x);
	buf[2] = get_8b_to_16b(x);
	buf[3] = get_0b_to_8b(x);
	ili_register_write(pins, pack, part, ILI9488_CMD_COLUMN_ADDRESS_SET, buf, cnt);
	ili_register_write(ILI9488_CMD_NOP, 0, 0);


	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(y);
	buf[1] = get_0b_to_8b(y);
	buf[2] = get_8b_to_16b(y);
	buf[3] = get_0b_to_8b(y);
	ili9488_write_register(ILI9488_CMD_PAGE_ADDRESS_SET, buf, cnt);
	ili9488_write_register(ILI9488_CMD_NOP, 0, 0);
}


func_ack ili_initialize(ili_init *init){

    fun stat;

    stat += managed_command_spi(init->packet, init->parts, SOFT_RESET, 0, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts, Sleep_OUT, 0, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts,Memory_Access_Control, 0x48, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts, CABC_Control_Nine, 0x04, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts, Interface_Pixel_Format, 0x05, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts, Normal_Display_Mode_ON, 0, 0);
    ili_delay(200);

    stat += managed_command_spi(init->packet, init->parts, Display_ON, 0, 0);
    ili_delay(200);

    stat += ili_set_display_dir(init->packet, true);
    ili_delay(200);





}




#endif