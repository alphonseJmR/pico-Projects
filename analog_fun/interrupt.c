#include <stdio.h>
#include <stdlib.h>
#include "pico.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define button 9
#define buttontwo 14

#define gate_pin1 15
#define gate_pin2 10
#define gate_pin3 11
#define gate_pin4 12
#define gate_pin5 13


int place_marker = 3;

void place_check_func(placer) {


    if(placer < 0) {
        printf("Place marker was less than zero, reset.\n");
            sleep_ms(50);
        place_marker == 0;

    } else if(placer > 5) {
        printf("Placer is too high.\n");
            sleep_ms(50);
        place_marker -= 1;
    }
}


void button_callback(uint gpio, uint32_t events) {

        printf("Interrupt occured at pin %d, with event %d\n", gpio, events);

        if(gpio_get(button) == 0x000) {

                printf("Button one pressed.\n");

                place_marker++;
                printf("Place is %d.\n", place_marker);

        } else if(gpio_get(buttontwo) == 0x000) {
    
                printf("Button Two Pressed.\n");
        
                place_marker--;
                printf("Place is %d.\n", place_marker);
            
            
        } else {

    }
}
    
void led_fire(place) {

    if(place == 1) {

        for(int aa = 0; aa < 20; aa++) {

        gpio_put(gate_pin1, 1);
            sleep_ms(128);
        gpio_put(gate_pin1, 0);

        }

    } else if(place == 2) {

        for(int ab = 0; ab < 20; ab++) {

        gpio_put(gate_pin2, 1);
            sleep_ms(64);
        gpio_put(gate_pin2, 0);

        }

    } else if(place == 3) {

        for(int ac = 0; ac < 20; ac++) {

        gpio_put(gate_pin3, 1);
            sleep_ms(32);
        gpio_put(gate_pin3, 0);

        }

    } else if(place == 4) {

        for(int ad = 0; ad < 20; ad++) {
        gpio_put(gate_pin4, 1);
            sleep_ms(16);
        gpio_put(gate_pin4, 0);

        }

    } else if(place == 5) {

        for(int ae = 0; ae < 20; ae++) {

        gpio_put(gate_pin5, 1);
            sleep_ms(8);
        gpio_put(gate_pin5, 0);

        }

    } else {

        for(int b = 0; b < 10; b++) {

            printf("Placer hasn't yet incremented.\n");
            
            sleep_ms(10);
            gpio_put(gate_pin1, 1);
            sleep_ms(10);
            gpio_put(gate_pin1, 0);
            sleep_ms(10);
            gpio_put(gate_pin2, 1);
            sleep_ms(10);
            gpio_put(gate_pin2, 0);
            sleep_ms(10);
            gpio_put(gate_pin3, 1);
            sleep_ms(10);
            gpio_put(gate_pin3, 0);
            sleep_ms(10);
            gpio_put(gate_pin4, 1);
            sleep_ms(10);
            gpio_put(gate_pin4, 0);
            sleep_ms(10);
            gpio_put(gate_pin5, 1);
            sleep_ms(10);
            gpio_put(gate_pin5, 0);
            sleep_ms(10);

        }

    }
}

int main()
{
    stdio_init_all();

    gpio_init(button);
    gpio_set_dir(button, GPIO_IN);
    gpio_pull_up(button);

    gpio_init(buttontwo);
    gpio_set_dir(buttontwo, GPIO_IN);
    gpio_pull_up(buttontwo);

    gpio_init(gate_pin1);
    gpio_set_dir(gate_pin1, GPIO_OUT);
    gpio_init(gate_pin2);
    gpio_set_dir(gate_pin2, GPIO_OUT);
    gpio_init(gate_pin3);
    gpio_set_dir(gate_pin3, GPIO_OUT);
    gpio_init(gate_pin4);
    gpio_set_dir(gate_pin4, GPIO_OUT);
    gpio_init(gate_pin5);
    gpio_set_dir(gate_pin5, GPIO_OUT);
    
    
    gpio_set_irq_enabled_with_callback(button, 0x04, 1, &button_callback);
    gpio_set_irq_enabled_with_callback(buttontwo, 0x04, 1, &button_callback);
    
    
    while(1) {

        place_check_func(place_marker);
        printf("Loop running.  Awating button press.\n");
             sleep_ms(500);
        led_fire(place_marker);

            sleep_ms(500);
        tight_loop_contents();

        }

}