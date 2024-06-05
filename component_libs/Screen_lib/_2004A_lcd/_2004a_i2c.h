#ifndef _2004A_I2C_DRIVER_H
#define _2004A_I2C_DRIVER_H

#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "2004a_error_management.h"
#include "_2004A_commands.h"

#define ebit uint8_t
#define lcd_delay 600
static uint8_t ADDR = 0x27;  // I2C address for 2004A LCD screen.


typedef struct lcd_pins_s {

    uint SDA;
    uint SCK;

}lcd_pins;

lcd_pins my_pins = {

    .SDA = 11,
    .SCK = 10

};

typedef struct i2c_screen_s {
    
    lcd_pins *pins;
   struct i2c_inst *inst;
    uint baud;

}lcd;

lcd my_lcd = {

    //  Will add function later to check input pins and automatically set i2c0/1.

    .pins = &my_pins,
    .inst = i2c1,
    .baud = (100 * 1000)

};


func_akk write_byte(ebit value){

    int write_return;
    write_return = 0;

   // printf("Writing Byte: 0x%02x.\n", value);

    write_return = i2c_write_blocking(my_lcd.inst, ADDR, &value, 1, false);
   // printf("Value of i2c_write_blocking return: %i.\n", write_return);

    return device_data_success;

}

func_akk lcd_ENABLE_toggle(ebit value){

    ebit s;
    s = 0;

    sleep_us(lcd_delay);
        s += write_byte(value | LCD_ENABLE_BIT);
    sleep_us(lcd_delay);
        s += write_byte(value & ~LCD_ENABLE_BIT);
    sleep_us(lcd_delay);

    return (s == 4) ? byte_write_success : err;
}


func_akk send_lcd_byte(ebit value, ebit com_dat, ebit backlight){
    ebit high, low, s;
    s = 0;

    if(backlight){

        high = com_dat | (value & 0xF0) | LCD_BACKLIGHT;
        low = com_dat | ((value << 4) & 0xF0) | LCD_BACKLIGHT;

    }else{

        high = com_dat | (value & 0xF0);
        low = com_dat | ((value << 4) & 0xF0);

    }

    s += write_byte(high);
      s += lcd_ENABLE_toggle(high);
    s +=  write_byte(low);
      s += lcd_ENABLE_toggle(low);

    return (s == 8) ? byte_write_success : err;

}

func_akk clear_lcd(void){
    ebit s;
    s = 0;
    send_lcd_byte(LCD_CLEARDISPLAY, LCD_COMMAND, 1);

    return (s == 2) ? byte_write_success : err;
}

func_akk set_lcd_cursor(ebit line, ebit pos){
    ebit s;
    s = 0;
    ebit line_offset[] = { 0x00, 0x40, 0x14, 0x54 };
     ebit val = 0x80 + line_offset[line] + pos;
    s += send_lcd_byte(val, LCD_COMMAND, 1);
    ebit status;

    return (s > 0) ? device_command_success : err;
}

func_akk write_lcd(char value){
    ebit s;
    s = 0;

    s = send_lcd_byte(value, LCD_CHARACTER, 1);

    return (s == 2) ? byte_write_success : err;
}

func_akk print_lcd(const char *s){

    printf("Attempting to print: '%s'.\n", s);
    ebit sa;
    sa = 0;
    while(*s){
     //   printf("Printing: 0x%02x.\n", *s);
       sa += write_lcd(*s++);
        sleep_ms(2);
    }
    return (sa > 0) ? line_write_success : err;
}

func_akk create_lcdChar(ebit location, ebit c_map[]){
    ebit s;
    s = 0;
    location &= 0x7;
    s += send_lcd_byte(LCD_SETCGRAMADDR | (location << 3), LCD_COMMAND, 1);
        for(int i = 0; i < 8; i++){

          s+= send_lcd_byte(c_map[i], 1, 0);
        }
    return (s > 0) ? device_command_success : err;
}

func_akk F_lcd_home(){ebit s;s=0;s+=set_lcd_cursor(0, 0);return(s>0)?byte_write_success:err;}

func_akk lcd_pin_init(lcd_pins *pins){

    printf("Initializing Pins:\n\tSDA: %i\n\tSCK: %i\n\n", pins->SDA, pins->SCK);

    ebit s;
    s = 0;
    gpio_set_function(pins->SDA, GPIO_FUNC_I2C);
    s+=1;
    gpio_set_function(pins->SCK, GPIO_FUNC_I2C);
    s+=1;
    gpio_pull_up(pins->SDA);
    s+=1;
    gpio_pull_up(pins->SCK);
    s+=1;

    printf("Pins Initialized With: %i.\n", s);

    return (s == 4) ? pin_init : err;

}

//  Should return 1 if correctly initialized.
func_akk initialize_lcd(lcd *init){

    ebit s, f;
    uint32_t bau;
    s, f = 0;
    printf("Initiating I2C%i, with baudrate of: %i.\n", (init->inst == i2c1) ? 1 : 0, init->baud);
    bau = i2c_init(init->inst, init->baud);
    printf("I2C Baud Rate: %i.\n", bau);
    s += lcd_pin_init(init->pins);
        sleep_ms(4000);

    if(s == 3){
    s = 0;
    
    s = send_lcd_byte(0x03, LCD_COMMAND, 1);
        f += s;
    s = send_lcd_byte(0x03, LCD_COMMAND, 1);
        f += s;
    s = send_lcd_byte(0x03, LCD_COMMAND, 1);
        f += s;


    s = send_lcd_byte(0x02, LCD_COMMAND, 1);
        f += s;


    s = send_lcd_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND, 1);
        f += s;
    s = send_lcd_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND, 1);
        f += s;
    s = clear_lcd();
        f += s;
    s = send_lcd_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND, 1);
        f += s;

    printf("F status: %i.\n", f);

        return ((f == 12) || (f == 14)) ? device_init : err;

    }else {
        printf("Error Setting up I2C Pins.\n");
    }


}
#endif