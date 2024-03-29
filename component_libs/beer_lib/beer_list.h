#ifndef BEERLIST
#define BEERLIST

#include <stdio.h>
#include <stdint.h>

#define ebit uint8_t

//  Current_score is used as such, [13] is styles, [7] is individual beer selection, [5] are beer scores.
//  Previous_score is used the same.
uint8_t current_score[13][7][5][2];
uint8_t previous_score[13][7][5][2];
char input_array[21];

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


//  uint8_t variables for menu item identification.
typedef struct int_variables {

    ebit t_menu_id;
    ebit style_id;
    ebit beer_id;
    ebit sense_id;
    //  [styles][beer][sense][curr/prev]
    ebit current_scores[15][7][5][2];
    ebit previous_scores[15][7][5][2];


}int_vars;


//  character arrays with related names.
typedef struct char_variables {

    const char top_menu[4][21];
    const char style_menu[14][21];
    const char beer_menu[15][7][21];
    const char data_menu[6][21];
    char menu_input[21];

}char_vars;


//  Initialise int_variables struct with zero's, use function to fill scores menus
int_vars my_vars = {
    .t_menu_id = 0,
    .style_id = 0,
    .beer_id = 0,
    .sense_id = 0,

};

char_vars my_menus = {

    .top_menu = {

 {' ', ' ', ' ', 'B', 'e', 'e', 'r', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'S', 't', 'y', 'l', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'F', 'a', 'v', 'o', 'r', 'i', 't', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'D', 'i', 's', 'l', 'i', 'k', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

},
    .style_menu = {
 {'S', 't', 'y', 'l', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'I', 'p', 'a', '`', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'S', 't', 'o', 'u', 't', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'P', 'o', 'r', 't', 'e', 'r', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'L', 'a', 'g', 'e', 'r', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'P', 'i', 'l', 's', 'n', 'e', 'r', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'W', 'h', 'e', 'a', 't', ' ', 'B', 'e', 'e', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'P', 'a', 'l', 'e', ' ', 'A', 'l', 'e', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'A', 'm', 'b', 'e', 'r', ' ', 'A', 'l', 'e', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'B', 'r', 'o', 'w', 'n', ' ', 'A', 'l', 'e', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'B', 'e', 'l', 'g', 'i', 'u', 'm', ' ', 'A', 'l', 'e', '`', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'B', 'e', 'l', 'g', 'i', 'u', 'm', ' ', 'D', 'u', 'b', 'b', 'e', 'l', ' ', ' ', ' ', ' ', ' ', ' '},
 {'T', 'r', 'i', 'p', 'e', 'l', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'S', 'a', 'i', 's', 'o', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

},
    .beer_menu = {//  First index [0][0] are IPA's
    {

     {'V', 'o', 'o', 'd', 'o', 'o', ' ', 'R', 'a', 'n', 'g', 'e', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'n', 'a', 'k', 'e', ' ', 'D', 'o', 'g', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'i', 'g', ' ', 'L', 'i', 't', 't', 'l', 'e', ' ', 'F', 'i', 's', 'h', ' ', ' ', ' ', ' ', ' '},
     {'U', 'n', 'i', 'o', 'n', ' ', 'J', 'a', 'c', 'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'H', 'a', 'r', 'v', 'e', 's', 't', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'O', 'b', 's', 'i', 'd', 'i', 'a', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'M', 'i', 'l', 'k', ' ', 'S', 't', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'T', 'h', 'e', ' ', 'P', 'o', 'e', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'X', 'o', 'c', 'o', 'v', 'e', 'z', 'a', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'G', 'u', 'i', 'n', 'e', 's', 's', ' ', 'E', 'x', 't', 'r', 'a', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'o', 'u', 'b', 'l', 'e', ' ', 'C', 'r', 'e', 'a', 'm', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'u', 'r', 'b', 'o', 'n', ' ', 'C', 'o', 'u', 'n', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    
    },
    {

     {'C', 'o', 'f', 'f', 'e', 'e', ' ', 'B', 'a', 'l', 't', 'i', 'c', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'G', 'r', 'a', 'v', 'i', 't', 'y', ' ', 'D', 'r', 'o', 'p', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'e', 'x', 't', 'u', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'e', 'e', 'r', ' ', 'B', 'a', 's', 'h', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'a', 'l', 't', 'i', 'c', ' ', 'S', 't', 'y', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'T', 'h', 'e', ' ', 'T', 'a', 'r', 't', 'e', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'B', 'u', 'd', 'w', 'e', 'i', 's', 'e', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'M', 'i', 'l', 'l', 'e', 'r', ' ', 'L', 'i', 't', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'H', 'e', 'i', 'n', 'e', 'k', 'e', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 't', 'e', 'l', 'l', 'a', ' ', 'A', 'r', 't', 'o', 'i', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'C', 'o', 'r', 'o', 'n', 'a', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'U', 'r', 'q', 'u', 'e', 'l', 'l', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'T', 'S', ' ', 'P', 'i', 'l', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'H', 'e', 'a', 't', 'e', 'r', ' ', 'A', 'l', 'l', 'e', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'L', 'i', 'v', 'e', ' ', 'O', 'a', 'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'J', 'e', 'v', 'e', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'K', 'e', 'l', 'l', 'e', 'r', ' ', 'P', 'i', 'l', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {
    
     {'A', 'l', 'l', 'a', 'g', 'a', 's', 'h', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'u', 'm', 'm', 'e', 'r', ' ', 'A', 'l', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'l', 'o', 'o', 'd', ' ', 'O', 'r', 'a', 'n', 'g', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'e', 'l', 'g', 'i', 'n', ' ', 'W', 'h', 'i', 't', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'L', 'a', 'm', 'b', 'i', 'c', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 't', 'o', 'o', 'p', 'i', 'd', ' ', 'W', 'i', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'1', '5', '0', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'o', 'g', 'g', 'i', 'e', ' ', 'S', 't', 'y', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'a', 'i', 's', 'e', 'y', ' ', 'C', 'u', 't', 't', 'e', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'W', 'h', 'i', 'r', 'l', 'p', 'o', 'o', 'l', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'P', 'a', 'l', 'e', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'w', 'e', 'e', 't', ' ', 'A', 'c', 't', 'i', 'o', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'F', 'a', 't', ' ', 'T', 'i', 'r', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'N', 'u', 'g', 'g', 'e', 't', ' ', 'N', 'e', 'c', 't', 'o', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'H', 'o', 'p', ' ', 'H', 'e', 'a', 'd', ' ', 'R', 'e', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'A', 'm', 'b', 'e', 'r', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'F', 'l', 'i', 'p', 's', 'i', 'd', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'B', 'r', 'o', 'w', 'n', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'N', 'u', 't', ' ', 'B', 'r', 'o', 'w', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'e', 's', 't', ' ', 'B', 'r', 'o', 'w', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'r', 'e', 'k', 'l', 'e', 's', ' ', 'B', 'r', 'o', 'w', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'P', 'a', 'l', 'o', ' ', 'S', 'a', 'n', 't', 'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'R', 'e', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'l', 'o', 'n', 'd', 'e', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'R', 'e', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'K', 'r', 'i', 'c', 'k', ' ', 'L', 'a', 'm', 'b', 'i', 'c', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'u', 'v', 'e', 'l', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'A', 'b', 'b', 'e', 'y', ' ', 'A', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'o', 'e', 's', 't', ' ', 'R', 'o', 'a', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'P', 'e', 'r', 'e', ' ', 'J', 'a', 'c', 'q', 'u', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'B', 'e', 'n', 'e', 'd', 'i', 'c', 't', 'i', 'i', 'o', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'W', 'i', 'l', 'd', ' ', 'D', 'u', 'c', 'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'u', 'b', ' ', 'S', 'a', 'c', 'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'S', 'a', 'i', 's', 'o', 'n', ' ', 'D', 'u', ' ', 'B', 'u', 'f', 'f', ' ', ' ', ' ', ' ', ' ', ' '},
     {'P', 'r', 'o', 'p', ' ', 'C', 'u', 'l', 't', 'u', 'r', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'a', 'i', 's', 'o', 'n', ' ', '8', '3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'a', 'i', 's', 'o', 'n', ' ', 'D', 'u', ' ', 'S', 'w', 'a', 'm', 'p', ' ', ' ', ' ', ' ', ' '},
     {'E', 'u', 'k', 'a', 'r', 'y', 'o', 't', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 't', 'o', 'n', 'e', ' ', 'S', 'a', 'i', 's', 'o', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    },
    {

     {'M', 'i', 'a', 'm', 'i', ' ', 'M', 'a', 'd', 'n', 'e', 's', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'S', 'u', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'L', 'e', ' ', 'T', 'e', 'r', 'r', 'o', 'i', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'D', 'u', 'c', 'k', ' ', 'D', 'u', 'c', 'k', ' ', 'G', 'o', 'o', 's', 'e', ' ', ' ', ' ', ' ', ' '},
     {'A', 'm', 'o', 'r', 'p', 'h', 'i', 'a', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'R', 'u', 'b', 'a', 'e', 'u', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

    }

},
    .data_menu = {
 {'L', 'o', 'o', 'k', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'T', 'a', 's', 't', 'e', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'S', 'm', 'e', 'l', 'l', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'H', 'e', 'a', 'd', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'A', 'f', 't', 'e', 'r', 't', 'a', 's', 't', 'e', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

},
    .menu_input = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

};

//  Use to reset input menu array to all spaces.
void reset_menu_in(char_vars *in){

    for(int i = 0; i < 21; i++){
        in->menu_input[i] = ' ';
    }

}

void set_score(uint8_t style, uint8_t beer, uint8_t sense, uint8_t score){

    current_score[style][beer][sense][0] = score;
    
}


void menu_bool_setter(int_vars *id, boo_vars *boo){

    if(id->t_menu_id == 1){
        printf("Entering S Menu.\n");
        boo->styles_menu = true;
        boo->top_menu = false;
        boo->nv_enter = false;

    }else if(id->t_menu_id == 2){
        printf("Entering F Menu.\n");
        boo->faves_menu = true;
        boo->top_menu = false;
        boo->nv_enter = false;

    }else if(id->t_menu_id == 3){
        printf("Entering G Menu.\n");
        boo->gross_menu = true;
        boo->top_menu = false;
        boo->nv_enter = false;

    }else if(id->t_menu_id == 4){
        printf("Entering About.\n");
        boo->menu_about = true;
        boo->top_menu = false;
        boo->nv_enter = false;

    }else {
        //  Top menu error, stay in t_menu loop.
        printf("Error?\n\n");
        boo->nv_enter = false;
    }
}


//  runs inside top level menu while loop
void top_selection_menu(char_vars *names, ebit current_sel){

    set_lcd_cursor(0,0);
    print_lcd(names->top_menu[0]);

  if(current_sel == 0){
                          
    set_lcd_cursor(1,0);   
        menu_highlighter(names->top_menu[(current_sel + 1)]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[(current_sel + 2)]);

    set_lcd_cursor(3,0);
    print_lcd(names->top_menu[(current_sel + 3)]);

  }else if(current_sel == 1) {

    set_lcd_cursor(1,0);   
    print_lcd(names->top_menu[(current_sel - 1)]);
        
        menu_highlighter(names->top_menu[current_sel]);
    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->top_menu[(current_sel + 1)]);

  }else if(current_sel == 2){
  
    set_lcd_cursor(1,0);   
    print_lcd(names->top_menu[(current_sel - 2)]);
        
        
    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[(current_sel - 1)]);
        
        menu_highlighter(names->top_menu[current_sel]);
    set_lcd_cursor(3,0);
    print_lcd(names->top_menu[(current_sel + 1)]);
        menu_dehighlighter(names->top_menu[current_sel]);
    }else {

    set_lcd_cursor(1,0);   
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->data_menu[5]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);
    }

}

//  runs inside style menu while loop
void style_selection_menu(int_vars *level, char_vars *names, uint8_t current_sel){

    if(current_sel == 0){
        
    set_lcd_cursor(0,0);
    print_lcd(names->style_menu[0]);
                    
    set_lcd_cursor(1,0);   
        menu_highlighter(names->style_menu[current_sel]);
    print_lcd(names->style_menu[current_sel]);
        menu_dehighlighter(names->style_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->style_menu[(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel + 2)]);

    }else if((current_sel > 0) && (current_sel < 14)) {

    set_lcd_cursor(0,0);
    print_lcd(names->style_menu[0]);
                    
    set_lcd_cursor(1,0);   
    print_lcd(names->style_menu[(current_sel - 1)]);
        
        menu_highlighter(names->style_menu[current_sel]);
    set_lcd_cursor(2,0);
    print_lcd(names->style_menu[current_sel]);
        menu_dehighlighter(names->style_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel + 1)]);

    }else {

        set_lcd_cursor(0,0);
    print_lcd(names->style_menu[0]);
                    
    set_lcd_cursor(1,0);   
    print_lcd(names->style_menu[(current_sel - 2)]);
        
        
    set_lcd_cursor(2,0);
    print_lcd(names->style_menu[(current_sel - 1)]);
        
        menu_highlighter(names->style_menu[current_sel]);
    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel + 1)]);
        menu_dehighlighter(names->style_menu[current_sel]);
    }

}

//  runs inside beer selection menu while loop
void beer_selection_menu(int_vars *level, char_vars *names, uint8_t current_sel){

    set_lcd_cursor(0,0);
    print_lcd(names->style_menu[level->style_id]);

  if(current_sel == 0){           
    set_lcd_cursor(1,0);
        menu_highlighter(names->beer_menu[level->style_id][current_sel]);
    print_lcd(names->beer_menu[level->style_id][current_sel]);
        menu_dehighlighter(names->beer_menu[level->style_id][current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel + 2)]);

  }else if((current_sel > 0) && (current_sel < 7)){

    set_lcd_cursor(1,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->beer_menu[level->style_id][current_sel]);
    print_lcd(names->beer_menu[level->style_id][current_sel]);
        menu_dehighlighter(names->beer_menu[level->style_id][current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel + 1)]);

  }else {

    set_lcd_cursor(1,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel - 2)]);
        
    set_lcd_cursor(2,0);
    print_lcd(names->beer_menu[level->style_id][(current_sel - 1)]);

    set_lcd_cursor(3,0);
        menu_highlighter(names->beer_menu[level->style_id][current_sel]);
    print_lcd(names->beer_menu[level->style_id][current_sel]);
        menu_dehighlighter(names->beer_menu[level->style_id][current_sel]);
  }

}

//runs inside data selector menu
void data_selector_menu(int_vars *menus, char_vars *names, uint8_t current_sel){

    set_lcd_cursor(0,0);
    print_lcd(names->data_menu[menus->beer_id]);

  if(current_sel == 0){

    set_lcd_cursor(1,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);


    set_lcd_cursor(2,0);
    print_lcd(names->data_menu[(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 2)]);

  }else if((current_sel > 0) && (current_sel < 4)){

    set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 2)]);

  }else {

    set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 2)]);
        
    set_lcd_cursor(2,0);
    print_lcd(names->data_menu[(current_sel - 1)]);

    set_lcd_cursor(3,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

  }
}

//  runs inside sense menu (aka data entry)
void sense_menu(int_vars *menus, char_vars *names, uint8_t current_sel){

    //  Set cursor, and print out currently selected sense menu item (look, taste, smell, etc)
    set_lcd_cursor(0,0);
    print_lcd(names->data_menu[menus->sense_id]);
                    
    set_lcd_cursor(1,0);   
    sprintf(names->menu_input, "Enter Score: %i/20", rotary.rotary_total);
    print_lcd(names->menu_input);

    reset_menu_in(names);

    set_lcd_cursor(2,0);
    sprintf(names->menu_input, " Prev Score: %i/20", previous_score[style_m][data_m][0]);
    print_lcd(names->menu_input);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);

}




#endif