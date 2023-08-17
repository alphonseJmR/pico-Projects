#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

#define hall_sensor_one 26      // Set gpio pin for hall effect sensor one.

#define clk 1                   // Rotary encoder pin clk
#define dt 2                    // Rotary encoder pin dt
#define rotEnc_button 0         //  Rotary encoder button pin
#define buzzer 3

#define rgb_r 13
#define rgb_g 14
#define rgb_b 15

int rgbColor_Select = 0;          //  Current position of rgb led pin output.
volatile int rotCounter;


typedef struct hall_data{
    uint16_t hall_one;              //   Current reading of hall effect sensor 1.  adc port 0
    long hall_one_pwm;              //   Result of long map conversion of hall effect sensor 1.
    uint16_t rot_a_read;
    long rot_a_pwm;
    uint16_t rot_b_read;
    long rot_b_pwm;
} hall_data;
hall_data hall;

typedef struct rotValues{
    volatile bool lastClk;
    volatile bool Clk;
    volatile bool lastDt;
    volatile bool Dt;
    volatile bool Button;
    volatile int a;
    volatile int b;
}rotValues;
rotValues rot;

// Function declaration area
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//  Function call to adc pins.  All three can be read from this function, provided they're replaced in the brackets.
void adc_data_call() {

    adc_select_input(0);
    hall.hall_one = adc_read();
    hall.hall_one_pwm = map(hall.hall_one, 0, 4095, 0, 255);
/*
    adc_select_input(1);
    hall.rot_a_read = adc_read();
    hall.rot_a_pwm= map(hall.rot_a_read, 0, 4095, 0, 255);

    adc_select_input(2);
    hall.rot_b_read = adc_read();
    hall.rot_b_pwm = map(hall.rot_b_read, 0, 4095, 0, 255);
    sleep_ms(20);
  */      
}

void color_check(int num) {
        if(num > 4){

        rgbColor_Select = 0;
    } 
}

void button_callback(uint gpio, uint32_t events) {

   // printf("Interrupt occured at pin: %i, with events: %i.\n\n", gpio, events);
    if(gpio == rotEnc_button){
     //   printf("\n\nInterrupt occured at Button.\n\n");
        rot.Button = true;
        gpio_acknowledge_irq(gpio, events);
    }
    if(gpio == clk){
       // printf("\n\nInterrupt occured at CLK pin.\n\n");
        rot.Clk = gpio_get(clk);
        gpio_acknowledge_irq(gpio, events);
        rot.a++;
    }
    if(gpio == dt){
      //  printf("\n\nInterrupt occured at DT pin.\n\n");
        rot.Dt = gpio_get(dt);
        gpio_acknowledge_irq(gpio, events);
        rot.b++;
    }
}

int main () {

   stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    uint slice_num[1];

        gpio_init(rgb_r);
            gpio_set_function(rgb_r, GPIO_FUNC_PWM);
            slice_num[1] = pwm_gpio_to_slice_num(rgb_r);
                pwm_set_wrap(slice_num[1], 255);
                pwm_set_chan_level(slice_num[1], PWM_CHAN_B, 0);
                pwm_set_enabled(slice_num[1], true);

        gpio_init(rgb_g);
            gpio_set_function(rgb_g, GPIO_FUNC_PWM);
            slice_num[2] = pwm_gpio_to_slice_num(rgb_g);
                pwm_set_wrap(slice_num[2], 255);
                pwm_set_chan_level(slice_num[2], PWM_CHAN_A, 0);
                pwm_set_enabled(slice_num[2], true);

        gpio_init(rgb_b);
            gpio_set_function(rgb_b, GPIO_FUNC_PWM);
            slice_num[3] = pwm_gpio_to_slice_num(rgb_b);
                pwm_set_wrap(slice_num[3], 255);
                pwm_set_chan_level(slice_num[3], PWM_CHAN_B, 0);
                pwm_set_enabled(slice_num[3], true);

        gpio_init(buzzer);
            gpio_set_function(buzzer, GPIO_FUNC_PWM);
            slice_num[4] = pwm_gpio_to_slice_num(buzzer);
                pwm_set_wrap(slice_num[4], 255);
                pwm_set_chan_level(slice_num[4], PWM_CHAN_A, 0);
                pwm_set_enabled(slice_num[4], true);

        gpio_init(rotEnc_button);
            gpio_set_dir(rotEnc_button, GPIO_IN);
            gpio_pull_down(rotEnc_button);

        gpio_init(clk);
            gpio_set_dir(clk, GPIO_IN);
          //  gpio_pull_up(clk);

        gpio_init(dt);
            gpio_set_dir(dt, GPIO_IN);
          //   gpio_pull_down(dt);


    gpio_set_irq_enabled_with_callback(rotEnc_button, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(clk, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(dt, 0x04 | 0x08, true, &button_callback);

    rot.lastClk = gpio_get(clk);

while(1){

    rot.Clk = gpio_get(clk);

    adc_data_call();

    if(rot.Button == true) {
        rgbColor_Select++;
        rot.Button = false;
    }else {
       // printf("\n\nNo change to color selection.\n\n");
    }

    if(rot.Clk != rot.lastClk) {
        if(gpio_get(dt)!= rot.Clk){
            printf("Rotating clockwise.\n");
            if(rotCounter < 255){
                rotCounter++;
                printf("Counter Incremented?\n\n");
            }else {
                printf("\nAt maximum rotation value.\n\n");
            }
            
        }else {
            printf("Rotating counter clockwise.\n");
            if(rotCounter > 0){
                rotCounter--;
                printf("Counter Decremented?\n\n");
            }else  {
                printf("\nAt minimum rotation value.\n\n");
            }

        } 
        rot.lastClk = rot.Clk;
        rot.Dt = gpio_get(dt);
    }

    printf("Hall sensor one is currently at: %ld.\n", hall.hall_one_pwm);
    printf("Clk: %d. Dt: %d. lastClk: %d. lastDt: %d.\n\n", rot.Clk, rot.Dt, rot.lastClk, rot.lastDt);
    printf("clk: %i.  dt: %i.\n\n", gpio_get(clk), gpio_get(dt));
    printf("Rot Counter: %i.\n\n", rotCounter);
    printf("Color Selected: %i.\n", rgbColor_Select);
    printf("Encode values: %i and %i.\n\n", rot.Clk, rot.lastClk);
    printf("Rotary interrupt amount: clk: %i, dt: %i.\n\n", rot.a, rot.b);
    color_check(rgbColor_Select);

    if(rgbColor_Select == 0) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rotCounter);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, 0);
    }else if(rgbColor_Select == 1) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, 0);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, rotCounter);
    }else if(rgbColor_Select == 2){
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, 0);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rotCounter);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, hall.hall_one_pwm);
    }else if(rgbColor_Select == 3) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, rotCounter);
    }else if(rgbColor_Select == 4) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, 0);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, hall.hall_one_pwm);
        
    }else {

        printf("Error occured within the rotButton function system.\n");
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, hall.hall_one_pwm);
    }
    pwm_set_chan_level(slice_num[4], PWM_CHAN_A, hall.hall_one_pwm);
    sleep_ms(hall.hall_one_pwm);
    
 }
        
tight_loop_contents();
}

