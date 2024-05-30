#ifndef BEER_FUNCTIONS_H
#define BEER_FUNCTIONS_H

#include <stdio.h>
#include "beer_list.h"
#include "pin_manager.h"
#include "error_manager.h"
#define ebit uint8_t

typedef struct high_pairing_s {

    ebit high_pair[6][2];
    ebit low_pair[6][2];

    ebit transfer_out;

    ebit high_place;
    ebit second_place;
    ebit third_place;
    ebit fourth_place;
    ebit low_place;

}high_duos;

typedef struct master_pairs_s {


    ebit master_fave_pairs[6][2];
    ebit master_gros_pairs[6][2];

    ebit master_fav_one;
    ebit master_fav_two;
    ebit master_fav_three;
    ebit master_fav_four;
    ebit master_fav_five;

    ebit master_gros_one;
    ebit master_gros_two;
    ebit master_gros_three;
    ebit master_gros_four;
    ebit master_gros_five;

}master_pairs;

typedef struct ipc_buffer_s {
    uint8_t buf_one[8];
    uint8_t buf_two[8];
    uint8_t buf_three[8];
    uint8_t buf_four[8];
    uint8_t buf_five[8];

    uint8_t buf_locations[6];
}ipc_bufs;

//  Volatile bools to use with interrupt function.
//  Initialise in function.
typedef struct vol_bool_variables {
    
    volatile bool up;
    volatile bool down;
    volatile bool backlight;
    volatile bool back;
    volatile bool enter;
    volatile bool reset;

}vol_bool_variables;
vol_bool_variables vol_boos;


//  Regular bools to set while loops status
//  initialize in function.
typedef struct bool_variables {

    bool top_menu;
    bool styles_menu;
    bool beers_menu;
    bool faves_menu;
    bool gross_menu;
    bool menu_about;
    bool data_menu;
    bool data_entry_menu;
    //  nv_ a non_volatile bool
    bool nv_enter;
    bool nv_back;
    bool nv_reset;

}boo_vars;
boo_vars my_bools;

bool debug;

//  uint8_t variables for menu item identification.
typedef struct int_variables {

    ebit t_menu_id;
    ebit style_id;
    ebit beer_id;
    ebit sense_id;
    ebit rotary_input;
    //  [styles][beer][sense][curr/prev]
    ebit current_scores[15][7][5][2];
    ebit previous_scores[15][7][5][2];
    ebit mri_status[2][2][2];


}int_vars;


//  character arrays with related names.
typedef struct char_variables {

    char top_menu[6][21];
    char style_menu[14][21];
    char beer_menu[15][7][21];
    char data_menu[6][21];
    char menu_input[21];

}char_vars;


//  Define SPI header packet parts.
#define Master_Write 0x80
#define Master_Read 0x40

//  define protofunctions.
void menu_highlighter(char input_arr[]);
void menu_dehighlighter(char in[]);
void reset_menu_in(char_vars *in);
void set_score(bool selis, master_pairs *sets, high_duos *pairs);
void read_score(ipc_bufs *bufs);
void menu_bool_setter(int_vars *id, boo_vars *boo);
void top_selection_menu(char_vars *names, ebit current_sel);
void style_selection_menu(int_vars *level, char_vars *names, uint8_t current_sel);
void beer_selection_menu(int_vars *level, char_vars *names, uint8_t current_sel);
void data_selector_menu(int_vars *menus, char_vars *names, uint8_t current_sel);
void sense_menu(int_vars *menus, char_vars *names);
void faves_menu(high_duos *hs, ebit cur_sel);
void display_faves(char_vars *names, ebit line, ebit style, ebit beer);
void debug_display(char_vars *name, int_vars *levels);
void value_inits(boo_vars *bools, int_vars *i_vars, high_duos *arrs);
high_duos top_five_selector(int_vars *total, high_duos *pairs);
void program_intro_display(void);


//  Take in current beers likiablity score, and map to led register output.
ebit led_mapping(ebit liking_status){
    ebit mapped;
    mapped = 0;

    if(liking_status <= 100){

        mapped = (liking_status < 11) ? 0x01 : 0x00;
        mapped = (liking_status < 21) ? 0x03 : 0x00;
        mapped = (liking_status < 31) ? 0x07 : 0x00;
        mapped = (liking_status < 41) ? 0x0F : 0x00;
        mapped = (liking_status < 51) ? 0x1F : 0x00;
        mapped = (liking_status < 61) ? 0x3F : 0x00;
        mapped = (liking_status < 71) ? 0x7F : 0x00;
        mapped = (liking_status > 80) ? 0xFF : 0x00;

    }
    return mapped;
}


#endif