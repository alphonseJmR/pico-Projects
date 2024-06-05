#ifndef DIRECT_CURRENT_MOTOR_S_H
#define DIRECT_CURRENT_MOTOR_S_H

#include <stdio.h>

#define idleValue(x, y, z) (bool)(((uint16_t)y >= (uint16_t)x) && ((uint16_t)x <= (uint16_t)z))
#define inputAbove(a, b) (bool)((uint16_t)a > (uint16_t)b)
#define inputBelow(a, b) (bool) ((uint16_t)a < (uint16_t)b)
#define input_Test(a, b, c) (bool)( ( ((uint16_t)a <= (uint16_t)c ) && ((uint16_t)b >= (uint16_t)c) ))

uint16_t five_K_inc_wrap[8] = {
    0x1388,
    0x2710,
    0x3A98,
    0x4E20,
    0x61A8,
    0x7530,
    0x88B8,
    0x9C40,
    0xAFC8
};
uint16_t analog_checks[17] = {

    0x00, 0x70,
    0x71, 0xE1,
    0xE2, 0x152,
    0x153, 0x1C3,
    0x1C4, 0x234,
    0x235, 0x2A5,
    0x2A6, 0x316,
    0x317, 0x387,
    0x388, 0x3F8

};
uint16_t rotary_checks[17] = {

    0x01, 0x31,
    0x32, 0x63,
    0x64, 0x95,
    0x96, 0xC7,
    0xC8, 0xF9,
    0xFA, 0x12B,
    0x12C, 0x15D,
    0x15E, 0x18F,
    0x190, 0x1F3

};


//  Holds only two settings for two motors, forward, and reverse.
typedef struct dc_motor_pin_s {
    uint8_t motor_one_forward;
    uint8_t motor_one_reverse;
    
    uint8_t motor_two_forward;
    uint8_t motor_two_reverse;
}dc_pins;


typedef struct dc_motor_pwm_configuration_s {

    uint8_t dc_slice_one;
    uint8_t dc_chan_one;
    uint16_t dc_wrap_one;
    uint16_t dc_one_level;

    uint8_t dc_slice_two;
    uint8_t dc_chan_two;
    uint16_t dc_wrap_two;
    uint16_t dc_two_level;

}dc_pwm;

typedef struct dc_motor_configuration_s {

    dc_pins *my_pins;
    dc_pwm *my_pwm;

}dc_motor;

//  Only handles a single motor ATM.  Adding a second is just a conditional.
void set_new_configuration(dc_motor *dc, uint16_t wrap, uint16_t level, uint8_t motor_num, bool direction){

    printf("\t////\tSetting New DC PWM Configuration\t////\n");

 if(motor_num == 1){

  if(direction){

    pwm_set_wrap(dc->my_pwm->dc_slice_one, wrap);
    pwm_set_chan_level(dc->my_pwm->dc_slice_one, dc->my_pwm->dc_chan_one, level);

  }else {

    pwm_set_wrap(dc->my_pwm->dc_slice_two, wrap);
    pwm_set_chan_level(dc->my_pwm->dc_slice_two, dc->my_pwm->dc_chan_two, level);

  }
 }
 if(motor_num == 2){

  if(direction){
    pwm_set_wrap(dc->my_pwm->dc_slice_one, wrap);
    pwm_set_chan_level(dc->my_pwm->dc_slice_one, dc->my_pwm->dc_chan_one, level);
  }else {
    pwm_set_wrap(dc->my_pwm->dc_slice_two, wrap);
    pwm_set_chan_level(dc->my_pwm->dc_slice_two, dc->my_pwm->dc_chan_two, level);
  }
 }

}


uint16_t set_drive_power(dc_motor *dc, uint16_t rotary_in, uint16_t *duty_tbl){

    uint16_t buffer_out;
    buffer_out = 0;

    for(int i = 0; i < 18; i += 2){

        buffer_out = (input_Test(duty_tbl[i], duty_tbl[i+1], rotary_in)) ? (dc->my_pwm->dc_wrap_one * (i / .1)) : 0;
        i = (buffer_out > 0) ? 17 : i;
        printf("Drive Power Now: 0x%04x.\n", buffer_out);
        
    }

    return buffer_out;
}


uint16_t set_drive_wrap(dc_motor *dc, uint16_t analog_in, uint16_t *wrap_tbl_chk, uint16_t *wrap_tbl){

    uint16_t buffer_out;
    buffer_out = 0;

    for(int i = 0; i < 18; i += 2){

        buffer_out = (input_Test(wrap_tbl_chk[i], wrap_tbl_chk[i+1], analog_in)) ? wrap_tbl[ ((i % 2 == 0) ? (i / 2) : (i - 1))] : 0;
        i = (buffer_out > 0) ? 17 : i;
        printf("Drive Wrap Now: 0x%04X.\n", buffer_out);

    }

    return buffer_out;

}


void set_motor_values(dc_motor *DC, uint16_t an_in, uint16_t ro_in){

    printf("\n\t////\tSetting New Values For DC Motor\t////\n");

    DC->my_pwm->dc_wrap_one = set_drive_wrap(DC, an_in, analog_checks, five_K_inc_wrap);
    DC->my_pwm->dc_one_level = set_drive_power(DC, ro_in, rotary_checks);

    printf("\n\n\t////\tNewest Motor Settings\t////\n\n");
    printf("\tWrap: 0x%04x.\n", DC->my_pwm->dc_wrap_one);
    printf("\tDuty Cycle: 0x%04x.\n", DC->my_pwm->dc_one_level);
    
}






#endif