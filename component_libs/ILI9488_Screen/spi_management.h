#ifndef SPI_MANAGEMENT_H
#define SPI_MANAGEMENT_H

#include "ILI9488_PACKET_BUILDER.h"
#include "ili9488_screen_commands.h"
#include "ili9488_pin_management.h"
#include "ili9488_error_management.h"
#include "hardware/spi.h"

typedef struct spi_packet_s {
    
    spi_inst_t *instance;
    const uint8_t *tx_buf;
    uint8_t *rx_buf;
    uint32_t baud;
    size_t length;
    uint8_t spi_func_status;

}spi_packet_s;

typedef enum baudrates_s {

  ONE_HUND_K = 100000,
  TWO_FIFTY_K = 250000,
  FIVE_HUND_K = 500000,
  ONE_MBS = 1000000

}baud;

func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate){

      // initialise SPI instance at the specified baudrate (Hz).
  spi_init(instance, baudrate);

    return general_ack;
}

func_ack deinitialize_spi_management(spi_inst_t *instance){
    
    // Deinitialize SPI instance.  
  spi_deinit(instance);

    return general_ack;
}

func_ack spi_managed_transfer(spi_inst_t *instance, const uint8_t *tx_buffer, uint8_t *rx_buffer, size_t len) {

//  printf("Length: %u.\n", len);
//  printf("Tx Buffer: %02x.\n", tx_buffer);

  sleep_us(2);
  uint8_t bytes = spi_write_read_blocking(instance, tx_buffer, rx_buffer, len);
//  printf("Bytes Length: %u.\n", bytes);
  sleep_us(2);

  // check that bytes written/read match bytes in tx_buffer & rx_buffer
  func_ack status = (bytes == len) ? spi_ack : func_error;
//  printf("Status: %i.\n", status);

  return status;
}

func_ack managed_spi(spi_packet_s *packet){

    packet->spi_func_status = 0;

    //  Initialize communication.
    initialize_spi_management(packet->instance, packet->baud);
    packet->spi_func_status += 1;
        sleep_us(3);

    //  Initialize data transfer.
    spi_managed_transfer(packet->instance, packet->tx_buf, packet->rx_buf, packet->length);
    packet->spi_func_status += 1;
        sleep_us(3);

    //  Deinitialize communication.
    deinitialize_spi_management(packet->instance);
    packet->spi_func_status += 1;
        sleep_us(3);

    return spi_ack;
}


#endif