#ifndef SPI_MANAGEMENT_H
#define SPI_MANAGEMENT_H

#include "ILI9488_PACKET_BUILDER.h"
#include "ili9488_screen_commands.h"
#include "ili9488_pin_management.h"
#include "ili9488_error_management.h"
#include "ili9488_func_def.h"
#include "hardware/spi.h"

#define txb uint8_t
#define rxb uint8_t
#define len size_t
#define baud uint32_t

typedef struct spi_packet_s {
    
    spi_inst_t *instance;
    const txb *tx_buf;
    rxb *rx_buf;
    baud rate;
    len length;
    uint8_t spi_func_status;

}spi_packet_s;

typedef enum baudrates_s {

  ONE_HUND_K = 100000,
  TWO_FIFTY_K = 250000,
  FIVE_HUND_K = 500000,
  ONE_MBS = 1000000

}baud_r;

spi_packet_s packet_buffer(spi_packet_s *packet, spi_packet_parts_s *part, int buf_inc, bool l_s){    // bool l_s refers to LPA or SPA

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


spi_packet_s spi_length(spi_packet_s *inst){

  inst->length = (sizeof(inst->tx_buf) / sizeof(txb));

  return *inst;
}


spi_packet_parts_s color_to_data(spi_packet_parts_s *parts, uint32_t color){


  parts->Data_Zero = get_0b_to_8b(color);
  parts->Data_One = get_8b_to_16b(color);
  parts->Data_Two = get_16b_to_24b(color);
  parts->Data_Three = get_24b_to_32b(color);

  return *parts;
}


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


func_ack spi_managed_data_write(spi_packet_s *pack, spi_packet_parts_s *part, int parts, bool l_s) {

//  printf("Length: %u.\n", len);
//  printf("Tx Buffer: %02x.\n", tx_buffer);
  txb packet_buf;
  

    for(int i = 0; i <= parts; i++){

      packet_buffer(pack, part, i, l_s);
      spi_length(pack);

    }

  sleep_us(2);
  uint8_t bytes = spi_write_read_blocking(pack->instance, pack->tx_buf, pack->rx_buf, pack->length);
//  printf("Bytes Length: %u.\n", bytes);
  sleep_us(2);

  // check that bytes written/read match bytes in tx_buffer & rx_buffer
  func_ack status = (bytes == pack->length) ? spi_ack : func_error;
//  printf("Status: %i.\n", status);

  return status;
}


func_ack spi_managed_command_write(spi_packet_s *packet, spi_packet_parts_s *part, uint8_t command, uint8_t param, uint8_t size){

  uint8_t bytes;
  fun stat;

  bytes = spi_write_read_blocking(packet->instance, command, 0, 0);

  if(param != 0){

  packet->spi_func_status += spi_write_read_blocking(packet->instance, param, 0, 0);
  
  }
    if(size > 0){
  
    for(int i = 0; i < size; i++){

      if(i == 0){

        packet->spi_func_status += spi_write_read_blocking(packet, part->Data_Zero, 0, 0);

      }
      if(i == 1){

        packet->spi_func_status += spi_write_read_blocking(packet, part->Data_One, 0, 0);

      }
      if(i == 2){

        packet->spi_func_status += spi_write_read_blocking(packet, part->Data_Two, 0, 0);

      }
      if(i == 3){

        packet->spi_func_status += spi_write_read_blocking(packet, part->Data_Three, 0, 0);

      }
      if(i == 4){

        packet->spi_func_status += spi_write_read_blocking(packet, part->Data_Four, 0, 0);

      }


  }

  } 

  stat = (bytes > 0) ? spi_ack : func_error;

  return stat;

}


uint8_t spi_check_data_size(spi_packet_parts_s *parts){

  uint8_t packet_size;
  //  packet_size is a minimum of two data, data_zero and data_one.
  packet_size = 0x02;

  if(parts->Data_Two > 0){

    packet_size += 0x01;
  }
  if(parts->Data_Three > 0){

    packet_size += 0x01;
  }
  if(parts->Data_Zero > 0){

    packet_size += 0x01;
  }

  return packet_size;
}


func_ack managed_data_spi(spi_packet_s *packet, spi_packet_parts_s *part){

  uint8_t fun_stat;
  uint8_t data_size;
  bool smol_packet;

  data_size = spi_check_data_size(part);
  smol_packet = (data_size > 2) ? false : true;

    packet->spi_func_status = 0;

    //  Initialize communication.
  packet->spi_func_status += initialize_spi_management(packet->instance, packet->rate);
        sleep_us(3);

    //  Initialize data transfer.
    packet->spi_func_status += spi_managed_transfer(packet, part, data_size, smol_packet);
        sleep_us(3);

    //  Deinitialize communication.
    packet->spi_func_status += deinitialize_spi_management(packet->instance);
        sleep_us(3);

    fun_stat = (packet->spi_func_status == 0x09) ? spi_ack : func_error;

    return fun_stat;
}


func_ack managed_command_spi(spi_packet_s *packet, spi_packet_parts_s *parts, fun command, fun param, fun size){
  
  uint8_t fun_stat;
  packet->spi_func_status = 0;

  packet->spi_func_status += initialize_spi_management(packet->instance, packet->rate);
    ili_delay(100);

  packet->spi_func_status += spi_managed_command_write(packet, parts, command, param, size);
    ili_delay(100);

  packet->spi_func_status += deinitialize_spi_management(packet->instance);
    ili_delay(100);

  fun_stat = (packet->spi_func_status == 0x09) ? spi_ack : func_error;
  
  return fun_stat;
}



#endif