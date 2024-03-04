#ifndef ILI9488_PACKET_BUILDER
#define ILI9488_PACKET_BUILDER

#include <stdio.h>
#include <stdint.h>
#include "hardware/spi.h"
#include "ili9488_screen_commands.h"
#include "ili9488_error_management.h"
#include "ili9488_pin_management.h"
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

typedef enum word_count_s {
    ONE_BYTE = 0,
    TWO_BYTE,
    THREE_BYTE,
    FOUR_BYTE,
    FIVE_BYTE
}word_count;

typedef struct spi_variables_s {

    spi_inst_t *instance;
    uint baudrate;

    uint8_t spi_src;
    uint8_t spi_dst;
    size_t spi_len;

}spi_vars;

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
   uint32_t send_packet;

}spi_packet_parts_s;

spi_packet_parts_s L_pa;
spi_packet_parts_s S_pa;

typedef struct spi_packet_builder_s {

    uint32_t (*spa_build_packet)(spi_packet_parts_s packet);
    uint64_t (*lpa_build_packet)(spi_packet_parts_s packet, uint word_count);

    func_ack (*spi_send_packet)(spi_packet_parts_s packet);
    func_ack (*build_DI)(spi_packet_parts_s packet, uint spa_parameter);
    func_ack (*build_DZero)(spi_packet_parts_s packet, uint spa_parameter);
    func_ack (*build_DOne)(spi_packet_parts_s packet, uint spa_parameter);
    func_ack (*build_DTwo)(spi_packet_parts_s packet, uint lpa_parameter);
    func_ack (*build_DThree)(spi_packet_parts_s packet, uint lpa_parameter);
    func_ack (*build_DFour)(spi_packet_parts_s packet, uint lpa_parameter);
    func_ack (*build_Ecc)(spi_packet_parts_s packet, uint spa_parameter);

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

func_ack build_DI(spi_packet_parts_s *part_buffer, uint8_t SPa_Parameter){

    //  ili9488 device code for virtual channel is always 0b00.
    part_buffer->Data_Id = SPa_Parameter;

    return packet_part;
}

func_ack build_word_count(spi_packet_parts_s *parts_buffer, uint8_t LPa_Parameter){

    parts_buffer->Word_Count_LSB = LPa_Parameter;

    return packet_part;
}

func_ack build_DZero(spi_packet_parts_s *part_buffer, uint8_t SPa_Parameter){

    part_buffer->Data_Zero = SPa_Parameter;

    return packet_part;
}

func_ack build_DOne(spi_packet_parts_s *part_buffer, uint8_t SPa_Parameter){

    part_buffer->Data_One = SPa_Parameter;

    return packet_part;
}

func_ack build_DTwo(spi_packet_parts_s *part_buffer, uint8_t LPa_Parameter){

    part_buffer->Data_Two = LPa_Parameter;

    return packet_part;
}

func_ack build_DThree(spi_packet_parts_s *part_buffer, uint8_t LPa_Parameter){

    part_buffer->Data_Three = LPa_Parameter;

    return packet_part;
}

func_ack build_DFour(spi_packet_parts_s *part_buffer, uint8_t LPa_Parameter){

    part_buffer->Data_Four = LPa_Parameter;

    return packet_part;
}

func_ack build_Ecc(spi_packet_parts_s *packet_buffer, uint SPa_Parameter){
    
    packet_buffer->Ecc = SPa_Parameter;

    return packet_part;

}


void SPI_send_packet(spi_packet_parts_s *packet_part, spi_vars *array, bool ls_pa, uint word_count){

    uint8_t spi_out_buffer;

    if(ls_pa == true){

        // True == SPa

    for(int i = 0; i < 4; i++){
    
        switch(i){

          case 0:

            spi_out_buffer = 0x00;
                spi_out_buffer += ((packet_part->send_packet & 0xFF000000) >> 24);
            spi_write_blocking(array->instance, &array->spi_src, array->spi_len);
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
  }


#endif