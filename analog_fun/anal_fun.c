#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

typedef struct anal_data{
    uint16_t vertical;     // Current reading of vertical analog stick data.
    long vert_pwm;         // Result of long map conversion of vertical analog data.
    uint16_t horizontal;   // Current reading of horizontal analog stick data.
    long hori_pwm;         // Result of long map conversion on horizontal analog data.
    uint16_t temp_sensor;  // Current reading of adc port 2
    long result;           // Result of long map conversion on temp sensor ... or whatever analog fun is connected.
    int speed_multiplier;  // Button call to increase drive speed.
} anal_data;

anal_data analog;

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

    rgbColors.r = (analog.result / analog.speed_multiplier);
    rgbColors.g = (analog.result / analog.speed_multiplier);
    rgbColors.b = (analog.result / analog.speed_multiplier);

}

void adc_data_call() {
        for(int d = 0; d < 3; d++){
          switch(d){
            case 0:
              adc_select_input(0);
              analog.temp_sensor = adc_read();
              analog.result = map(analog.temp_sensor, 0, 4095, 0, 4095);
              break;
            case 1:
              adc_select_input(d);
              analog.vertical = adc_read();
              analog.vert_pwm = map(analog.vertical, 0, 4095, 0, 4095);
              break;
            case 2:
              adc_select_input(d);
              analog.horizontal = adc_read();
              analog.hori_pwm = map(analog.horizontal, 0, 4095, 0, 4095);
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

void pwm_init_pin(uint8_t pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
}


int main () {

    time_t t;
    srand((unsigned) time(&t));

   stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(2);

    pwm_init_pin(motor_drive);
        uint slice_num[0] = pwm_gpio_to_slice_num(motor_drive);
    pwm_init_pin(motor_reverse);
        uint slice_num[1] = pwm_gpio_to_slice_num(motor_reverse);
    pwm_init_pin(rgb_r);
        uint slice_num[2] = pwm_gpio_to_slice_num(rgb_r);
    pwm_init_pin(rgb_g);
        uint slice_num[3] = pwm_gpio_to_slice_num(rgb_g);
    pwm_init_pin(rgb_b);
        uint slice_num[4] = pwm_gpio_to_slice_num(rgb_b);

    for(int a = 0; a < 5; a++) {

        pwm_set_wrap(slice_num[a], 4095);
        pwm_init(slice_num[a], 0, true);

    }
    
    gpio_set_irq_enabled_with_callback(anal_speed, 0x04, 1, &button_callback);
    adc_irq_set_enabled(true);

while(1){

    adc_data_call();
        if(analog.vertical != 0){
            printf("Motor forward value: %l.\n", analog.vertical);
            while(analog.vertical != 0) {

                if(analog.vertical > 0){
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, analog.vert_pwm);
                    printf("Driving forward.\n");
                    printf("Pin number 10 firing confirmation.");
                }else if(analog.vertical < 0) {
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, analog.vert_pwm);
                    printf("Reversing.\n");
                    printf("Pin number 11 firing confirmation.");
                }

                }
                  
        }else {}

    


    }
        tight_loop_contents();
    }