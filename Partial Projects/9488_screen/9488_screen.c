#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ILI9488_4_WIRE_PRO.h"
// #include "ili_all_func_proto.h"

ebit init_success;

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

    sbit device_id;
    ebit status;
    stdio_init_all();
        sleep_ms(7000);
    printf("Seven Second Wait Finished.\n");
        sleep_ms(7000);
      printf("Four Second Wait Finished.\n");
      printf("User Pin Initialization.\n");
    user_pin_initialize((my_init.data_packet), *(&my_init.my_pins));
        sleep_ms(4000);

    button_interrupt_init(&enabled_buttons);
/*
      printf("Run Device ID Reader.\n");
    status = 0;
    device_id = spi_read_data(&data_packet);

   (device_id != 0) ? printf("Read From Device Successful.\n") : printf("Read Error.\n");
        sleep_ms(3000);
*/
  printf("Initializing ILI9488.\n");
  reset_pulse(my_pins.reset);
  printf("Reset Signal Sent.\n");
  sleep_ms(200);
  
  init_success = ili_initialize(&my_init);
 (init_success != 0) ?  printf("The ILI9488 Correctly Initialized with Status: %i.\n\n\n", init_success) : printf("Error In ILI Initilization.\n");

  initialize_spi_management(data_packet.instance, ONE_MBS);
        sleep_ms(6000);

while(1) {

  if(init_success != 7){
    printf("Initilization Failed.\n");
    return 0;
  }
  

  status = ili_set_background_color(&data_packet, 0x0039E6);
  status = 0;
  //  X and Y coords are end side.  Both begin at 0.
  status = ili_fill_color(&data_packet, pixel_cache, 20, 40, 0xFFFF00);
  sleep_ms(4000);

  status = ili_draw_line(&data_packet, 0xCC5200, 10, 10, 30, 60);
  
/*
  printf("Ili Draw String Function Called.\n");
  status = ili_draw_string( *(&my_init.data_packet), 20, 90, "ReitzNow", 0xCC5200);
  printf("ILI String Write Status Return: %i\n", status);
  status = 0;
*/

  printf("\nLoop End.\n\n");
  sleep_ms(2000);
}
tight_loop_contents();
}