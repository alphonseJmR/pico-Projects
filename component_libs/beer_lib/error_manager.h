#ifndef ERROR_MANAGEMENT_H
#define ERROR_MANAGEMENT_H

#include <stdio.h>

typedef enum function_acknowledgement {

    er_r = 0,
    spi_ack,
    pins_ack,
    ili_ack,
    general_ack,
    pins_init_ack,
    register_written,
    ili_initialized,
    coords_ack,
    write_akk

}func_ack;

#endif