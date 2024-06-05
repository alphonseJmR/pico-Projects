#include <stdio.h>
#include "stdlib.h"
#include "lcd_16x2.h"
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

// Breadboard Correct 1/27/2024 10:22pm
LCDPins lcd_pins = {

    .back_light = GPIO_THIRTEEN,
    .rs_pin = GPIO_ELEVEN,
    .e_pin = GPIO_TWELVE,
    .data4_pin = GPIO_SEVEN,
    .data5_pin = GPIO_EIGHT,
    .data6_pin = GPIO_NINE,
    .data7_pin = GPIO_TEN

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

// Breadboard Correct 1/27/2024 10:22pm
button_types enabled_buttons = {
  .rotary_button_clk = GPIO_SIXTEEN,
  .rotary_button_dt = GPIO_FIFTEEN,
  .rotary_button_button = GPIO_FOURTEEN,
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

uint8_variables u_vars = {
    .register_value_zero = 0x00,
    .register_value_one = 0x00,
    .register_value_two = 0x00
};

uint8_t register_buffer_incrementer;

char menu_line[16];
char laser_position_line[16];

void lcd_prep(int menu, uint8_t value, char *top, char *bottom){

    sprintf(top, "Menu Item: %i", menu);
    sprintf(bottom, "Positions: %02x", value);

}

int main(){

    stdio_init_all();

    sleep_ms(7000);
    sleep_ms(4000);

    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);
    
    rotary_encoder_init(&enabled_buttons);
    button_interrupt_init(&enabled_buttons);

    shift_register_pin_init(&reg_pins);
    
    uint slice_num;
    uint chan;
    int slice_num_two;
    uint chan_two;
    gpio_set_function(0, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(0);
    chan = pwm_gpio_to_channel(0);

    pwm_set_wrap(slice_num, 5000);
    pwm_set_chan_level(slice_num, chan, (5000 * .5));
    pwm_set_enabled(slice_num, true);

    gpio_set_function(1, GPIO_FUNC_PWM);
    slice_num_two = pwm_gpio_to_slice_num(1);
    chan_two = pwm_gpio_to_channel(1);

    pwm_set_wrap(slice_num_two, 5000);
    pwm_set_chan_level(slice_num_two, chan_two, (5000 * .5f));
    pwm_set_enabled(slice_num_two, true);

uint16_t menu_choice;

gpio_put(GPIO_THIRTEEN, 1);

while(1) {

    printf("\nMain Loop:\n\n");
    printf("Menu Choice: %i.\n", rotary.rotary_total);
    
    menu_choice = rotary.rotary_total;
  if(button_status.rotary_button_status == true){
    register_buffer_incrementer = 0x00;
    switch(menu_choice){

        case 0:

            printf("Case Zero: Increment Until: %02x.\n", l_config.config_one);

            while(register_buffer_incrementer <= l_config.config_one){

            u_vars.register_value_zero = register_buffer_incrementer;
            
            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);
            
            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 1:

            printf("Case One: Increment Until: %02x.\n", l_config.config_two);

            while(register_buffer_incrementer <= l_config.config_two){

            u_vars.register_value_zero = register_buffer_incrementer;
            
            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);
            
            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 2:

            while(register_buffer_incrementer <= l_config.config_three){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 3:

            printf("Case Two: Increment Until: %02x.\n", l_config.config_three);

            while(register_buffer_incrementer <= l_config.config_four){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;
                
        case 4:

            printf("Case Four: Increment Until: %02x.\n", l_config.config_five);

            while(register_buffer_incrementer <= l_config.config_five){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 5:

            printf("Case Five: Increment Until: %02x.\n", l_config.config_six);

            while(register_buffer_incrementer <= l_config.config_six){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 6:

            printf("Case Six: Increment Until: %02x.\n", l_config.config_seven);

            while(register_buffer_incrementer <= l_config.config_seven){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 7:

            printf("Case Seven: Increment Until: %02x.\n", l_config.config_eight);

            while(register_buffer_incrementer <= l_config.config_eight){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 8:

            printf("Case Eight: Increment Until: %02x.\n", l_config.config_nine);

            while(register_buffer_incrementer <= l_config.config_nine){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 9:

            printf("Case Nine: Increment Until: %02x.\n", l_config.config_ten);

            while(register_buffer_incrementer <= l_config.config_ten){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        case 10:

            printf("Case Ten: Increment Until: %02x.\n", l_config.config_eleven);

            while(register_buffer_incrementer <= l_config.config_eleven){

            u_vars.register_value_zero = register_buffer_incrementer;

            lcd_prep(menu_choice, register_buffer_incrementer, menu_line, laser_position_line);
            write_display(&lcd_pins, menu_line, laser_position_line);

            serial_register_output(&reg_pins, &u_vars, 0, -1);

            register_buffer_incrementer += 0x01;
                sleep_ms(20);
            }
            break;

        default:

            printf("Outside of menu selection choices.");
            break;

    }
    button_status.rotary_button_status = false;
    u_vars.register_value_zero = 0x00;
    serial_register_output(&reg_pins, &u_vars, 0, -1);
  }

  printf("Loop End.\n\n");
  sleep_ms(500);

}
tight_loop_contents();
}