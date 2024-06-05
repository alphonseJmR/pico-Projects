cmake_minimum_required(VERSION 3.5)
include(pico_sdk_import.cmake)

project(lcd_screen)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lcd_screen 2004a_screen.c)
target_include_directories(lcd_screen PUBLIC home/alphonse/Downloads/component_libs/rand_lib)
target_include_directories(lcd_screen PUBLIC home/alphonse/Downloads/component_libs/_2004A_lcd)
target_include_directories(lcd_screen PUBLIC home/alphonse/Downloads/component_libs/beer_lib)

pico_enable_stdio_usb(lcd_screen 1)
pico_enable_stdio_uart(lcd_screen 0)
pico_add_extra_outputs (lcd_screen)


target_link_libraries(lcd_screen
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_i2c

)


