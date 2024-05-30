#ifndef INTER_PICO_COMMUNICATION_ERROR_H
#define INTER_PICO_COMMUNICATION_ERROR_H

#include <stdio.h>
#include <stdint.h>

//  I understand com_dom sounds like condom
//  DOM is short for Data or Malfunction

enum com_dom {

    error_com = 0,
    i2c_pin_valid = 1,
    i2c_all_pin_valid = 2,
    i2c_instance_valid = 3,
    i2c_validation_complete = 4,
    i2c_initialisation_valid = 5,
    i2c_tx_valid = 6,
    i2c_rx_valid = 7,
    i2c_gpio_initialised = 8,
    i2c_valid = 9,
    spi_pin_valid = 13,
    spi_all_pin_valid = 14,
    spi_instance_valid = 15,
    spi_initialisation_valid = 16,
    spi_tx_valid = 17,
    spi_rx_valid = 18,
    spi_gpio_initialised = 19

};

#endif