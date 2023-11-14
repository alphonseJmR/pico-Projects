cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(servod)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(servod served.c)

pico_enable_stdio_usb(servod 1)
pico_enable_stdio_uart(servod 0)
pico_add_extra_outputs (servod)

target_link_libraries(servod pico_stdlib hardware_gpio hardware_pwm hardware_adc)


