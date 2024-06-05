#ifndef _2004A_LCD_6WIRE_COMMUNICATION_S_H
#define _2004A_LCD_6WIRE_COMMUNICATION_S_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "stdlib.h"
#include "hardware/gpio.h"

typedef struct lcd_communication_pins {

    uint8_t RS;
    uint8_t Enable;

    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;

}lcd_pins;

//  RS_COM is conditional to pulling RS LOW
void rs_com(lcd_pins *rs){

    gpio_put(rs->RS, 0);

}

//  RS_CHAR is conditional to pulling RS HIGH
void rs_char(lcd_pins *rs){

    gpio_put(rs->RS, 1);
}

//  Pull Enable HIGH.
void e_high(lcd_pins *e){

    gpio_put(e->Enable, 1);

}

//  Pull Enable LOW.
void e_low(lcd_pins *e){

    gpio_put(e->Enable, 0);

}


void lcd_nibble(lcd_pins *pins, uint8_t input_nib){

    e_high(pins);
        sleep_us(20);

    gpio_put(pins->A, (input_nib & 0x01));
    gpio_put(pins->B, (input_nib & 0x02));
    gpio_put(pins->C, (input_nib & 0x04));
    gpio_put(pins->D, (input_nib & 0x08));

        sleep_us(20);
    e_low(pins);
}


void lcd_byte(lcd_pins *pins, uint8_t byte_input){

    uint8_t lsb, msb;
    lsb, msb = 0x00;

    lsb = (byte_input & 0x0F);
    msb = ((byte_input & 0xF0) >> 4);

    lcd_nibble(pins, lsb);
        sleep_ms(10);



}

#endif