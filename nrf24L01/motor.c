#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "nrf24_driver.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "hardware/spi.h"


//  Poteniometer pin is ADC pin 26
#define speed_pot_pin 28
#define analog_stick_vertical 26
#define analog_stick_horizontial 27

//  Pins for motor controls are set as pin 0 and 1
#define motor_forward 6
#define motor_reverse 7
#define servo_turn 16

// define buzzer and led toggle pins
#define led_toggle 8
#define buzzer 9

volatile bool buzzer_sound;
volatile bool toggle_led;
volatile bool vertical;
volatile bool horizontal;

int currentMillis = 400;


/*
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
*/

typedef struct analog_data{
    uint16_t vertical;
    long vertical_pwm;
    uint16_t horizontal;
    long horizontal_pwm;

    uint16_t pot_speed;
    long pwm_value;
}analog_Data;
analog_Data data_read;

// Function declaration area
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void adc_interrupt_call() {
        for(int d = 0; d < 3; d++){
          switch(d){
            case 0:
              adc_select_input(0);
              data_read.vertical = adc_read();
              data_read.vertical_pwm = (uint16_t) map(data_read.vertical, 0, 4095, 0, 255);
              break;
            case 1:
              adc_select_input(d);
              data_read.horizontal = adc_read();
              data_read.horizontal_pwm = (uint16_t) map(data_read.horizontal, 0, 4095, 0, 255);
              break;
            case 2:
              adc_select_input(d);
              data_read.pot_speed = adc_read();
              data_read.pwm_value = (uint16_t) map(data_read.pot_speed, 0, 4095, 0, 255);
              break;
            default:
              printf("Error at adc interrupt call.\n");
          }
        }
};

void button_callback(uint gpio, uint32_t events) {

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
    uint slice_num = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);

    pwm_init(slice_num, &config, true);

    set_Degrees(pin, beginMillis);

}

void engage_servo(int pin, float mills) {
        while(true){
          mills += (vertical)?1:-1;
          if(mills >= 2000) {
          vertical = false;
          }else {
            set_Degrees(15, mills);
          }

          if(mills <= 400) {
          vertical = true;
          } else {
            set_Degrees(14, mills);
          }
          sleep_ms(10);
        }
}


int main() {
    int z;
    time_t t;
    srand((unsigned) time(&t));

    stdio_init_all();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(2);
    
    gpio_init(servo_turn);
    gpio_set_dir(servo_turn, GPIO_OUT);

    gpio_init(motor_forward);
    gpio_set_function(motor_forward, GPIO_FUNC_PWM);

    uint8_t slice_num = pwm_gpio_to_slice_num(motor_forward);

    pwm_set_wrap(slice_num, 255);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, true);

    gpio_init(motor_reverse);
    gpio_set_function(motor_reverse, GPIO_FUNC_PWM);

    uint8_t num_slice = pwm_gpio_to_slice_num(motor_reverse);

    pwm_set_wrap(num_slice, 255);
    pwm_set_chan_level(num_slice, PWM_CHAN_A, 0);
    pwm_set_enabled(num_slice, true);

    gpio_init(buzzer);
    gpio_set_dir(buzzer, GPIO_OUT);
    gpio_pull_up(buzzer);

    gpio_init(led_toggle);
    gpio_set_dir(led_toggle, GPIO_OUT);
    gpio_pull_up(led_toggle);

    gpio_set_irq_enabled_with_callback(buzzer, 0x04, 1, &button_callback);
    gpio_set_irq_enabled_with_callback(led_toggle, 0x04, 1, &button_callback);
    adc_irq_set_enabled(true);
    
    nrf_client_t my_nrf;

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

    // SPI baudrate
  uint32_t my_baudrate = 5000000;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise(NULL)) 
  my_nrf.initialise(&my_config);

  // set to Standby-I Mode
  my_nrf.standby_mode();
    
  // payload sent to receiver data pipe 0
  uint8_t payload_zero = data_read.pot_speed;        //  SET to data_read.${adc(potentiometer pin)}

  // payload sent to receiver data pipe 1
  uint8_t payload_one = data_read.pwm_value;       //  SET TO data_read.pwm_value

  // result of packet transmission
  fn_status_t success = 0;

  uint64_t time_sent = 0; // time packet was sent
  uint64_t time_reply = 0; // response time after packet sent

    while(1)
    {
        adc_interrupt_call();
        engage_servo(16, data_read.pwm_value);

    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_0 address
    success = my_nrf.send_packet(&payload_zero, sizeof(payload_zero));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %d\n", time_reply - time_sent, payload_zero);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    sleep_ms(30);

    // send to receiver's DATA_PIPE_1 address
    my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_1 address
    success = my_nrf.send_packet(&payload_one, sizeof(payload_one));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %s\n", time_reply - time_sent, payload_one);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    /*

    // send to receiver's DATA_PIPE_2 address
    my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_2 address
    success = my_nrf.send_packet(&payload_two, sizeof(payload_two));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %d & %d\n",time_reply - time_sent, payload_two.one, payload_two.two);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    sleep_ms(3000);
  }
  */
  
        if(vertical == true){

            printf("Motor forward value: %l.\n", data_read.vertical);
            buzzbuzz();
            while(data_read.vertical != 0) {
                if(data_read.vertical > 0){
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, data_read.vertical_pwm);
                    printf("Driving forward.\n");
                    printf("Pin number 12 firing confirmation.");
                }else if(data_read.vertical < 0) {
                    pwm_set_chan_level(num_slice, PWM_CHAN_A, data_read.vertical_pwm);
                    printf("Reversing.\n");
                    printf("Pin number 13 firing confirmation.");
                }

                }
                    vertical = false;
        }else {
                if(vertical && horizontal == false) {
                printf("Ideling...\n\n");
                }
            }


        tight_loop_contents();
    }
}