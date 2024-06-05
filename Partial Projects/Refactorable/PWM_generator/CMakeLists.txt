cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(PWM_GENERATOR)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(PWM_GENERATOR pwm_gen.c)
target_include_directories(PWM_GENERATOR PUBLIC /home/alphonse/Downloads/component_libs)

pico_enable_stdio_usb(PWM_GENERATOR 1)
pico_enable_stdio_uart(PWM_GENERATOR 0)
pico_add_extra_outputs (PWM_GENERATOR)


target_link_libraries(PWM_GENERATOR
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
)

