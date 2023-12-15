#ifndef LCD16X2_H
#define LCD16X2_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "peri_header.h"
#include "hardware/gpio.h"

  //    Define a structure to hold lcd configuration.
typedef struct LCDPins{
    uint rs_pin;
    uint e_pin;
    uint back_light;
    uint data0_pin;
    uint data1_pin;
    uint data2_pin;
    uint data3_pin;
    uint data4_pin;
    uint data5_pin;
    uint data6_pin;
    uint data7_pin;
}LCDPins;
LCDPins lcd_pins;

typedef struct {
    char menu_top_one[16];
    char menu_bottom_one[16];
    char menu_top_two[16];
    char menu_bottom_two[16];
    char menu_top_three[16];
    char menu_bottom_three[16];
    char menu_top_four[16];
    char menu_bottom_four[16];
    char menu_top_five[16];
    char menu_bottom_five[16];
} menu_arrays;

// LCD commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

// Display control constants
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

// Function set constants
#define LCD_8BIT_MODE 0x10  // 8-bit data bus
#define LCD_4BIT_MODE 0x00  // 4-bit data bus
#define LCD_2LINE 0x08      // 2-line display
#define LCD_1LINE 0x00      // 1-line display
#define LCD_5x10DOTS 0x04   // 5x10 pixel font
#define LCD_5x8DOTS 0x00    // 5x8 pixel font

// Entry mode set constants
#define LCD_ENTRY_INCREMENT 0x02  // Increment cursor position
#define LCD_ENTRY_DECREMENT 0x00  // Decrement cursor position
#define LCD_ENTRY_SHIFT_ON 0x01    // Enable display shift
#define LCD_ENTRY_SHIFT_OFF 0x00   // Disable display shift

static const uint8_t ddram_line1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static const uint8_t ddram_line2[] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};

static void send_nibble(const LCDPins *pins, uint8_t data) {

    // printf("Printing: 0b%04x.\n", data);
    gpio_put(pins->e_pin, 1);
        sleep_ms(5);
    // Set the data pins based on the lower 4 bits of the data
    gpio_put(pins->data4_pin, (data & 0x01) >> 0); // Set data4
    sleep_us(10);
    gpio_put(pins->data5_pin, (data & 0x02) >> 1); // Set data5
    sleep_us(10);
    gpio_put(pins->data6_pin, (data & 0x04) >> 2); // Set data6
    sleep_us(10);
    gpio_put(pins->data7_pin, (data & 0x08) >> 3); // Set data7
    sleep_us(10);

    gpio_put(pins->e_pin, 0);
}

void lcd_command(const LCDPins *pins, uint8_t cmd) {

  //  printf("\tLCD Command sent: 0b%04x.\n", cmd);

    // Set RS to 0 for a command
    gpio_put(pins->rs_pin, 0);
        sleep_ms(2);

    // Send the higher 4 bits
    send_nibble(pins, cmd >> 4);
        sleep_us(100); // Short delay
    
    // Send the lower 4 bits
    send_nibble(pins, cmd & 0x0F);   
        sleep_us(100); // Short delay

}

void lcd_4_bit_init(const LCDPins *pins){

        // Initialization sequence
    gpio_put(pins->rs_pin, 0);
    printf("\n\n\n\tRS Pin LOW:  Command Initialize.\n\n\n");
        sleep_ms(20); // Wait for more than 15ms
        
    send_nibble(pins, 0x02); // Function set (8-bit)
    printf("Initialize with 0x02.\n");
    printf("Configure for 4_bit_mode with 0x20.\n");
        sleep_ms(20);
    
    send_nibble(pins, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(10); // Wait for more than 4.1ms

    send_nibble(pins, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(5); // Wait for more than 100us

    send_nibble(pins, 0x03); // Function set (8-bit)
    printf("Initialize with 0x03.\n");
        sleep_ms(20);

    send_nibble(pins, 0x28); // Function set (8-bit)
    printf("Initialize with 0x28.\n");
    printf("Configure for 2_line_mode & 5x8_mode.\n");
        sleep_ms(20);

    lcd_command(pins, LCD_DISPLAY_CONTROL | LCD_CURSOR_OFF | LCD_BLINK_OFF); // Display on, cursor off, blink off
    printf("Initialize with Display_Control & Cursor_on & Blink_off.\n");
        sleep_ms(10);

    lcd_command(pins, LCD_CLEAR_DISPLAY); // Clear display
    printf("Initialize with Display CLEAR.\n");
        sleep_ms(10);

    lcd_command(pins, LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT | LCD_ENTRY_SHIFT_OFF); // Entry mode: Increment, no shift
    printf("Initialize with Entry Mode & Entry Increment & Entry Shift Off.\n");

    printf("Initialization Ending.\n");
        sleep_ms(100);
    printf("Initialization Ending..\n");
        sleep_ms(100);
    printf("Initialization Ending...\n");
        sleep_ms(100);
    printf("Initialization Ending....\n");
        sleep_ms(100);
    printf("Initialization Ending.....\n");
        sleep_ms(100);
    printf("Initialization Complete.\n");
       

}

void lcd_init(const LCDPins *pins) {

    printf("\n\nPin Initialization.\n");
    // Initialize GPIO pins for the LCD using the lcd_pins struct
    printf("Initializing RS_PIN..\n");
    printf("\tRS_PIN Pulled Low..\n");
    printf("\tRS_PIN: %d..\n", pins->rs_pin);
    gpio_init(pins->rs_pin);
    gpio_set_dir(pins->rs_pin, GPIO_OUT);
    gpio_pull_down(pins->rs_pin);

    printf("Initializing E_PIN..\n");
    printf("\tE_PIN Pulled Low..\n");
    printf("\tE_PIN: %d..\n", pins->e_pin);
    gpio_init(pins->e_pin);
    gpio_set_dir(pins->e_pin, GPIO_OUT);
    gpio_pull_down(pins->e_pin);

    printf("Initializing DATA4_PIN..\n");
    printf("\tDATA4_PIN Pulled Low..\n");
    printf("\tDATA4_PIN: %d..\n", pins->data4_pin);
    gpio_init(pins->data4_pin);
    gpio_set_dir(pins->data4_pin, GPIO_OUT);
    gpio_pull_down(pins->data4_pin);

    printf("Initializing DATA5_PIN..\n");
    printf("\tDATA5_PIN Pulled Low..\n");
    printf("\tDATA5_PIN: %d..\n", pins->data5_pin);
    gpio_init(pins->data5_pin);
    gpio_set_dir(pins->data5_pin, GPIO_OUT);
    gpio_pull_down(pins->data5_pin);

    printf("Initializing DATA6_PIN..\n");
    printf("\tDATA6_PIN Pulled Low..\n");
    printf("\tDATA6_PIN: %d..\n", pins->data6_pin);
    gpio_init(pins->data6_pin);
    gpio_set_dir(pins->data6_pin, GPIO_OUT);
    gpio_pull_down(pins->data6_pin);

    printf("Initializing DATA7_PIN..\n");
    printf("\tDATA6_PIN Pulled Low..\n");
    printf("\tDATA7_PIN: %d..\n", pins->data7_pin);
    gpio_init(pins->data7_pin);
    gpio_set_dir(pins->data7_pin, GPIO_OUT);
    gpio_pull_down(pins->data7_pin);
    
    // Implementation of the initialization sequence
}
    
void lcd_clear(const LCDPins *pins) {

  //  printf("\n\t\tDisplay Clear.\n");

    gpio_put(pins->rs_pin, 0);   
        sleep_ms(10);
    lcd_command(pins, LCD_CLEAR_DISPLAY);
        sleep_ms(5); // Wait for clear to complete

}

void lcd_home(const LCDPins *pins) {

  //  printf("\n\t\tCursor Return Home.\n");

    gpio_put(pins->rs_pin, 0);
        sleep_ms(10);
    lcd_command(pins, LCD_RETURN_HOME);
        sleep_ms(5); // Wait for return home to complete
   
}

void lcd_write_char(const LCDPins *pins, char data) {
   // printf("LCD Writing.\n");
    // Set RS to 1 for character data

    gpio_put(pins->rs_pin, 1);
        sleep_ms(2);
    // Send the higher 4 bits
    send_nibble(pins, data >> 4);
        sleep_ms(5);
    
    // Send the lower 4 bits
    send_nibble(pins, data & 0x0F);
        sleep_ms(5);
    
    sleep_us(100);

}

void lcd_write_string(const LCDPins *pins, const char *str) {
    // Iterate through the characters in the string and write them one by one
    for (size_t i = 0; str[i] != '\0'; i++) {
     //   (i == 0) ? printf("\nWriting: %c.\n", str[i]) : printf("Writing: %c.\n", str[i]);
        lcd_write_char(pins, str[i]);
    }
}

void lcd_set_cursor(const LCDPins *pins, uint8_t row, uint8_t col) {
    // Define DDRAM addresses for line one and line two

    // Calculate the DDRAM address based on row and column
    uint8_t ddram_addr;
    if (row == 1 && col >= 1 && col <= 16) {
        ddram_addr = ddram_line1[col - 1];
      //  printf("ddram addr row 1: %04x.\n", ddram_addr);
    } else if (row == 2 && col >= 1 && col <= 16) {
        ddram_addr = ddram_line2[col - 1];
      //  printf("ddram addr row 2: %04x.\n", ddram_addr);
    } else {
        // Invalid row or column, do nothing
        return;
    }

    // Send the command to set the cursor position
    gpio_put(pins->rs_pin, 0);
        sleep_ms(5);
      //  printf("\n\tCursor set at: %i, %i.\n", row, col);
    lcd_command(pins, LCD_SET_DDRAM_ADDR | ddram_addr);

}

void write_display(const LCDPins *pins, char *top, char *bottom, int top_data, int bottom_data){
    
    // printf("\n\n\nDisplay Writting function.\n\n");

    gpio_put(pins->rs_pin, 0);
        sleep_ms(5);
    lcd_clear(pins);
        sleep_ms(20);
    lcd_home(pins);
        sleep_ms(20);
        
    gpio_put(pins->rs_pin, 1);
      //  printf("\n\nRS Pulled High:  Data.\n");
        sleep_ms(5);
    lcd_write_string(pins, top);
        sleep_ms(5);

    gpio_put(pins->rs_pin, 0);
        sleep_ms(5);
      //  printf("\n\nRS Pulled Low:  Command.\n");
    lcd_set_cursor(pins, 2, 1);
        sleep_ms(5);

   // printf("\n\tCommand:  Set Cursor Position.\n");
        sleep_ms(5);

    gpio_put(pins->rs_pin, 1);
 //   printf("\n\nRS Pulled High:  Data.\n");
        sleep_ms(5);
    lcd_write_string(pins, bottom);
        sleep_ms(500);
        
}


void photoresistor_status_toggle(back_timed_t * times, adc_port_values *adc, LCDPins *pins){

    printf("\n\nLight Level LCD Toggle Function.\n\n");

    times->current_darkness = time_us_32();
    times->elapsed_darkness = times->current_darkness - times->first_light_fall;
    
    printf("Current Dark Time: %llu.\n", times->current_darkness);
    printf("Elapsed Darkness: %llu.\n", times->elapsed_darkness);

      if(times->elapsed_darkness >= times->darkness_threshold){

         gpio_put(pins->back_light, 1);

      }

      times->first_light_fall = times->current_darkness;
}


#endif