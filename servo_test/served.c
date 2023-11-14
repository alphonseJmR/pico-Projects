#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define servod 13

int freq = 410;
bool up = true;
bool down = false;

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
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);

    pwm_init(slice_num, &config, true);

    setMillis(servoPin, startMillis);
}
 
int main () {

   stdio_init_all();
setServo(servod, 600);

while(1){


    freq_sweep();
    setMillis(servod, freq);
    printf("Servo set to angle: %d degrees\n", freq);
    sleep_ms(10);
 }
        
tight_loop_contents();
}

