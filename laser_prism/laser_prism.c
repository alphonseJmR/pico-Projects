#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
#include "peri_header.h"
#include "hardware/gpio.h"

typedef struct laser_place_i {
   uint8_t config_one;
   uint8_t config_two;
   uint8_t config_three;
   uint8_t config_four;
   uint8_t config_five;
   uint8_t config_six;
   uint8_t config_seven;
   uint8_t config_eight;
   uint8_t config_nine;
   uint8_t config_ten;
   uint8_t config_eleven;
   
} laser_place_i;
laser_place_i l_config;

LCDPins lcd_pins = {
    .back_light = GPIO_SEVENTEEN,
    .rs_pin = GPIO_SEVEN,
    .e_pin = GPIO_EIGHT,
};

register_pins reg_pins = {

  .register_one_data = GPIO_TWO,
  .register_one_latch = GPIO_THREE,
  .register_one_enable = GPIO_FOUR,
  .register_two_data = UNDEFINED,
  .register_two_latch = UNDEFINED,
  .register_two_enable = UNDEFINED,
  .register_clk_line = GPIO_FIVE

};

button_types enabled_buttons = {
  .rotary_button_clk = GPIO_SIXTEEN,
  .rotary_button_dt = GPIO_FIFTEEN,
  .rotary_button_button = GPIO_SIX,
  .analog_button_one = UNDEFINED,
  .analog_button_two = UNDEFINED,
  .button_one = UNDEFINED,
  .button_two = UNDEFINED,
  .button_three = UNDEFINED
};


rotary_encoder_struct rotary = {

  .max_rotation_value = 500,
  .minimum_required_interrupt = 2000

};

laser_place_i l_config = {

        .config_one = 0x11,
        .config_two = 0x22,
        .config_three = 0x33,
        .config_four = 0x44,
        .config_five = 0x55,
        .config_six = 0x66,
        .config_seven = 0x77,
        .config_eight = 0x88,
        .config_nine = 0x99,
        .config_ten = 0xAA,
        .config_eleven = 0x73

    };

uint8_t register_buffer_incrementer;

char menu_line[16];
char laser_position_line[16];

void lcd_prep(int menu, uint8_t value, char *top, char *bottom){

    sprintf(top, "Menu Item: %i", menu);
    sprintf(bottom, "Positions: %02x.", value);

}

int main(){

    stdio_init_all();

    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);
    
    rotary_encoder_init(&enabled_buttons);
    button_interrupt_init(&enabled_buttons);

    shift_register_pin_init(&reg_pins);

uint16_t menu_choice;

while(1) {
    menu_choice = rotary.rotary_value;

    switch(menu_choice){

        case 0:

            while(register_buffer_incrementer <= l_config.config_one){

            u_vars.register_value_zero = register_buffer_incrementer;
            
            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);
            
            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 1:

            while(register_buffer_incrementer <= l_config.config_two){

            u_vars.register_value_zero = register_buffer_incrementer;
            
            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);
            
            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 2:

            while(register_buffer_incrementer <= l_config.config_three){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 3:

            while(register_buffer_incrementer <= l_config.config_four){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;
                
        case 4:

            while(register_buffer_incrementer <= l_config.config_five){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 5:

            while(register_buffer_incrementer <= l_config.config_six){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 6:

            while(register_buffer_incrementer <= l_config.config_seven){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 7:

            while(register_buffer_incrementer <= l_config.config_eight){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 8:

            while(register_buffer_incrementer <= l_config.config_nine){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 9:

            while(register_buffer_incrementer <= l_config.config_ten){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        case 10:

            while(register_buffer_incrementer <= l_config.config_eleven){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 1, -1);

            register_buffer_incrementer++;
                sleep_ms(20);
            }
            break;

        default:

            printf("Outside of menu selection choices.");
            break;

    }




   
}
tight_loop_contents();
}