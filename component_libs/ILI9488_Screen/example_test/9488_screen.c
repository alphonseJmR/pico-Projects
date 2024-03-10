#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "peri_header.h"
#include "hardware/gpio.h"
#include "ili_all_func_proto.h"

fun init_success;

// Declared Structs

button_types enabled_buttons = {
    .rotary_button_clk = UNDEFINED,
    .rotary_button_dt = UNDEFINED,
    .rotary_button_button = UNDEFINED,
    .analog_button_one = UNDEFINED,
    .analog_button_two = UNDEFINED,
    .button_one = GPIO_SIXTEEN,  // Button one will be the program reset button.
    .button_two = GPIO_SEVENTEEN,
    .button_three = UNDEFINED
};


interrupt_times_t callback_times = {

    .button_max = 1200

};


int main(){

  fud device_id;

    stdio_init_all();
    sleep_ms(7000);
    printf("Seven Second Wait Finished.\n");
    sleep_ms(7000);
    printf("Four Second Wait Finished.\n");

    fun status;
  
    printf("User Pin Initialization.\n");
  user_pin_initialize((my_init.data_packet), *(&my_init.my_pins));
  sleep_ms(4000);

  button_interrupt_init(&enabled_buttons);

  printf("Run Device ID Reader.\n");
  status = 0;
  status = spi_write_blocking(data_packet.instance, Read_ID_TWO, 1);
    if(status != 0){
   device_id = spi_read_data(&data_packet);

   (device_id != 0) ? printf("Read From Device Successful.\n") : printf("Read Error.\n");
    }
    
   sleep_ms(3000);

  printf("Initializing ILI9488.\n");
  init_success = ili_initialize(&my_init);
 (init_success > 0) ?  printf("The ILI9488 Correctly Initialized with Status: %i.\n\n\n", init_success) : printf("Error In ILI Initilization.\n");

  sleep_ms(6000);

while(1) {

  if(init_success != 10){
    printf("Initilization Failed.\n");
    return 0;
  }

  printf("Ili Fill Function Called.\n");
  status = ili_fill(0x006400u, LPa_DCSRR_L, *(&my_init.data_packet));
  printf("ILI Fill Status Return: %i\n", status);
  status = 0;

  status = ili_delay(1000);
  printf("Delay Function Return Test: %i\n", status);
  status = 0;
  sleep_ms(1000);

  printf("Ili Draw String Function Called.\n");
  status = ili_draw_string(20, 100, "ReitzNow", *(&my_init.my_parts), LPa_DCSRR_L, *(&my_init.data_packet));
  printf("ILI String Write Status Return: %i\n", status);
  status = 0;

  printf("\nLoop End.\n\n");
  sleep_ms(7000);
}
tight_loop_contents();
}