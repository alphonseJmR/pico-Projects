cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(rotodis)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(rotodis rotary_to_display.c)

pico_enable_stdio_usb(rotodis 1)
pico_enable_stdio_uart(rotodis 0)
pico_add_extra_outputs (rotodis)

target_link_libraries(rotodis pico_stdlib hardware_gpio hardware_pwm hardware_adc)


