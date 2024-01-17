cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(rotary)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(rotary hall_round.c)

pico_enable_stdio_usb(rotary 1)
pico_enable_stdio_uart(rotary 0)
pico_add_extra_outputs (rotary)

target_link_libraries(rotary pico_stdlib hardware_gpio hardware_pwm hardware_adc)


