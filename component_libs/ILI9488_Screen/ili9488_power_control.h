#ifndef ILI9488_POWER_CONTROL_H
#define ILI9488_POWER_CONTROL_H

//  Hex values for positive gamma control.
uint8_t VRH_ONE[32] = {

    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
    0x1E, 0x1F

}

//  Hex values for negative gamma control.
uint8_t VRH_TWO[32] = {

    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
    0x1E, 0x1F

}

//  (VCL x 2) | (-(VCL x 2) | - VCL | VCL x (6,5,4) | (-VCL x 5, 4, 5, 4, 3, 4, 3))
typedef enum step_up_factor = {

    HIGH_VCL = 0X00,
    HIGH_LOW_VCL = 0X01,
    INHIBITED = 0X02,
    MEDIUM_HIGH_VCL = 0X03,
    MEDIUM_MEDIUM_VCL = 0X04,
    MEDIUM_LOW_VCL = 0X05,
    LOW_HIGH_VCL = 0X06,
    LOW_LOW_VCL = 0X07

};

typedef enum DCA_zero_N = {

    ONE_EIGHTH_H = 0X00,
    ONE_QUARTER_H = 0X01,
    ONE_HALF_H = 0X02,
    ONE_H = 0X03,
    TWO_H = 0X04

};

typedef enum DCA_one_N = {
    ONE_HALF_H = 0X00,
    ONE_H = 0X10,
    TWO_H = 0X20,
    FOUR_H = 0X30,
    EIGHT_H = 0X40

};




#endif