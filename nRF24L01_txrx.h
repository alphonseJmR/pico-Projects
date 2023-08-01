#include <stdio.h>
#include "pico/stdlib.h"
#include "nrf24L01/nrf24l01.h"
//  Macros for SPI, CE, and CSN pin configuration.
#define MISO_PIN 16
#define NRF24_CSN 17
#define SCK_PIN 18
#define MOSI_PIN 19
#define SS_PIN 20
#define NRF24_CE 21

void delay_function(uint32_t duration_ms)
{
    sleep_ms(duration_ms);
}

void SPI_Initializer()
{
    gpio_init(MOSI_PIN);
    gpio_set_dir(MOSI_PIN, GPIO_OUT);

    gpio_init(MISO_PIN);
    gpio_set_dir(MISO_PIN, GPIO_IN);

    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);

    gpio_init(SS_PIN);
    gpio_set_dir(SS_PIN, GPIO_OUT);
}