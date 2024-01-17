cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include(pico_sdk_import.cmake)

project(rc_controller_revision_one)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(lib/nrf24l01)

add_executable(rc_controller_revision_one rc_controller_revision_1.c)
target_include_directories(rc_controller_revision_one PUBLIC /home/alphonse/Downloads/component_libs)

pico_enable_stdio_usb(rc_controller_revision_one 1)
pico_enable_stdio_uart(rc_controller_revision_one 0)
pico_add_extra_outputs (rc_controller_revision_one)


target_link_libraries(rc_controller_revision_one
    PRIVATE 
      nrf24_driver 
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)

