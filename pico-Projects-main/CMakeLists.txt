cmake_minimum_required(VERSION 3.5)

include(pico_sdk_import.cmake)
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

target_include_directories(PUBLIC home/alphonse/Downloads/component_libs)
  add_subdirectory(component_libs/beer_lib)
  add_subdirectory(component_libs/Periphereals)
  add_subdirectory(component_libs/Pico_Communication)
  add_subdirectory(component_libs/rand_lib)
  add_subdirectory(component_libs/Screen_lib)


target_link_libraries(
   PRIVATE
      nrf24_driver
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_pwm
      hardware_spi
      hardware_i2c
)

