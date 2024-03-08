#ifndef ILI9488_PIN_MANAGEMENT  
#define ILI9488_PIN_MANAGEMENT

#include <stdio.h>
#include "ili9488_func_def.h"
#include "hardware/gpio.h"
#include "ili9488_error_management.h"
#include "spi_management.h"


typedef struct spi_pin_manager_s {

    uint miso;
    uint mosi;
    uint csn;
    uint sck;

    uint dc_rs;
    uint led_bl;

}spi_pins;

static func_ack pin_validate(spi_pins *pins) {

    //  Validate four main SPI pins.

  typedef struct validate_pin_s
  {
    uint spi_pin;
    uint check_one;
    uint check_two;
    uint check_three;
    uint check_four;

  } validate_pin_t;

  validate_pin_t spi_pins[5] = {

    (validate_pin_t) { .spi_pin = pins->mosi, .check_one = spi_tx_zz, .check_two = spi_tx_zo, .check_three = spi_tx_oo, .check_four = spi_tx_zt},
    (validate_pin_t) { .spi_pin = pins->miso, .check_one = spi_rx_zz, .check_two = spi_rx_zo, .check_three = spi_rx_oo, .check_four = spi_rx_zt},
    (validate_pin_t) { .spi_pin = pins->csn, .check_one = spi_csn_zz, .check_two = spi_csn_zo, .check_three = spi_csn_oo, .check_four = spi_csn_zt},
    (validate_pin_t) { .spi_pin = pins->sck, .check_one = spi_sck_zz, .check_two = spi_sck_zo, .check_three = spi_sck_oo, .check_four = spi_sck_zt}

  };

    //  Valid Pin setup: mosi - 0x01, miso - 0x02, csn - 0x04, sck - 0x08
  uint8_t valid_pins = 0;

    for(int i = 0; i < 4; i++){

        if(i == 0){

            if(spi_pins[0].spi_pin == (spi_pins[0].check_one || spi_pins[0].check_two || spi_pins[0].check_three || spi_pins[0].check_four)){
                
                valid_pins += 0x01;

            }

        }
        if(i == 1){

            if(spi_pins[1].spi_pin == (spi_pins[1].check_one || spi_pins[1].check_two || spi_pins[1].check_three || spi_pins[1].check_four)){
                
                valid_pins += 0x02;

            }

        }
        if(i == 2){

            if(spi_pins[2].spi_pin == (spi_pins[2].check_one || spi_pins[2].check_two || spi_pins[2].check_three || spi_pins[2].check_four)){
                
                valid_pins += 0x04;

            }

        }
        if(i == 3){

            if(spi_pins[1].spi_pin == (spi_pins[4].check_one || spi_pins[4].check_two || spi_pins[3].check_three || spi_pins[3].check_four)){
                
                valid_pins += 0x08;

            }

        }

    }


  func_ack status = ((valid_pins & 0x0F) == true) ? pins_ack : func_error;
  
  return status;
}

spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins){

  if(pins->miso == 0){
    instance->instance = spi0;
  }
  if(pins->miso == 4){
    instance->instance = spi0;
  }
  if(pins->miso == 8){
    instance->instance = spi1;
  }
  if(pins->miso == 12){
    instance->instance = spi1;
  }
  if(pins->miso == 16){
    instance->instance = spi0;
  }

return *instance;
}

//  Pin Manager Configure
    //  Mosi  - Master Out Slave In
    //  Miso  - Master In Slave Out
    //  Sck   - Clock Signal
    //  CSN   - Chip Select Not
    //  Dc_Rs - Data/Command Register Select

func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins) {

  func_ack status = pin_validate(pins);

  configure_instance(packet, pins);

  if (status == 2)
  {
    // set GPIO function as SPI for SCK, MOSI & MISO & CSN
    gpio_set_function(pins->sck, GPIO_FUNC_SPI);
    gpio_set_function(pins->mosi, GPIO_FUNC_SPI);
    gpio_set_function(pins->miso, GPIO_FUNC_SPI);

    // initialise CE & CSN
    gpio_init(pins->csn);
    gpio_init(pins->dc_rs);

    // set direction for CE & CSN
    gpio_set_dir(pins->csn, GPIO_OUT);
    gpio_set_dir(pins->dc_rs, GPIO_OUT);
  }
  
  return status;
};

func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins){

    printf("Initializing user input pins for SPI.\n");

    func_ack fun_status;
    func_ack configure_status;
    func_ack exit_status;

    printf("\nUser Defined Pins:\n\tMosi: %i\n\tMosi: %i\n\tSck: %i\n\tCSN: %i\n\tDC-RS: %i\n\n", pins->mosi, pins->miso, pins->sck, pins->csn, pins->dc_rs);

    configure_status = pin_manager_configure(inst, pins);

    fun_status = general_ack;

    exit_status = ((configure_status == pins_ack) && (fun_status == general_ack)) ? pins_init_ack : func_error;

        return exit_status;

}

void csn_put_high(uint8_t csn) {

  gpio_put(csn, HIGH);

  return;
}

void csn_put_low(uint8_t csn) {

  gpio_put(csn, LOW);

  return;
}

void dcrs_put_high(uint8_t ce) {

  gpio_put(ce, HIGH);

  return;
}

void dcrs_put_low(uint8_t ce) {

  gpio_put(ce, LOW);

  return;
}

func_ack selector(bool csn, bool dc_rs, bool enable, spi_pins *pins){

    if(csn == true){

        if(enable == true){

            csn_put_high(pins->csn);

        }else{
            csn_put_low(pins->csn);
        }

    }
    if(dc_rs == true){

        if(enable == true){

            dcrs_put_high(pins->dc_rs);

        }else{

            dcrs_put_low(pins->dc_rs);

        }

    }

    return general_ack;

}

#endif