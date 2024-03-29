#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "peri_header.h"
#include "beer_list.h"
#include "beer_peri.h"
#include "_2004a_i2c.h"
#include "error_manager.h"

#define ebit uint8_t

volatile uint8_t menu_selection;
uint8_t previous_menu_sel;
uint8_t item_selector_val, menu_selector_val;
uint8_t menuSelected;
uint8_t prev_item, item_sel;
uint8_t newMenu, itemTwo_val;

bool t_menu;
bool s_menu;
bool f_menu;
bool g_menu;
bool faves;
bool grosses;
bool beer_menu;
bool about;
bool data_menu;
bool d_entry;
bool style_selected;
bool style_m_reset;

ebit t_menu_pos;    //  The next few values will be incremented / decremented by UP/DOWN buttons.
ebit s_menu_pos;
ebit style_pos;
ebit fav_pos;
ebit gross_pos;

//  Attributed C on these values is short for Current.
//  Current Top menu, current S menu, current Style Menu, Current Data menu.
ebit ct_menu, cs_menu, css_menu, cd_menu, css_menu_pos;

//  Use these variables to store the most recent ENTER value per menu.
ebit top_m, style_m, fave_m, gross_m, data_m, data_e_m;


ebit beer_menu_pos; //  Used by beer_array_menu to select the beer from the first [] menus styles. 
ebit data_menu_pos; //  Used by beer_array_menu to select the data input from the 2nd(3rd?) [].

volatile bool up;
volatile bool down;
volatile bool backlight;
volatile bool back;
volatile bool enter;
volatile bool reset;
bool styles_arr[16];

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

void menu_highlighter(char input_arr[]){
    
        if(input_arr[19] == (char)' '){
            input_arr[19] = '-';
        }

        if(input_arr[18] == (char)' '){
            input_arr[18] = '<';
        }

    sleep_ms(10);
}

void menu_dehighlighter(char in[]){

        if(in[19] == (char)'-'){
            in[19] = ' ';
        }

        if(in[18] == (char)'<'){
            in[18] = ' ';
        }
    
    sleep_ms(10);
}


void interrupt(uint gpio, uint32_t events){

    gpio_acknowledge_irq(gpio, events);
    printf("\n\nNew Interrupt Callback Function Working.\n\n");
    printf("Pin: %i activated\n", gpio);
    
  //  Main callback interruption time.
  callback_times.current_interruption = time_us_32();
  callback_times.last_interruption = callback_times.current_interruption - callback_times.initial_interruption; 

  // Callback time for rotary encoder interruption time (rei).
  callback_times.rei_current_interrupt = time_us_32();
  callback_times.rotary_last = callback_times.rei_current_interrupt - callback_times.rotary_initial;

  // Callback time for enabled buttons interruption time.
  callback_times.bcb_current_interrupt = time_us_32();
  callback_times.button_last = callback_times.bcb_current_interrupt - callback_times.button_initial;

  // printf("\nInterruption occured:\n\tLast Interrupt: %u.\n", callback_times.last_interruption);
    callback_times.initial_interruption = callback_times.current_interruption;
    
    if(callback_times.last_interruption >= callback_times.interruption_max){
    
    if(callback_times.rotary_last > rotary.minimum_required_interrupt){
    callback_times.rotary_initial = callback_times.rei_current_interrupt;
  
    if(gpio == enabled_buttons.rotary_button_clk && (events & 0x04)){
      rotary.current_rotary_clk = gpio_get(enabled_buttons.rotary_button_clk);
      rotary.current_rotary_dt = gpio_get(enabled_buttons.rotary_button_dt);
      
      if(rotary.current_rotary_clk != rotary.previous_rotary_clk){
        if(rotary.current_rotary_dt != rotary.current_rotary_clk){
        //  printf("B != A.\n");
            if(rotary.rotary_total <= rotary.max_rotation_value){
          rotary.rotary_total++;
            }else {
              printf("Maximum Rotation Value.\n");
            }
        }else {
          //  printf("B == A.\n");
            if(rotary.rotary_total > 0){
            rotary.rotary_total--;
            } else {
              printf("Minimum Rotation Value.\n");
          }     
      }
    }
    }
    rotary.previous_rotary_clk = rotary.current_rotary_clk;
    rotary.previous_rotary_dt = rotary.current_rotary_dt;
    } 
  
    if(gpio == enabled_buttons.button_one){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nUP Button Pressed.\n");
            if(menu_selection > 0){
        menu_selection--;
            }else {
                printf("Top of menu.\n");
            }
        
    }else if(gpio == enabled_buttons.button_two){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nDOWN Button Pressed.\n");

        if(t_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of t_menu.\n");
            }
        }else if(s_menu){
            if(menu_selection < 13){
                menu_selection++;
            }else{
                printf("Bottom of s_menu.\n");
            }
        }else if(f_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else{
                printf("Bottom of f_menu.\n");
            }
        }else if(g_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of g_menu.\n");
            }
        }else if(style_selected){
            if(menu_selection < 13){
                menu_selection++;
            }else {
                printf("Bottom of styles_menu.\n");
            }
        }else if(data_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of styles_menu.\n");
            }
        }else if(d_entry){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of styles_menu.\n");
            }
        }else {
            printf("\n\nMenu's bool selection error.\n\n");
        }
        printf("Current menu_selection value: %i.\n", menu_selection);
    }else if(gpio == enabled_buttons.button_three){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nENTER Button Pressed.\n");
        
        enter = true;

    }else if(gpio == enabled_buttons.button_four){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nRESET Button Pressed.\n");
        
        reset = true;

    }else if(gpio == enabled_buttons.button_five){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nBACK Button Pressed.\n");
        
        back = true;

    }else if(gpio == enabled_buttons.rotary_button_button){

      printf("Rotary Button Press Occured.\n");
      gpio_acknowledge_irq(gpio, events);
      callback_times.button_initial = callback_times.bcb_current_interrupt;
      if(callback_times.button_last > callback_times.button_max){
        callback_times.button_initial = callback_times.bcb_current_interrupt;
        button_status.rotary_button_status = true;
      }
    }else{
        printf("\n\n\nNo Enabled Button Was Pressed.  Find Error.\n\n\n");
    }
  }
   
}


//  This just sets bool values and ebit values to program initialisation values.
void value_inits(void){

    printf("Top Menu set as true.\n");
    t_menu = true;
        sleep_ms(100);

    printf("Styles Menu set as false.\n");
    s_menu = false;
        sleep_ms(100);

    printf("Faves Menu set as false.\n");
    f_menu = false;
        sleep_ms(100);

    printf("Grosses Menu set as false.\n");
    g_menu = false;
        sleep_ms(100);

    printf("About Menu set as false.\n");
    about = false;
        sleep_ms(100);

    printf("Style Selected set as false.\n");
    style_selected = false;
        sleep_ms(100);

    printf("Data Menu set as false.\n");
    data_menu = false;
        sleep_ms(100);

    printf("Data entry set as false.\n");
    d_entry = false;
        sleep_ms(100);

    printf("RESET set as FALSE.\n");
    reset = false;
        sleep_ms(100);

    printf("ENTER set as FALSE.\n");
    enter = false;
        sleep_ms(100);

    printf("BACK set as FALSE.\n");
    back = false;
        sleep_ms(100);

    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 7; j++){
            for(int k = 0; k < 5; k++){

            previous_score[i][j][k][0] = 0;

            }
        }
    }


    t_menu_pos, s_menu_pos, fav_pos, gross_pos, style_pos, data_menu_pos = 0;
    ct_menu, cs_menu, css_menu, cd_menu = 0;
    menu_selection = 1;

    sleep_ms(750);

}

//  Display intro text on the LCD.
void program_intro_display(void){
    
    sleep_ms(3000);
                set_lcd_cursor(0,0);;
        print_lcd("Beer rate for dudes");
            set_lcd_cursor(1,0);
        print_lcd("Silly saffrinola");
            set_lcd_cursor(2,0);
        print_lcd("Hoighty Toighty");
            set_lcd_cursor(3,0);
        print_lcd("Eat my shorts man");
    sleep_ms(1500);
            
}

int main(){
    ebit rturned;

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

    program_intro_display();
    printf("\n\n\n");
    value_inits();
    printf("\n\n\n");
while(1){

//  Run loop while reset bool is not true.
while(!reset){
    // Initialize menu selection variable to 0.
    menu_selection = 0;
  
  while(t_menu){

    my_vars.t_menu_id = menu_selection;

    printf("Current t_menu_pos: %i.\n", t_menu_pos);

    if(enter){
        printf("ENTER valid.\n");

    menu_bool_setter(&my_vars, &my_bools);

    }else{

    
            printf("\n\n\n\nTHIS IS THE ELSE STATEMENT, ENTER ISN'T TRUE\n\n\n\n");
        
        if(back){
            back = false;
        }

        switch(t_menu_pos){

            case 1:     // Style Menu Selected.
                set_lcd_cursor(0,0);
                 print_lcd(menu_arr[0]);
                set_lcd_cursor(1,0);
                  menu_highlighter(menu_arr[1]);
                 print_lcd(menu_arr[1]);
                  menu_dehighlighter(menu_arr[1]);
                set_lcd_cursor(2,0);
                 print_lcd(menu_arr[2]);
                set_lcd_cursor(3,0);
                 print_lcd(menu_arr[3]);
            break;

            case 2:     //  Faves menu selected.
                set_lcd_cursor(0,0);
                 print_lcd(menu_arr[0]);
                set_lcd_cursor(1,0);
                 print_lcd(menu_arr[1]);
                set_lcd_cursor(2,0);
                    menu_highlighter(menu_arr[2]);
                 print_lcd(menu_arr[2]);
                    menu_dehighlighter(menu_arr[2]);
                set_lcd_cursor(3,0);
                 print_lcd(menu_arr[3]);

            break;

            case 3:     // Grosses menu selected.
                set_lcd_cursor(0,0);
                 print_lcd(menu_arr[0]);
                set_lcd_cursor(1,0);
                 print_lcd(menu_arr[1]);
                set_lcd_cursor(2,0);
                 print_lcd(menu_arr[2]);
                set_lcd_cursor(3,0);
                    menu_highlighter(menu_arr[3]);
                 print_lcd(menu_arr[3]);
                    menu_dehighlighter(menu_arr[3]);

            break;

            case 4:  // About Menu Selection.
                set_lcd_cursor(0,0);
                 print_lcd(menu_arr[0]);
                set_lcd_cursor(1,0);
                 print_lcd(menu_arr[2]);
                set_lcd_cursor(2,0);
                 print_lcd(menu_arr[3]);
                set_lcd_cursor(3,0);
                    menu_highlighter(menu_arr[4]);
                 print_lcd(menu_arr[4]);
                    menu_highlighter(menu_arr[4]);

            break;

            default:
                
                printf("Defaulted Top Menu Selection.\n");
            break;
        }

    }
    sleep_ms(100);
  }
    
    menu_selection = 1;
  while(s_menu){
    s_menu_pos = menu_selection;
    printf("Current s_menu_pos: %i.\n", s_menu_pos);

  if(!back){
  if(!enter){
    switch(s_menu_pos){

        case 1:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
                menu_highlighter(style_arr[1]);
             print_lcd(style_arr[1]);
                menu_dehighlighter(style_arr[1]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[2]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[3]);
        break;

        case 2:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[1]);
            set_lcd_cursor(2,0);
                menu_highlighter(style_arr[2]);
             print_lcd(style_arr[2]);
                menu_dehighlighter(style_arr[2]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[3]);
        break;

        case 3:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[1]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[2]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[3]);
             print_lcd(style_arr[3]);
                menu_dehighlighter(style_arr[3]);
        break;

        case 4:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
                menu_highlighter(style_arr[4]);
             print_lcd(style_arr[4]);
                menu_dehighlighter(style_arr[4]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[5]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[6]);
        break;

        case 5:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[4]);
            set_lcd_cursor(2,0);
                menu_highlighter(style_arr[5]);
             print_lcd(style_arr[5]);
                menu_dehighlighter(style_arr[5]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[6]);
        break;

        case 6:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[4]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[5]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[6]);
             print_lcd(style_arr[6]);
                menu_dehighlighter(style_arr[6]);
        break;

        case 7:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
                menu_highlighter(style_arr[7]);
             print_lcd(style_arr[7]);
                menu_dehighlighter(style_arr[7]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[8]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[9]);
        break;

        case 8:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[7]);
            set_lcd_cursor(2,0);
                menu_highlighter(style_arr[8]);
             print_lcd(style_arr[8]);
                menu_dehighlighter(style_arr[8]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[9]);
        break;

        case 9:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[7]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[8]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[9]);
             print_lcd(style_arr[9]);
                menu_dehighlighter(style_arr[9]);
        break;

        case 10:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
                menu_highlighter(style_arr[10]);
             print_lcd(style_arr[10]);
                menu_dehighlighter(style_arr[10]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[11]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[12]);
        break;

        case 11:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[10]);
            set_lcd_cursor(2,0);
                menu_highlighter(style_arr[11]);
             print_lcd(style_arr[11]);
                menu_dehighlighter(style_arr[11]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[12]);
        break;

        case 12:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[10]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[11]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[12]);
             print_lcd(style_arr[12]);
                menu_dehighlighter(style_arr[12]);
        break;

        case 13:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
                menu_highlighter(style_arr[13]);
             print_lcd(style_arr[13]);
                menu_dehighlighter(style_arr[13]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[14]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[15]);
        break;

        case 14:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[13]);
            set_lcd_cursor(2,0);
                menu_highlighter(style_arr[14]);
             print_lcd(style_arr[14]);
                menu_dehighlighter(style_arr[14]);
            set_lcd_cursor(3,0);
             print_lcd(style_arr[15]);
        break;

        case 15:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[13]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[14]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[15]);
             print_lcd(style_arr[15]);
                menu_dehighlighter(style_arr[15]);
        break;

        case 16:     // Style Menu Selected.
            set_lcd_cursor(0,0);
             print_lcd(style_arr[0]);
            set_lcd_cursor(1,0);
             print_lcd(style_arr[14]);
            set_lcd_cursor(2,0);
             print_lcd(style_arr[15]);
            set_lcd_cursor(3,0);
                menu_highlighter(style_arr[16]);
             print_lcd(style_arr[16]);
                menu_dehighlighter(style_arr[16]);
        break;

        default:

            printf("\n\nValue of s_menu_pos is out of range.\n\n");
        break;
    }
    
  }else{
    if(s_menu_pos == 0){
        s_menu = true;
        enter = false;
    }else{
    style_selected = true;
    s_menu = false;
    enter = false;

    cs_menu = s_menu_pos;
    style_m = s_menu_pos;
    
    }
  }
  }else {
    s_menu = false;
    t_menu = true;
    back = false;
  }
}
    
    menu_selection = 0;
    if(cs_menu == 0){
        cs_menu = 1;
    }else {
        printf("\n\nCurrent Style selected is: %i.\n\n", cs_menu);
    }
  while(style_selected){    // This will select which beer will be displayed for data collection.
       css_menu_pos = menu_selection;

  if(!back){
    if(!enter){
      
      switch((cs_menu)){

            case 1:     // Case 1 is equal to styles IPA.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[1]);
              if(css_menu_pos < 5){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[0][0]);
                     print_lcd(beer_array[0][0]);
                        menu_dehighlighter(beer_array[0][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[0][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[0][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[0][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[0][1]);
                     print_lcd(beer_array[0][1]);
                        menu_dehighlighter(beer_array[0][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[0][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[0][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[0][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[0][2]);
                     print_lcd(beer_array[0][2]);
                        menu_dehighlighter(beer_array[0][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[0][3]);
                     print_lcd(beer_array[0][3]);
                        menu_dehighlighter(beer_array[0][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[0][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[0][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[0][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[0][4]);
                     print_lcd(beer_array[0][4]);
                        menu_dehighlighter(beer_array[0][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[0][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[0][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[0][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[0][5]);
                     print_lcd(beer_array[0][5]);
                        menu_dehighlighter(beer_array[0][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 2:     // Case 2 is equal to styles Stouts.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[2]);
                    
                    if(css_menu_pos < 7){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[1][0]);
                     print_lcd(beer_array[1][0]);
                        menu_dehighlighter(beer_array[1][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[1][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[1][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[1][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[1][1]);
                     print_lcd(beer_array[1][1]);
                        menu_dehighlighter(beer_array[1][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[1][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[1][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[1][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[1][2]);
                     print_lcd(beer_array[1][2]);
                        menu_dehighlighter(beer_array[1][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[1][3]);
                     print_lcd(beer_array[1][3]);
                        menu_dehighlighter(beer_array[1][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[1][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[1][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[1][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[1][4]);
                     print_lcd(beer_array[1][4]);
                        menu_dehighlighter(beer_array[1][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[1][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[1][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[1][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[1][5]);
                     print_lcd(beer_array[1][5]);
                        menu_dehighlighter(beer_array[1][5]);
                    break;

                    case 6:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[1][4]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[1][5]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[1][6]);
                     print_lcd(beer_array[1][6]);
                        menu_dehighlighter(beer_array[1][6]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 3:     // Case 3 is equal to styles Porters.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[3]);
             
                    if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[2][0]);
                     print_lcd(beer_array[2][0]);
                        menu_dehighlighter(beer_array[2][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[2][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[2][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[2][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[2][1]);
                     print_lcd(beer_array[2][1]);
                        menu_dehighlighter(beer_array[2][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[2][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[2][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[2][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[2][2]);
                     print_lcd(beer_array[2][2]);
                        menu_dehighlighter(beer_array[2][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[2][3]);
                     print_lcd(beer_array[2][3]);
                        menu_dehighlighter(beer_array[2][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[2][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[2][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[2][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[2][4]);
                     print_lcd(beer_array[2][4]);
                        menu_dehighlighter(beer_array[2][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[2][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[2][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[2][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[2][5]);
                     print_lcd(beer_array[2][5]);
                        menu_dehighlighter(beer_array[2][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;      

            case 4:     // Case 4 is equal to styles Lagers.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[4]);

                    if(css_menu_pos < 5){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[3][0]);
                     print_lcd(beer_array[3][0]);
                        menu_dehighlighter(beer_array[3][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[3][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[3][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[3][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[3][1]);
                     print_lcd(beer_array[3][1]);
                        menu_dehighlighter(beer_array[3][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[3][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[3][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[3][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[3][2]);
                     print_lcd(beer_array[3][2]);
                        menu_dehighlighter(beer_array[3][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[3][3]);
                     print_lcd(beer_array[3][3]);
                        menu_dehighlighter(beer_array[3][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[3][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[3][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[3][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[3][4]);
                     print_lcd(beer_array[3][4]);
                        menu_dehighlighter(beer_array[3][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[3][5]);
                    break;

                    default:
                    break;
                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 5:     // Case 5 is equal to styles Pilsners.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[5]);
             
                    if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[4][0]);
                     print_lcd(beer_array[4][0]);
                        menu_dehighlighter(beer_array[4][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[4][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[4][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[4][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[4][1]);
                     print_lcd(beer_array[4][1]);
                        menu_dehighlighter(beer_array[4][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[4][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[4][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[4][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[4][2]);
                     print_lcd(beer_array[4][2]);
                        menu_dehighlighter(beer_array[4][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[4][3]);
                     print_lcd(beer_array[4][3]);
                        menu_dehighlighter(beer_array[4][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[4][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[4][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[4][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[4][4]);
                     print_lcd(beer_array[4][4]);
                        menu_dehighlighter(beer_array[4][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[4][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[4][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[4][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[4][5]);
                     print_lcd(beer_array[4][5]);
                        menu_dehighlighter(beer_array[4][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 6:     // Case 6 is equal to styles Wheat Beer.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[6]);
             
                    if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[5][0]);
                     print_lcd(beer_array[5][0]);
                        menu_dehighlighter(beer_array[5][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[5][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[5][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[5][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[5][1]);
                     print_lcd(beer_array[5][1]);
                        menu_dehighlighter(beer_array[5][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[5][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[5][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[5][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[5][2]);
                     print_lcd(beer_array[5][2]);
                        menu_dehighlighter(beer_array[5][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[5][3]);
                     print_lcd(beer_array[5][3]);
                        menu_dehighlighter(beer_array[5][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[5][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[5][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[5][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[5][4]);
                     print_lcd(beer_array[5][4]);
                        menu_dehighlighter(beer_array[5][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[5][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[5][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[5][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[5][5]);
                     print_lcd(beer_array[5][5]);
                        menu_dehighlighter(beer_array[5][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 7:     // Case 7 is equal to styles Pale Ales.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[7]);

                   if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[6][0]);
                     print_lcd(beer_array[6][0]);
                        menu_dehighlighter(beer_array[6][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[6][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[6][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[6][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[6][1]);
                     print_lcd(beer_array[6][1]);
                        menu_dehighlighter(beer_array[6][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[6][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[6][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[6][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[6][2]);
                     print_lcd(beer_array[6][2]);
                        menu_dehighlighter(beer_array[6][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[6][3]);
                     print_lcd(beer_array[6][3]);
                        menu_dehighlighter(beer_array[6][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[6][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[6][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[6][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[6][4]);
                     print_lcd(beer_array[6][4]);
                        menu_dehighlighter(beer_array[6][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[6][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[6][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[6][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[6][5]);
                     print_lcd(beer_array[6][5]);
                        menu_dehighlighter(beer_array[6][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 8:     // Case 8 is equal to styles Amber Ales.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[8]);
                   
                   if(css_menu_pos < 5){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[7][0]);
                     print_lcd(beer_array[7][0]);
                        menu_dehighlighter(beer_array[7][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[7][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[7][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[7][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[7][1]);
                     print_lcd(beer_array[7][1]);
                        menu_dehighlighter(beer_array[7][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[7][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[7][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[7][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[7][2]);
                     print_lcd(beer_array[7][2]);
                        menu_dehighlighter(beer_array[7][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[7][3]);
                     print_lcd(beer_array[7][3]);
                        menu_dehighlighter(beer_array[7][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[7][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[7][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[7][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[7][4]);
                     print_lcd(beer_array[7][4]);
                        menu_dehighlighter(beer_array[7][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[7][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 9:     // Case 9 is equal to styles Brown Ales.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[9]);
             
                   if(css_menu_pos < 5){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[8][0]);
                     print_lcd(beer_array[8][0]);
                        menu_dehighlighter(beer_array[8][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[8][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[8][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[8][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[8][1]);
                     print_lcd(beer_array[8][1]);
                        menu_dehighlighter(beer_array[8][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[8][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[8][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[8][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[8][2]);
                     print_lcd(beer_array[8][2]);
                        menu_dehighlighter(beer_array[8][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[8][3]);
                     print_lcd(beer_array[8][3]);
                        menu_dehighlighter(beer_array[8][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[8][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[8][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[8][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[8][4]);
                     print_lcd(beer_array[8][4]);
                        menu_dehighlighter(beer_array[8][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[8][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;  

            case 10:     // Case 10 is equal to styles Belgium Ales.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[10]);

                   if(css_menu_pos < 5){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[9][0]);
                     print_lcd(beer_array[9][0]);
                        menu_dehighlighter(beer_array[9][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[9][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[9][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[9][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[9][1]);
                     print_lcd(beer_array[9][1]);
                        menu_dehighlighter(beer_array[9][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[9][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[9][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[9][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[9][2]);
                     print_lcd(beer_array[9][2]);
                        menu_dehighlighter(beer_array[9][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[9][3]);
                     print_lcd(beer_array[9][3]);
                        menu_dehighlighter(beer_array[9][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[9][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[9][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[9][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[9][4]);
                     print_lcd(beer_array[9][4]);
                        menu_dehighlighter(beer_array[9][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[9][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 11:     // Case 1 is equal to styles Belgium Dubbel.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[11]);
             
                   if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[10][0]);
                     print_lcd(beer_array[10][0]);
                        menu_dehighlighter(beer_array[10][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[10][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[10][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[10][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[10][1]);
                     print_lcd(beer_array[10][1]);
                        menu_dehighlighter(beer_array[10][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[10][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[10][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[10][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[10][2]);
                     print_lcd(beer_array[10][2]);
                        menu_dehighlighter(beer_array[10][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[10][3]);
                     print_lcd(beer_array[10][3]);
                        menu_dehighlighter(beer_array[10][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[10][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[10][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[10][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[10][4]);
                     print_lcd(beer_array[10][4]);
                        menu_dehighlighter(beer_array[10][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[10][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[10][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[10][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[10][5]);
                     print_lcd(beer_array[10][5]);
                        menu_dehighlighter(beer_array[10][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            case 12:     // Case 1 is equal to styles Tripel.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[12]);
             
                   if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[11][0]);
                     print_lcd(beer_array[11][0]);
                        menu_dehighlighter(beer_array[11][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[11][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[11][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[11][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[11][1]);
                     print_lcd(beer_array[11][1]);
                        menu_dehighlighter(beer_array[11][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[11][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[11][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[11][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[11][2]);
                     print_lcd(beer_array[11][2]);
                        menu_dehighlighter(beer_array[11][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[11][3]);
                     print_lcd(beer_array[11][3]);
                        menu_dehighlighter(beer_array[11][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[11][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[11][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[11][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[11][4]);
                     print_lcd(beer_array[11][4]);
                        menu_dehighlighter(beer_array[11][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[11][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[11][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[11][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[11][5]);
                     print_lcd(beer_array[11][5]);
                        menu_dehighlighter(beer_array[11][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break; 

            case 13:     // Case 1 is equal to styles Tripel.
                set_lcd_cursor(0,0);
             print_lcd(style_arr[13]);
             
                   if(css_menu_pos < 6){
                switch(css_menu_pos){
                
                    case 0:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[12][0]);
                     print_lcd(beer_array[12][0]);
                        menu_dehighlighter(beer_array[12][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[12][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[12][2]);
                    break;

                    case 1:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[12][0]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[12][1]);
                     print_lcd(beer_array[12][1]);
                        menu_dehighlighter(beer_array[12][1]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[12][2]);
                    break;

                    case 2:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[12][0]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[12][1]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[12][2]);
                     print_lcd(beer_array[12][2]);
                        menu_dehighlighter(beer_array[12][2]);
                    break;

                    case 3:
                    set_lcd_cursor(1,0);
                        menu_highlighter(beer_array[12][3]);
                     print_lcd(beer_array[12][3]);
                        menu_dehighlighter(beer_array[12][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[12][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[12][5]);
                    break;

                    case 4:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[12][3]);
                    set_lcd_cursor(2,0);
                        menu_highlighter(beer_array[12][4]);
                     print_lcd(beer_array[12][4]);
                        menu_highlighter(beer_array[12][4]);
                    set_lcd_cursor(3,0);
                     print_lcd(beer_array[12][5]);
                    break;

                    case 5:
                    set_lcd_cursor(1,0);
                     print_lcd(beer_array[12][3]);
                    set_lcd_cursor(2,0);
                     print_lcd(beer_array[12][4]);
                    set_lcd_cursor(3,0);
                        menu_highlighter(beer_array[12][5]);
                     print_lcd(beer_array[12][5]);
                        menu_highlighter(beer_array[12][5]);
                    break;

                    default:
                    break;

                }
              }else{
                printf("No More of this style.\n");
              }
            break;

            default:

                printf("No value currently selected.\n");
            break;


        }
      
      }else{

        data_menu = true;
        style_selected = false;
        enter = false;

        data_m = css_menu_pos;
        style_m_reset = false;
      }
    }else{

        style_selected = false;
        s_menu = true;
        back = false;
    }
  }
    

    menu_selection = 1;
  while(data_menu){

    data_menu_pos = menu_selection;

if(!style_m_reset){

  if(style_m > 0){
    style_m -= 1;
  }else {
    printf("Normal Value.\n");
  }
  style_m_reset = true;
}

    if(!back){
      if(!enter){

    switch(data_menu_pos){

        case 0:
             set_lcd_cursor(0,0);
                 print_lcd(beer_array[(style_m)][data_m]);
                    
                set_lcd_cursor(1,0);
                    menu_highlighter(data_array[0]);
                 print_lcd(data_array[0]);
                    menu_dehighlighter(data_array[0]);

                set_lcd_cursor(2,0);
                 print_lcd(data_array[1]);


                set_lcd_cursor(3,0);
                 print_lcd(data_array[2]);
        break;

        case 1:

            set_lcd_cursor(0,0);
             print_lcd(beer_array[(style_m)][data_m]);
                    
            set_lcd_cursor(1,0);
             print_lcd(data_array[0]);
                    
            set_lcd_cursor(2,0);
                menu_highlighter(data_array[1]);
             print_lcd(data_array[1]);
                menu_dehighlighter(data_array[1]);

            set_lcd_cursor(3,0);
             print_lcd(data_array[2]);
        break;

        case 2:

            set_lcd_cursor(0,0);
             print_lcd(beer_array[(style_m)][data_m]);
                    
            set_lcd_cursor(1,0);
             print_lcd(data_array[0]);
                
            set_lcd_cursor(2,0);
             print_lcd(data_array[1]);

            set_lcd_cursor(3,0);
                menu_highlighter(data_array[2]);
             print_lcd(data_array[2]);
                menu_dehighlighter(data_array[2]);
        break;

        case 3:
            set_lcd_cursor(0,0);
             print_lcd(beer_array[(style_m)][data_m]);
                    
            set_lcd_cursor(1,0);
                menu_highlighter(data_array[3]);
             print_lcd(data_array[3]);
                menu_dehighlighter(data_array[3]);

            set_lcd_cursor(2,0);
             print_lcd(data_array[4]);

            set_lcd_cursor(3,0);
             print_lcd(data_array[5]);
        break;

        case 4:

            set_lcd_cursor(0,0);
             print_lcd(beer_array[(style_m)][data_m]);
                    
            set_lcd_cursor(1,0);
             print_lcd(data_array[3]);
                    
            set_lcd_cursor(2,0);
                menu_highlighter(data_array[4]);
             print_lcd(data_array[4]);
                menu_dehighlighter(data_array[4]);

            set_lcd_cursor(3,0);
             print_lcd(data_array[5]);

        default:

            printf("No more items on this list.\n");

    }

      }else {

        d_entry = true;
        data_menu = false;

        enter = false;

      }
    }else {

        data_menu = false;
        style_selected = true;

        back = false;
    }
  }

    menu_selection = 0;
  while(d_entry){

    data_menu_pos = menu_selection;


  if(!back){
    if(data_menu_pos == 0){
        if(!enter){

        set_lcd_cursor(0,0);
            print_lcd(data_array[0]);
                    
        set_lcd_cursor(1,0);   
            sprintf(input_array[0][0], " Enter Score: %i/20", rotary.rotary_total);
            current_score[style_m][data_m][] = rotary.rotary_total;
        print_lcd(input_array[0][0]);

        set_lcd_cursor(2,0);
         sprintf(input_array[0][1], " Prev Score: %i/20", previous_score[style_m][data_m][0]);
        print_lcd(input_array[0][1]);

        set_lcd_cursor(3,0);
         print_lcd(data_array[5]);
        
        }else {

            data_e_m = data_menu_pos;

                previous_score[style_m][data_m][0] = rotary.rotary_total;
                rotary.rotary_total = 0;

            enter = false;

            /*
                Upon enter of data, call function to store rotary encoder's value for beer score into
                struct / array / struct of arrays / array of structs??

            */
        }
    }

  }else {
    d_entry = false;
    data_menu = true;

    back = false;
  }
  }


  while(f_menu){
    if(!back){

        /*

            Use this while to select & display the top five beers, read from the MEMPICO
        
        */

    }else {
        f_menu = false;
        t_menu = true;
        back = false;
    }
  }


  while(g_menu){
    if(!back){

        /*

            Use this while to select & display the lowest five beers, read from the MEMPICO
        
        */

    }else {
        g_menu = false;
        t_menu = true;
        back = false;
    }
  }


  while(about){

    if(!back){

    set_lcd_cursor(0,0);
     print_lcd("       ABOUT        ");
    set_lcd_cursor(1,0);
     print_lcd("Beer Rater Program  ");
    set_lcd_cursor(2,0);
     print_lcd("Written By: Alphonse");
    set_lcd_cursor(3,0);
     print_lcd("Current Version 1.1 ");
    }else {
        about = false;
        t_menu = true;
    }

  }
    
    
    }

    if(reset){
    printf("Resetting program.\n");
    value_inits();
    sleep_ms(1500);
    reset = false;
    }
}
}
