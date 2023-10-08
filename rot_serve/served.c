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

#define rotary_button 0         //  Rotary encoder button pin
#define clk 10                   // Rotary encoder pin clk
#define dt 11                   // Rotary encoder pin dt
#define servod 13
#define motor_drive 14
#define motor_back 15
#define analog_up 26
#define analog_lr 27

    int freq = 0;
    bool up = true;
    bool down = false;
    int motor_speed = 1300; // Adjust as needed
    int duty_cycle;
    uint32_t wrap;


typedef struct anal_data{
    uint16_t vertical;     // Current reading of vertical analog stick data.
    long vert_pwm;         // Result of long map conversion of vertical analog data.
    uint16_t vertLast;
    uint16_t horizontal;   // Current reading of horizontal analog stick data.
    long hori_pwm;         // Result of long map conversion on horizontal analog data.
    volatile bool butt;
} anal_data;
anal_data analog;

void button_callback(uint gpio, uint32_t events) {
 
    if(gpio == rotary_button){
     //   printf("\n\nInterrupt occured at Button.\n\n");
        analog.butt = true;  
    }

}
  
// Function declaration area
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - (in_min)) * (out_max - out_min) / (in_max - (in_min)) + out_min;
}


void adc_data_call() {

    adc_select_input(0);
    analog.vertical = adc_read();
     analog.vert_pwm = map(analog.vertical, 0.f, 4095.0f, 400, 2400);
    
    adc_select_input(1);
    analog.horizontal = adc_read();
    analog.hori_pwm = map(analog.horizontal, 0.f, 4095.0f, 0, 100);

}

void freq_sweep(){
    
    if(up == true){
        //  Maximum value for ping-pong effect is set at < x value.
        if(freq <= 2400){
        freq += 10;
        up = true;
        }else{
            up = false;
        }
    }else {
        down = true;
        //  `Minimum value for the ping-pong effect is set as != x value.
        if(freq != 400){
            freq -= 10;
        }else {
            up = true;
        }
    }

}

void setMillis(int servoPin, float millis){
    pwm_set_gpio_level(servoPin, (millis/20000.f)*39062.f);
}

void setServo(int servoPin, float startMillis){
    gpio_set_function(servoPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servoPin);

    pwm_config config = pwm_get_default_config();
    if(servoPin == servod){
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);
    pwm_init(slice_num, &config, true);
    }
    setMillis(servoPin, startMillis);
    
    
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d){
        uint32_t clock = 125000000;
        uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
        if (divider16 / 16 == 0) {
            divider16 = 16;
            wrap = clock * 16 / divider16 / f - 1;
            pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
            pwm_set_wrap(slice_num, wrap);
            pwm_set_chan_level(slice_num, chan, wrap * d / 100);
        return wrap;
        }
}

void analog_convert(){
    
    if(motor_speed >= 2600){
            printf("At maximum value.\n");
            motor_speed = 2590;
    }else if(motor_speed <= 400){
            printf("At minimum value.\n");
            motor_speed = 410;
    }else if(motor_speed != 400) {
        //  Median value is between 1750 - 1900
            if(analog.vert_pwm <= 1750 && analog.vert_pwm >= 1250){
                motor_speed += 10;
            }else if(analog.vert_pwm <= 1249 && analog.vert_pwm >= 750){
                motor_speed += 25;
            }else if(analog.vert_pwm <= 749){
                motor_speed += 50;
            }else if(analog.vert_pwm >= 1900 && analog.vert_pwm <= 2100){
                motor_speed -= 10;
            }else if(analog.vert_pwm >= 2101 && analog.vert_pwm <= 2300){
                motor_speed -= 25;
            }else if(analog.vert_pwm >= 2301){
                motor_speed -= 50;
            }
    }else {
        printf("What.. Figure out this logic.\n");
    }
}


void motor_direction(){
    duty_cycle = ((motor_speed - 1.5) / (3.7 - 1.5) * 100);
    if(analog.vertical >= 3500){
        pwm_set_chan_level(motor_drive, PWM_CHAN_A, duty_cycle);
        printf("\nMotor forward.\n");
    }else if(analog.vertical <= 2000){
        pwm_set_chan_level(motor_back, PWM_CHAN_B, duty_cycle);
        printf("\nMotor reverse.\n");
    }else {
        duty_cycle = 0;
    }
}
 
int main () {
    
   stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    adc_select_input(0);
    analog.vertical = adc_read();
     analog.vert_pwm = map(analog.vertical, 25, 2047, 400, 2400);

    setServo(servod, analog.vert_pwm);
   
        gpio_init(rotary_button);
            gpio_set_dir(rotary_button, GPIO_IN);
            gpio_pull_up(rotary_button);

       gpio_set_function(motor_drive, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor_drive);
    uint chan = pwm_gpio_to_channel(motor_drive);
    pwm_set_freq_duty(slice_num, chan, 5, 50);
    pwm_set_enabled(slice_num, true);

       gpio_set_function(motor_back, GPIO_FUNC_PWM);
        slice_num = pwm_gpio_to_slice_num(motor_back);
        chan = pwm_gpio_to_channel(motor_back);
    pwm_set_freq_duty(slice_num, chan, 5, 50);
    pwm_set_enabled(slice_num, true);

    gpio_set_irq_enabled_with_callback(rotary_button, 0x04 | 0x08, true, &button_callback);


while(1){

    adc_data_call();
    if(analog.butt == true) {
        
        printf("Hehe, that tickled.\n\n");
        motor_direction();

        analog.butt = false;
    }

    freq_sweep();
    analog_convert();

    setMillis(servod, motor_speed);

    

   // printf("\nFrequency of shift_clk pwm signal: %i\n\n", freq);
   printf("Analog read- Vertical: %i.  Horizontal: %i.\n", analog.vertical, analog.horizontal);
    printf("\n\nServo set to angle: %d degrees\n", analog.vert_pwm);
    printf("Horizontal pwm value: %d.\n", analog.hori_pwm);
    printf("Vertical pwm value: %d.\n", analog.vert_pwm);
    printf("Motor speed: %d.\n", motor_speed);
    printf("\n\n Wrap speed is: %ld.\n\n", wrap);
    sleep_ms(10);
 }
        
tight_loop_contents();
}

