#ifndef DC_MOTOR_HEADER
#define DC_MOTOR_HEADER

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define idleValue(x, y, z) ((uint8_t)(z >= x && z <= y))
#define float_to_int(a,b,c) ((a * b > 0) ? (c = (a*b)) : 0)

typedef struct {
    bool vdtp;  //  Vertical Drive Threshold Positive.
    bool vdti;  //  Vertical Drive Threshold Idle.
    bool vdtn;  //  Vertical Drive Threshold Negative.
    bool hdtr;  //  Horizontal Drive Threshold Right.
    bool hdti;  //  Horizontal Drive Threshold Idle.
    bool hdtl;  //  Horizontal Drive Threshold Left.
}motor_flags_status;
motor_flags_status motor_flags;

typedef struct pwm_setting_configuration_s {
   const uint pwm_pin_one;
   const uint pwm_pin_two;
   const uint pwm_pin_three;
   const uint pwm_pin_four;
   const uint pwm_pin_five;
   const uint pwm_pin_six;

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

    uint16_t float_converted_corrected;

    bool pwm_enabled_status;

}pwm_settings_t;
pwm_settings_t pwm_header_config;

void motor_data_display_buffer(pwm_settings_t *pwm){

    if(pwm->pwm_pin_one != (-1)){

        if(pwm->wrap_output_level_one > 0){
            sprintf(pwm->wrap_value_one, "Wrap One: %d", pwm->wrap_output_level_one);
        }
        if(pwm->duty_cycle_output_one > 0 && pwm->duty_cycle_output_one <= sizeof(float)) {
            sprintf(pwm->duty_cycle_one, "D Cycle: %f", pwm->duty_cycle_output_one);
        } else {
            sprintf(pwm->duty_cycle_one, "D Cycle: %d", 0);
        }
    }
    if(pwm->pwm_pin_two != (-1)){

        if(pwm->wrap_output_level_two > 0){
            sprintf(pwm->wrap_value_two, "Wrap Two: %d", pwm->wrap_output_level_two);
        }
        if(pwm->duty_cycle_output_two > 0) {
            sprintf(pwm->duty_cycle_two, "D Cycle: %f", pwm->duty_cycle_output_two);
        } else {
            sprintf(pwm->duty_cycle_two, "D Cycle: %d", 0);
        }
    }
    if(pwm->pwm_pin_three != (-1)){

        if(pwm->wrap_output_level_three > 0){
            sprintf(pwm->wrap_value_three, "Wrap Three: %d", pwm->wrap_output_level_three);
        }
        if(pwm->duty_cycle_output_three > 0) {
            sprintf(pwm->duty_cycle_three, "D Cycle: %f", pwm->duty_cycle_output_three);
        } else {
            sprintf(pwm->duty_cycle_three, "D Cycle: %d", 0);
        }
    }
    if(pwm->pwm_pin_four != (-1)){

        if(pwm->wrap_output_level_four > 0){
            sprintf(pwm->wrap_value_four, "Wrap Four: %d", pwm->wrap_output_level_four);
        }
        if(pwm->duty_cycle_output_one > 0) {
            sprintf(pwm->duty_cycle_four, "D Cycle: %f", pwm->duty_cycle_output_four);
        } else {
            sprintf(pwm->duty_cycle_four, "D Cycle: %d", 0);
        }
    }

}

//  The wrap value would be taken from a rotary encoder input, 0 - 500, to determine the wrap level.
void drive_motor_wrap_set(pwm_settings_t *pwm, int rotary_input){

    printf("\nMotor Wrap Configuration.\n");

    pwm->wrap_output_level_one = 0;
    pwm->wrap_output_level_two = 0;
    pwm->wrap_output_level_three = 0;
    pwm->wrap_output_level_four = 0;
    pwm->wrap_output_level_five = 0;
    pwm->wrap_output_level_six = 0;

    printf("Rotary Input Value: %d.\n", rotary_input);
    pwm->rotary_wrap_input_level = (rotary_input * 5);
    printf("Rotary Wrap Input Level: %d.\n", pwm->rotary_wrap_input_level);

    if(pwm->pwm_pin_one != UNDEFINED){

        (pwm->rotary_wrap_input_level > 0) ? printf("Wrap Value to Convert: 0b%04x.\n", pwm->rotary_wrap_input_level) : printf("Input wrap value is (int)NULL.\n");

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_one += 0x3E8) : printf("0-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_one += 0xDAC) : printf("100-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_one += 0x1388) : printf("200-299 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_one += 0x1964) : printf("300-399 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_one += 0x1F40) : printf("400-499 (int)NULL.\n");

        (pwm->wrap_output_level_one > 0) ? printf("Outputting Wrap: 0b%04x.\n", pwm->wrap_output_level_one) : printf("\nMotor Wrap Set (int)NULL.\n");

    }else if(pwm->pwm_pin_two != UNDEFINED){

        (pwm->rotary_wrap_input_level > 0) ? printf("Wrap Value to Convert: 0b%04x.\n", pwm->rotary_wrap_input_level) : printf("Input wrap value is (int)NULL.\n");

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_two += 0x3E8) : printf("0-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_two += 0xDAC) : printf("100-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_two += 0x1388) : printf("200-299 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_two += 0x1964) : printf("300-399 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_two += 0x1F40) : printf("400-499 (int)NULL.\n");

        (pwm->wrap_output_level_two > 0) ? printf("Outputting Wrap: 0b%08x.\n", pwm->wrap_output_level_two) : printf("\nMotor Wrap Set (int)NULL.\n");

    }else if(pwm->pwm_pin_three != UNDEFINED){

        (pwm->rotary_wrap_input_level > 0) ? printf("Wrap Value to Convert: 0b%04x.\n", pwm->rotary_wrap_input_level) : printf("Input wrap value is (int)NULL.\n");

        (pwm->rotary_wrap_input_level >= 0x00 && pwm->rotary_wrap_input_level <= 0x63) ? (pwm->wrap_output_level_three += 0x3E8) : printf("0-99 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x64 && pwm->rotary_wrap_input_level <= 0xC7) ? (pwm->wrap_output_level_three += 0xDAC) : printf("100-199 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0xC8 && pwm->rotary_wrap_input_level <= 0x12B) ? (pwm->wrap_output_level_three += 0x1388) : printf("200-299 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x12C && pwm->rotary_wrap_input_level <= 0x18F) ? (pwm->wrap_output_level_three += 0x1964) : printf("300-399 (int)NULL.\n");
        (pwm->rotary_wrap_input_level >= 0x190 && pwm->rotary_wrap_input_level <= 0x1F3) ? (pwm->wrap_output_level_three += 0x1F40) : printf("400-499 (int)NULL.\n");

        (pwm->wrap_output_level_three > 0) ? printf("Outputting Wrap: 0b%04x.\n", pwm->wrap_output_level_three) : printf("\nMotor Wrap Set (int)NULL.\n");
    }
    
}

void motor_flag_status(pwm_settings_t *pwm, motor_flags_status *flag, adc_port_values *adc){

    printf("\n\nFlag State Function.\n\n");

    pwm->analog_vertical_input_level = adc->adc1_mapped_value;
    printf("\n\n\tVertical Joystick Input: %i.\n", adc->adc1_mapped_value);
    pwm->analog_horizontal_input_level = adc->adc2_mapped_value;
    printf("\n\n\tHorizontal Joystick Input: %i.\n\n", adc->adc2_mapped_value);
    
    bool vertical_idle = idleValue(135, 140, pwm->analog_vertical_input_level);
    bool horizontal_idle = idleValue(140, 151, pwm->analog_horizontal_input_level);

    if(pwm->analog_vertical_input_level >= 145){
        flag->vdtp = true;
        flag->vdti = false;
          printf("Vertical Drive Threshold Crossed.\n");
    }else if(vertical_idle){
        flag->vdti = true;
        flag->vdtp = false;
          printf("Vertical Drive Threshold Idle.\n");
    }else if(pwm->analog_vertical_input_level <= 130){
        flag->vdtn = true;
          printf("Vertical Drive Threshold Reverse.\n");
    }else {
        printf("No flag status set.\n");
        flag->vdti = false;
        flag->vdtn = false;
        flag->vdtp = false;
    }

    if(pwm->analog_horizontal_input_level >= 155){
        flag->hdtr = true;
        flag->hdti = false;
        flag->hdtl = false;
          printf("Horizontal Threshold Right Turn.\n");
    }else if(horizontal_idle){
        flag->hdti = true;
        flag->hdtr = false;
        flag->hdtl = false;
          printf("Horizontal Threshold Idle.\n");
    }else if(pwm->analog_horizontal_input_level <= 135){
        flag->hdtl = true;
        flag->hdti = false;
        flag->hdtr = false;
          printf("Horizontal Threshold Left Turn.\n");
    }else {
        printf("No horizontal flag status set.\n");
        flag->hdti = false;
        flag->hdtl = false;
        flag->hdtr = false;
    }

}

void motor_drive_function(pwm_settings_t *pwm, adc_port_values *adc){

    pwm->duty_cycle_output_one = 0;
    pwm->duty_cycle_output_two = 0;
    pwm->duty_cycle_output_three = 0;
    pwm->duty_cycle_output_four = 0;
    pwm->duty_cycle_output_five = 0;
    pwm->duty_cycle_output_six = 0;

    pwm->analog_vertical_input_level = adc->adc1_mapped_value;
    printf("\n\nDuty Cycle Determination Input: %d.\n", pwm->analog_vertical_input_level);

    if(!(pwm->analog_vertical_input_level >= 0x87 && pwm->analog_vertical_input_level <= 0x8C)){

        if(pwm->pwm_pin_one != (-1)){

            (pwm->analog_vertical_input_level >= 0x8D && pwm->analog_vertical_input_level <= 0xA3) ? (pwm->duty_cycle_output_one += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xA4 && pwm->analog_vertical_input_level <= 0xBA) ? (pwm->duty_cycle_output_one += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xBB && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_one += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE8) ? (pwm->duty_cycle_output_one += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE9 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_one += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_one > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_one) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM One: No Output.\n");
        }
        if(pwm->pwm_pin_two != (-1)){

            (pwm->analog_vertical_input_level >= 0xB4 && pwm->analog_vertical_input_level <= 0xC2) ? (pwm->duty_cycle_output_two += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xC3 && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_two += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE0) ? (pwm->duty_cycle_output_two += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE1 && pwm->analog_vertical_input_level <= 0xEF) ? (pwm->duty_cycle_output_two += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xF0 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_two += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_two > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_two) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM Two: No Output.\n");
        }
        if(pwm->pwm_pin_three != (-1)){

            (pwm->analog_vertical_input_level >= 0xB4 && pwm->analog_vertical_input_level <= 0xC2) ? (pwm->duty_cycle_output_three += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xC3 && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_three += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE0) ? (pwm->duty_cycle_output_three += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE1 && pwm->analog_vertical_input_level <= 0xEF) ? (pwm->duty_cycle_output_three += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xF0 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_three += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_three > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_three) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM Three: No Output.\n");
        }
        if(pwm->pwm_pin_four != (-1)){

            (pwm->analog_vertical_input_level >= 0xB4 && pwm->analog_vertical_input_level <= 0xC2) ? (pwm->duty_cycle_output_four += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xC3 && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_four += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE0) ? (pwm->duty_cycle_output_four += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE1 && pwm->analog_vertical_input_level <= 0xEF) ? (pwm->duty_cycle_output_four += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xF0 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_four += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_four > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_four) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM Four: No Output.\n");
        }
        if(pwm->pwm_pin_five != (-1)){

            (pwm->analog_vertical_input_level >= 0xB4 && pwm->analog_vertical_input_level <= 0xC2) ? (pwm->duty_cycle_output_five += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xC3 && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_five += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE0) ? (pwm->duty_cycle_output_five += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE1 && pwm->analog_vertical_input_level <= 0xEF) ? (pwm->duty_cycle_output_five += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xF0 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_five += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_five > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_five) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM Five: No Output.\n");
        }
        if(pwm->pwm_pin_six != (-1)){

            (pwm->analog_vertical_input_level >= 0xB4 && pwm->analog_vertical_input_level <= 0xC2) ? (pwm->duty_cycle_output_six += .1) : printf("180-189 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xC3 && pwm->analog_vertical_input_level <= 0xD1) ? (pwm->duty_cycle_output_six += .3) : printf("190-199 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xD2 && pwm->analog_vertical_input_level <= 0xE0) ? (pwm->duty_cycle_output_six += .5) : printf("200-209 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xE1 && pwm->analog_vertical_input_level <= 0xEF) ? (pwm->duty_cycle_output_six += .7) : printf("210-219 (int)NULL.\n");
            (pwm->analog_vertical_input_level >= 0xF0 && pwm->analog_vertical_input_level <= 0xFF) ? (pwm->duty_cycle_output_six += .9) : printf("220-225 (int)NULL.\n");
            (pwm->duty_cycle_output_six > 0) ? printf("\tMotor Speed Set Value: %f.\n", pwm->duty_cycle_output_six) : printf("\nMotor Speed Idle\n");
        } else {
            printf("PWM Six: No Output.\n");
        }
    } else {
        printf("Analog Stick Vertical:  IDLE.\n");
    }
}

//  Completed
void initial_pwm_pin_init(pwm_settings_t *pwm){

    printf("\n\nPWM Initialization.\n");

    pwm->float_converted_corrected = float_to_int(pwm->duty_cycle_one_init, pwm->wrap_one_init, pwm->float_converted_corrected);
    printf("Float Converted Value Macro: %u.\n", pwm->float_converted_corrected);

    if(pwm->pwm_pin_one != UNDEFINED){

        printf("PWM Pin One:  Initializing.\n");

        gpio_init(pwm->pwm_pin_one);
        gpio_set_function(pwm->pwm_pin_one, GPIO_FUNC_PWM);
            printf("\tPWM Pin %d set as PWM.\n", pwm->pwm_pin_one);

    pwm->slice_one = pwm_gpio_to_slice_num(pwm->pwm_pin_one);
    pwm->chan_one = pwm_gpio_to_channel(pwm->pwm_pin_one);
    
    pwm_set_clkdiv(pwm->slice_one, 64.f);
        printf("Clkdiv set as 64.f.\n");
    pwm_set_wrap(pwm->slice_one, pwm->wrap_one_init);
    pwm_set_chan_level(pwm->slice_one, pwm->chan_one, pwm->float_converted_corrected);
        printf("Duty Cycle Init: %u.\n", pwm->float_converted_corrected);
        pwm_set_enabled(pwm->slice_one, true);

    printf("\n\tPWM Slice Num: %d.\n\tPWM Chan: %d.\n\tPWM Wrap: %d.\n", pwm->slice_one, pwm->chan_one, pwm->wrap_one_init);

    }else {
        printf("PWM Pin One:  Not Initialized.\n");
    }

    if(pwm->pwm_pin_two != UNDEFINED){

        gpio_init(pwm->pwm_pin_two);
        gpio_set_function(pwm->pwm_pin_two, GPIO_FUNC_PWM);

    pwm->slice_two = pwm_gpio_to_slice_num(pwm->pwm_pin_two);
    pwm->chan_two = pwm_gpio_to_channel(pwm->pwm_pin_two);
    pwm_set_clkdiv(pwm->slice_two, 64.f);
    pwm_set_wrap(pwm->slice_two, pwm->wrap_two_init);
    pwm_set_chan_level(pwm->slice_two, pwm->chan_two, 0);
    pwm_set_enabled(pwm->slice_two, true);
    }

    if(pwm->pwm_pin_three != UNDEFINED){

        gpio_init(pwm->pwm_pin_three);
        gpio_set_function(pwm->pwm_pin_three, GPIO_FUNC_PWM);

    pwm->slice_three = pwm_gpio_to_slice_num(pwm->pwm_pin_three);
    pwm->chan_three = pwm_gpio_to_channel(pwm->pwm_pin_three);
    pwm_set_clkdiv(pwm->slice_three, 64.f);
    pwm_set_wrap(pwm->slice_three, pwm->wrap_three_init);
    pwm_set_chan_level(pwm->slice_three, pwm->chan_three, 0);
    pwm_set_enabled(pwm->slice_three, true);
    }

    if(pwm->pwm_pin_four != UNDEFINED){

        gpio_init(pwm->pwm_pin_four);
        gpio_set_function(pwm->pwm_pin_four, GPIO_FUNC_PWM);

    pwm->slice_four = pwm_gpio_to_slice_num(pwm->pwm_pin_four);
    pwm->chan_four = pwm_gpio_to_channel(pwm->pwm_pin_four);
    pwm_set_clkdiv(pwm->slice_four, 64.f);
    pwm_set_wrap(pwm->slice_four, pwm->wrap_four_init);
    pwm_set_chan_level(pwm->slice_four, pwm->chan_four, 0);
    pwm_set_enabled(pwm->slice_four, true);
    }

    if(pwm->pwm_pin_five != UNDEFINED){

        gpio_init(pwm->pwm_pin_five);
        gpio_set_function(pwm->pwm_pin_five, GPIO_FUNC_PWM);

    pwm->slice_five = pwm_gpio_to_slice_num(pwm->pwm_pin_five);
    pwm->chan_five = pwm_gpio_to_channel(pwm->pwm_pin_five);
    pwm_set_clkdiv(pwm->slice_five, 64.f);
    pwm_set_wrap(pwm->slice_five, pwm->wrap_five_init);
    pwm_set_chan_level(pwm->slice_five, pwm->chan_five, 0);
    pwm_set_enabled(pwm->slice_five, true);
    }

    if(pwm->pwm_pin_six != UNDEFINED){

        gpio_init(pwm->pwm_pin_six);
        gpio_set_function(pwm->pwm_pin_six, GPIO_FUNC_PWM);

    pwm->slice_six = pwm_gpio_to_slice_num(pwm->pwm_pin_six);
    pwm->chan_six = pwm_gpio_to_channel(pwm->pwm_pin_six);
    pwm_set_clkdiv(pwm->slice_six, 64.f);
    pwm_set_wrap(pwm->slice_six, pwm->wrap_six_init);
    pwm_set_chan_level(pwm->slice_six, pwm->chan_six, 0);
    pwm_set_enabled(pwm->slice_six, true);
    }

}

//  Completed
//  Pwm_set_channel_levels takes uint PWM_PIN, uint8_t Channel input value, and uint16_t wrap set value.
void pwm_set_motor_config(motor_flags_status *flag, pwm_settings_t *pwm){
   
    printf("\n\nPwm Set Motor Config Function Active.\n");

    pwm->float_converted_corrected = float_to_int(pwm->duty_cycle_output_one, pwm->wrap_output_level_one, pwm->float_converted_corrected);
    printf("Float Converted Value Macro: %u.\n", pwm->float_converted_corrected);

    if(flag->vdtp == true && flag->vdti != true){

        if(pwm->pwm_pin_one != UNDEFINED){

            printf("Disable PWM on %i, to configure.\n", pwm->pwm_pin_one);
           pwm_set_enabled(pwm->slice_one, false);
            printf("\tMotor One Configured.\n");
            printf("\tWrap: %u.\n\tDuty Cycle: %f.\n", pwm->wrap_output_level_one, pwm->float_converted_corrected);
            pwm_set_wrap(pwm->slice_one, pwm->wrap_output_level_one);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, pwm->float_converted_corrected);
          //  pwm_set_gpio_level(pwm->pwm_pin_one, pwm->float_converted_corrected);
          pwm_set_enabled(pwm->slice_one, true);
          printf("Re-enabled PWM on %i, configured.\n", pwm->pwm_pin_one);

        }else if(pwm->pwm_pin_two != UNDEFINED){

            printf("Motor Two Configured.\n");
            printf("Wrap: %lu.\nDuty Cycle: %f.\n", pwm->wrap_output_level_two, pwm->duty_cycle_output_two);
            pwm_set_wrap(pwm->slice_two, pwm->wrap_output_level_two);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, pwm->duty_cycle_output_two);

        }else if(pwm->pwm_pin_three != UNDEFINED){
            pwm_set_wrap(pwm->slice_three, pwm->wrap_output_level_three);
            pwm_set_chan_level(pwm->slice_three, pwm->chan_three, pwm->duty_cycle_output_three);
        }else if(pwm->pwm_pin_four != UNDEFINED){
            pwm_set_wrap(pwm->slice_four, pwm->wrap_output_level_four);
            pwm_set_chan_level(pwm->slice_four, pwm->chan_four, pwm->duty_cycle_output_four);
        }else if(pwm->pwm_pin_five != UNDEFINED){
            pwm_set_wrap(pwm->slice_five, pwm->wrap_output_level_five);
            pwm_set_chan_level(pwm->slice_five, pwm->chan_five, pwm->duty_cycle_output_five);
        }else if(pwm->pwm_pin_six != UNDEFINED){
            pwm_set_wrap(pwm->slice_six, pwm->wrap_output_level_six);
            pwm_set_chan_level(pwm->slice_six, pwm->chan_six, pwm->duty_cycle_output_six);
        }
    }else if(flag->vdti == true && flag->vdtp != true){
        if(pwm->pwm_pin_one != UNDEFINED){
            printf("Motor One Idle.\n");
            printf("Wrap: %lu.\nDuty Cycle: %f.\n", pwm->wrap_output_level_one, pwm->duty_cycle_output_one);
            pwm_set_wrap(pwm->slice_one, 0);
            pwm_set_chan_level(pwm->slice_one, pwm->chan_one, 0);
        }
        if(pwm->pwm_pin_two != UNDEFINED){
            pwm_set_wrap(pwm->slice_two, 0);
            pwm_set_chan_level(pwm->slice_two, pwm->chan_two, 0);
        }
        if(pwm->pwm_pin_three != UNDEFINED){
            pwm_set_wrap(pwm->slice_three, 0);
            pwm_set_chan_level(pwm->slice_three, pwm->chan_three, 0);
        }
        if(pwm->pwm_pin_four != UNDEFINED){
            pwm_set_wrap(pwm->slice_four, 0);
            pwm_set_chan_level(pwm->slice_four, pwm->chan_four, 0);
        }
        if(pwm->pwm_pin_five != UNDEFINED){
            pwm_set_wrap(pwm->slice_five, 0);
            pwm_set_chan_level(pwm->slice_five, pwm->chan_five, 0);
        }
        if(pwm->pwm_pin_six != UNDEFINED){
            pwm_set_wrap(pwm->slice_six, 0);
            pwm_set_chan_level(pwm->slice_six, pwm->chan_six, 0);
        }
    }
}


#endif