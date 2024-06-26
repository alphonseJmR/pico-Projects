
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/beer_lib)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/rand_lib)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Pico_Communication)
target_include_directories(PRIVATE home/alphonse/Downloads/component_libs/lib)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Periphereals)
    add_subdirectory(Motors)
    add_subdirectory(resources)
    add_subdirectory(interrupt_handler)
target_include_directories(PUBLIC home/alphonse/Downloads/component_libs/Screen_lib)
    add_subdirectory(Screen_lib/_2004A_lcd)
    add_subdirectory(Screen_lib/ILI9488_Screen)
    add_subdirectory(Screen_lib/ssd_1306_oled)
    add_subdirectory(Screen_lib/_1602A_lcd)


