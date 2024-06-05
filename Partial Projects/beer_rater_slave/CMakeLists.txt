cmake_minimum_required(VERSION 3.5)
include(pico_sdk_import.cmake)

project(beer_slave)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(beer_beer_slave beer_rater_s.c)
target_include_directories(beer_slave PUBLIC ../../component_libs/rand_lib)
target_include_directories(beer_slave PUBLIC ../../component_libs/_2004A_lcd)
target_include_directories(beer_slave PUBLIC ../../component_libs/beer_lib)
target_include_directories(beer_slave PUBLIC ../../component_libs/Pico_Communication)

pico_enable_stdio_usb(beer_slave 1)
pico_enable_stdio_uart(beer_slave 0)
pico_add_extra_outputs (beer_slave)


target_link_libraries(beer_slave
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_i2c
      hardware_spi

)


