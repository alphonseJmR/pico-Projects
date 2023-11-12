cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

include_directories(/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled)
include_directories(/home/alphonse/pico-sdk/src)
add_library(ssd1306 STATIC
/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled/BitBang_I2C.c
/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled/BitBang_I2C.h
/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled/ss_oled.c
/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled/ss_oled.h
/home/alphonse/Downloads/component_libs/SSD1306_OLED/ss_oled/ss_oled_demo.c
)

project(lcd_display)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lcd_display 16x2_driver.c)

pico_enable_stdio_usb(lcd_display 1)
pico_enable_stdio_uart(lcd_display 0)
pico_add_extra_outputs (lcd_display)


target_link_libraries(lcd_display
      ssd1306
      pico_stdlib
      hardware_gpio
      hardware_adc
      hardware_i2c
)


