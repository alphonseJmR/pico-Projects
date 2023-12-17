cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include(pico_sdk_import.cmake)


project(motor_header_tester)


pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(motor_header_tester motor_ht_test.c)

target_include_directories(motor_header_tester PUBLIC /home/main/component_libs)

pico_enable_stdio_usb(motor_header_tester 1)
pico_enable_stdio_uart(motor_header_tester 0)
pico_add_extra_outputs (motor_header_tester)


target_link_libraries(motor_header_tester
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)

