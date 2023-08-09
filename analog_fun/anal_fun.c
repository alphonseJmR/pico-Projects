#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define temp_senesor 26
#define analog_stick_vertical 27
#define analog_stick_horizontal 28

#define anal_speed 15 // Use button from analog_stick

#define motor_drive 10
#define motor_reverse 11

#define rgb_r 12
#define rgb_g 13
#define rgb_b 14

typedef anal_data{
    uint16_t vertical;
    long vert_pwm;
    uint16_t horizontal;
    long hori_pwm;
    uint16_t temp_sensor;
    long result;
    int speed_multiplier;
}anal_data;

anal_data analog;

// Function declaration area
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void adc_interrupt_call() {
        for(int d = 0; d < 3; d++){
          switch(d){
            case 0:
              adc_select_input(0);
              analog.button_selected = adc_read();
              analog.result = map(analog.button_selected, 0, 4095, 0, 255);
              break;
            case 1:
              adc_select_input(d);
              analog.vertical = adc_read();
              analog.vert_pwm = map(analog.vertical, 0, 4095, 0, 255);
              break;
            case 2:
              adc_select_input(d);
              analog.horizontal = adc_read();
              analog.hori_pwm = map(analog.horizontal, 0, 4095, 0, 255);
              break;
            default:
              printf("Error at adc interrupt call.\n");
          }
        }
};

volatile bool button_presses = false;
int button_pressed = 0;

void button_press() {

    if(button_pressed >= 6){
        button_pressed = 0;
    }
    button_pressed++;
    analog.speed_multiplier = button_pressed;

}

void button_callback(uint gpio, uint32_t events) {

        printf("Interrupt occured at pin %d, with event %d\n", gpio, events);

        if(gpio_get(anal_speed) != 1) {

            button_presses = true;

        }else {

            button_presses = false;
            printf("No current interrupts.\n");

        }
}

