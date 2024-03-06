#ifndef ILI9488_PACKET_BUILDER_H
#define ILI9488_PACKET_BUILDER_H

#include <stdio.h>
#include "hardware/spi.h"
#include "ili9488_screen_commands.h"
#include "ili9488_error_management.h"
#include "ili9488_pin_management.h"
#include "ILI9488_FUNCTION.h"
#include "ili9488_func_def.h"
#include "spi_management.h"

#define ili_VC 0x00                 //  The ili9488 virtual channel is only 0b00.
#define other_device_VC 0x01        //  !! Not an actual name setting, however, correct value.
#define other_device_one_VC 0x10    //  !! Not an actual name setting, however, correct value.
#define other_device_tw_VC 0x11     //  !! Not an actual name setting, however, correct value.

#define SPa 0x15
#define LPa 0x29

#define ECC 0x00
#define CS 0x00

#define SPa_EOTP 0x08
#define SPa_DCSWN_S 0x05
#define SPa_DCSW_OS 0x15
#define SPa_DCSRN_S 0x06
#define SPa_SMRPS_S 0x37
#define SPa_AwER 0x02
#define SPa_DCSRR_OS 0x21
#define SPA_DCSRR_TS 0x22
#define LPa_NP_L 0x09
#define LPa_DCSW_L 0x39
#define LPa_DCSRR_L 0x1C

#define LPA false
#define SPA true

#define ili9488_device code (0x9488u)

#define u8 uint8_t

typedef enum word_count_s {
    ONE_BYTE = 0,
    TWO_BYTE,
    THREE_BYTE,
    FOUR_BYTE,
    FIVE_BYTE
}word_count;

typedef struct spi_packet_parts_s {

    uint8_t Data_Id;
    uint8_t Word_Count_LSB;
    uint8_t Word_Count_MSB;
    uint8_t Ecc;
    uint8_t Data_Zero;
    uint8_t Data_One;
    uint8_t Data_Two;
    uint8_t Data_Three;
    uint8_t Data_Four;
    uint8_t CRC_LSB;
    uint8_t CRC_MSB;

}spi_packet_parts_s;

spi_packet_parts_s L_pa;
spi_packet_parts_s S_pa;

spi_packet_parts_s build_packet(spi_packet_parts_s *packet, u8 command, u8 word_lsb, u8 word_msb, u8 ecc, u8 d_zero, u8 d_one, u8 d_two, u8 d_three, u8 d_four, u8 crc_lsb, u8 crc_msb){

  packet->Data_Id = command;
  packet->Word_Count_LSB = word_lsb;
  packet->Word_Count_MSB = word_msb;
  packet->Ecc = ecc;
  packet->Data_Zero = d_zero;
  packet->Data_One = d_one;
  packet->Data_Two = d_two;
  packet->Data_Three = d_three;
  packet->Data_Four = d_four;
  packet->CRC_LSB = crc_lsb;
  packet->CRC_MSB = crc_msb;

  return *packet;
}

//  Use build_packet(&S_pa, , NULL, NULL, 0x09, 0x14, NULL, NULL, NULL, NULL, NULL)

/*

typedef struct spi_packet_builder_s {

    uint32_t (*spa_build_packet)(spi_packet_parts_s packet);
    uint64_t (*lpa_build_packet)(spi_packet_parts_s packet, uint word_count);
    func_ack (*spi_send_packet)(spi_packet_parts_s packet, bool ls_pa, uint word_count);
}spi_functions_s;

//  build_packet will use spi_packet_parts to build a SPa or LPa.
uint32_t spa_build_packet(spi_packet_parts_s *parts){

    uint32_t packet_buffer;
    packet_buffer = 0x00000000;

        (packet_buffer) += ((parts->Data_Id & 0x000000FF) << 24);
        (packet_buffer) += ((parts->Data_Zero & 0x000000FF) << 16);
        (packet_buffer) += ((parts->Data_One & 0x000000FF) << 8);
        (packet_buffer) += ((parts->Ecc & 0x000000FF));


    return packet_buffer;
}

uint64_t lpa_build_packet(spi_packet_parts_s *parts, uint word_count){

    uint64_t packet_buffer;
    packet_buffer = 0x0000000000000000;

      switch(word_count){

        case ONE_BYTE:

          (packet_buffer) += (( parts->Data_Id    & 0xFF)  << 24);
          (packet_buffer) += (( parts->Data_Zero  & 0xFF)  << 16);
          (packet_buffer) += (( parts->Data_One   & 0xFF)  << 8);
          (packet_buffer) += (( parts->Ecc        & 0xFF));

        return packet_buffer;

        case TWO_BYTE:

          (packet_buffer) += (( parts->Data_Id    & 0xFF)  << 32);
          (packet_buffer) += (( parts->Data_Zero  & 0xFF)  << 24);
          (packet_buffer) += (( parts->Data_One   & 0xFF)  << 16);
          (packet_buffer) += (( parts->Data_Two   & 0xFF)  << 8);
          (packet_buffer) += (( parts->Ecc        & 0xFF));

        return packet_buffer;

        case THREE_BYTE:

          (packet_buffer) += (( parts->Data_Id    & 0xFF)  << 40);
          (packet_buffer) += (( parts->Data_Zero  & 0xFF)  << 32);
          (packet_buffer) += (( parts->Data_One   & 0xFF)  << 24);
          (packet_buffer) += (( parts->Data_Two   & 0xFF)  << 16);
          (packet_buffer) += (( parts->Data_Three & 0xFF)  << 8);
          (packet_buffer) += (( parts->Ecc        & 0xFF));

        return packet_buffer;

        case FOUR_BYTE:

          (packet_buffer) += (( parts->Data_Id    & 0xFF)  << 48);
          (packet_buffer) += (( parts->Data_Zero  & 0xFF)  << 40);
          (packet_buffer) += (( parts->Data_One   & 0xFF)  << 32);
          (packet_buffer) += (( parts->Data_Two   & 0xFF)  << 24);
          (packet_buffer) += (( parts->Data_Three & 0xFF)  << 16);
          (packet_buffer) += (( parts->Data_Three & 0xFF)  << 8);
          (packet_buffer) += (( parts->Ecc        & 0xFF));

        return packet_buffer;

        default:

            printf("Error In LPa Packet Build Function.\n");
            return 0;
        }

        return;
}

*/

/*

func_ack SPI_send_packet(spi_packet_parts_s *inst, bool ls_pa, uint word_count){

    uint8_t spi_out_buffer;

    if(ls_pa == true){

        // True == SPa

    for(int i = 0; i < 4; i++){
    
        switch(i){

          case 0:

            spi_out_buffer = 0x00;
                spi_out_buffer += ((packet_part->send_packet & 0xFF000000) >> 24);
            spi_write_blocking(inst->instance, &array->spi_src, array->spi_len);
            break;

          case 1:

            spi_out_buffer = 0x00;
                spi_out_buffer += ((packet_part->send_packet & 0x00FF0000) >> 16);
            spi_write_blocking(array->instance, &array->spi_src, array->spi_len);
            break;

            case 2:

            spi_out_buffer = 0x00;
                spi_out_buffer += ((packet_part->send_packet & 0x0000FF00) >> 8);
            spi_write_blocking(array->instance, &array->spi_src, array->spi_len);
            break;

            case 3:

            spi_out_buffer = 0x00;
                spi_out_buffer += (packet_part->send_packet & 0x000000FF);
            spi_write_blocking(array->instance, &array->spi_src, array->spi_len);
            break;

            default:

            spi_out_buffer = 0x00;
                spi_out_buffer += ((packet_part->send_packet & 0xFFFFFF) >> (i * 4));
                break;
        }

    }

    } else if(ls_pa == false){

        // False == LPa

        switch(word_count){

          case 1:

            for(int j = 0; j < 5; j++){

              switch(j){

                case 0:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 24) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 1: 

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 16) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 2:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 8) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 3:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                default:

                  printf("Word Count is: %i.\n", word_count);
                break;

                }
              
            break;
            }

          case 2:

            for(int j = 0; j < 6; j++){

              switch(j){

                case 0:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 32) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 1: 

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 24) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 2:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 16) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 3:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 8) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 4:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                default:

                  printf("Word Count is: %i.\n", word_count);
                break;

                }
              
            break;
            }

          case 3:

            for(int j = 0; j < 7; j++){

              switch(j){

                case 0:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 40) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 1: 

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 32) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 2:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 24) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 3:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 16) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 4:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 8) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 5:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                default:

                  printf("Word Count is: %i.\n", word_count);
                break;

                }
              break;
            }

          case 4:

            for(int j = 0; j < 7; j++){

              switch(j){

                case 0:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 48) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 1: 

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 40) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 2:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 32) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 3:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 24) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 4:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 16) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 5:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet >> 8) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                case 6:

                  spi_out_buffer = 0x00;
                  spi_out_buffer += ((packet_part->send_packet) & 0xFF);
                  spi_write_blocking(array->instance, &array->spi_src, array->spi_len);

                break;

                default:

                  printf("Word Count is: %i.\n", word_count);
                break;

                }
              break;
            }

          default:
            printf("Error Somewhere.\n");
                break;
      }
    }
  
  sleep_us(2);
  uint8_t bytes = spi_write_read_blocking(pack->instance, pack->tx_buf, pack->rx_buf, pack->length);
//  printf("Bytes Length: %u.\n", bytes);
  sleep_us(2);

  // check that bytes written/read match bytes in tx_buffer & rx_buffer
  func_ack status = (bytes == pack->length) ? spi_ack : func_error;
//  printf("Status: %i.\n", status);

  return spi_ack;
  
  }

*/
#endif