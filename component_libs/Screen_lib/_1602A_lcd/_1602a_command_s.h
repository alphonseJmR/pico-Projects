#ifndef _1602A_COMMAND_DEFINE_S_H
#define _1602A_COMMAND_DEFINE_S_H

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

#endif