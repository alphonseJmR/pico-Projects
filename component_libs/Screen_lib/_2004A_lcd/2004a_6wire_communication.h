#ifndef _2004A_LCD_6WIRE_COMMUNICATION_S_H
#define _2004A_LCD_6WIRE_COMMUNICATION_S_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "_2004A_commands.h"
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


void init_lcd_pins(lcd_pins *pins){

    gpio_init(pins->RS);
    gpio_set_dir(pins->RS, GPIO_OUT);
        sleep_ms(50);

    gpio_init(pins->Enable);
    gpio_set_dir(pins->Enable, GPIO_OUT);
        sleep_ms(50);

    gpio_init(pins->A);
    gpio_set_dir(pins->A, GPIO_OUT);
        sleep_ms(50);

    gpio_init(pins->B);
    gpio_set_dir(pins->B, GPIO_OUT);
        sleep_ms(50);

    gpio_init(pins->C);
    gpio_set_dir(pins->C, GPIO_OUT);
        sleep_ms(50);

    gpio_init(pins->D);
    gpio_set_dir(pins->D, GPIO_OUT);
        sleep_ms(50);

}


uint8_t flip_it(uint8_t in){
    uint8_t buf;
    buf = 0;

    buf += (in & 0x01) ? 0x80 : 0x00;
    buf += (in & 0x02) ? 0x40 : 0x00;
    buf += (in & 0x04) ? 0x20 : 0x00;
    buf += (in & 0x08) ? 0x10 : 0x00;
    buf += (in & 0x10) ? 0x08 : 0x00;
    buf += (in & 0x20) ? 0x04 : 0x00;
    buf += (in & 0x40) ? 0x02 : 0x00;
    buf += (in & 0x80) ? 0x01 : 0x00;

    return buf;
}


void enabled_toggle(lcd_pins *pins){

    e_high(pins);
        sleep_us(10);
    e_low(pins);

}


void lcd_nibble(lcd_pins *pins, uint8_t input_nib){

    printf("LCD_Nibble_-> 0x%02x.\n", input_nib);

    printf("Nibble: 0x%02x\n\n", (input_nib & 0x0F));

    gpio_put(pins->A, (input_nib & 0x01) >> 0);
    gpio_put(pins->B, (input_nib & 0x02) >> 1);
    gpio_put(pins->C, (input_nib & 0x04) >> 2);
    gpio_put(pins->D, (input_nib & 0x08) >> 3);

        sleep_us(20);

}


void lcd_byte(lcd_pins *pins, uint8_t byte_input){

    printf("LCD_Byte_Write->");

    uint8_t lsb, msb;
    lsb, msb = 0x00;

    if(byte_input >= 0x00 && byte_input <= 0xFF){

    lsb = (byte_input & 0x0F);
    msb = ((byte_input >> 4) & 0x0F);
    
    }else {
        printf("\nLCD Input Byte Out of Range.\n\n");
        lsb = 0x00;
        msb = 0x00;
    }
    printf("LCD_byte: 0x%02x.\n", byte_input);

    e_high(pins);
        sleep_us(10);
    lcd_nibble(pins, (msb & 0x0F));
    printf("msb: 0x%02x.\n", msb);
        sleep_us(10);
    e_low(pins);

    sleep_us(10);
    e_high(pins);
        sleep_us(10);
    lcd_nibble(pins, (lsb & 0x0F));
    printf("lsb: 0x%02x.\n", lsb);
        sleep_us(10);
    e_low(pins);
        sleep_us(10);

}


void lcd_command(lcd_pins *pins, uint8_t command){

    printf("LCD_Write_Command->");

  gpio_put(pins->RS, 0);
    sleep_us(10);

    lcd_byte(pins, command);
}


void lcd_character(lcd_pins *pins, uint8_t character){

    printf("LCD_Write_Character->");

  gpio_put(pins->RS, 1);
    sleep_us(5);

    lcd_byte(pins, character);

}


void lcd_clear_display(lcd_pins *pins){

    lcd_command(pins, 0x01);
        sleep_us(20);

}


void lcd_set_cursor(lcd_pins *pins, uint8_t line, uint8_t position){

    uint8_t output_position;
    output_position = 0x00;

    uint8_t offset_lines[] = {0x00, 0x40, 0x14, 0x54};
    output_position = (LCD_SETDDRAMADDR + ( offset_lines[line] + position ));

    lcd_command(pins, output_position);
        sleep_us(500);
}


void lcd_write_line(lcd_pins *pins, uint8_t line, char *string){

    lcd_set_cursor(pins, line, 0);
    printf("LCD_Write_Line->");

    for(int i = 0; i < 20; i++){

        lcd_character(pins, string[i]);
        
    }

}


void lcd_write_display(lcd_pins *pins, char *ln_on, char *ln_tw, char *ln_th, char *ln_fo){

    lcd_clear_display(pins);

    lcd_write_line(pins, 0, ln_on);
        sleep_ms(5);
    lcd_write_line(pins, 1, ln_tw);
        sleep_ms(5);
    lcd_write_line(pins, 2, ln_th);
        sleep_ms(5);
    lcd_write_line(pins, 3, ln_fo);

}


void lcd_default_display(lcd_pins *pins, uint16_t i_o, uint16_t i_tw, uint16_t i_th, uint8_t i_co){

    char buf_one[20], buf_two[20], buf_thr[20], buf_for[20];

  sprintf(buf_one, "__RC Cont: Active__");
  sprintf(buf_two, "Ver: %04x Hor: %04x", i_o, i_tw);
  sprintf(buf_thr, "VRoom Amount: %04x ", i_th);
  sprintf(buf_for, "FL: %02x RL: %02x ", (i_co & 0xF0), (i_co & 0x0F));

    lcd_write_display(pins, buf_one, buf_two, buf_thr, buf_for);
}


void lcd_init_sequence(lcd_pins *pins){

    printf("\n////\tInitialiasing 2004A LCD Screen\t////\n");

        sleep_ms(20);
    lcd_command(pins, (0x02 | 0x80));
      printf("Command Hex: 0x28\n");
        sleep_ms(20);
    lcd_command(pins, (0x08 | 0x04));
      printf("Command Hex: 0xC0\n");
        sleep_ms(20);
    lcd_command(pins, (0x04 | 0x02));
      printf("Command Hex: 0x60\n");
        sleep_ms(20);

}


void lcd_fill_screen(lcd_pins *pins, uint8_t spec_char){

    lcd_clear_display(pins);
        sleep_us(20);
    lcd_set_cursor(pins, 1, 0);
    for(int i = 0; i < 40; i++){

      lcd_character(pins, spec_char);
        sleep_us(20);
      lcd_byte(pins, (spec_char - 0x04));
      sleep_us(20);
    }

}

#endif