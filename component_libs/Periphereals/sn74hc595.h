#ifndef SN74HC595_SHIFT_REGISTER_S_H
#define SN74HC595_SHIFT_REGISTER_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "resources/pico_pin_enum.h"


typedef struct register_595_pin_s {

    const uint8_t DATA_LINE_ONE;
    const uint8_t LATCH_LINE_ONE;
    const uint8_t ENABLE_LINE_ONE;

    const uint8_t DATA_LINE_TWO;
    const uint8_t LATCH_LINE_TWO;
    const uint8_t ENABLE_LINE_TWO;

    const uint8_t REGISTERS_CLK_LINE;

} registered_pins;


//  Return of 0x02 implies success, 0x01 implies error.
uint8_t register_pin_initialisation(registered_pins *pins){
    printf("\n\n////\tCD74HC595 Pin Initialisation Begins.////\n\n");
    uint8_t pindition;
    pindition = 0;

      printf("Initializing Register One.\n");
  if(pins->DATA_LINE_ONE != UNDEFINED){
    printf("Register One Data Initializing.\n\n");
    gpio_init(pins->DATA_LINE_ONE);
    gpio_set_dir(pins->DATA_LINE_ONE, GPIO_OUT);
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->DATA_LINE_ONE);
    pindition += pins->DATA_LINE_ONE;
  }
  if(pins->LATCH_LINE_ONE != UNDEFINED){
    gpio_init(pins->LATCH_LINE_ONE);
    gpio_set_dir(pins->LATCH_LINE_ONE, GPIO_OUT);
    printf("Register One Latch Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->LATCH_LINE_ONE);
    pindition += pins->LATCH_LINE_ONE;
  }
  if(pins->ENABLE_LINE_ONE != UNDEFINED){
    gpio_init(pins->ENABLE_LINE_ONE);
    gpio_set_dir(pins->ENABLE_LINE_ONE, GPIO_OUT);
    printf("Register One Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->ENABLE_LINE_ONE);
    pindition += pins->ENABLE_LINE_ONE;
  }
  printf("Initializing Register Two.\n");
  if(pins->DATA_LINE_TWO != UNDEFINED){
    gpio_init(pins->DATA_LINE_TWO);
    gpio_set_dir(pins->DATA_LINE_TWO, GPIO_OUT);
    printf("Register Two Data Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->DATA_LINE_TWO);
    pindition += pins->DATA_LINE_TWO;
  }
  if(pins->LATCH_LINE_TWO != UNDEFINED){
    gpio_init(pins->LATCH_LINE_TWO);
    gpio_set_dir(pins->LATCH_LINE_TWO, GPIO_OUT);
    printf("Register Two Latch Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->LATCH_LINE_TWO);
    pindition += pins->LATCH_LINE_TWO;
  }
  if(pins->ENABLE_LINE_TWO != UNDEFINED){
    gpio_init(pins->ENABLE_LINE_TWO);
    gpio_set_dir(pins->ENABLE_LINE_TWO, GPIO_OUT);
    printf("Register Two Enable Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n", pins->ENABLE_LINE_TWO);
    pindition += pins->ENABLE_LINE_TWO;
  }
  if(pins->REGISTERS_CLK_LINE != UNDEFINED){
    gpio_init(pins->REGISTERS_CLK_LINE);
    gpio_set_dir(pins->REGISTERS_CLK_LINE, GPIO_OUT);
    printf("Register Clk Line Initializing.\n\n");
    printf("GPIO PIN %d Initialized.\n\tDirection: OUTPUT.\n\n", pins->REGISTERS_CLK_LINE);
    pindition += pins->REGISTERS_CLK_LINE;
  }
  printf("Register pins initialized.\n");

    return (pindition < 100) ? 0x02 : 0x01;
}

void clock_pulse(registered_pins *clk_line){

    gpio_put(clk_line->REGISTERS_CLK_LINE, 1);
        sleep_ms(2);
    gpio_put(clk_line->REGISTERS_CLK_LINE, 0);
      sleep_ms(2);

}

void latch_register(registered_pins *latch, bool which){

    if(!which){
        gpio_put(latch->LATCH_LINE_ONE, 1);
            sleep_ms(2);
        gpio_put(latch->LATCH_LINE_ONE, 0);
          sleep_ms(2);
    }else{
        gpio_put(latch->LATCH_LINE_TWO, 1);
            sleep_ms(2);
        gpio_put(latch->LATCH_LINE_TWO, 0);
    }
}

void register_595_output(registered_pins *pins, uint8_t data_input, bool reg_sel, uint8_t delay_test){

    //  Current delay_test value: 20 (ms).

  printf("\n\n////\tOutputting to register.\t////\n\n");

    if(!reg_sel){
        printf("////\tRegister One Enabled.\t////\n");
        gpio_put(pins->ENABLE_LINE_ONE, 1);

        for(int i = 0; i < 8; i++){
            gpio_put(pins->DATA_LINE_ONE, (((data_input >> i) & 0x01) ? 1 : 0));
            printf("////\tDataOut: %i\tBit: %i.\t////\n", (((data_input >> i) & 0x01) ? 1 : 0), i);
            clock_pulse(pins);
              sleep_ms(delay_test);
            

        }
        latch_register(pins, 0);
        // latch_register(pins, 0);
        clock_pulse(pins);
        gpio_put(pins->ENABLE_LINE_ONE, 0);
    }else 
    {
        printf("////Register Two Enabled.////\n");
        gpio_put(pins->ENABLE_LINE_TWO, 1);

        for(int i = 0; i < 8; i++){
            gpio_put(pins->DATA_LINE_TWO, (((data_input >> i) & 0x01) ? 1 : 0));
            clock_pulse(pins);
        }
        latch_register(pins, 1);
        clock_pulse(pins);
        gpio_put(pins->ENABLE_LINE_TWO, 0);
    }

    printf("\n///\tEnd Of Register Output\t////\n\n");
}

void snhc595_test(registered_pins *pins, const uint8_t *test_inputs){

  printf("\n\n////\tTesting Various Inputs and delays for 595 function\t////\n");

  register_595_output(pins, test_inputs[0], false, test_inputs[1]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[2], false, test_inputs[3]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[4], false, test_inputs[5]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[6], false, test_inputs[7]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[8], false, test_inputs[9]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[10], false, test_inputs[11]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[12], false, test_inputs[13]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[14], false, test_inputs[15]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[16], false, test_inputs[17]);
    sleep_ms(500);
  register_595_output(pins, test_inputs[18], false, test_inputs[19]);
    sleep_ms(500);

}


#endif
