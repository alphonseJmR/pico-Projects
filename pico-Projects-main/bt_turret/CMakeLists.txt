cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include(pico_sdk_import.cmake)

project(Ball_Turret_Receiver)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(lib/nrf24l01)

add_executable(Ball_Turret_Receiver ball_turret.c)
target_include_directories(Ball_Turret_Receiver PUBLIC /home/alphonse/Downloads/component_libs)

pico_enable_stdio_usb(Ball_Turret_Receiver 1)
pico_enable_stdio_uart(Ball_Turret_Receiver 0)
pico_add_extra_outputs (Ball_Turret_Receiver)


target_link_libraries(Ball_Turret_Receiver
    PRIVATE 
      nrf24_driver 
      pico_stdlib
      pico_multicore
      hardware_gpio
      hardware_adc
      hardware_pwm
      hardware_dma
      hardware_i2c
)

