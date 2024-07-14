
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib/_1602A_lcd)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib/_2004A_lcd)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib/ILI9488_Screen)
	add_subdirectory(resources)
	add_subdirectory(includes)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib/ssd1306_oled)

    target_link_libraries(
      pico_stdlib
      hardware_gpio
      hardware_spi

)

