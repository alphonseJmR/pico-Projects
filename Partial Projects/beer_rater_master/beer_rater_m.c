#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "beer_list.h"
#include "beer_peri.h"
#include "_2004a_i2c.h"
#define COM_I2C
#define IPC_ERROR_HANDLER

#include "IPC.h"
#include "error_manager.h"

i2c_com my_coms = {

  .CSX = 6,
  .RWR = 7,
  .SDA = 4,
  .SCK = 5,

  .device_id = 0x5C,
  .baud_rate = (100 * 1000),
  .moss = true

};

button_types enabled_buttons = {
  .rotary_button_clk = GPIO_FIFTEEN,
  .rotary_button_dt = GPIO_FOURTEEN,
  .rotary_button_button = GPIO_THIRTEEN,    //  LCD Backlight button.
  .analog_button_one = (int)NULL,
  .analog_button_two = (int)NULL,
  .button_one = GPIO_SIXTEEN,       //  UP button.
  .button_two = GPIO_EIGHTEEN,     //  DOWN button.
  .button_three = GPIO_TWO,    //  ENTER button.
  .button_four = GPIO_TWENTY,     //  RESET button.
  .button_five = GPIO_FOUR        //  BACK button.
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

rotary_encoder_struct rotary = {

    .minimum_required_interrupt = 1700,
    .max_rotation_value = 20

};

interrupt_times_t callback_times = {
    .interruption_max = 1500
};


int main(){
    ebit rturned;
    vol_boos.up = false;
    vol_boos.down = false;
    vol_boos.backlight = false;
    vol_boos.back = false;
    vol_boos.enter = false;
    vol_boos.reset = false;
    debug = false;

    ebit test_value;
    test_value = 0;

    stdio_init_all();
    sleep_ms(7000);
    sleep_ms(3000);
    adc_init();
    adc_pin_setup(&pico_adc);
        sleep_ms(1000);
        printf("LCD Initialization.\n");
    rturned = initialize_lcd(&my_lcd);
    printf("rturned should equal 1:%i.\n", rturned);
        sleep_ms(1000);
    rotary_encoder_init(&enabled_buttons);
        sleep_ms(1000);
    
    button_interrupt_init(&enabled_buttons);
    sleep_ms(1000);

    i2c_protocol_initialisation(&my_coms);

    program_intro_display();
    printf("\n\n\n");
    value_inits(&my_bools, &my_vars, &my_pairs);
    printf("\n\n\n");
    
while(1){

//  Run loop while reset bool is not true.
while(!my_bools.nv_reset){

    // Initialize menu selection variable to 0.
    menu_selection = 1;
  while(my_bools.top_menu){

    test_value = i2c_master_write(&my_coms, 0x93);

    my_vars.t_menu_id = menu_selection;
        printf("Current Top Menu Pos: %i.\n", my_vars.t_menu_id);
    if(my_bools.nv_enter){
          printf("ENTER valid.\n");
        menu_bool_setter(&my_vars, &my_bools);
    }else{
          printf("\n\n\n\nTHIS IS THE ELSE STATEMENT, ENTER ISN'T TRUE\n\n\n\n");
        
        top_selection_menu(&my_menus, my_vars.t_menu_id);
      if(my_bools.nv_back){
        my_bools.nv_back = false;
      }
    }
    sleep_ms(100);
  }
    
    menu_selection = 1;
  while(my_bools.styles_menu){

    my_vars.style_id = menu_selection;
    printf("Current Style Pos: %i.\n", my_vars.style_id);

  if(!my_bools.nv_back){
  if(!my_bools.nv_enter){
    
    style_selection_menu(&my_vars, &my_menus, my_vars.style_id);

  }else{
    my_vars.mri_status[1][0][0] = menu_selection;

    if(my_vars.style_id == 0){
      my_bools.styles_menu = true;
      my_bools.nv_enter = false;

    }else{
      my_bools.beers_menu = true;
      my_bools.styles_menu = false;
      my_bools.nv_enter = false;

    }
  }
  }else {
    my_bools.top_menu = true;
    my_bools.styles_menu = false;
    my_bools.nv_back = false;

  }
}
    
    menu_selection = 1;
  while(my_bools.beers_menu){    // This will select which beer will be displayed for data collection.
    
    my_vars.beer_id = menu_selection;
    printf("Current Beer Selection: %i\n", my_vars.beer_id);

  if(!my_bools.nv_back){
    if(!my_bools.nv_enter){
      
      beer_selection_menu(&my_vars, &my_menus, my_vars.beer_id);

      }else{
        my_vars.mri_status[1][1][0] = menu_selection;

        my_bools.data_menu = true;
        my_bools.beers_menu = false;
        my_bools.nv_enter = false;

      }
    }else{

        my_bools.beers_menu = false;
        my_bools.styles_menu = true;
        my_bools.nv_back = false;

    }
  }
    
    menu_selection = 1;
  while(my_bools.data_menu){

    my_vars.sense_id = menu_selection;
    
    if(!my_bools.nv_back){
      if(!my_bools.nv_enter){

        data_selector_menu(&my_vars, &my_menus, my_vars.sense_id);

      }else {

        my_vars.mri_status[1][1][1] = menu_selection;

        my_bools.data_entry_menu = true;
        my_bools.data_menu = false;
        my_bools.nv_enter = false;

      }
    }else {
        my_bools.data_menu = false;
        my_bools.beers_menu = true;
        my_bools.nv_back = false;
    }
  }

    menu_selection = 1;
  while(my_bools.data_entry_menu){

  if(!my_bools.nv_back){
      if(!my_bools.nv_enter){

            /*
                before calling sense menu, read previous score value, and enter
                it into previous_score array.
            */

        sense_menu(&my_vars, &my_menus);
        
      }else {

        my_vars.previous_scores[my_vars.style_id][my_vars.beer_id][my_vars.sense_id][1] = rotary.rotary_total;
            rotary.rotary_total = 0;
            my_bools.nv_enter = false;

            /*
                Upon enter of data, call function to store rotary encoder's value for beer score into
                struct / array / struct of arrays / array of structs??

            */
        }

  }else {
    my_bools.data_entry_menu = false;
    my_bools.data_menu = true;
    my_bools.nv_back = false;
  }
  }


  while(my_bools.faves_menu){
    #define M_Read
    #define M_Faves

    ebit current_sel = menu_selection;
      
    //  Perfgrm Master_to_slave read.
    master_to_slave(0, 0, &my_pairs);

    if(!my_bools.nv_back){

      faves_menu(&my_pairs, current_sel);
        /*

            Use this while to select & display the top five beers, read from the MEMPICO
        
        */

    }else {
        my_bools.faves_menu = false;
        my_bools.top_menu = true;
        my_bools.nv_back = false;
    }
  }


  while(my_bools.gross_menu){
    if(!my_bools.nv_back){

        /*

            Use this while to select & display the lowest five beers, read from the MEMPICO
        
        */

    }else {
        my_bools.gross_menu = false;
        my_bools.top_menu = true;
        my_bools.nv_back = false;
    }
  }


  while(my_bools.menu_about){

    if(!my_bools.nv_back){

    set_lcd_cursor(0,0);
     print_lcd("       ABOUT        ");
    set_lcd_cursor(1,0);
     print_lcd("Beer Rater Program  ");
    set_lcd_cursor(2,0);
     print_lcd("Written By: Alphonse");
    set_lcd_cursor(3,0);
     print_lcd("Current Version 1.1 ");
    }else {
        my_bools.menu_about = false;
        my_bools.top_menu = true;
    }

  }
    
  while(debug){

    if(!my_bools.nv_back){

      debug_display(&my_menus, &my_vars);

    }else {

      debug = false;
      my_bools.top_menu = true;

    }


  }
}

    if(my_bools.nv_reset){
    printf("Resetting program.\n");
    value_inits(&my_bools, &my_vars, &my_pairs);
    sleep_ms(1500);
    my_bools.nv_reset = false;
    }
}
}
