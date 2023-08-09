// # Adds a library target called nrf24_driver for the NRF24L01 driver
// # STATIC libraries are archives of object files for use when linking other targets
// # INTERFACE libraries do not compile sources and don't produce a library artifact on disk
add_library(home/pico/nrf24l01/nrf24_driver INTERFACE)

// # PRIVATE means these sources should only be added to nrf24_driver
// # PUBLIC wuould mean these sources should be added to nrf24_driver and to any 
// # target that links to nrf24_driver library. 
target_sources{nrf24_driver}(
    INTERFACE 
      nrf24_driver.c
)


// # Link nrf24_driver against pico-sdk;
// # pico_stdlib, hardware_spi & hardware_gpio libraries
target_link_libraries(nrf24_driver 
    INTERFACE
)
