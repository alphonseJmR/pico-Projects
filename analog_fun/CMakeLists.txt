cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(anal_fun)

pico_sdk_init()


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(anal_fun anal_fun.c)

pico_enable_stdio_usb(anal_fun 1)
pico_enable_stdio_uart(anal_fun 0)
pico_add_extra_outputs (anal_fun)

target_link_libraries(anal_fun pico_stdlib hardware_pwm hardware_adc)


