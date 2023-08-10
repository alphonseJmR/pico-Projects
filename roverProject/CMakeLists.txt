cmake_minimum_required(VERSION 3.13)
set(C:\\Program Files\\Raspberry Pi\\Pico SDK v1.5.1\\pico-sdk\)
include(pico_sdk_import.cmake)

project(roverBody)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(roverBody roverBody.c)

pico_enable_stdio_usb(roverBody 1)
pico_enable_stdio_uart(roverBody 0)
pico_add_extra_outputs(roverBody)

target_link_libraries(roverBody pico_stdlib hardware_gpio hardware_adc hardware_timer hardware_pwm hardware_spi nrf24)