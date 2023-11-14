cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(motorun)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(motorun motor.c)

pico_enable_stdio_usb(motorun 1)
pico_enable_stdio_uart(motorun 0)
pico_add_extra_outputs (motorun)

target_link_libraries(motorun pico_stdlib hardware_gpio hardware_pwm)

