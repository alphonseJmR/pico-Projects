#ifndef SSD1306_DRIVER_COMMAND_S_H
#define SSD1306_DRIVER_COMMAND_S_H

#define set_mem_mode         0x20
#define set_col_addr         0x21
#define set_page_addr        0x22
#define set_hori_scroll      0x26
#define set_scroll           0x2E

#define set_disp_start_line  0x40
#define set_contrast         0x81
#define set_charge_pump      0x8D

#define set_seg_remap        0xA0
#define set_entire_on        0xA4
#define set_all_on           0xA5
#define set_norm_disp        0xA6
#define set_inv_disp         0xA7
#define set_mux_ratio        0xA8
#define set_disp             0xAE
#define set_com_out_dir      0xC0
#define set_com_out_dir_flip 0xC0

#define set_disp_offset      0xD3
#define set_disp_clk_div     0xD5
#define set_precharge        0xD9
#define set_com_pin_cfg      0xDA
#define set_vcom_desel       0xDB

#define page_height 8
#define num_pages (ssd_height / page_height)
#define buf_len (num_pages / ssd_width)

#define write_mode           0xFE
#define read_mode            0xFF



#endif