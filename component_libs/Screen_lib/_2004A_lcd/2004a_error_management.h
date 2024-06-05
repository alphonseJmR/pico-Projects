#ifndef _2004A_LCD_ERROR_MANAGEMENT
#define _2004A_LCD_ERROR_MANAGEMENT

#include <stdio.h>

typedef enum func_akk {

    err = 0,
    pin_init,
    device_init,
    device_command_success,
    device_data_success,
    byte_write_success,
    line_write_success

}func_akk;


#endif
