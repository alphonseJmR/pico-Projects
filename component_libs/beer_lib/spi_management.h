#ifndef SPI_MANAGEMENT_H
#define SPI_MANAGEMENT_H

#include "pin_manager.h"
#include "error_manager.h"
#include "hardware/spi.h"

#define len size_t
#define baud uint32_t
#define ebit uint8_t

typedef struct spi_config {

  spi_inst_t *instance;
  ebit baud_rate;

}spi_fig;

/*
spi_packet_s packet_buffer(spi_packet_s *packet, spi_l_packet *part, int buf_inc, bool l_s){

  if(l_s == true){

      switch(buf_inc){

    case 0:
      packet->tx_buf = part->Data_Id;
    break;

    case 1:
      packet->tx_buf = part->Data_Zero;
    break;

    case 2:
      packet->tx_buf = part->Data_One;
    break;

    case 3:
      packet->tx_buf = part->Ecc;
    break;
    
    default:
      printf("Loop Out of range.\n");
      break;
    }
  }else {

      if(part->Data_Two == 0 && buf_inc == 5){
        buf_inc++;
      }
      if(part->Data_Three == 0 && buf_inc == 6){
        buf_inc++;
      }
      if(part->Data_Four == 0 && buf_inc == 7){
        buf_inc++;
      }

          switch(buf_inc){

    case 0:
      packet->tx_buf = part->Data_Id;
    break;

    case 1:
      packet->tx_buf = part->Word_Count_LSB;
    break;

    case 2:
      packet->tx_buf = part->Word_Count_MSB;
    break;

    case 3:
      packet->tx_buf = part->Data_One;
    break;

    case 4:
      packet->tx_buf = part->Data_Two;
    break;

    case 5:
      packet->tx_buf = part->Data_Three;
    break;

    case 6:
      packet->tx_buf = part->Data_Four;
    break;

    case 7:
      packet->tx_buf = part->CRC_LSB;
    break;

    case 8:
      packet->tx_buf = part->CRC_MSB;
    break;
    
    default:
      printf("Loop Out of range.\n");
      break;
    }

  }
  return *packet;
}
*/

//  returns 4.
func_ack initialize_spi_management(spi_inst_t *instance, uint32_t baudrate){

      // initialise SPI instance at the specified baudrate (Hz).
  spi_init(instance, baudrate);

    return general_ack;
}

//  returns 4.
func_ack deinitialize_spi_management(spi_inst_t *instance){
    
    // Deinitialize SPI instance.  
  spi_deinit(instance);

    return general_ack;
}

#endif