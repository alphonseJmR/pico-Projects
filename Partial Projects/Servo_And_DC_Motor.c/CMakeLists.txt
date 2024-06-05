cmake_minimum_required(VERSION 3.5)
include(pico_sdk_import.cmake)

project(motors)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(motors PWM_To_Hbridge_test.c)
target_include_directories(motors PUBLIC home/alphonse/Downloads/component_libs/rand_lib)

pico_enable_stdio_usb(motors 1)
pico_enable_stdio_uart(motors 0)
pico_add_extra_outputs(motors)


target_link_libraries(motors
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)


