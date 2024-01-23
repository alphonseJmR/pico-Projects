cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(laser_prism_two)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


include_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(laser_prism_two laser_prism.c)
target_include_directories(laser_prism_two PUBLIC /home/main/component_libs)

pico_enable_stdio_usb(laser_prism_two 1)
pico_enable_stdio_uart(laser_prism_two 0)
pico_add_extra_outputs (laser_prism_two)


target_link_libraries(laser_prism_two
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)

