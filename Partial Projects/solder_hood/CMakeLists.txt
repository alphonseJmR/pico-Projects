cmake_minimum_required(VERSION 3.5)
include(pico_sdk_import.cmake)

project(solder_hood)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(solder_hood hooded.c)
target_include_directories(solder_hood PUBLIC home/alphonse/Downloads/component_libs/rand_lib)

pico_enable_stdio_usb(solder_hood 1)
pico_enable_stdio_uart(solder_hood 0)
pico_add_extra_outputs(solder_hood)


target_link_libraries(solder_hood
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)


