#ifndef ILI9488_PIN_MANAGEMENT  
#define ILI9488_PIN_MANAGEMENT

#include <stdio.h>
#include "hardware/gpio.h"
#include "ili9488_error_management.h"
#include "spi_management.h"

//  Function Prototypes for PIN usage.
spi_packet_s configure_instance(spi_packet_s *instance, spi_pins *pins);
func_ack pin_manager_configure(spi_packet_s *packet, spi_pins *pins);
func_ack user_pin_initialize(spi_packet_s *inst, spi_pins *pins);
func_ack gpio_setup(spi_pins *pins);


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