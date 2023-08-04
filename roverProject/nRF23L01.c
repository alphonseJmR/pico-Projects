#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

//DDS parameters
#define two32 4294967296.0; // 2^32
#define Fs 40000;   // Sample rate

//  the DDS units:
volatile unsigned int phasse_accum_main;
volatile unsigned int phase_incr_main = (800.0*two32)/Fs;

// SPI data
uint16_t NRF_data ; // output value

//DAC parameters
// A-channel, 1x, active
#define NRF_config_chan_a 0b011000000000000
// B-channel, 1x, active
#define NRF_config_chan_b 0b101100000000000

//SPI configurations
#define MOSI 11
#define MISO 12
#define CSn 13
#define SCK 14
#define CE 15


#define SPI_PORT spi1

//  DDS sine table
#define sine_table_size 256
volatile int sin_table[sine_table_size] ;

// Timer ISR
bool repeating_timer_callback(struct repeating_timer *t) {

    // DDS phase and sine table lookup
    phase_accum_main += phase_incr_main;
    NRF_data = (NRF_config_chan_A) | ((sin_table[phase_accum_main>>24] + 2048) & 0xffff)  ;

    spi_write16_blocking(SPI_PORT, &NRF_data, 1);

    return true;
}

int main() {
    // Initialize stdio
    stdio_init_all();
    printf("Hello, DAC!\n");

    //  Initialize SPI channel (channel, baud rate set to 20MHz)
    spi_init(SPI_PORT, 2000000) ;
    //  Format (channel, data bits per transfer, polarity, shift, order)
    spi_set_format(SPI_PORT, 8, 0, 0, 0);

    //  Map SPI signals to GPIO ports
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(CS, GPIO_FUNC_SPI);

    //  === build the sine lookup table =======
        //  scaled to produce values between 0 and 4096
    int ii;
    for (ii = 0; ii < sine_table_size; ii++) {
        sin_table[ii] = (int)(2037*sin((float)ii * 6.283/(float)sine_table_size);)
    }

    /*
        Create a repeating timer that calls repeating_timer_callback.
        If the delay is > 0 then this is the delay between the previous callback ending and the next starting.
        If the delay is negative (see below) then the next call to the callback will be exactly x uS after the 
        start of the call to the last callback.
    
    */

struct repeating_timer timer;

    //  Negative delay so means we will call repeating_timer_callback, and call it again
    //  25uS (40KHz) later regardless of how long the callback took to execute.

    add_repeating_timer_us(-25, repeating_timer_callback, NULL, &timer);
    while(1) {

    }
}