#ifndef DC_MOTOR_HEADER
#define DC_MOTOR_HEADER

#include <stdio.h>
#include <stdint.h>
#include "rc_nrf_configs.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define idleValue(x, y, z) ((uint8_t)(x >= x && z <= y))
#define setValue(a, b, c) ((uint8_t)(c >= a && c <= b))

typedef struct {
    bool vdtp;  //  Vertical Drive Threshold Positive.
    bool vdti;  //  Vertical Drive Threshold Idle.
    bool vdtn;  //  Vertical Drive Threshold Negative.
    bool hdtr;  //  Horizontal Drive Threshold Right.
    bool hdti;  //  Horizontal Drive Threshold Idle.
    bool hdtl;  //  Horizontal Drive Threshold Left.
}motor_flags_status;
motor_flags_status motor_flags;

typedef struct {
    uint pwm_pin_one;
    uint pwm_pin_two;
    uint pwm_pin_three;
    uint pwm_pin_four;
    uint pwm_pin_five;
    uint pwm_pin_six;

    uint slice_one;
    uint slice_two;
    uint slice_three;
    uint slice_four;
    uint slice_five;
    uint slice_six;

    uint chan_one;
    uint chan_two;
    uint chan_three;
    uint chan_four;
    uint chan_five;
    uint chan_six;

    uint16_t wrap_one_init;
    uint16_t wrap_two_init;
    uint16_t wrap_three_init;
    uint16_t wrap_four_init;
    uint16_t wrap_five_init;
    uint16_t wrap_six_init;

    float duty_cycle_one_init;
    float duty_cycle_two_init;
    float duty_cycle_three_init;
    float duty_cycle_four_init;
    float duty_cycle_five_init;
    float duty_cycle_six_init;

    char wrap_value_one[16];
    char wrap_value_two[16];
    char wrap_value_three[16];
    char wrap_value_four[16];
    char wrap_value_five[16];
    char wrap_value_six[16];

    char duty_cycle_one[16];
    char duty_cycle_two[16];
    char duty_cycle_three[16];
    char duty_cycle_four[16];
    char duty_cycle_five[16];
    char duty_cycle_six[16];

    uint16_t rotary_wrap_input_level;          //   Value taken from rotary encoder.
    uint16_t analog_vertical_input_level;      //   Vertical value taken from analog joystick.  Premapped.
    uint16_t analog_horizontal_input_level;    //   Horizontal value from analog joystick.      Premapped.

    uint16_t wrap_output_level_one;
    uint16_t wrap_output_level_two;
    uint16_t wrap_output_level_three;
    uint16_t wrap_output_level_four;
    uint16_t wrap_output_level_five;
    uint16_t wrap_output_level_six;

    float duty_cycle_output_one;
    float duty_cycle_output_two;
    float duty_cycle_output_three;
    float duty_cycle_output_four;
    float duty_cycle_output_five;
    float duty_cycle_output_six;
}pwm_settings_t;
pwm_settings_t pwm_header_config;

void motor_data_display_buffer(pwm_settings_t *pwm){

    if(pwm->pwm_pin_one != -1){

        if(pwm->wrap_output_level_one > 0){
            sprintf(pwm->wrap_value_one, "Wrap One: %d", pwm->wrap_output_level_one);
        }
        if(pwm->duty_cycle_output_one > 0) {
            sprintf(pwm->duty_cycle_one, "D Cycle: %d", pwm->duty_cycle_output_one);
        }
    }
    if(pwm->pwm_pin_two != -1){

        if(pwm->wrap_output_level_two > 0){
            sprintf(pwm->wrap_value_two, "Wrap Two: %d", pwm->wrap_output_level_two);
        }
        if(pwm->duty_cycle_output_two > 0) {
            sprintf(pwm->duty_cycle_two, "D Cycle: %d", pwm->duty_cycle_output_two);
        }
    }
    if(pwm->pwm_pin_three != -1){

        if(pwm->wrap_output_level_three > 0){
            sprintf(pwm->wrap_value_three, "Wrap Three: %d", pwm->wrap_output_level_three);
        }
        if(pwm->duty_cycle_output_three > 0) {
            sprintf(pwm->duty_cycle_three, "D Cycle: %d", pwm->duty_cycle_output_three);
        }
    }
    if(pwm->pwm_pin_four != -1){

        if(pwm->wrap_output_level_four > 0){
            sprintf(pwm->wrap_value_four, "Wrap Four: %d", pwm->wrap_output_level_four);
        }
        if(pwm->duty_cycle_output_one > 0) {
            sprintf(pwm->duty_cycle_four, "D Cycle: %d", pwm->duty_cycle_output_four);
        }
    }

}

//  The wrap value would be taken from a rotary encoder input, 0 - 500, to determine the wrap level.
void drive_motor_wrap_set(pwm_settings_t *pwm){

    if(pwm->pwm_pin_one != UNDEFINED){

    //    (pwm->rotary_wrap_input_level > 0) ? printf("Wrap Value to Convert: 0b%04x.\n", pwm->rotary_wrap_input_level) : printf("Input wrap value is (int)NULL.\n");

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x31) ? (pwm->wrap_output_level_one += 0x1388) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x32 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_one += 0x2710) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0x95) ? (pwm->wrap_output_level_one += 0x3A98) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x96 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_one += 0x4E20) : printf("150-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0xF9) ? (pwm->wrap_output_level_one += 0x61A8) : printf("200-249 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xFA && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_one += 0x7530) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x15D) ? (pwm->wrap_output_level_one += 0x88B8) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x15E && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_one += 0x9C40) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_one += 0xAFC8) : printf("150-199 (int)NULL.\n");

        (pwm->wrap_output_level_one > 0) ? printf("Outputting Wrap: 0b%08x.\n", pwm->wrap_output_level_one) : printf("\nMotor Wrap Set (int)NULL.\n");
    }
    else if(pwm->pwm_pin_two != UNDEFINED){

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x31) ? (pwm->wrap_output_level_one += 0x1388) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x32 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_one += 0x2710) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0x95) ? (pwm->wrap_output_level_one += 0x3A98) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x96 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_one += 0x4E20) : printf("150-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0xF9) ? (pwm->wrap_output_level_one += 0x61A8) : printf("200-249 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xFA && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_one += 0x7530) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x15D) ? (pwm->wrap_output_level_one += 0x88B8) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x15E && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_one += 0x9C40) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_one += 0xAFC8) : printf("150-199 (int)NULL.\n");

        (pwm->wrap_output_level_one > 0) ? printf("Outputting Wrap: 0b%08x.\n", pwm->wrap_output_level_one) : printf("\nMotor Wrap Set (int)NULL.\n");
    }
    else if(pwm->pwm_pin_three != UNDEFINED){

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x31) ? (pwm->wrap_output_level_one += 0x1388) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x32 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_one += 0x2710) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0x95) ? (pwm->wrap_output_level_one += 0x3A98) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x96 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_one += 0x4E20) : printf("150-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0xF9) ? (pwm->wrap_output_level_one += 0x61A8) : printf("200-249 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xFA && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_one += 0x7530) : printf("0-49 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x15D) ? (pwm->wrap_output_level_one += 0x88B8) : printf("50-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x15E && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_one += 0x9C40) : printf("100-149 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_one += 0xAFC8) : printf("150-199 (int)NULL.\n");

        (pwm->wrap_output_level_one > 0) ? printf("Outputting Wrap: 0b%08x.\n", pwm->wrap_output_level_one) : printf("\nMotor Wrap Set (int)NULL.\n");
    }
    
}

void motor_flag_status(pwm_settings_t *pwm, motor_flags_status *flag){

    if(pwm->analog_vertical_input_level > 175){
        flag->vdtp = true;
          printf("Vertical Drive Threshold Crossed.\n");
    }else if(pwm->analog_vertical_input_level > 160 && pwm->analog_vertical_input_level < 170){
        flag->vdti = true;
          printf("Vertical Drive Threshold Idle.\n");
    }else if(pwm->analog_vertical_input_level < 155){
        flag->vdtn = true;
          printf("Vertical Drive Threshold Reverse.\n");
    }else {
        printf("No flag status set.\n");
        flag->vdti = false;
        flag->vdtn = false;
        flag->vdtp = false;
    }

    if(pwm->analog_horizontal_input_level > 175){
        flag->hdtr = true;
          printf("Horizontal Threshold Right Turn.\n");
    }else if(pwm->analog_horizontal_input_level > 160 & pwm->analog_horizontal_input_level < 170){
        flag->hdti = true;
          printf("Horizontal Threshold Idle.\n");
    }else if(pwm->analog_horizontal_input_level < 155){
        flag->hdtl = true;
          printf("Horizontal Threshold Left Turn.\n");
    }else {
        printf("No horizontal flag status set.\n");
        flag->hdti = false;
        flag->hdtl = false;
        flag->hdtr = false;
    }

}

void motor_drive_function(pwm_settings_t *pwm){

    pwm->duty_cycle_output_one = 0;
    pwm->duty_cycle_output_two = 0;
    pwm->duty_cycle_output_three = 0;
    pwm->duty_cycle_output_four = 0;
    pwm->duty_cycle_output_five = 0;
    pwm->duty_cycle_output_six = 0;

    if(pwm->analog_vertical_input_level >= 0x96 && pwm->analog_vertical_input_level < 0xAA){

        if(pwm->pwm_pin_one != UNDEFINED){
            
            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_one > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_one) : printf("\nMotor Speed Idle\n");
        }
        else if(pwm->pwm_pin_two != UNDEFINED){

            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_two > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_two) : printf("\nMotor Speed Idle\n");
        }
        else if(pwm->pwm_pin_three != UNDEFINED){

            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_three > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_three) : printf("\nMotor Speed Idle\n");
        }
        else if(pwm->pwm_pin_four != UNDEFINED){

            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_four > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_four) : printf("\nMotor Speed Idle\n");
        }
        else if(pwm->pwm_pin_five != UNDEFINED){

            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_five > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_five) : printf("\nMotor Speed Idle\n");
        }
        else if(pwm->pwm_pin_six != UNDEFINED){

            (setValue(0x00, 0x70, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .1) : printf("Read between 0 - 112.\n");
            (setValue(0x71, 0xE1, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .2) : printf("Read between 113 - 225.\n");
            (setValue(0xE2, 0x152, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .3) : printf("Read between 226 - 338.\n");
            (setValue(0x153, 0x1C3, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .4) : printf("Read between 339 - 451.\n");
            (setValue(0x1C4, 0x234, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .5) : printf("Read between 452 - 564.\n");
            (setValue(0x235, 0x2A5, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .6) : printf("Read between 565 - 677.\n");
            (setValue(0x2A6, 0x316, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .7) : printf("Read between 678 - 790.\n");
            (setValue(0x317, 0x387, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .8) : printf("Read between 791 - 903.\n");
            (setValue(0x388, 0x3F8, pwm->analog_vertical_input_level)) ? (pwm->duty_cycle_output_one += .9) : printf("Read between 904 - 1016.\n");
            (pwm->duty_cycle_output_six > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_six) : printf("\nMotor Speed Idle\n");
        }
    
    
    }
}

//  Completed
void initial_pwm_pin_init(pwm_settings_t *pwm){

    if(pwm->pwm_pin_one != UNDEFINED){

        gpio_init(pwm->pwm_pin_one);
        gpio_set_function(pwm->pwm_pin_one, GPIO_FUNC_PWM);

    pwm->slice_one = pwm_gpio_to_slice_num(pwm->pwm_pin_one);
    pwm->chan_one = pwm_gpio_to_channel(pwm->pwm_pin_one);
    pwm_set_wrap(pwm->slice_one, pwm->wrap_one_init);
    pwm_set_chan_level(pwm->slice_one, pwm->chan_one, 0);
    pwm_set_enabled(pwm->slice_one, true);
    }

    if(pwm->pwm_pin_two != UNDEFINED){

        gpio_init(pwm->pwm_pin_two);
        gpio_set_function(pwm->pwm_pin_two, GPIO_FUNC_PWM);

    pwm->slice_two = pwm_gpio_to_slice_num(pwm->pwm_pin_two);
    pwm->chan_two = pwm_gpio_to_channel(pwm->pwm_pin_two);
    pwm_set_wrap(pwm->slice_two, pwm->wrap_two_init);
    pwm_set_chan_level(pwm->slice_two, pwm->chan_two, 0);
    pwm_set_enabled(pwm->slice_two, true);
    }

    if(pwm->pwm_pin_three != UNDEFINED){

        gpio_init(pwm->pwm_pin_three);
        gpio_set_function(pwm->pwm_pin_three, GPIO_FUNC_PWM);

    pwm->slice_three = pwm_gpio_to_slice_num(pwm->pwm_pin_three);
    pwm->chan_three = pwm_gpio_to_channel(pwm->pwm_pin_three);
    pwm_set_wrap(pwm->slice_three, pwm->wrap_three_init);
    pwm_set_chan_level(pwm->slice_three, pwm->chan_three, 0);
    pwm_set_enabled(pwm->slice_three, true);
    }

    if(pwm->pwm_pin_four != UNDEFINED){

        gpio_init(pwm->pwm_pin_four);
        gpio_set_function(pwm->pwm_pin_four, GPIO_FUNC_PWM);

    pwm->slice_four = pwm_gpio_to_slice_num(pwm->pwm_pin_four);
    pwm->chan_four = pwm_gpio_to_channel(pwm->pwm_pin_four);
    pwm_set_wrap(pwm->slice_four, pwm->wrap_four_init);
    pwm_set_chan_level(pwm->slice_four, pwm->chan_four, 0);
    pwm_set_enabled(pwm->slice_four, true);
    }

    if(pwm->pwm_pin_five != UNDEFINED){

        gpio_init(pwm->pwm_pin_five);
        gpio_set_function(pwm->pwm_pin_five, GPIO_FUNC_PWM);

    pwm->slice_five = pwm_gpio_to_slice_num(pwm->pwm_pin_five);
    pwm->chan_five = pwm_gpio_to_channel(pwm->pwm_pin_five);
    pwm_set_wrap(pwm->slice_five, pwm->wrap_five_init);
    pwm_set_chan_level(pwm->slice_five, pwm->chan_five, 0);
    pwm_set_enabled(pwm->slice_five, true);
    }

    if(pwm->pwm_pin_six != UNDEFINED){

        gpio_init(pwm->pwm_pin_six);
        gpio_set_function(pwm->pwm_pin_six, GPIO_FUNC_PWM);

    pwm->slice_six = pwm_gpio_to_slice_num(pwm->pwm_pin_six);
    pwm->chan_six = pwm_gpio_to_channel(pwm->pwm_pin_six);
    pwm_set_wrap(pwm->slice_six, pwm->wrap_six_init);
    pwm_set_chan_level(pwm->slice_six, pwm->chan_six, 0);
    pwm_set_enabled(pwm->slice_six, true);
    }

}

//  Completed
//  Pwm_set_channel_levels takes uint PWM_PIN, uint8_t Channel input value, and uint16_t wrap set value.
void pwm_set_motor_config(motor_flags_status *flag, pwm_settings_t *pwm){
   
    printf("Pwm Set Motor Config Function Active.\n");

    if(flag->vdtp == true){
        if(pwm->pwm_pin_one != UNDEFINED){
            pwm_set_wrap(pwm->slice_one, pwm->wrap_output_level_one);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, pwm->duty_cycle_output_one);
            pwm_set_enabled(pwm->slice_one, true);
        }
        if(pwm->pwm_pin_two != UNDEFINED){
            pwm_set_wrap(pwm->slice_two, pwm->wrap_output_level_two);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, pwm->duty_cycle_output_two);
            pwm_set_enabled(pwm->slice_two, true);
        }
        if(pwm->pwm_pin_three != UNDEFINED){
            pwm_set_wrap(pwm->slice_three, pwm->wrap_output_level_three);
            pwm_set_chan_level(pwm->slice_three, pwm->chan_three, pwm->duty_cycle_output_three);
            pwm_set_enabled(pwm->slice_three, true);
        }
        if(pwm->pwm_pin_four != UNDEFINED){
            pwm_set_wrap(pwm->slice_four, pwm->wrap_output_level_four);
            pwm_set_chan_level(pwm->slice_four, pwm->chan_four, pwm->duty_cycle_output_four);
            pwm_set_enabled(pwm->slice_four, true);
        }
        if(pwm->pwm_pin_five != UNDEFINED){
            pwm_set_wrap(pwm->slice_five, pwm->wrap_output_level_five);
            pwm_set_chan_level(pwm->slice_five, pwm->chan_five, pwm->duty_cycle_output_five);
            pwm_set_enabled(pwm->slice_five, true);
        }
        if(pwm->pwm_pin_six != UNDEFINED){
            pwm_set_wrap(pwm->slice_six, pwm->wrap_output_level_six);
            pwm_set_chan_level(pwm->slice_six, pwm->chan_six, pwm->duty_cycle_output_six);
            pwm_set_enabled(pwm->slice_six, true);
        }
    }else if(flag->vdti == true){
        if(pwm->pwm_pin_one != UNDEFINED){
            pwm_set_wrap(pwm->slice_one, pwm->wrap_output_level_one);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, 0);
            pwm_set_enabled(pwm->slice_one, true);
        }
        if(pwm->pwm_pin_two != UNDEFINED){
            pwm_set_wrap(pwm->slice_two, pwm->wrap_output_level_two);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, 0);
            pwm_set_enabled(pwm->slice_two, true);
        }
        if(pwm->pwm_pin_three != UNDEFINED){
            pwm_set_wrap(pwm->slice_three, pwm->wrap_output_level_three);
            pwm_set_chan_level(pwm->slice_three, pwm->chan_three, 0);
            pwm_set_enabled(pwm->slice_three, true);
        }
        if(pwm->pwm_pin_four != UNDEFINED){
            pwm_set_wrap(pwm->slice_four, pwm->wrap_output_level_four);
            pwm_set_chan_level(pwm->slice_four, pwm->chan_four, 0);
            pwm_set_enabled(pwm->slice_four, true);
        }
        if(pwm->pwm_pin_five != UNDEFINED){
            pwm_set_wrap(pwm->slice_five, pwm->wrap_output_level_five);
            pwm_set_chan_level(pwm->slice_five, pwm->chan_five, 0);
            pwm_set_enabled(pwm->slice_five, true);
        }
        if(pwm->pwm_pin_six != UNDEFINED){
            pwm_set_wrap(pwm->slice_six, pwm->wrap_output_level_six);
            pwm_set_chan_level(pwm->slice_six, pwm->chan_six, 0);
            pwm_set_enabled(pwm->slice_six, true);
        }
    }
}

void motor_activation(pwm_settings_t *pwm, motor_flags_status *flag, payload_active_s *payload){

    printf("Motor Activation Function.\n");

    pwm->analog_vertical_input_level = payload->pay_one_buffer_s.vertical_active;
    pwm->analog_horizontal_input_level = payload->pay_one_buffer_s.horizontal_active;

    motor_data_display_buffer(pwm);
    motor_flag_status(pwm, flag);
    motor_drive_function(pwm);
    motor_data_display_buffer(pwm);

    pwm_set_motor_config(flag, pwm);

}
#endif