#ifndef RC_CONFIGS_H
#define RC_CONFIGS_H

#include <stdio.h>
#include "nrf24_driver.h"
// #include "dc_motors.h"
#include "peri_header.h"
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "pico/time.h"

#define front_l 0x80
#define rear_l 0x40
#define f_r 0x20
#define f_g 0x10
#define f_b 0x08
#define r_r 0x04
#define r_g 0x02
#define r_b 0x01

//  Contains four sets of uint8, 16, && 32's variables for use in payload buffer function.  
typedef struct buffer_uints {

    uint8_t buffer_one;
    uint16_t buffer_two;
    uint32_t buffer_three;

    uint8_t buffer_four;
    uint16_t buffer_five;
    uint32_t buffer_six;

    uint8_t buffer_seven;
    uint16_t buffer_eight;
    uint32_t buffer_nine;

    uint8_t buffer_ten;
    uint16_t buffer_eleven;
    uint32_t buffer_tweleve;
    
}buffer_uints;
buffer_uints uints;

//  Holds four current "payloads" to be sent via rf pipelines.  Up to six can be used.
typedef struct payloads_t {

    uint8_t payload_zero;

    struct payload_one_t {

      uint16_t vertical_buffer; 
      uint16_t horizontal_buffer;

    }payload_one_t;

    uint8_t payload_two;

    uint16_t payload_three;

}payloads_t;
payloads_t load_o;

typedef struct payload_recieved_buffer_t {

    uint8_t payload_zero_r;

    struct payload_one_buffer_r {

        uint16_t vertical_buffer;
        uint16_t horizontal_buffer;

    }payload_one_buffer_r;

    uint8_t payload_two_r;

    uint16_t payload_three_r;

}payload_recieved_buffer_t;
payload_recieved_buffer_t payload_recieved;

typedef struct payload_active_s {

        uint8_t pay_zero;

    struct pay_one_buffer_s {

        uint16_t vertical_active;
        uint16_t horizontal_active;

    }pay_one_buffer_s;

    uint8_t pay_two;

    uint16_t pay_three;

}payload_active_s;
payload_active_s active_payload;

typedef struct rf_lcd_array_s {

  char rf_channel[16];

  char rf_data[16];
  char rf_data_two[16];

  char top_string[16];
  char bottom_string[16];

 char rf_function[16];
 char rf_transmitting[16];
 char void_char_d[16];
 char error_char_t[16];
 char error_char_b[16];

}rf_lcd_array_s;
rf_lcd_array_s rf_arr;

void initialize_print_debug(nrf_manager_t *config, pin_manager_t *pins, uint32_t baud){
    printf("\nNRF Initializion:\n");

    printf("\nPINS:\n\tSCK: %u\n", pins->sck);
        sleep_ms(100);
    printf("\tMOSI: %u", pins->mosi);
        sleep_ms(100);
    printf("\n\tMISO: %u", pins->miso);
        sleep_ms(100);
    printf("\n\tCSN: %u", pins->csn);
        sleep_ms(100);
    printf("\n\tCE: %u\n", pins->ce);
        sleep_ms(100);
    printf("Channel:\t%u.\n", config->channel);
        sleep_ms(100);
    printf("Address Width:\t%u.\n", config->address_width);
        sleep_ms(100);
    printf("Dynamic Payload: %u.\n", config->dyn_payloads);
        sleep_ms(100);
    printf("Data Rate:\t%u.\n", config->data_rate);
        sleep_ms(100);
    printf("Power Level:\t%u.\n", config->power);
        sleep_ms(100);
    printf("Retry Count:\t%u.\n", config->retr_count);
        sleep_ms(100);
    printf("Retry Delay:\t%u.\n", config->retr_delay);
        sleep_ms(100);
    printf("BAUDRATE:\t%u", baud);
        sleep_ms(100);
}

void rc_light_register_initialize(uint8_variables *reg_value){

    reg_value->register_value_zero = 0x00;

    reg_value->register_value_zero += front_l;
    reg_value->register_value_zero += rear_l;
    reg_value->register_value_zero += f_b;
    reg_value->register_value_zero += r_g;

}

//  Completed
/*
  Rgb_color_cycle function:
    Takes parameters of BOOL light_toggle, BOOL reverse_toggle, INT cycle_count, and INT reverse_count.

    light_toggle is to be used with analog.analog_button_status bool variable.
    cycle_count is to be used with rgb.rgb_cycle_value.

    Initial step, check if light_toggle is true, if 1 -> print previous values rgb bool values.
    Second, check if rgb cycle count variable is less than 6.
    Third, move to switch on cycle_count variable.
    Fourth, per case, change rgb.frontRGB bool values.
*/
void rgb_color_cycle(uint8_variables *rgb_t, int input_value){

        if(input_value < 6){

            switch(input_value){

                case 0:
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += f_g;
                    rgb_t->register_value_zero += 0;
                    
                break;
                case 1:
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += f_b;
                    
                break;
                case 2:
                    rgb_t->register_value_zero += f_r;
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += 0;
                    
                break;
                case 3:
                    rgb_t->register_value_zero += f_r;
                    rgb_t->register_value_zero += f_g;
                    rgb_t->register_value_zero += 0;
                    
                break;
                case 4:
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += f_g;
                    rgb_t->register_value_zero += f_b;
                    
                break;
                case 5:
                    rgb_t->register_value_zero += f_r;
                    rgb_t->register_value_zero += f_g;
                    rgb_t->register_value_zero += f_b;
                    
                break;
                default:
                    rgb_t->register_value_zero += 0;
                    rgb_t->register_value_zero += 0;
                break;
            }
           
        }else {
            printf("Cycle resetting to zero.\n");
            rgb_t->register_value_zero = 0;
        }


    }

void payload_buffer(payloads_t *load, adc_port_values *adc_mapped, rotary_encoder_struct *res, uint8_variables *regs){

    printf("Transmitter Payload Buffer: \n");
    
  //  Payload sending to nrf receiever data pipe 0.
    if(res->rotary_total == 0){
        res->rotary_total = 1;
    }
        load->payload_zero = res->rotary_total;
            printf("Transmitter Data Pipe 0: %04x.\n\n", load->payload_zero);
  
  //  Payload sending to nrf receiver data pipe 1.
        load->payload_one_t.vertical_buffer = adc_mapped->adc1_mapped_value;
        load->payload_one_t.horizontal_buffer = adc_mapped->adc2_mapped_value;
            printf("Transmitter Data Pipe 1 Vertical: %04x.\n", load->payload_one_t.vertical_buffer);
            printf("Transmitter Data Pipe 1 Horizontal: %04x.\n\n", load->payload_one_t.horizontal_buffer);
  //  Payload sending to nrf receiver data pipe 1
        load->payload_two = regs->register_value_zero;
            printf("Transmitter Data Pipe 2: %04x.\n", load->payload_two);

  //  Payload sending to nrf receiver data pipe 1
        load->payload_three = regs->register_value_one;
            printf("Transmitter Data Pipe 3: %04x.\n\n", load->payload_three);

    printf("Transmitter Buffer Complete.\n\n");

    
}

void cycle_handler(uint8_variables *reg_value, button_types *button, int input){
    if(button->rotary_button_button){

        input++;
    }
    rgb_color_cycle(reg_value, input);

    if(button->analog_button_one){

        reg_value->register_value_zero ^= front_l;
    }

    if(pico_adc.adc1_mapped_value >= 140){
        reg_value->register_value_zero ^= rear_l;
    }
}

#endif