#ifndef ILI9488_SCREEN_COMMANDS
#define ILI9488_SCREEN_COMMANDS

//________________________________________________________________________________
//  Ilitek ILI9488 a-Si TFT LCD Single Chip Driver commands start: page 140.
//  Bit Setup: | D/Cx | RDX | WRX | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | hex |
//________________________________________________________________________________


#define RDII 0x04                   //  Read Display Identification Information
#define RNE_DSI 0x05                //  Read Number of the Errors on DSI

//  Display Command Set Write, no Parameter (DSCWN-S)
#define NOP 0x00
#define SOFT_RESET 0X01
#define Sleep_IN 0x10
#define Sleep_OUT 0x11
#define Parital_Mode_ON 0x12
#define Normal_Display_Mode_ON 0x13
#define Display_OFF 0x28
#define Display_ON 0x29
#define Tearing_Effect_Line_OFF 0x34
#define Tearing_Effect_Line_ON 0x35
#define Idle_Mode_OFF 0x38
#define Idle_Mode_ON 0x39


//  Display Command Set Write, 1 Parameter (DCSW1-S)
#define Memory_Write 0x2C
#define Memory_Access_Control 0x36
#define Interface_Pixel_Format 0x3A
#define Memory_Write_Continue 0x3C
#define Write_Display_Brightness_Value 0x51
#define Write_CTRL_Display_Value 0x53
#define WCABCV 0x55                    //   Write Content Adaptive Brightness Control Value
#define Write_CABC_MB 0x5E              //   Write CABC Minimum Brightness Value


//  Display Command Set Read, No Parameter (DCSRN-S)
#define Read_Display_Power_Mode 0x0A
#define Read_Display_MADCTL 0x0B
#define Read_Pixel_Format 0x0C
#define Read_Display_Image_Mode 0x0D
#define Read_Display_Signal_Mode 0x0E
#define Read_ID_ONE 0xDA
#define Read_ID_TWO 0xDB
#define Read_ID_THREE 0xDC
#define RDSDR 0x0F                  //  Read Display Self_Diagnostic Result
#define Memory_Read 0x2E
#define Read_Display_Brightness_Value 0x52
#define Read_CTRL_Display_Value 0x54
#define RCABCV 0x56                    //   Read Content Adaptive Brightness Control Value
#define Read_CABC_MB 0x5F               //   Read CABC Minimum Brightness Value


#define Read_Display_Status 0x09
#define Display_Inversion_OFF 0x20
#define Display_Inversion_ON 0x21

#define All_Pixel_OFF 0x22
#define All_Pixel_ON 0x23

#define Column_Address_Set 0x2A
#define Page_Address_Set 0x2B
#define Partial_Area 0x30

#define Vertical_Scolling_Definition 0x33
#define Vertial_Scolling_Start_Address 0x37

#define Memory_Read_Continue 0x3E
#define Write_Tear_Scan_Line 0x44
#define Read_Tear_Scan_line 0x45
#define Read_Automatic_Brightness 0x66

//     Extended Command List

#define Interface_Mode_Control 0xB0
#define Frame_Rate_Control_Normal 0xB1 // In Normal Mode/Full Colors
#define Frame_Rate_Control_Idle 0xB2
#define Frame_Rate_Control_Partial 0xB3
#define Display_Inversion_Control 0xB4
#define Blanking_Porch_Control 0xB5
#define Display_Function_Control 0xB6
#define Entry_Mode_Set 0xB7
#define Color_Enhancement_Control_One 0xB9
#define Color_Enhancement_Control_Two 0xBA

#define HS_Lanes_Control    0xBE
#define Power_Control_One   0xC0
#define Power_Control_Two   0xC1
#define Power_Control_Three 0xC2
#define Power_Control_Four  0xC3
#define Power_Control_Five  0xC4

#define VCOM_Control_One    0xC5

#define CABC_Control_One    0xC6
#define CABC_Control_Two    0xC8
#define CABC_Control_Three  0xC9
#define CABC_Control_Four   0xCA
#define CABC_Control_Five   0xCB
#define CABC_Control_Six    0xCC
#define CABC_Control_Seven  0xCD
#define CABC_Control_Eight  0xCE
#define CABC_Control_Nine   0xCF

#define NV_Memory_Write 0xD0

#define NV_Memory_Protection_Key 0xD1
#define NV_Memory_Status_Read 0xD2

#define Read_ID_FOUR 0xD3
#define Adjust_Control_One 0xD7
#define Read_ID_Four_Check 0xD8

#define PGAMCTRL 0xE0   //  Positive Gamma Control
#define NGAMCTRL 0xE1   //  Negative Gamma Control

#define Digital_Gamma_Control_One 0xE2
#define Digitial_Gamma_Control_Two 0xE3
#define Set_Image_Function 0xE9

#define Adjust_Control_Two 0xF2
#define Adjust_Control_Three 0xF7
#define Adjust_Control_Four 0xF8
#define Adjust_Control_Five 0xF9
#define Adjust_Control_Six 0xFC
#define Adjust_Control_Seven 0xFF

#define Packet_Header_DI_EOT 0x08       //  End of Transmission Packet
#define Packet_Header_DI_DCSWN_S 0x05    //  DCS Write, No Parameter
#define Packet_Header_DI_DCSWO_S 0x15    //  DCS Write, One Parameter
#define Packet_Header_DI_DCSRN_S 0x06    //  DCS Read, No Parameter
#define Packet_Header_DI_SMRPS_S 0x37     //  Set Maximum Return Packet Size

#define Packet_Header_DI_AwER 0x02      //  Acknowledge with Error Report
#define Packet_Header_DI_DCSRRO_S 0x21  //  DCS Read Short Response, 1 byte returned.
#define Packet_Header_DI_DCSRRT_S 0x22  //  DCS Read Short Response, 2 byte retunred.

#endif