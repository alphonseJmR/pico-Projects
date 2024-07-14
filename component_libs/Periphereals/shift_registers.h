#ifndef SHIFT_REGISTER_S_H
#define SHIFT_REGISTER_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "resources/pico_pin_enum.h"
#include "hardware/gpio.h"

void shift_register_pin_init(register_pins *config){

  printf("\n\t\tCD74HC595E Pin Initialization.\n\n");

  printf("Initializing Register One.\n");
  if(config->register_one_data != UNDEFINED){
    printf("Register One Data Initializing.\n\n");
    gpio_init(config->register_one_data);
    gpio_set_dir(config->register_one_data, GPIO_OUT);
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_data);
  }
  if(config->register_one_latch != UNDEFINED){
    gpio_init(config->register_one_latch);
    gpio_set_dir(config->register_one_latch, GPIO_OUT);
    printf("Register One Latch Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_latch);
  }
  if(config->register_one_enable != UNDEFINED){
    gpio_init(config->register_one_enable);
    gpio_set_dir(config->register_one_enable, GPIO_OUT);
    printf("Register One Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_one_enable);
  }
  printf("Initializing Register Two.\n");
  if(config->register_two_data != UNDEFINED){
    gpio_init(config->register_two_data);
    gpio_set_dir(config->register_two_data, GPIO_OUT);
    printf("Register Two Data Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_data);
  }
  if(config->register_two_latch != UNDEFINED){
    gpio_init(config->register_two_latch);
    gpio_set_dir(config->register_two_latch, GPIO_OUT);
    printf("Register Two Latch Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_latch);
  }
  if(config->register_two_enable != UNDEFINED){
    gpio_init(config->register_two_enable);
    gpio_set_dir(config->register_two_enable, GPIO_OUT);
    printf("Register Two Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", config->register_two_enable);
  }
  if(config->register_clk_line != UNDEFINED){
    gpio_init(config->register_clk_line);
    gpio_set_dir(config->register_clk_line, GPIO_OUT);
    printf("Register Clk Line Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n\n", config->register_clk_line);
  }
  printf("Register pins initialized.\n");
}

//  Function to create pulse for shift latching.
//  Takes a uint GPIO pin to allow multiple shift registers.
void latch_data(register_pins *config) {
  if(config->register_one_latch != UNDEFINED){
    gpio_put(config->register_one_latch, 1); // Generate latch pulse for register one.
      sleep_ms(2);
    gpio_put(config->register_one_latch, 0);
  }
  if(config->register_two_latch != UNDEFINED){
    gpio_put(config->register_two_latch, 1); // Generate latch pulse for register two.
      sleep_ms(2);
    gpio_put(config->register_two_latch, 0);
  }
}

//  Function to create pulse for clock pulses.
//  Takes a uint GPIO pin to allow multiple shift registers.
void clk_pulse(register_pins *config) {

  if(config->register_clk_line != UNDEFINED){
    gpio_put(config->register_clk_line, 1);
      sleep_ms(2);
    gpio_put(config->register_clk_line, 0);
  }
}

//  Function takes register_pins struct, uint8_variable data to output, int between 0-7 for both selections.  Use -1 on either for non-use.
void serial_register_output(register_pins *config, uint8_variables *vars, int register_selection, int reg_two_sel) {

  printf("Serial Register Function Initialized.\n\n");
    //  Set display enable high for duration of data transfer.
    if(config->register_one_data != UNDEFINED){
  printf("Register One Data Line Outputting.\n");
  printf("Currently outputting register %i.\n", register_selection);
    gpio_put(config->register_one_enable, 1);
      switch(register_selection){

        case -1:
          printf("Register One Not Selected.\n");
          break;

        case 0:
        //  Create loop to shift data into selected register output.
        printf("Register ZERO selected.\n");
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_zero >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_zero >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 1:
        //  Create loop to shift data into selected register output.
        printf("Register ONE selected.\n");
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_one >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_one >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 2:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_two >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_two >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 3:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_three >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_three >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 4:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_four >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_four >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 5:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_five >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_five >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 6:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_six >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_six >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 7:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_seven >> a) & 0x01) ? gpio_put(config->register_one_data, 1) : gpio_put(config->register_one_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_seven >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_one_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        default:
          printf("Shift Register One Not Selected / Error Occured.\n");

    }
    }

    if(config->register_two_data != UNDEFINED){
  printf("Register One Data Outputting.\n");
    gpio_put(config->register_two_enable, 1);

            switch(reg_two_sel){

              case -1:
              printf("Register Two Not Used.\n");
              break;

        case 0:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_zero >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_zero >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 1:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_one >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_one >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 2:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_two >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_two >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 3:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_three >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_three >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 4:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_four >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_four >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 5:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_five >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_five >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 6:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_six >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_six >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        case 7:
        //  Create loop to shift data into selected register output.
          for(int a = 0; a < 8; a++){
        //  Put data_in line high or low, pulse register clock.
        //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
          ((vars->register_value_seven >> a) & 0x01) ? gpio_put(config->register_two_data, 1) : gpio_put(config->register_two_data, 0);
  printf("Shifting: 0b%01x.\n", (vars->register_value_seven >> a));
          clk_pulse(config);                          //  Pulse clock per iteration.              
    }
            latch_data(config);                               //  After loop iterates, call latch function to pulse register latch.
            clk_pulse(config);                                //  Pulse clock finish input loop.
            gpio_put(config->register_two_enable, 0);         //  Set display enable low to enable register output pins.
        break;

        default:
          printf("Shift Register Two Not Selected / Error Occured.\n");

    }
    
    }

}

#endif
