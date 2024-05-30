#ifndef DIRECT_CURRENT_MOTOR_S_H
#define DIRECT_CURRENT_MOTOR_S_H

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





//  The wrap value would be taken from a rotary encoder input, 0 - 500, to determine the wrap level.
void drive_motor_wrap_set(pwm_settings_t *pwm){

    printf("\nMotor Wrap Function.\n");

    pwm->wrap_output_level_one = 0x00;
    printf("Wrap Output: 0x%04x.\n", pwm->wrap_output_level_one);

    if(pwm->pwm_pin_one != UNDEFINED){
        printf("\nPWM Pin %i Value: %i.\n", pwm->pwm_pin_one, pwm->rotary_wrap_input_level);

    //    (pwm->rotary_wrap_input_level > 0) ? printf("Wrap Value to Convert: 0b%04x.\n", pwm->rotary_wrap_input_level) : printf("Input wrap value is (int)NULL.\n");

        pwm->wrap_output_level_one += setValue(0x01, 0x31, pwm->rotary_wrap_input_level) ? (0x1388) : 0x00;      // printf("0-49 (int)NULL.\n");
            printf("\nWrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x32, 0x63, pwm->rotary_wrap_input_level) ? (0x2710) : 0x00;      // printf("50-99 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x64, 0x95, pwm->rotary_wrap_input_level) ? (0x3A98) : 0x00;      //printf("100-149 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x96, 0xC7, pwm->rotary_wrap_input_level) ? (0x4E20) : 0x00;      //printf("150-199 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0xC8, 0xF9, pwm->rotary_wrap_input_level) ? (0x61A8) : 0x00;      //printf("200-249 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0xFA, 0x12B, pwm->rotary_wrap_input_level) ? (0x7530) : 0x00;     //printf("250-299 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x12C, 0x15D, pwm->rotary_wrap_input_level) ? (0x88B8) : 0x00;    //printf("300-349 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x15E, 0x18F, pwm->rotary_wrap_input_level) ? (0x9C40) : 0x00;    //printf("350-399 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);
        pwm->wrap_output_level_one += setValue(0x190, 0x1F3, pwm->rotary_wrap_input_level) ? (0xAFC8) : 0x00;    //printf("400-499 (int)NULL.\n");
            printf("Wrap Value: 0x%04x.\n", pwm->wrap_output_level_one);

        (pwm->wrap_output_level_one > 0) ? printf("Outputting Wrap: 0x%04x.\n", pwm->wrap_output_level_one) : printf("\nMotor Wrap Set (int)NULL.\n");
    }
    else if(pwm->pwm_pin_two != UNDEFINED){

        setValue(0x00, 0x31, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x1388) : printf("0-49 (int)NULL.\n");
        setValue(0x32, 0x63, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x2710) : printf("50-99 (int)NULL.\n");
        setValue(0x64, 0x95, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x3A98) : printf("100-149 (int)NULL.\n");
        setValue(0x96, 0xC7, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x4E20) : printf("150-199 (int)NULL.\n");
        setValue(0xC8, 0xF9, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x61A8) : printf("200-249 (int)NULL.\n");
        setValue(0xFA, 0x12B, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x7530) : printf("250-299 (int)NULL.\n");
        setValue(0x12C, 0x15D, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x88B8) : printf("300-349 (int)NULL.\n");
        setValue(0x15E, 0x18F, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0x9C40) : printf("350-399 (int)NULL.\n");
        setValue(0x190, 0x1F3, pwm->rotary_wrap_input_level) ? (pwm->wrap_output_level_two += 0xAFC8) : printf("400-499 (int)NULL.\n");

        (pwm->wrap_output_level_two > 0) ? printf("Outputting Wrap: 0b%08x.\n", pwm->wrap_output_level_two) : printf("\nMotor Wrap Set (int)NULL.\n");
    }

}


void motor_drive_function(pwm_settings_t *pwm){

    printf("Motor Drive Function:  Duty Cycle Setting.\n");

    pwm->duty_cycle_output_one = 0x00;
    pwm->duty_cycle_output_two = 0x00;

    printf("Joystick -> DC Value: %i.\n", pwm->analog_vertical_input_level);

        if(pwm->pwm_pin_one != UNDEFINED){

            if(flag->vdtp){

            pwm->duty_cycle_output_one += setValue(0x00, 0x70, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .1) : 0;      //printf("Read between 0 - 112.\n");
            pwm->duty_cycle_output_one += setValue(0x71, 0xE1, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .2) : 0;      //printf("Read between 113 - 225.\n");
            pwm->duty_cycle_output_one += setValue(0xE2, 0x152, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .3) : 0;     //printf("Read between 226 - 338.\n");
            pwm->duty_cycle_output_one += setValue(0x153, 0x1C3, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .4) : 0;    //printf("Read between 339 - 451.\n");
            pwm->duty_cycle_output_one += setValue(0x1C4, 0x234, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .5) : 0;    //printf("Read between 452 - 564.\n");
            pwm->duty_cycle_output_one += setValue(0x235, 0x2A5, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .6) : 0;    //printf("Read between 565 - 677.\n");
            pwm->duty_cycle_output_one += setValue(0x2A6, 0x316, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .7) : 0;    //printf("Read between 678 - 790.\n");
            pwm->duty_cycle_output_one += setValue(0x317, 0x387, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .8) : 0;    //printf("Read between 791 - 903.\n");
            pwm->duty_cycle_output_one += setValue(0x388, 0x3F8, pwm->analog_vertical_input_level) ? (pwm->wrap_output_level_one * .9) : 0;    //printf("Read between 904 - 1016.\n");
            
            (pwm->duty_cycle_output_one > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_one) : printf("\nMotor Speed Idle\n");
            }
        
        }
        if(pwm->pwm_pin_two != UNDEFINED){

            if(flag->vdtn){
            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .1f) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .2f) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .3f) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .4f) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .5f) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .6f) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .7f) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .8f) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_two += .9f) : printf("Read between 904 - 1016.\n");

            (pwm->duty_cycle_output_two > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_two) : printf("\nMotor Speed Idle\n");
            }
        
        }

}


#endif