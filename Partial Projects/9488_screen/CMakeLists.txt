cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(ILI9488_LIB_CREATION)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(ILI9488_LIB_CREATION 9488_screen.c)
target_include_directories(ILI9488_LIB_CREATION PUBLIC /home/alphonse/Downloads/component_libs)
target_include_directories(ILI9488_LIB_CREATION PUBLIC /home/alphonse/Downloads/component_libs/ILI9488_Screen)

pico_enable_stdio_usb(ILI9488_LIB_CREATION 1)
pico_enable_stdio_uart(ILI9488_LIB_CREATION 0)
pico_add_extra_outputs (ILI9488_LIB_CREATION)


target_link_libraries(ILI9488_LIB_CREATION
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
      hardware_spi
)

