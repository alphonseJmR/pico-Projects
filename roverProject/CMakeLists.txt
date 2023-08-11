cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
add_library(nrf24_driver INTERFACE)

project(roverBody)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(roverBody roverBody.c)

target_sources(roverBody
    INTERFACE 
      nrf24_driver.c
      gpio.c
)

pico_enable_stdio_usb(roverBody 1)
pico_enable_stdio_uart(roverBody 0)
pico_add_extra_outputs(roverBody)

target_link_directories(roverBody INTERFACE home/alphonse/pico/nrf24)

target_link_libraries(roverBody INTERFACE
    hardware_gpio
    nrf24_driver
    pico_stdlib
    hardware_adc
    hardware_timer
    hardware_pwm
    hardware_spi
    )