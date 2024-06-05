#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define UNDEFINED -1

typedef struct motor_config_e {
    uint8_t motor_one_pwm_signal_pos;
    uint8_t motor_one_pwm_signal_neg;
    uint8_t motor_two_pwm_signal_pos;
    uint8_t motor_two_pwm_signal_neg;
    uint8_t motor_slices[4];

    float motor_one_clkdiv;
    float motor_one_wrap;

    float motor_two_clkdiv;
    float motor_two_wrap;

    float motor_one_init_pos;
    float motor_two_init_pos;

    float dual_motor_init_clkdiv;
    float dual_motor_init_wrap;
}motor_pwm;

void pwm_pin_func_setup(motor_pwm *pins);
void setMotorOne_Pos(motor_pwm *dc, float millsec);
void setMotorOne_Neg(motor_pwm *dc, float millsec);
void setMotorTwo_Pos(motor_pwm *dc, float millsec);
void setMotorTwo_Neg(motor_pwm *dc, float millsec);
void setMillis(int servoPin, float millis);
void setServo(motor_pwm *c);

void pwm_pin_func_setup(motor_pwm *pins){

  if(pins->motor_one_pwm_signal_pos != UNDEFINED){
    gpio_set_function(pins->motor_one_pwm_signal_pos, GPIO_FUNC_PWM);
    pins->motor_slices[0] = pwm_gpio_to_slice_num(pins->motor_one_pwm_signal_pos);
  }
  if(pins->motor_one_pwm_signal_neg != UNDEFINED){
    gpio_set_function(pins->motor_one_pwm_signal_neg, GPIO_FUNC_PWM);
    pins->motor_slices[1] = pwm_gpio_to_slice_num(pins->motor_one_pwm_signal_neg);
  }

  if(pins->motor_two_pwm_signal_pos != UNDEFINED){
    gpio_set_function(pins->motor_two_pwm_signal_pos, GPIO_FUNC_PWM);
    pins->motor_slices[2] = pwm_gpio_to_slice_num(pins->motor_two_pwm_signal_pos);
  }
  if(pins->motor_two_pwm_signal_neg != UNDEFINED){
    gpio_set_function(pins->motor_two_pwm_signal_neg, GPIO_FUNC_PWM);
    pins->motor_slices[3] = pwm_gpio_to_slice_num(pins->motor_two_pwm_signal_neg);
  }
  
}

void setMotorOne_Pos(motor_pwm *dc, float millsec){
    setMillis(dc->motor_one_pwm_signal_pos, millsec);
    setMillis(dc->motor_one_pwm_signal_neg, 0);
}

void setMotorOne_Neg(motor_pwm *dc, float millsec){
    setMillis(dc->motor_one_pwm_signal_neg, millsec);
    setMillis(dc->motor_one_pwm_signal_pos, 0);
}

void setMotorTwo_Pos(motor_pwm *dc, float millsec){
    setMillis(dc->motor_two_pwm_signal_pos, millsec);
    setMillis(dc->motor_two_pwm_signal_neg, 0);
}

void setMotorTwo_Neg(motor_pwm *dc, float millsec){
    setMillis(dc->motor_two_pwm_signal_neg, millsec);
    setMillis(dc->motor_two_pwm_signal_pos, 0);
}

void setMillis(int servoPin, float millis){
    pwm_set_gpio_level(servoPin, (millis/20000.f)*39062.f);
}

void setServo(motor_pwm *c){

    pwm_pin_func_setup(c);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, c->dual_motor_init_clkdiv);
    pwm_config_set_wrap(&config, c->dual_motor_init_wrap);
    pwm_init(c->motor_slices[0], &config, true);
    pwm_init(c->motor_slices[1], &config, true);
    pwm_init(c->motor_slices[2], &config, true);
    pwm_init(c->motor_slices[3], &config, true);

    setMillis(c->motor_one_pwm_signal_pos, c->motor_one_init_pos);
    setMillis(c->motor_one_pwm_signal_neg, 0);
    setMillis(c->motor_two_pwm_signal_pos, c->motor_two_init_pos);
    setMillis(c->motor_two_pwm_signal_neg, 0);
       
}

int test_laps;
bool forward_test, reverse_test;

int main(){

    stdio_init_all();
        sleep_ms(7000);
    test_laps = 0;
    forward_test, reverse_test = false;

    motor_pwm my_motors = {
        .motor_one_pwm_signal_pos = 0,
        .motor_one_pwm_signal_neg = 1,
        .motor_two_pwm_signal_pos = 2,
        .motor_two_pwm_signal_neg = 3,

        .motor_one_init_pos = 1500.f,
        .motor_two_init_pos = 2200.f,

        .dual_motor_init_clkdiv = 64.f,
        .dual_motor_init_wrap = 39062.f
    };

    setServo(&my_motors);

while(1){

    sleep_ms(2000);
    printf("Testing PWM.\n");


    if(test_laps >= 15){

        if(test_laps % 9 > 5){
            forward_test = true;
            reverse_test = false;
        }else{
            forward_test = false;
            reverse_test = true;
        }

        if(forward_test){

            printf("Forward Testing.\n\n");
        setMotorOne_Pos(&my_motors, 18000);
        setMotorTwo_Pos(&my_motors, 2300);
            printf("M_One Set as: 18000.\nM_Two Set as: 2300.\n");
        }else if(reverse_test){

            printf("Reverse Testing.\n\n");
        setMotorOne_Neg(&my_motors, 5000);
        setMotorTwo_Neg(&my_motors, 460);
            printf("M_One Set as: 5000.\nM_Two Set as: 460.\n");
        }else {
            printf("Test_laps Error at: %i.\n", test_laps);
        }

        test_laps++;
        printf("Test Laps: %i.\n", test_laps);
        printf("Test modulo: %i.\n\n", (test_laps % 5));
    }else{
    test_laps++;
    }
}
}