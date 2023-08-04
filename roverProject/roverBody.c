#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "nrf24_driver.h"
#include "hardware/timer.h"


//  Poteniometer pin is ADC pin 26
#define speed_pot_pin 26

//  Pins for motor controls are set as pin 0 and 1
#define motor_forward 14
#define motor_reverse 15

//  Define sensor i/o pins (interrupt's will occur on these pins);
#define motor_go 12
#define motor_back 13

// define buzzer pin
#define buzzer 9



long map(long x, long in_min, long in_max, long out_min, long out_max) {

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

typedef struct chan_level_data {
    
    uint8_t chan_one;       //  Analog stick left vertical input data
    uint8_t chan_two;       //  Analog stick left horizontal input data
    uint8_t chan_three;     //  Analog stick right vertical input
    uint8_t chan_four;      //  Analog stick right horizontal input data
    uint8_t chan_five;      //  
    uint8_t chan_six;
    uint8_t chan_seven;

} chan_level_data;
  chan_level_data Data_Send;

const rF_packet() {

    int ii = 0;
    while(ii < 7){
        switch(ii) {
            case 0:
                Data_Send.chan_one = 0; // Enter By Function
                break;
            case 1:
                Data_Send.chan_two = 0; // " "
                break;
            case 2:
                Data_Send.chan_three = 0; // " "
                break;
            case 3:
                Data_Send.chan_four = 0; // " "
                break;
            case 4:
                Data_Send.chan_five = 0; // " "
                break;
            case 5:
                Data_Send.chan_six = 0; // " "
                break;
            case 6:
                Data_Send.chan_seven = 0; // " "
                break;
            default:
                printf("Packet Error: Channel greater than set channels.\n");
                break;
        }
        ii++;
    }


}


void buzzbuzz() {

        gpio_put(buzzer, 1);
            sleep_ms(40);
        gpio_put(buzzer, 0);
        printf("Buzz Buzz Bitches.\n");

}

void buzzbuzzback() {

    for(int a = 0; a < 10; a++) {
        gpio_put(buzzer, 1);
            sleep_ms(100);
        gpio_put(buzzer, 0);
    }
}

volatile bool motors_go;
volatile bool motors_back;

void button_callback(uint gpio, uint32_t events) {

        printf("Interrupt occured at pin %d, with event %d\n", gpio, events);

        if(gpio_get(motor_go) != 0) {

                motors_go = true;

        } else if(gpio_get(motor_back) != 0) {
    
                motors_back = true;

        } else {

    }
}
/*
bool repeating_timer_callback(struct repeating_timer *t) {
    gpio_put(buzzer, 1);
        sleep_ms(40);
    gpio_put(buzzer, 0);

}
*/

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int main() {

    int z;
    time_t t;

    srand((unsigned) time(&t));



    stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);


    gpio_init(motor_forward);
    gpio_set_function(motor_forward, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(motor_forward);

    pwm_set_wrap(slice_num, 63);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, true);

    gpio_init(motor_reverse);
    gpio_set_function(motor_reverse, GPIO_FUNC_PWM);

    uint num_slice = pwm_gpio_to_slice_num(motor_reverse);

    pwm_set_wrap(num_slice, 63);
    pwm_set_chan_level(num_slice, PWM_CHAN_A, 0);
    pwm_set_enabled(num_slice, true);

    gpio_init(motor_go);
    gpio_set_dir(motor_go, GPIO_IN);
    gpio_pull_down(motor_go);

    gpio_init(motor_back);
    gpio_set_dir(motor_back, GPIO_IN);
    gpio_pull_down(motor_back);

    gpio_init(buzzer);
    gpio_set_dir(buzzer, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(motor_go, 0x04, 1, &button_callback);
    gpio_set_irq_enabled_with_callback(motor_back, 0x04, 1, &button_callback);

   // struct repeating_timer *timer;
   // add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);


    while(1)
    {
/*
        uint16_t result = adc_read();
        long pwm_value = map(result, 0, 4095, 0, 255);

        printf("Raw: %l /    PWM: %l", result, pwm_value);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_value);
*/  
        if(motors_go == true){
            printf("Motor go is true.\n");
            buzzbuzz();
            while(gpio_get(motor_go) != 0) {
                uint16_t results = adc_read();
                long pwm_value = map(results, 0, 4095, 0, 511);
                printf("Raw: %d/t PWM: %d\n", results, pwm_value);
                pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_value);
                printf("Driving.\n");
                printf("Pin number 12 firing confirmation.");
                

                }
                motors_go = false;
            }

        else if(motors_back == true){
            printf("Motor back is true.\n");
            buzzbuzzback();
            while(gpio_get(motor_back) != 0){    
                uint16_t result = adc_read();
                long pwm_values = map(result, 0, 4095, 0, 511);
                printf("Raw: %d/t PWM: %d\n", result, pwm_values);
                pwm_set_chan_level(num_slice, PWM_CHAN_A, pwm_values);
                printf("Reversing.\n");
                printf("Pin number 13 firing confirmation.");
                
            }
                motors_back = false;
            }
            else {
                if(motors_go && motors_back == false) {
                printf("Ideling...\n\n");
                }
            }


        tight_loop_contents();
    }
}

/*
    adc_init();
    adc_gpio_init(speed_pot_pin);
    adc_select_input(0);

    gpio_init(motor_forward);
    gpio_set_function(motor_forward, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor_forward);

    pwm_set_wrap(slice_num, 255);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, true);

    gpio_init(motor_reverse);
    gpio_set_function(motor_reverse, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(motor_reverse);
    uint channel = pwm_gpio_to_channel(motor_reverse);

    pwm_set_wrap(slice, 127);
    pwm_set_chan_level(slice, PWM_CHAN_A, 0);
    pwm_set_enabled(slice, true);

 
    */
/*
    while(1){



        tight_loop_contents();
    }
*/
    
/*
           if(motors_go == true) {
            printf("Time to Drive.\n");
            gpio_put(motor_forward, 1);

            while(gpio_get(motor_go) != 1) {

                if(gpio_get(motor_forward) > 0) {
                    printf("Driving.\n");
                }
                else {

                    printf("Bat shit crazy driver.\n");

                }
            
            }
            sleep_ms(30);
            gpio_put(motor_forward, 0);
            motors_go = false;
            
        }
        else if(motors_back == true) {
            printf("Backing up time.\n");
            gpio_put(motor_reverse, 1);

         while(gpio_get(motor_back) != 1) {

                if(gpio_get(motor_reverse) > 0) {
                    printf("Driving.\n");
                }
                else {

                    printf("Bat shit crazy driver.\n");

                }
            
            }
            sleep_ms(30);
            gpio_put(motor_reverse, 0);
            motors_back = false;

        }
    
*/

