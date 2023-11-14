#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "pico/time.h"

#define hall_sensor_one 26      // Set gpio pin for hall effect sensor one.

#define clk 2                   // Rotary encoder pin clk
#define dt 4                   // Rotary encoder pin dt
#define rotary_button 0         //  Rotary encoder button pin
#define buzzer 3

#define rgb_r 13
#define rgb_g 14
#define rgb_b 15

    int freq = 0;
    bool up = true;
    bool down = false;


int rgbColor_Select = 6;          //  Current position of rgb led pin output.
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
    uint32_t lastInterruptTime;
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
    hall.hall_one_pwm = map(hall.hall_one, 4095, 0, 255, 0);
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
        if(num > 6){

        rgbColor_Select = 0;
    } 
}

void button_callback(uint gpio, uint32_t events) {
    uint32_t currentTime = time_us_32();
    uint32_t timeSinceLastInterrupt = currentTime - rot.lastInterruptTime;

    if(gpio == rotary_button){
     //   printf("\n\nInterrupt occured at Button.\n\n");
        rot.Button = true;  
    }

    //  The > # is the debounce threshold.  It is measured in uS.
    //  8/20 previous interrupt time set as: 300.  Current best time:  250:
    if (timeSinceLastInterrupt > 205) {
        rot.lastInterruptTime = currentTime;
        gpio_acknowledge_irq(gpio, events);

    if(gpio == clk) {
        rot.Clk = gpio_get(clk);
        rot.Dt = gpio_get(dt);
        if(!rot.Clk && rot.Dt){
            if(rot.Clk != rot.lastClk){
                rot.a++;
                if(rotCounter < 8) {
                    rotCounter++;
                }else {
                    printf("At maximum value.\n\n");
                }
            }
        }else {
        if(rot.Dt != rot.lastDt){
            if(rot.Dt && rot.Clk){
                rot.b++;
                if(rotCounter != 0){
                    rotCounter--;
                }else {
                    printf("At minimum value.\n\n");
                }
            }
        }
        }
        rot.lastClk = rot.Clk;
        rot.lastDt = rot.Dt;
    }
  
    }
}


void freq_sweep(){
    
    if(up == true){
        //  Maximum value for ping-pong effect is set at < x value.
        if(freq < 255){
        freq++;
        up = true;
        }else{
            up = false;
        }
    }else {
        down = true;
        //  `Minimum value for the ping-pong effect is set as != x value.
        if(freq != 0){
            freq--;
        }else {
            up = true;
        }
    }

}

 
int main () {

   stdio_init_all();

    adc_init();
    adc_gpio_init(26);
   // adc_gpio_init(27);
   // adc_gpio_init(28);

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


        gpio_init(rotary_button);
            gpio_set_dir(rotary_button, GPIO_IN);
            gpio_pull_up(rotary_button);

        gpio_init(clk);
            gpio_set_dir(clk, GPIO_IN);
            gpio_pull_up(clk);

        gpio_init(dt);
            gpio_set_dir(dt, GPIO_IN);
            gpio_pull_up(dt);


    gpio_set_irq_enabled_with_callback(rotary_button, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(clk, 0x08 | 0x04, true, &button_callback);
    gpio_set_irq_enabled_with_callback(dt, 0x08 | 0x04, true, &button_callback);

    rot.Clk = false;
    rot.lastClk = false;

while(1){

    adc_data_call();

    if(rot.Button == true) {
        rgbColor_Select++;
        rot.Button = false;
    }
    printf("Rot Counter: %i.\n\n", rotCounter);
    printf("Color Selected: %i.\n", rgbColor_Select);
    printf("Current photoResistor adc value: %i.\n", hall.hall_one_pwm);
    color_check(rgbColor_Select);

    freq_sweep();
    printf("\nFrequency of shift_clk pwm signal: %i\n\n", freq);
    

    if(rgbColor_Select == 0) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, hall.hall_one_pwm);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rotCounter);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, 0);
    }else if(rgbColor_Select == 1) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, 0);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, freq);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, rotCounter);
    }else if(rgbColor_Select == 2){
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, freq);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rotCounter);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, 0);
    }else if(rgbColor_Select == 3) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, rotCounter);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, 0);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, freq);
    }else if(rgbColor_Select == 4) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, (hall.hall_one_pwm % 15));
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, (hall.hall_one_pwm % 25));
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, (hall.hall_one_pwm % 50));   
    }else if(rgbColor_Select == 5) {
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, rotCounter);
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, rotCounter);
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, 0);
    }else if(rgbColor_Select == 6){
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, (freq % 15));
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, (freq % 15));
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, (freq % 15));
    }else {

        printf("Error occured within the rotButton function system.\n");
        pwm_set_chan_level(slice_num[1], PWM_CHAN_B, (hall.hall_one_pwm % 15));
        pwm_set_chan_level(slice_num[2], PWM_CHAN_A, (hall.hall_one_pwm % 15));
        pwm_set_chan_level(slice_num[3], PWM_CHAN_B, (hall.hall_one_pwm % 15));
    }
    pwm_set_chan_level(slice_num[4], PWM_CHAN_A, freq);
    sleep_ms(100);

 }
        
tight_loop_contents();
}

