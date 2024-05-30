#ifndef SPI_PIN_MANAGEMENT  
#define SPI_PIN_MANAGEMENT

#include <stdio.h>
#include "hardware/gpio.h"
#include "error_manager.h"
#include "spi_pin_check.h"
#include "spi_management.h"


typedef struct spi_manager_s {

    //  Define pins for SPI communication.
    uint miso;
    uint mosi;
    uint csn;
    uint sck;
    // Piggy back MOSI, SCK for cd74hc595 register pins.
    uint _OE;
    uint _srClear;
    spi_fig *config;

}spi_pins;

//  Tie to Peri_header for use with interrupts, or copy interrupt code to new header?
typedef struct button_pins_s {

  uint8_t menu_up;
  uint8_t menu_down;
  uint8_t enter;
  uint8_t back;
  uint8_t reset;
  uint8_t lcd_backlight_toggle;

}butt_pins;

//  Should return 2.
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


  func_ack status = ((valid_pins & 0x0F) == true) ? pins_ack : er_r;
  
  return status;
}

//  Return spi_inst_t instance configured to either SPI0 or SPI1
spi_fig configure_instance(spi_fig *instance, spi_pins *pins){

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
    //  _OE   - Enable pin on CD74HC595 Shift register.
    //  _srCLear  - Shift register clear.

func_ack pin_manager_configure(spi_fig *config, spi_pins *pins) {

  func_ack status = pin_validate(pins);

  configure_instance(config, pins);

  if (status == 2)
  {
    // set GPIO function as SPI for SCK, MOSI & MISO & CSN
    gpio_set_function(pins->sck, GPIO_FUNC_SPI);
    gpio_set_function(pins->mosi, GPIO_FUNC_SPI);
    gpio_set_function(pins->miso, GPIO_FUNC_SPI);

    // initialise CSN, _OE, _srClear.
    gpio_init(pins->csn);
    gpio_init(pins->_OE);
    gpio_init(pins->_srClear);

    // set direction for CSN, _OE, _srCLear
    gpio_set_dir(pins->csn, GPIO_OUT);
    gpio_set_dir(pins->_OE, GPIO_OUT);
    gpio_set_dir(pins->_srClear, GPIO_OUT);

    //  Pull _OE && _srClear HIGH for non-use.
    gpio_pull_up(pins->_OE);
    gpio_pull_up(pins->_srClear);

  }
  
  return status;
};

//  Should return 5.
func_ack user_pin_initialize(spi_fig *inst, spi_pins *pins){

    printf("Initializing user input pins for SPI.\n");

    func_ack fun_status;
    func_ack configure_status;
    func_ack exit_status;

    printf("\nUser Defined Pins:\n\tMosi: %i\n\tMosi: %i\n\tSck: %i\n\tCSN: %i\n\t_OE: %i\n\t_srClear: %i\n\n", pins->mosi, pins->miso, pins->sck, pins->csn, pins->_OE, pins->_srClear);

    configure_status = pin_manager_configure(inst, pins);

    fun_status = general_ack;

    exit_status = ((configure_status == pins_ack) && (fun_status == general_ack)) ? pins_init_ack : er_r;

        return exit_status;

}

void csn_put_high(uint8_t csn) {

  gpio_put(csn, 1);

  return;
}

void csn_put_low(uint8_t csn) {

  gpio_put(csn, 0);

  return;
}

void _OE_put_high(uint8_t oe) {

  gpio_put(oe, 1);

  return;
}

void _OE_put_low(uint8_t oe) {

  gpio_put(oe, 0);

  return;
}

void _srClr_put_high(uint8_t _clr){

  gpio_put(_clr, 1);

}

void _srClr_put_low(uint8_t _clr){

  gpio_put(_clr, 0);

}

void register_reset(uint8_t _clr){

  _srClr_put_low(_clr);
    sleep_us(10);
  _srClr_put_high(_clr);

}

func_ack selector(bool Slave_SPI, bool Register, spi_pins *pins){

    if(Slave_SPI == true){

    }
    if(Register == true){

    }

    return general_ack;

}

#endif
