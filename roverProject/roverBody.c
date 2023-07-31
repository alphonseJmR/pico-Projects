#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"


//  Potentiometer pin is ADC pin 26
#define speed_pot_pin 26

//  Pins for motor controls are set as pin 0 and pin 1

#define motor_forward 0
#define motor_reverse 1

//  Define sensor i/o pins (interrupt's will occur on these pins);
#define ir_drive_sensor_port_one 14
#define ir_drive_sensor_port_two 15

//  Define buzzer pin
#define buzzer 2

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

volatile bool motor_go;
volatile bool motor_back;

void button_callback(uint gpio, uint32_t events) {
    printf("Interrupt occured at pin %d with event %d", gpio, events);

    if(gpio_get(motor_forward) == 0) {

        motor_go = true;
    
    } else if(gpio_get(motor_reverse) == 0) {

      motor_back = true;
    }
}

/*

bool repeating_timer_callback(struct repeating_timer *t) {
gpio_put(buzzer, 1);
    sleep_ms(40);
gpio_put(buzzer, 0);

}

*/

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio(speed_pot_pin);
    adc_select_input(0);

    gpio_set_function(motor_forward, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor_forward);

    pwm_set_wrap(slice_num, 1023);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);

    pwm_set_enabled(slice_num, true);

    gpio_set_function(motor_reverse, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(motor_reverse);
    uint channel = pwm_gpio_to_channel(motor_reverse);

    pwm_set_wrap(slice, 1023);
    pwm_set_channel_level(slice, PWM_CHAN_B, 0);

    pwm_set_enabled(slice, true);

    gpio_init(ir_drive_sensor_port_one);
    gpio_set_dir(ir_drive_sensor_port_one, GPIO_IN);
    gpio_pull_up(ir_drive_sensor_port_one);

    gpio_init(ir_drive_sensor_port_two);
    gpio_set_dir(ir_drive_sensor_port_two, GPIO_IN);
    gpio_pull_up(ir_drive_sensor_port_two);
    
    gpio_set_irq_enabled_with_callback(ir_drive_sensor_port_one, 0x04, 1, &button_callback);
    gpio_set_irq_enabled_with_callback(ir_drive_sensor_port_two, 0x04, 1, &button_callback);
    

    //  struct repeating_timer *timer{
    //  add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
    //  }
while(1){

    if(motor_go == true) {

        while(gpio_get(ir_drive_sensor_port_one) != 1) {

            uint16_t result = adc_read();
            long pwm_value = map(result, 0, 4095, 0, 127);

            pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_value);
            sleep_ms(50);   
    }

    motor_go = false;

    }
    else if(motor_back == true) {

        while(gpio_get(ir_drive_sensor_port_two) != 1) {

            uint16_t results = adc_read();
            long pwm_values = map(results, 0, 4095, 0, 127);

            pwm_set_chan_level(slice, PWM_CHAN_B, pwm_values);
            sleep_ms(50);

    }

    motor_back = false;

    }
    tight_loop_contents();
}
}