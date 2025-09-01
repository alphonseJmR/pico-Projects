cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include(pico_sdk_import.cmake)

project(ssd1306_driver_dev_pro)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(ssd1306_driver_dev_pro
 	ssd1306_dev.c
 	../pio_files/cdhc_in.pio)
 	

pico_generate_pio_header(ssd1306_driver_dev_pro ${CMAKE_CURRENT_LIST_DIR}/../pio_files/cdhc_in.pio)

target_include_directories(ssd1306_driver_dev_pro PUBLIC /home/alphonse/Downloads/component_libs)

pico_enable_stdio_usb(ssd1306_driver_dev_pro 1)
pico_enable_stdio_uart(ssd1306_driver_dev_pro 0)
pico_add_extra_outputs (ssd1306_driver_dev_pro)


target_link_libraries(ssd1306_driver_dev_pro
      pico_stdlib
      pico_time
      pico_multicore
      hardware_gpio
      hardware_pio
      hardware_spi
      hardware_adc
      hardware_i2c
      hardware_dma
      hardware_pwm
      
)

