#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define hall_sensor_one 26      // Set gpio pin for hall effect sensor one.
#define hall_sensor_two 27      // Set gpio pin for hall effect sensor two.
#define hall_sensor_three 28    // Set gpio pin for hall effect sensor three.

#define motor_engage 15         //  Engage / disengage the structure spin motor via button.
#define motor_drive 14          //  GPIO pin related to the motor output pin.

#define rgb_r 11
#define rgb_g 12
#define rgb_b 13

typedef struct hall_data{
    uint16_t hall_one;         // Current reading of hall effect sensor 1.  adc port 0
    long hall_one_pwm;         // Result of long map conversion of hall effect sensor 1.
    uint16_t hall_two;         // Current reading of hall effect sensor 2.  adc port 1
    long hall_two_pwm;         // Result of long map conversion of hall effect sensor 2.
    uint16_t hall_three;       // Current reading of hall effect sensor 3.  adc port 2
    long hall_three_pwm;       // Result of long map conversion of hall effect sensor 3.
            // Button call to engage or disengage motor drive.
} hall_data;
hall_data hall;

//  Struct to hold values for rgb data.
typedef struct rgb_data {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb_data;
rgb_data rgbColors;



// Function declaration area
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void rgb_color() {

    rgbColors.r = hall.hall_one_pwm;
    rgbColors.g = hall.hall_two_pwm;
    rgbColors.b = hall.hall_three_pwm;

}

void adc_data_call() {

    adc_select_input(0);
    hall.hall_one = adc_read();
    hall.hall_one_pwm = map(hall.hall_one, 0, 4095, 0, 4095);

    adc_select_input(1);
    hall.hall_two = adc_read();
    hall.hall_two_pwm = map(hall.hall_two, 0, 4095, 0, 4095);
             
    adc_select_input(2);
    hall.hall_three = adc_read();
    hall.hall_three_pwm = map(hall.hall_three, 0, 4095, 0, 4095);
            
}

void button_callback(uint gpio, uint32_t events) {
        printf("Interrupt occured at pin %d, with event %d\n", gpio, events);

    if(motor_switch == false) {
        motor_switch == true;
    }else if(motor_switch == true){
        motor_switch == false;
    }
}

volatile bool motor_switch;

int main () {

   stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(2);

    uint slice_num[1];

    pwm_init_pin(motor_drive);
        slice_num[1] = pwm_gpio_to_slice_num(motor_drive);
        pwm_set_wrap(slice_num[1], 255);
        pwm_set_chan_level(slice_num[1], PWM_CHAN_A, 0);
        pwm_init(slice_num[1], 0, true);
    pwm_init_pin(rgb_r);
        slice_num[2] = pwm_gpio_to_slice_num(rgb_r);
        pwm_set_wrap(slice_num[2], 255);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_B, 0);
        pwm_init(slice_num[2], 0, true);
    pwm_init_pin(rgb_g);
        slice_num[3] = pwm_gpio_to_slice_num(rgb_g);
        pwm_set_wrap(slice_num[3], 255);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_A, 0);
        pwm_init(slice_num[3], 0, true);
    pwm_init_pin(rgb_b);
        slice_num[4] = pwm_gpio_to_slice_num(rgb_b);
        pwm_set_wrap(slice_num[4], 255);
        pwm_set_chan_level(slice_num[4], PWM_CHAN_B, 0);
        pwm_init(slice_num[4], 0, true);


    gpio_set_irq_enabled_with_callback(motor_switch, 0x04, 1, &button_callback);

while(1){

    adc_data_call();
    rgb_color();

    if(motor_switch == true) {
        printf("Motor drive engaged.\n");
        pwm_set_chan_level(motor_drive, PWM_CHAN_A, 127); 
    } else {
        printf("Motor drive disengaged.\n");
        pwm_set_chan_level(motor_drive, PWM_CHAN_A, 0);
    }
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rgbColors.r);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_A, rgbColors.g);
        pwm_set_chan_level(slice_num[4], PWM_CHAN_A, rgbColors.b);
        printf("RGB Colors are: R- %d, G- %d, B- %d.\n", rgbColors.r, rgbColors.g, rgbColors.b);

    }
        tight_loop_contents();
}