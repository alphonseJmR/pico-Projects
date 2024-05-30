#ifndef COMMUNICATION_PIN_CLUSTERS_H
#define COMMUNICATION_PIN_CLUSTERS_H

#include <stdio.h>
#include <stdint.h>

    //  This header is just to hold the values of pins for the I2C and SPI communication protocols.
    //  All protocol pins will be listed, no others.


uint8_t i2cZ_pin_cluster[6][2] = 
{
  {0,1},
  {4, 5},
  {8, 9},
  {12, 13},
  {16, 17},
  {20, 21}
};

uint8_t i2cO_pin_cluster[6][2] =
{
  {2, 3},
  {6, 7},
  {10, 11},
  {14, 15},
  {18, 19},
  {26, 27}
};

uint8_t spiZ_pin_cluster[3][4] =
{
  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {16, 17, 18, 19}
};

uint8_t spiO_pin_cluster[2][4] = 
{
  {8, 9, 10, 11},
  {12, 13, 14, 15}
};


#endif