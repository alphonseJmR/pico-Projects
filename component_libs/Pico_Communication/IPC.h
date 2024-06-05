#ifndef INTER_PICO_COMMUNICATION_H
#define INTER_PICO_COMMUNICATION_H

#include <stdio.h>
#include <stdint.h>
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "ipc_errors.h"
#include "com_pins.h"

//  These are just naming conventions I've been using.  Regular uint8_t or uint16_t will work fine as well.
#define ebit uint8_t
#define sbit uint16_t

#define i2c_chip_lo gpio_put(regs->CSX, 0);
#define i2c_chip_hi gpio_put(regs->CSX, 1);
#define i2c_read_sel gpio_put(regs->RWR, 0);
#define i2c_write_sel gpio_put(regs->RWR, 1);

#define spi_chip_lo gpio_put(regs->CSX, 0);
#define spi_chip_hi gpio_put(regs->CSX, 1);

#ifdef IPC_ERROR_HANDLER

#ifdef COM_I2C
//  Lines 90-222 are i2c protocol initialisation functions.
//  Lines 225-  are i2c communication functions.

//  Define I2C struct here
typedef struct i2c_communication_s {
    // "Required" pins first
    uint CSX;
    uint RWR;
    uint SDA;
    uint SCK;

    //  Required communications needs.
    uint16_t device_id;
    uint32_t baud_rate;
    i2c_inst_t *inst;
    bool moss;      //  True for master, false for slave.

    //  Uint8_t buffers for holding transmit & receive.
    uint8_t tx_buffer[8];
    uint8_t rx_buffer[8];

    //  Uint8_t value for checking function returns
    uint8_t f_stat;

}i2c_com;

i2c_com my_coms;

#ifdef i2c_slave

typedef struct i2c_slave_unit_s {

    volatile bool wake_state;
    volatile bool read_write_state;
    uint8_t tx_buffer[8];
    uint8_t sel_tx_buffer[4];   //  Used to hold the values of style,beer,and sense.

    uint8_t rx_buffer[8];
    uint8_t sel_rx_buffer[4];


}i2c_sb;

i2c_sb my_slave;

void master_interrupts(uint gpio, uint32_t events){

    printf("Master Pico is altering communication: %i.\n", gpio);
    gpio_acknowledge_irq(gpio, events);

    if(gpio == my_coms.CSX){

      if(gpio_get(my_coms.CSX)){
        my_slave.wake_state = true;
      }else {
        my_slave.wake_state = false;
      }
    }else if(gpio == my_coms.RWR){

        if(gpio_get(my_coms.RWR)){
            my_slave.read_write_state = true;
        }else {
            my_slave.read_write_state = false;
        }
    }
}

void beer_scores_zeroed(){

    for(int i = 0; i < 16; i++){
     for(int j = 0; j < 6; j++){
      for(int k = 0; k < 6; k++){
        beer_score[i][j][k] = 0;
      }
     }
    }

}

#endif

//  This function checks the values of the labelled structs values VS an array of pin configuration and sets bools upon true
//  It returns i2c_validation_complete(4);
enum com_dom i2c_pin_validation(i2c_com *check){
    //  Reset f_stat to 0 at beginning at function.
    check->f_stat = 0;

    // Need to create a way to ensure that pin pairs are valid connections.

    uint8_t uip_d = check->SDA;
    uint8_t uip_c = check->SCK;

        // Secure bools will be used to verify if User Input Pin (uip) matches an i2c[0||1] array.
    bool secure_one;
    bool secure_two;
    
        //  Valid pair will be set true if both secure's are true, ensuring i2c[0||1] instance match.
    bool i2c0_valid_pair;
    bool i2c1_valid_pair;

    bool invalid_pair;
    invalid_pair = (uip_d == uip_c) ? true : false;

    if(!invalid_pair){

    for(int i = 0; i < 6; i++){

        secure_one = ((uip_d = i2cZ_pin_cluster[i][0]) || (uip_d = i2cZ_pin_cluster[i][1])) ? true : false;
            check->f_stat += (secure_one) ? i2c_pin_valid : error_com;

        secure_two = ((uip_c = i2cZ_pin_cluster[i][0]) || (uip_c = i2cZ_pin_cluster[i][1])) ? true : false;
            check->f_stat += (secure_two) ? i2c_pin_valid : error_com;

      if(secure_one && secure_two){
        i2c0_valid_pair = true;
      }else {
        i2c0_valid_pair = false;
      }

      secure_one = false;
      secure_two = false;
    }
            //  If pins weren't matched with values from the i2c0 array, reset secure's in case of accidential tom-boolary.
        if(!i2c0_valid_pair){
            secure_one = false;
            secure_two = false;
        }


    for(int i = 0; i < 6; i++){

        secure_one = ((uip_d = i2cO_pin_cluster[i][0]) || (uip_d = i2cO_pin_cluster[i][1])) ? true : false;
            check->f_stat += (secure_one) ? i2c_pin_valid : error_com;
        secure_two = ((uip_c = i2cO_pin_cluster[i][0]) || (uip_c = i2cO_pin_cluster[i][1])) ? true : false;
            check->f_stat += (secure_one) ? i2c_pin_valid : error_com;

      if(secure_one && secure_two){
        i2c1_valid_pair = true;
      }else {
        i2c1_valid_pair = false;
      }

      secure_one = false;
      secure_two = false;
    }

    }else {
        /*
            Display to user in some form of pin selection error.
        */
    }

    //  At this point either i2c0_valid pair, or i2c1_valid_pair should be true.
    //  
    check->inst = (i2c0_valid_pair) ? i2c0 : i2c1;
    check->f_stat += i2c_instance_valid;
    return (check->f_stat == 7) ? i2c_validation_complete : error_com;

}

enum com_dom i2c_gpio_initialisation(i2c_com *pin){


        //  Reset f_stat.
    pin->f_stat = 0;

    pin->f_stat = i2c_pin_validation(pin);

    //  Check that user selected pins are valid input pairs.
  if(pin->f_stat == i2c_validation_complete){

#ifdef i2c_master

    //  Check per pin value if doesn't set as UNDEFINED.
    if(pin->CSX != UNDEFINED){
        gpio_init(pin->CSX);
        gpio_set_dir(pin->CSX, GPIO_OUT);
    }
    if(pin->RWR != UNDEFINED){
        gpio_init(pin->RWR);
        gpio_set_dir(pin->RWR, GPIO_OUT);
    }
    if(pin->SDA != UNDEFINED){
        gpio_init(pin->SDA);
        gpio_set_function(pin->SDA, GPIO_FUNC_I2C);
    }
    if(pin->SCK != UNDEFINED){
        gpio_init(pin->SCK);
        gpio_set_function(pin->SCK, GPIO_FUNC_I2C);
    }

#endif

#ifdef i2c_slave

    //  Check per pin value if doesn't set as UNDEFINED.
    if(pin->CSX != UNDEFINED){
        gpio_init(pin->CSX);
        gpio_set_dir(pin->CSX, GPIO_IN);
        gpio_set_irq_enabled_with_callback(pin->CSX, 0x04 | 0x08, true, &master_interrupts);
    }
    if(pin->RWR != UNDEFINED){
        gpio_init(pin->RWR);
        gpio_set_dir(pin->RWR, GPIO_IN);
        gpio_set_irq_enabled_with_callback(pin->RWR, 0x04 | 0x08, true, &master_interrupts);
    }
    if(pin->SDA != UNDEFINED){
        gpio_init(pin->SDA);
        gpio_set_function(pin->SDA, GPIO_FUNC_I2C);
    }
    if(pin->SCK != UNDEFINED){
        gpio_init(pin->SCK);
        gpio_set_function(pin->SCK, GPIO_FUNC_I2C);
    }
#endif

    }

  return (pin->f_stat == i2c_validation_complete) ? i2c_gpio_initialised : error_com;

}

enum com_dom i2c_protocol_initialisation(i2c_com *reqs){
    reqs->f_stat = 0;

    reqs->f_stat = i2c_gpio_initialisation(reqs);

    if(reqs->f_stat == i2c_gpio_initialised){

      if(reqs->moss){
        i2c_init(reqs->inst, reqs->baud_rate);
      }else {
        i2c_init(reqs->inst, reqs->baud_rate);
        i2c_set_slave_mode(reqs->inst, reqs->moss, reqs->device_id);
      }
    }else {
        /*
            Display to user error in the gpio initialisation.
        */
    }

    return (reqs->f_stat == i2c_gpio_initialised) ? i2c_valid : error_com;

}

enum com_dom i2c_master_write(i2c_com *regs, uint8_t data_out){

    regs->f_stat = 0;
  for(int i = 7; i > 0; i--){
    regs->tx_buffer[i] = ((data_out >> i) & 0x01);
  }

    i2c_chip_lo
    i2c_write_sel

    regs->f_stat = i2c_write_blocking(regs->inst, regs->device_id, regs->tx_buffer, 1, false);

    i2c_chip_hi

    return (regs->f_stat == 1) ? i2c_tx_valid : error_com;
}

enum com_dom i2c_master_read(i2c_com *regs){

    regs->f_stat = 0;

    i2c_chip_lo
    i2c_read_sel
    
    regs->f_stat = i2c_read_blocking(regs->inst, regs->device_id, regs->rx_buffer, 1, false);

    i2c_chip_hi

    return (regs->f_stat == 1) ? i2c_rx_valid : error_com;
}

enum com_dom i2c_slave_write(i2c_com *regs, uint8_t transfer_out){

    regs->f_stat = 0;

    for(int i = 7; i > 0; i--){
    regs->tx_buffer[i] = ((transfer_out >> i) & 0x01);
  }
    regs->f_stat = i2c_write_blocking(regs->inst, regs->device_id, regs->tx_buffer, 1, false);

    return (regs->f_stat == 1) ? i2c_tx_valid : error_com;
}

enum com_dom i2c_slave_read(i2c_com *regs){

    regs->f_stat = 0;

    regs->f_stat = i2c_read_blocking(regs->inst, regs->device_id, regs->rx_buffer, 1, false);

    return (regs->f_stat == 1) ? i2c_rx_valid : error_com;

}
#endif

#ifdef COM_SPI
//  Define SPI struct here
typedef struct spi_communication_s {
    
    // Required pins first
    uint CSX;
    uint MOSI;
    uint MISO;
    uint SCK;

    //  Required communications needs.
    uint16_t device_id; //  Not required for SPI, but why not?
    uint32_t baud_rate;
    spi_inst *inst;

    //  Uint8_t buffers for holding transmit & receive.
    uint8_t tx_buffer[8];
    uint8_t rx_buffer[8];

    //  Uint8_t value for checking function returns
    uint8_t f_stat;

}spi_com;

enum com_dom spi_pin_validation(spi_com *pin){

    pin->f_stat = 0;

    ebit uip_mo = pin->MOSI;
    ebit uip_mi = pin->MISO;
    ebit uip_cs = pin->CSX;

    bool sec_one;
    bool sec_two;
    bool sec_three;

    bool spi0_valid;
    bool spi1_valid;

    bool invalid_spi;
    invalid_spi = (uip_mo == uip_mi) ? true : false;

    if(!invalid_spi){

        for(int i = 0; i < 3; i++){
            
            sec_one = (uip_mo = spi0_pin_cluster[i][3]) ? true : false;
                pin->f_stat += (sec_one) ? spi_pin_valid : error_com;
            sec_two = (uip_mi = spi0_pin_cluster[i][0]) ? true : false;
                pin->f_stat += (sec_two) ? spi_pin_valid : error_com;
            sec_three = (uip_cs = spi0_pin_cluster[i][1]) ? true : false;
                pin->f_stat += (sec_three) ? spi_pin_valid : error_com;

        if(sec_one && sec_two && sec_three){
            spi0_valid;
        }

        sec_one = false;
        sec_two = false;
        sec_three = false;

        }
            if(!spi0_valid){
                sec_one = false;
                sec_two = false;
                sec_three = false;
            }

        for(int i = 0; i < 2; i++){

            sec_one = (uip_mo = spi1_pin_cluster[i][3]) ? true : false;
                pin->f_stat += (sec_one) ? spi_pin_valid : error_com;
            sec_two = (uip_mi = spi1_pin_cluster[i][0]) ? true : false;
                pin->f_stat += (sec_two) ? spi_pin_valid : error_com;
            sec_three = (uip_cs = spi1_pin_cluster[i][1]) ? true : false;
                pin->f_stat += (sec_three) ? spi_pin_valid : error_com;

        if(sec_one && sec_two && sec_three){
            spi1_valid;
        }

        //  Reset secure bools so as not to increment f_stat accidentally.
        sec_one = false;
        sec_two = false;
        sec_three = false;

        }

    }else {

        //  Display to user that error occured in spi pins.

    }

    pin->inst = (spi0_valid) ? spi0 : spi1;
    pin->f_stat += spi_instance_valid;
    return (pin->f_stat == 93) ? spi_all_pin_valid : error_com;
}

enum com_dom spi_gpio_initialisation(spi_com *pin){

    pin->f_stat = 0;

    pin->f_stat = spi_pin_validation(pin);

    if(pin->f_stat == spi_all_pin_valid){

        if(pin->CSX != UNDEFINED){
            gpio_init(pin->CSX);
            gpio_set_dir(pin->CSX, GPIO_OUT);
        }
        if(pin->SCK != UNDEFINED){
            gpio_init(pin->SCK);
            gpio_set_function(pin->SCK, GPIO_FUNC_SPI);
        }
        if(pin->MOSI != UNDEFINED){
            gpio_init(pin->MOSI);
            gpio_set_function(pin->MOSI, GPIO_FUNC_SPI);
        }
        if(pin->MISO != UNDEFINED){
            gpio_init(pin->MISO);
            gpio_set_function(pin->MISO, GPIO_FUNC_SPI);
        }
    }

    return (pin->f_stat == spi_all_pin_valid) ? spi_gpio_initialised : error_com;

}

enum com_dom spi_protocol_initialisation(spi_com *reqs){
    
    sbit baurat;
    baurat = 0;

    reqs->f_stat = 0;
    reqs->f_stat = spi_gpio_initialisation(reqs);

    if(reqs->f_stat == spi_gpio_initialised){

       baurat = spi_init(reqs->inst, reqs->baud_rate);
        
    }else {
        //  Display to user, error in gpio initialisation.
    }

    return (baurat > 0) ? spi_initialisation_valid : error_com;

}

enum com_dom spi_master_write(spi_com *regs, uint8_t data_out){

    regs->f_stat = 0;
  for(int i = 8; i > 0; i--){
    regs->tx_buffer[i] = ((data_out >> i) & 0x01);
  }

    spi_chip_lo

    regs->f_stat = spi_write_blocking(regs->inst, regs->tx_buffer, 1);

    spi_chip_hi

    return (regs->f_stat == 1) ? spi_tx_valid : error_com;
}

enum com_dom spi_master_read(spi_com *regs){
    regs->f_stat = 0;

    spi_chip_lo

    regs->f_stat = spi_read_blocking(regs->inst, regs->device_id, regs->rx_buffer, 1);

    spi_chip_hi

    return (regs->f_stat == 1) ? spi_rx_valid : error_com;

}

enum com_dom spi_slave_write(spi_com *regs, uint8_t slave_out){

    regs->f_stat = 0;

  for(int i = 8; i > 0; i--){
    regs->tx_buffer[i] =  ((slave_out >> i) & 0x01);
  }
    regs->f_stat = spi_write_blocking(regs->inst, regs->tx_buffer, 1);

    return (regs->f_stat == 1) ? spi_tx_valid : error_com;

}

enum com_dom spi_slave_read(spi_com *regs){

    regs->f_stat = 0;

    regs->f_stat = spi_read_blocking(regs->inst, regs->device_id, regs->rx_buffer, 1);

    return (regs->f_stat == 1) ? spi_rx_valid : error_com;

}

#endif
#endif
#endif