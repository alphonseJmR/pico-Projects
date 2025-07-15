cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include(pico_sdk_import.cmake)

project(nema17)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(nema17 nema_17.c)
target_include_directories(nema17 PUBLIC /home/alphonse/Downloads/component_libs)

pico_enable_stdio_usb(nema17 1)
pico_enable_stdio_uart(nema17 0)
pico_add_extra_outputs (nema17)


target_link_libraries(nema17
      pico_stdlib
      pico_time
      hardware_gpio
      hardware_pio
      hardware_spi
      hardware_adc
      hardware_i2c
      hardware_dma
      hardware_pwm
      
)

