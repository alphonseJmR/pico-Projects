#ifndef RADIO_CONTROLLER_VERSION_4_CONFIGURATIONS_S_H
#define RADIO_CONTROLLER_VERSION_4_CONFIGURATIONS_S_H

#include "controller_structures.h"



ADC analog_sticks = {
  .ADC_1 = {
    .adc_instance = 0,
    .pinum = 26,
    .mapped = 0,
    .miin = 10,
    .main = 4095,
    .miout = 0,
    .maout = 2048,
    .raw_avg = 0
  },
  .ADC_2 = {
    .adc_instance = 1,
    .pinum = 27,
    .mapped = 0,
    .miin = 10,
    .main = 4095,
    .miout = 0,
    .maout = 2048,
    .raw_avg = 0
  }
};


//  Text array crap atm

char lchannel[20];
char data_rate[20];
char power[20]; 
char *tesxt = "Test Code          ";
char numtesk[20];
char vertical[20];
char horizontal[20];

char *init_err = "     Init Error    ";
char *init_err1 = "   Please Reset   ";
char *init_tex = "   Initialisation  ";
char *init_text = "  Was A Success    ";
char *adc_test = " Testing ADC Input  ";



#endif