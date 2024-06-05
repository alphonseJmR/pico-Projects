#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#define ebit uint8_t
#define MaX 255

#define pin_A 10
#define pin_B 11
#define pin_C 12
#define pin_D 13


uint16_t bizcuit;
ebit forward, reverse;

//  Directions values will be shifted to acheive motor movement.
//  forward << x, reverse >> x

void indi_step(uint pin, uint8_t delay){
    gpio_put(pin, 1);
        sleep_ms(delay);
    gpio_put(pin, 0);
}

void motor_steps(ebit t_delay, int num_steps, bool direction){

    if(direction){

        for(int i = 0; i < num_steps; i++){

            indi_step(pin_A, t_delay);
            indi_step(pin_C, t_delay);
            indi_step(pin_B, t_delay);
            indi_step(pin_D, t_delay);

        }
    }else {
        for(int i = 0; i < num_steps; i++){

            indi_step(pin_D, t_delay);
            indi_step(pin_B, t_delay);
            indi_step(pin_C, t_delay);
            indi_step(pin_A, t_delay);

        }
    }

}

void gpio_start(ebit a, ebit b, ebit c, ebit d){

    gpio_init(a);
    gpio_set_dir(a, GPIO_OUT);
       // gpio_pull_down(a);

    gpio_init(b);
    gpio_set_dir(b, GPIO_OUT);
       // gpio_pull_down(b);

    gpio_init(c);
    gpio_set_dir(c, GPIO_OUT);
       // gpio_pull_down(c);

    gpio_init(d);
    gpio_set_dir(d, GPIO_OUT);
       // gpio_pull_down(d);
}

int main(){
    bizcuit = 0;
    stdio_init_all();

        sleep_ms(7000);
        sleep_ms(3000);
    gpio_start(pin_A, pin_B, pin_C, pin_D);

    
    printf("Beginning Sequences.\n");
        sleep_ms(750);
while(1){


    printf("\nMain Loop.\n\n");
    printf("Current Iteration: %i.\n", bizcuit);

    if(bizcuit <= 10){
            printf("Delay Speed Currently: 3.\n");
        motor_steps(3, 2048, true);
        printf("Current Revolution Set: 2048.\n");
            sleep_us(100);
    }else if(bizcuit > 10 && bizcuit <= 20){
            printf("Delay Speed Currently: 4.\n");
        motor_steps(3, 1024, true);
        printf("Current Revolution Set: 1024.\n");
            sleep_us(100);
    }else if(bizcuit > 20 && bizcuit <= 30){
            printf("Delay Speed Currently: 3.\n");
        motor_steps(3, 512, true);
        printf("Current Revolution Set: 512.\n");
            sleep_us(100);
    }else if(bizcuit > 30 && bizcuit <= 60){
            printf("Delay Speed Currently: 2.\n");
        motor_steps(3, 256, true);
        printf("Current Revolution Set: 256.\n");
            sleep_us(100);
    }else if(bizcuit > 40 && bizcuit <= 100){
            printf("Delay Speed Currently: 1.\n");
        motor_steps(3, 128, true);
        printf("Current Revolution Set: 128.\n");
            sleep_us(100);
    }else {
        bizcuit = 0;
        printf("Restarting loop testing.\n");
    }
        bizcuit++;
        sleep_us(10);


}
tight_loop_contents();
}