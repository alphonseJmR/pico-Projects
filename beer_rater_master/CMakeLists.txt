cmake_minimum_required(VERSION 3.5)
include(pico_sdk_import.cmake)

project(beer_master)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(beer_master beer_rater_m.c)
target_include_directories(beer_master PUBLIC ../../component_libs/rand_lib)
target_include_directories(beer_master PUBLIC ../../component_libs/_2004A_lcd)
target_include_directories(beer_master PUBLIC ../../component_libs/beer_lib)

pico_enable_stdio_usb(beer_master 1)
pico_enable_stdio_uart(beer_master 0)
pico_add_extra_outputs (beer_master)


target_link_libraries(beer_master
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_i2c
      hardware_spi

)


