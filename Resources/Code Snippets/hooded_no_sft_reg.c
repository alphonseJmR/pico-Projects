#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#define ebit uint8_t
#define MaX 255

#define reg_data 10
#define reg_oe 11
#define reg_latch 12
#define reg_clk 13


uint16_t bizcuit;
ebit forward, reverse;

//  Directions values will be shifted to acheive motor movement.
//  forward << x, reverse >> x





void gpio_start(ebit a, ebit b, ebit c, ebit d){

    gpio_init(a);
    gpio_set_dir(a, GPIO_OUT);
       gpio_pull_down(a);

    gpio_init(b);
    gpio_set_dir(b, GPIO_OUT);
       gpio_pull_down(b);

    gpio_init(c);
    gpio_set_dir(c, GPIO_OUT);
       gpio_pull_down(c);

    gpio_init(d);
    gpio_set_dir(d, GPIO_OUT);
        gpio_pull_up(d);
}

int main(){
    bizcuit = 0;
    stdio_init_all();

        sleep_ms(7000);
        sleep_ms(3000);
    gpio_start(reg_data, reg_oe, reg_latch, reg_clk);

    
    printf("Beginning Sequences.\n");
        sleep_ms(750);
while(1){


    printf("\nMain Loop.\n\n");
    printf("Current Iteration: %i.\n", bizcuit);

    if(bizcuit <= 150){
            printf("Delay Speed Currently: 5.\n");
        motor_steps(5, 200, true);
            sleep_ms(2);
    }else if(bizcuit > 150 && bizcuit <= 300){
            printf("Delay Speed Currently: 4.\n");
        motor_steps(4, 200, false);
            sleep_ms(2);
    }else if(bizcuit > 300 && bizcuit <= 450){
            printf("Delay Speed Currently: 3.\n");
        motor_steps(3, 200, true);
            sleep_ms(2);
    }else if(bizcuit > 450 && bizcuit <= 600){
            printf("Delay Speed Currently: 2.\n");
        motor_steps(2, 200, false);
            sleep_ms(2);
    }else if(bizcuit > 600 && bizcuit <= 750){
            printf("Delay Speed Currently: 1.\n");
        motor_steps(1, 200, true);
            sleep_ms(2);
    }else {
        bizcuit = 0;
        printf("Restarting loop testing.\n");
    }
        bizcuit++;
        sleep_ms(150);


}
tight_loop_contents();
}