#ifndef ILI9488_ERROR_MANAGEMENT
#define ILI9488_ERROR_MANAGEMENT

#include <stdio.h>

typedef enum function_acknowledgement {

    func_error = 0,
    spi_ack,
    pins_ack,
    ili_ack,
    general_ack,
    pins_init_ack,
    register_written,
    ili_initialized,
    coords_ack

}func_ack;

#endif