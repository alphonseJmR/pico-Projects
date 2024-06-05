cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(rotary_lcd_tester)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(rotary_lcd_tester rotary_lcd_testing.c)

pico_enable_stdio_usb(rotary_lcd_tester 1)
pico_enable_stdio_uart(rotary_lcd_tester 0)
pico_add_extra_outputs (rotary_lcd_tester)


target_link_libraries(rotary_lcd_tester
      pico_stdlib
      hardware_gpio
      hardware_adc
)


