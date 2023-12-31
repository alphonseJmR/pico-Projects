#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf24_driver.h"
/*
#include "hardware/gpio.h"
#include "hardware/spi.h"
*/
#include "hardware/pwm.h"
#include "hardware/adc.h"

//  Poteniometer pin is ADC pin 26
//  Analog stick vertical is 26, and horizontal is 27.
#define speed_pot_pin 28
#define analog_vertical 26
#define analog_horizontial 27

//  Pins for motor controls are set as pin 14 and 15
#define motor_drive 14
#define motor_reverse 15
#define servo_turn 16

// define buzzer and led toggle pins
#define led_toggle 8
#define buzzer 9

volatile bool buzzer_sound;
volatile bool toggle_led;

int currentMillis = 400;

typedef struct chan_level_data {
    
    uint8_t chan_0;       //  Analog stick left vertical input data
    uint8_t chan_1;       //  Analog stick left horizontal input data
    uint8_t chan_2;       //  Analog stick right vertical input
    uint8_t chan_3;       //  Analog stick right horizontal input data
    uint8_t chan_4;       //  
    uint8_t chan_5;
    uint8_t chan_6;
    uint8_t chan_7;

} chan_level_data;
  chan_level_data Data_Send;

typedef struct analog_data{
    uint16_t vertical;
    long v_pwm;
    uint16_t horizontal;
    long h_pwm;
    uint16_t anal_sensor;
    long anal_result;
}analog_Data;
analog_Data data_read;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function declaration area
void adc_value_read() {

    adc_select_input(1);
    data_read.vertical = adc_read();
    data_read.v_pwm = (map(data_read.vertical, 0, 4095, 0, 2047));
    adc_select_input(2);
    data_read.horizontal = adc_read();
    data_read.h_pwm = (map(data_read.horizontal, 0, 4095, 0, 2047));
    adc_select_input(0);
    data_read.anal_sensor = adc_read();
    data_read.anal_result = (map(data_read.anal_sensor, 0, 4095, 0, 2047));

    sleep_ms(50);

}


void button_callback(uint8_t gpio, uint32_t events) {

        printf("Interrupt occured at pin %d, with event %d\n", gpio, events);

        if(gpio_get(buzzer) != 1) {

                buzzer_sound = true;

        } else if(gpio_get(led_toggle) != 1) {
    
                toggle_led = true;

        }else {

            printf("No current interrupts.\n");

        }
}

void set_Degrees(int pin, float millis) {
    pwm_set_gpio_level(pin, (millis/20000.f)*39062.f);
}

void set_servo(int pin, float beginMillis) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    int slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);
    pwm_init(slice_num, &config, true);

    set_Degrees(pin, beginMillis);

}

engage_servo(int pin, float mills) {
        while(true){
          mills += (data_read.vertical)?1:-1;
          if(mills >= 2000) {
          }else {
            set_Degrees(15, mills);
          }

          if(mills <= 400) {
          } else {
            set_Degrees(14, mills);
          }
          sleep_ms(10);
        }
}


int main() {

    stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    gpio_init(servo_turn);
    gpio_set_dir(servo_turn, GPIO_OUT);

    gpio_init(motor_drive);
    gpio_set_function(motor_drive, GPIO_FUNC_PWM);

    uint8_t slice_num[1];

     slice_num[0]= pwm_gpio_to_slice_num(motor_drive);

    pwm_set_wrap(slice_num[0], 63);
    pwm_set_chan_level(slice_num[0], PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num[0], true);

    gpio_init(motor_reverse);
    gpio_set_function(motor_reverse, GPIO_FUNC_PWM);

    slice_num[1] = pwm_gpio_to_slice_num(motor_reverse);

    pwm_set_wrap(slice_num[1], 63);
    pwm_set_chan_level(slice_num[1], PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num[1], true);

    gpio_init(buzzer);
    gpio_set_dir(buzzer, GPIO_OUT);
    gpio_pull_up(buzzer);

    gpio_init(led_toggle);
    gpio_set_dir(led_toggle, GPIO_OUT);
    gpio_pull_up(led_toggle);

    gpio_set_irq_enabled_with_callback(buzzer, 0x04, 1, &button_callback);
    gpio_set_irq_enabled_with_callback(led_toggle, 0x04, 1, &button_callback);
    adc_irq_set_enabled(true);

    Data_Send.chan_0 = data_read.vertical;
    Data_Send.chan_1 = data_read.horizontal;
    Data_Send.chan_2 = data_read.anal_sensor;
    
    pin_manager_t my_pins = {

        .mosi = 11,
        .miso = 12,
        .csn = 13,
        .sck = 14,
        .ce = 15

};

    nrf_manager_t my_config = {
    // RF Channel 
    .channel = 120,

    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,

    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,

    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_1MBPS,

    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_12DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US 

    };
    for(int b = 0; b < 7; b++) {
    // SPI baudrate
  uint32_t my_baudrate = 5000000;

  nrf_client_t my_nrf;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise(NULL)) 
  my_nrf.initialise(&my_config);

  // set to Standby-I Mode
  my_nrf.standby_mode();
    
  // payload sent to receiver data pipe 0
  uint8_t payload_zero =   Data_Send.chan_0;            //  SET TO Data_Send(chan_#)

  // payload sent to receiver data pipe 1
  uint8_t payload_one =  Data_Send.chan_1;           //  SET TO Data_Send(chan_#)

  // payload sent to reveiver data pipe 2
  uint8_t payload_two = Data_Send.chan_2;

  // result of packet transmission
  fn_status_t success = 0;

  uint64_t time_sent = 0; // time packet was sent
  uint64_t time_reply = 0; // response time after packet sent
}
 
    while(1)
    {
        adc_call();
        engage_servo(16, adc_call());
  
            printf("Motor forward value: %l.\n", data_read.vertical);
            buzzbuzz();
        if(data_read.vertical < 3700 && data_read.vertical > 3000){

            printf("Currently Ideling.\n");
        }else {

            if(data_read.vertical < 3000){
                pwm_set_chan_level(slice_num[0], PWM_CHAN_A, data_read.anal_result);
                printf("Driving forward.\n");
                printf("Pin number 12 firing confirmation.");
            }else if(data_read.vertical > 3800) {
                pwm_set_chan_level(slice_num[1], PWM_CHAN_A, data_read.anal_result);
                printf("Reversing.\n");
                printf("Pin number 13 firing confirmation.");
            }

            }
    }
        tight_loop_contents();
}