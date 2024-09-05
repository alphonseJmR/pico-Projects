
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/beer_lib)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/rand_lib)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Pico_Communication)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/hi_lvl_resources)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Periphereals)
    add_subdirectory(Motors)
    add_subdirectory(resources)
    add_subdirectory(interrupt_handler)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib)
    add_subdirectory(_2004A_lcd)
    add_subdirectory(ILI9488_Screen)
    add_subdirectory(ssd_1306_oled)
    add_subdirectory(_1602A_lcd)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/displays)
    
    target_link_libraries(
      pico_stdlib
      pico_multicore
      hardware_gpio
      hardware_adc
      hardware_pwm
      hardware_spi
      hardware_i2c
      hardware_dma
)


