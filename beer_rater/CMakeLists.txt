cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(lcd_display)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(beer_rate beer_rate.c)

pico_enable_stdio_usb(beer_rate 1)
pico_enable_stdio_uart(beer_rate 0)
pico_add_extra_outputs (beer_rate)


target_link_libraries(beer_rate
      pico_stdlib
      hardware_gpio
      hardware_adc
)


