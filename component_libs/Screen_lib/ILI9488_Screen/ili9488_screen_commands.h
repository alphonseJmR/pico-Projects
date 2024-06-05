#ifndef ILI9488_SCREEN_COMMANDS
#define ILI9488_SCREEN_COMMANDS

#include <stdio.h>

typedef enum baudrates_s {

  ONE_HUND_K = 100000,
  TWO_FIFTY_K = 250000,
  FIVE_HUND_K = 500000,
  ONE_MBS = 1000000

}baud_r;

typedef enum pin_logic_level {
    LOW = 0,
    HIGH
}p_logic_l;


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


//________________________________________________________________________________
//  Ilitek ILI9488 a-Si TFT LCD Single Chip Driver commands start: page 140.
//  Bit Setup: | D/Cx | RDX | WRX | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | hex |
//________________________________________________________________________________


#define RDII (uint8_t)0x04                   //  Read Display Identification Information
#define RNE_DSI (uint8_t)0x05                //  Read Number of the Errors on DSI

//  Display Command Set Write, no Parameter (DSCWN-S)
#define NOP (uint8_t)0x00
#define SOFT_RESET (uint8_t)0X01
#define Sleep_IN (uint8_t)0x10
#define Sleep_OUT (uint8_t)0x11
#define Parital_Mode_ON (uint8_t)0x12
#define Normal_Display_Mode_ON (uint8_t)0x13
#define Display_OFF (uint8_t)0x28
#define Display_ON (uint8_t)0x29
#define Tearing_Effect_Line_OFF (uint8_t)0x34
#define Tearing_Effect_Line_ON (uint8_t)0x35
#define Idle_Mode_OFF (uint8_t)0x38
#define Idle_Mode_ON (uint8_t)0x39


//  Display Command Set Write, 1 Parameter (DCSW1-S)
#define Memory_Write (uint8_t)0x2C
#define Memory_Access_Control (uint8_t)0x36
#define Interface_Pixel_Format (uint8_t)0x3A
#define Memory_Write_Continue (uint8_t)0x3C
#define Write_Display_Brightness_Value (uint8_t)0x51
#define Write_CTRL_Display_Value (uint8_t)0x53
#define WCABCV (uint8_t)0x55                    //   Write Content Adaptive Brightness Control Value
#define Write_CABC_MB (uint8_t)0x5E              //   Write CABC Minimum Brightness Value


//  Display Command Set Read, No Parameter (DCSRN-S)
#define Read_Display_Power_Mode (uint8_t)0x0A
#define Read_Display_MADCTL (uint8_t)0x0B
#define Read_Pixel_Format (uint8_t)0x0C
#define Read_Display_Image_Mode (uint8_t)0x0D
#define Read_Display_Signal_Mode (uint8_t)0x0E
#define Read_ID_ONE (uint8_t)0xDA
#define Read_ID_TWO (uint8_t)0xDB
#define Read_ID_THREE (uint8_t)0xDC
#define RDSDR (uint8_t)0x0F                  //  Read Display Self_Diagnostic Result
#define Memory_Read (uint8_t)0x2E
#define Read_Display_Brightness_Value (uint8_t)0x52
#define Read_CTRL_Display_Value (uint8_t)0x54
#define RCABCV (uint8_t)0x56                    //   Read Content Adaptive Brightness Control Value
#define Read_CABC_MB (uint8_t)0x5F               //   Read CABC Minimum Brightness Value


#define Read_Display_Status (uint8_t)0x09
#define Display_Inversion_OFF (uint8_t)0x20
#define Display_Inversion_ON (uint8_t)0x21

#define All_Pixel_OFF (uint8_t)0x22
#define All_Pixel_ON (uint8_t)0x23

#define Column_Address_Set (uint8_t)0x2A
#define Page_Address_Set (uint8_t)0x2B
#define Partial_Area (uint8_t)0x30

#define Vertical_Scolling_Definition (uint8_t)0x33
#define Vertial_Scolling_Start_Address (uint8_t)0x37

#define Memory_Read_Continue (uint8_t)0x3E
#define Write_Tear_Scan_Line (uint8_t)0x44
#define Read_Tear_Scan_line (uint8_t)0x45
#define Read_Automatic_Brightness (uint8_t)0x66

//     Extended Command List

#define Interface_Mode_Control (uint8_t)0xB0
#define Frame_Rate_Control_Normal (uint8_t)0xB1 // In Normal Mode/Full Colors
#define Frame_Rate_Control_Idle (uint8_t)0xB2
#define Frame_Rate_Control_Partial (uint8_t)0xB3
#define Display_Inversion_Control (uint8_t)0xB4
#define Blanking_Porch_Control (uint8_t)0xB5
#define Display_Function_Control (uint8_t)0xB6
#define Entry_Mode_Set (uint8_t)0xB7
#define Color_Enhancement_Control_One (uint8_t)0xB9
#define Color_Enhancement_Control_Two (uint8_t)0xBA

#define HS_Lanes_Control    (uint8_t)0xBE
#define Power_Control_One   (uint8_t)0xC0
#define Power_Control_Two   (uint8_t)0xC1
#define Power_Control_Three (uint8_t)0xC2
#define Power_Control_Four  (uint8_t)0xC3
#define Power_Control_Five  (uint8_t)0xC4

#define VCOM_Control_One    (uint8_t)0xC5

#define CABC_Control_One    (uint8_t)0xC6
#define CABC_Control_Two    (uint8_t)0xC8
#define CABC_Control_Three  (uint8_t)0xC9
#define CABC_Control_Four   (uint8_t)0xCA
#define CABC_Control_Five   (uint8_t)0xCB
#define CABC_Control_Six    (uint8_t)0xCC
#define CABC_Control_Seven  (uint8_t)0xCD
#define CABC_Control_Eight  (uint8_t)0xCE
#define CABC_Control_Nine   (uint8_t)0xCF

#define NV_Memory_Write (uint8_t)0xD0

#define NV_Memory_Protection_Key (uint8_t)0xD1
#define NV_Memory_Status_Read (uint8_t)0xD2

#define Read_ID_FOUR (uint8_t)0xD3
#define Adjust_Control_One (uint8_t)0xD7
#define Read_ID_Four_Check (uint8_t)0xD8

#define PGAMCTRL (uint8_t)0xE0   //  Positive Gamma Control
#define NGAMCTRL (uint8_t)0xE1   //  Negative Gamma Control

#define Digital_Gamma_Control_One (uint8_t)0xE2
#define Digitial_Gamma_Control_Two (uint8_t)0xE3
#define Set_Image_Function (uint8_t)0xE9

#define Adjust_Control_Two (uint8_t)0xF2
#define Adjust_Control_Three (uint8_t)0xF7
#define Adjust_Control_Four (uint8_t)0xF8
#define Adjust_Control_Five (uint8_t)0xF9
#define Adjust_Control_Six (uint8_t)0xFC
#define Adjust_Control_Seven (uint8_t)0xFF

#define Packet_Header_DI_EOT (uint8_t)0x08       //  End of Transmission Packet
#define Packet_Header_DI_DCSWN_S (uint8_t)0x05    //  DCS Write, No Parameter
#define Packet_Header_DI_DCSWO_S (uint8_t)0x15    //  DCS Write, One Parameter
#define Packet_Header_DI_DCSRN_S (uint8_t)0x06    //  DCS Read, No Parameter
#define Packet_Header_DI_SMRPS_S (uint8_t)0x37     //  Set Maximum Return Packet Size

#define Packet_Header_DI_AwER (uint8_t)0x02      //  Acknowledge with Error Report
#define Packet_Header_DI_DCSRRO_S (uint8_t)0x21  //  DCS Read Short Response, 1 byte returned.
#define Packet_Header_DI_DCSRRT_S (uint8_t)0x22  //  DCS Read Short Response, 2 byte retunred.

/* RGB 24-bits color table definition (RGB888). */
#define COLOR_BLACK          (0x000000u)
#define COLOR_WHITE          (0xFFFFFFu)
#define COLOR_BLUE           (0x0000FFu)
#define COLOR_GREEN          (0x00FF00u)
#define COLOR_RED            (0xFF0000u)
#define COLOR_NAVY           (0x000080u)
#define COLOR_DARKBLUE       (0x00008Bu)
#define COLOR_DARKGREEN      (0x006400u)
#define COLOR_DARKCYAN       (0x008B8Bu)
#define COLOR_CYAN           (0x00FFFFu)
#define COLOR_TURQUOISE      (0x40E0D0u)
#define COLOR_INDIGO         (0x4B0082u)
#define COLOR_DARKRED        (0x800000u)
#define COLOR_OLIVE          (0x808000u)
#define COLOR_GRAY           (0x808080u)
#define COLOR_SKYBLUE        (0x87CEEBu)
#define COLOR_BLUEVIOLET     (0x8A2BE2u)
#define COLOR_LIGHTGREEN     (0x90EE90u)
#define COLOR_DARKVIOLET     (0x9400D3u)
#define COLOR_YELLOWGREEN    (0x9ACD32u)
#define COLOR_BROWN          (0xA52A2Au)
#define COLOR_DARKGRAY       (0xA9A9A9u)
#define COLOR_SIENNA         (0xA0522Du)
#define COLOR_LIGHTBLUE      (0xADD8E6u)
#define COLOR_GREENYELLOW    (0xADFF2Fu)
#define COLOR_SILVER         (0xC0C0C0u)
#define COLOR_LIGHTGREY      (0xD3D3D3u)
#define COLOR_LIGHTCYAN      (0xE0FFFFu)
#define COLOR_VIOLET         (0xEE82EEu)
#define COLOR_AZUR           (0xF0FFFFu)
#define COLOR_BEIGE          (0xF5F5DCu)
#define COLOR_MAGENTA        (0xFF00FFu)
#define COLOR_TOMATO         (0xFF6347u)
#define COLOR_GOLD           (0xFFD700u)
#define COLOR_ORANGE         (0xFFA500u)
#define COLOR_SNOW           (0xFFFAFAu)
#define COLOR_YELLOW         (0xFFFF00u)

#define ili_VC (uint8_t)0x00                 //  The ili9488 virtual channel is only 0b00.
#define other_device_VC (uint8_t)0x01        //  !! Not an actual name setting, however, correct value.
#define other_device_one_VC (uint8_t)0x10    //  !! Not an actual name setting, however, correct value.
#define other_device_tw_VC (uint8_t)0x11     //  !! Not an actual name setting, however, correct value.

#define SPa (uint8_t)0x15
#define LPa (uint8_t)0x29

#define ECC (uint8_t)0x00
#define CS (uint8_t)0x00

#define SPa_EOTP (uint8_t)0x08
#define SPa_DCSWN_S (uint8_t)0x05
#define SPa_DCSW_OS (uint8_t)0x15
#define SPa_DCSRN_S (uint8_t)0x06
#define SPa_SMRPS_S (uint8_t)0x37
#define SPa_AwER (uint8_t)0x02
#define SPa_DCSRR_OS (uint8_t)0x21
#define SPA_DCSRR_TS (uint8_t)0x22
#define LPa_NP_L (uint8_t)0x09
#define LPa_DCSW_L (uint8_t)0x39
#define LPa_DCSRR_L (uint8_t)0x1C

#define LPA false
#define SPA true

#define ili9488_device code (0x9488u)



#endif