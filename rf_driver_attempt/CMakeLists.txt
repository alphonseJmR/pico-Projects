cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(nrfer)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(lib/nrf24l01)

add_executable(nrfer Nrfer.c)

pico_enable_stdio_usb(nrfer 1)
pico_enable_stdio_uart(nrfer 0)
pico_add_extra_outputs (nrfer)


target_link_libraries(nrfer 
    PRIVATE 
      nrf24_driver 
      pico_stdlib
      hardware_gpio
      hardware_pwm
      hardware_adc
      hardware_spi
)


