#ifndef SPI_PIN_SETUPS_H
#define SPI_PIN_SETUPS_H

#include <stdio.h>

typedef enum baudrates_s {

  ONE_HUND_K = 100000,
  TWO_FIFTY_K = 250000,
  FIVE_HUND_K = 500000,
  ONE_MBS = 1000000

}baud_r;

typedef enum spi_zero_zero {

spi_rx_zz = 0,
spi_csn_zz = 1,
spi_sck_zz = 2,
spi_tx_zz = 3

}spi_zz;

typedef enum spi_zero_one {

spi_rx_zo = 4,
spi_csn_zo = 5,
spi_sck_zo = 6,
spi_tx_zo = 7

}spi_zo;

typedef enum spi_one_zero {

spi_rx_oz = 8,
spi_csn_oz = 9,
spi_sck_oz = 10,
spi_tx_oz = 11

}spi_oz;

typedef enum spi_one_one {

spi_rx_oo = 12,
spi_csn_oo = 13,
spi_sck_oo = 14,
spi_tx_oo = 15

}spi_oo;

typedef enum spi_zero_two {

spi_rx_zt = 16,
spi_csn_zt = 17,
spi_sck_zt = 18,
spi_tx_zt = 19

}spi_zt;


#endif