cmake_minimum_required(VERSION 3.13)
include${env:PICO_SDK_PATH}/**
include(pico_sdk_import.cmake)

project(hall_round)

pico_sdk_init()


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(hall_round hall_round.c)

pico_enable_stdio_usb(hall_round 1)
pico_enable_stdio_uart(hall_round 0)
pico_add_extra_outputs (hall_round)

target_link_libraries(hall_round pico_stdlib hardware_gpio hardware_pwm hardware_adc)


