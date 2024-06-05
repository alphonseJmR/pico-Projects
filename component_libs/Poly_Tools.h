#ifndef POLY_TOOL_S_H
#define POLY_TOOL_S_H

/*
    COMMENT OUT ANY TOOL NOT BEING USED.  (caution: may be good chance to look for dependency errors.)

    **REMOVE PERI_HEADER.H FROM ANY DEPENDENCIES**
*/

// #define BEER_TOOL
#define PERI_TOOL
// #define PICO_COMS
#define RAND_TOOL
#define SCREEN_TOOL
#define NRF_TOOL

#include <stdio.h>


//  These headers are for the beer rater program project.
#ifdef BEER_TOOL

#include "beer_lib/beer_funcs.h"
#include "beer_lib/beer_list.h"
#include "beer_lib/beer_peri.h"
#include "beer_lib/beer_score.h"
#include "beer_lib/beer_transfer.h"
#include "beer_lib/error_manager.h"
#include "beer_lib/pin_manager.h"
#include "beer_lib/spi_management.h"
#include "beer_lib/spi_pin_check.h"

#endif

//  These headers are I/O's for the pico.
#ifdef PERI_TOOL

// #include "Periphereals/dc_motors.h"
#include "Periphereals/interrupt_handler.h"
#include "Periphereals/resources/pico_adc_input.h"
#include "Periphereals/resources/pico_pin_enum.h"
// #include "Periphereals/resources/rand_funcs.h"
#include "Periphereals/shift_registers.h"
// #include "Periphereals/sn74hc595.h"
// #include "Periphereals/ultra_sonic_sensor.h"
// #include "Periphereals/resources/Rc_RGB_Combinations.h"
// #include "Periphereals/pwm_motor_controller.h"

#endif

//  These headers are for the standard communication protocals for inner pico communications.
#ifdef PICO_COMS

#include "Pico_Communication/com_pins.h"
#include "Pico_Communication/IPC.h"
#include "Pico_Communication/ipc_errors.h"

#endif

//  These headers are random project headers, probably useless.
#ifdef RAND_TOOL

// #include "rand_lib/pwm_gener.h"
// #include "rand_lib/pwm_interrupt_mine.h"
#include "rand_lib/rc_nrf_configs.h"

#endif

#ifdef SCREEN_TOOL

#define _2004A
// #define _1602A
// #define _ILI9488
// #define _SSD1306

#ifdef _2004A

#include "Screen_lib/_2004A_lcd/_2004A_commands.h"
// #include "Screen_lib/_2004A_lcd/_2004a_i2c.h"
#include "Screen_lib/_2004A_lcd/2004A_4bit_wire.h"
#include "Screen_lib/_2004A_lcd/2004a_error_management.h"

#endif

#ifdef _1602A
#include "Screen_lib/_1602A_lcd/lcd_16x2.h"
#include "Screen_lib/_1602A_lcd/two_by_sixteen_lcd.h"
#endif

#ifdef _ILI9488

#include "/Screens/ILI9488_Screen/ili9488_error_management.h"
#include "/Screens/ILI9488_Screen/ili9488_pin_management.h"
#include "/Screens/ILI9488_Screen/ili9488_power_control.h"
#include "/Screens/ILI9488_Screen/ili9488_screen_commands.h"
#include "/Screens/ILI9488_Screen/ILI9488_4_WIRE_PRO.h"
#include "/Screens/ILI9488_Screen/ILI9488_4_WIRE_PRO_V2.h"
#include "/Screens/ILI9488_Screen/spi_management.h"

#endif

#ifdef _SSD1306

#endif

#endif

#ifdef NRF_TOOL

#include "lib/nrf24l01/device_config.h"
#include "lib/nrf24l01/nrf24_driver.h"

#endif


#endif
