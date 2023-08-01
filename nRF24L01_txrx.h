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

    SPCR = 0X51;
}

void pinout_Initializer() 
{
    gpio_init(NRF24_CSN);
    gpio_set_dir(NRF24_CSN, GPIO_OUT);
    gpio_init(NRF24_CE);
    gpio_set_dir(NRF24_CE, GPIO_OUT);

    gpio_put(NRF24_CSN, 0);
    gpio_pull_high(NRF24_CE);
}

void nrf24_SPI(uint8_t input)
{
    if (input == SPI_ON) {
        gpio_put(NRF24_CSN, 0);
    }else {
        gpio_put(NRF24_CSN, 1);
    }
}

// 1 byte SPI shift register send and receive routine
uint8_t SPI_send_command(uint8_t command)
{
    SPDR = command;
    while ((SPSR & (1 << SPIF)) == 0) {}
    return SPDR;
}

void nrf24_CE(uint8_t input)
{
    if (input == CE_ON) {
        gpio_put(NRF24_CE, 1);
    } else {
        gpio_put(NRF24_CE, 0);
    }
}