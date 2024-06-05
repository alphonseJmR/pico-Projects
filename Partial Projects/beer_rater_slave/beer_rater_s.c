#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define i2c_slave
#define COM_I2C
#define IPC_ERROR_HANDLER
#include "pico/stdlib.h"
#include "beer_list.h"
#include "IPC.h"
#include "error_manager.h"

bool slave_read;
bool slave_write;
bool slave_fave;
bool slave_gros;

i2c_com my_coms = {

  .CSX = 2,
  .RWR = 3,
  .SDA = 6,
  .SCK = 7,

  .device_id = 0x3D,
  .baud_rate = (100 * 1000),
  .moss = false

};

adc_port_values pico_adc = {
  .adc0_pin = 26,
  .adc1_pin = 27,
  .adc2_pin = (int)NULL,
  .adc0_raw_read = 0,
  .adc1_raw_read = 0,
  .adc2_raw_read = 0,
  .adc0_max_in_map_value = 4095,
  .adc0_min_in_map_value = 0,
  .adc0_min_out_map_value = 0,
  .adc0_max_out_map_value = 255,
  .adc1_max_in_map_value = 4095,
  .adc1_min_in_map_value = 0,
  .adc1_min_out_map_value = 0,
  .adc1_max_out_map_value = 255,
  .adc2_max_in_map_value = 4095,
  .adc2_min_in_map_value = 0,
  .adc2_min_out_map_value = 0,
  .adc2_max_out_map_value = 255
};

bool pref_sel;

int main(){

    stdio_init_all();
    sleep_ms(7000);
    sleep_ms(3000);

    adc_init();
    adc_pin_setup(&pico_adc);
        sleep_ms(1000);
  
    i2c_protocol_initialisation(&my_coms);
    printf("\n\n\n");
while(1){

  while(my_slave.wake_state){

    if(pref_sel){

      slave_fave = true;

      //  Pref_sel true means Faves have been selected.

    if(my_slave.read_write_state){
      //  If true, write values to master.

    slave_write = true;

      top_fivers(true, &my_pairs);

      slave_to_master(&my_pairs);

    }else{
      //  else read values from master.

      slave_to_master(&my_pairs);

    }

    }else {

      slave_fave = false;
      //  Pref_sel true means Faves have been selected.

    if(my_slave.read_write_state){
      //  If false, write grosses values to master.

      slave_write = true;

      top_fivers(false, &my_pairs);

      slave_to_master(&my_pairs);

    }else{
      //  else read values from master.

      slave_to_master(&my_pairs);

    }
    }
  while(!my_slave.wake_state){

    tight_loop_contents();

  }


}
}
}
