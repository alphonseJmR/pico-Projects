cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(motorun)

pico_sdk_init()


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_executable(motorun motor.c)


# Adds a library target called nrf24_driver for the NRF24L01 driver
# STATIC libraries are archives of object files for use when linking other targets
# INTERFACE libraries do not compile sources and don't produce a library artifact on disk
add_library(nrf24_driver INTERFACE)

# PRIVATE means these sources should only be added to nrf24_driver
# PUBLIC wuould mean these sources should be added to nrf24_driver and to any 
# target that links to nrf24_driver library. 
target_sources(nrf24_driver
    INTERFACE 
      nrf24_driver.c
)

# ${CMAKE_CURRENT_LIST_DIR} (nrf24_driver.h)
# ${CMAKE_CURRENT_LIST_DIR}/error_manager (error_manager.h)
# ${CMAKE_CURRENT_LIST_DIR}/pin_manager (pin_manager.h)
# ${CMAKE_CURRENT_LIST_DIR}/spi_manager (spi_manager.h)
target_include_directories(nrf24_driver 
    INTERFACE 
      
      error_manager
      pin_manager
      spi_manager
)

pico_enable_stdio_usb(motorun 1)
pico_enable_stdio_uart(motorun 0)
pico_add_extra_outputs (motorun)

# Link nrf24_driver against pico-sdk;
# pico_stdlib, hardware_spi & hardware_gpio libraries

target_link_libraries(
                      motorun INTERFACE
                      nrf24_driver
                      hardware_gpio 
                      hardware_adc 
                      hardware_pwm 
                      hardware_timer 
                      hardware_spi
                      hardware_clocks
                       
                      )

