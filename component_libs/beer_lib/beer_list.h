#ifndef BEERLIST
#define BEERLIST

#include <stdio.h>
#include <stdint.h>
#include "./Screens/_2004A_lcd/_2004a_i2c.h"
#include "beer_funcs.h"
#include "beer_score.h"

#define ebit uint8_t
#define high_max 100
#define high_min 51
#define low_max 50
#define low_min 1


//  Current_score is used as such, [13] is styles, [7] is individual beer selection, [5] are beer scores.
//  Previous_score is used the same.
uint8_t current_score[13][7][5][2];
uint8_t previous_score[13][7][5][2];

char input_array[21];
volatile ebit menu_selection;

ipc_bufs my_buffs;

master_pairs my_master;

//  Initialise int_variables struct with zeros, use function to fill scores menus
int_vars my_vars = {
    .t_menu_id = 0,
    .style_id = 0,
    .beer_id = 0,
    .sense_id = 0
};

high_duos my_pairs = {

    .high_place = 0,
    .second_place = 0,
    .third_place = 0,
    .fourth_place = 0,
    .low_place = 0

};

char_vars my_menus = {

    .top_menu = {

 {' ', ' ', ' ', 'B', 'e', 'e', 'r', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'S', 't', 'y', 'l', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'F', 'a', 'v', 'o', 'r', 'i', 't', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'D', 'i', 's', 'l', 'i', 'k', 'e', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
 {'D', 'e', 'b', 'u', 'g', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

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
    .beer_menu = {//  First index [0][0] are IPAs
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


//  Takes two array arguments, and simply sets the values of in[x] -> out[x]
void ar_to_ar(uint8_t *in, uint8_t *out){
    for(int i = 0; i < 8; i++){

        out[i] = in[i];

    }
}


//  Take in an array, and stores the single bits per location into a uint8_t value.
uint8_t ar_to_ebit(uint8_t *in){

    uint8_t output;

    output += ((in[0] & 0x01) << 8);
    output += ((in[1] & 0x01) << 7);
    output += ((in[2] & 0x01) << 6);
    output += ((in[3] & 0x01) << 5);
    output += ((in[4] & 0x01) << 4);
    output += ((in[5] & 0x01) << 3);
    output += ((in[6] & 0x01) << 2);
    output += ((in[7] & 0x01) << 1);

    return output;
}


//  Takes in three values, and bit manipulates a uint16_t value to hold those three values.
uint16_t sel_to_usix(uint8_t style, uint8_t beer){

    uint16_t selection_output;
    selection_output = 0;

    selection_output += ((style & 0x0F) << 8);
    selection_output += ((beer & 0x0F));

    return selection_output;
}


void usix_to_arr(uint16_t location, uint8_t *out){

    out[0] = (location & 0xFF00);
    out[1] = (location & 0x00FF);

}


//  Takes in a uint16_t created by sel_to_usix to fill buffers (buf_one, buf_two)
//  Inside a for loop, Total_score gets added by the values of previous_score[x][y][0]
//  Total_score is then returned
uint8_t total_scorer(uint16_t lined){

    //  Lined example is 0xC0304
    //  Such example indicated a style value of 0x0C, beer value of 0x03, and a sense of 0x04.
    uint8_t total_score;
    uint8_t buf_one, buf_two;
    buf_one, buf_two = 0;
    total_score = 0;

    buf_one = ((lined & 0xFF00) >> 8);
    buf_two = ((lined & 0x00FF));

    for(int i = 0; i < 6; i++){

        total_score += previous_score[buf_one][buf_two][i][0];

    }

    return total_score;
}


//  Takes in a bool, and a struct as arguments
//  Sel_type refers to Faves / Grosses
//  Loops go through all beers and evaluates it
//  They are then checked, and sorted into high->low places
//  Outputs are stored in the high_duos struct my_pairs.
void top_fivers(bool sel_type, high_duos *vals){

    uint16_t tops, bottoms;
    uint8_t current_scorer;
    bool zeroed_out;
    zeroed_out = false;

    tops, bottoms = 0;
    vals->high_place, vals->second_place, vals->third_place, vals->fourth_place, vals->low_place = 0;
    
    if(sel_type){

        for(int i = 0; i < 14; i++){
         for(int j = 0; j < 6; j++){
            tops = sel_to_usix(i, j);

            current_scorer = total_scorer(tops);

             for(int l = 0; l < 6; l++){

                if(current_scorer > vals->high_place){
                    
                    vals->high_place = current_scorer;

                    vals->high_pair[0][0] = i;
                    vals->high_pair[0][1] = j;
                }else if(current_scorer > vals->second_place){

                    vals->second_place = current_scorer;

                    vals->high_pair[1][0] = i;
                    vals->high_pair[1][1] = j;
                }else if(current_scorer > vals->third_place){

                    vals->third_place = current_scorer;

                    vals->high_pair[2][0] = i;
                    vals->high_pair[2][1] = j;
                }else if(current_scorer > vals->fourth_place){

                    vals->fourth_place = current_scorer;

                    vals->high_pair[3][0] = i;
                    vals->high_pair[3][1] = j;
                }else if(current_scorer > vals->low_place){

                    vals->low_place = current_scorer;

                    vals->high_pair[4][0] = i;
                    vals->high_pair[4][1] = j;
                }else {
                    printf("Prohibitted action?\n");
                }

             }
          
         }
        }


    }else{

                for(int i = 0; i < 14; i++){
         for(int j = 0; j < 6; j++){
            bottoms = sel_to_usix(i, j);

            current_scorer = total_scorer(bottoms);
             zeroed_out = (current_scorer == 0) ? true : false;

    // Lowest rated beer.
    //  what to do with 0 values?
        if(!zeroed_out && (current_scorer <= low_max)){

             for(int l = 0; l < 6; l++){

                if((current_scorer <= low_max) && (current_scorer > vals->second_place)){
                    
                    vals->high_place = current_scorer;

                    vals->low_pair[0][0] = i;
                    vals->low_pair[0][1] = j;
                }else if((current_scorer < vals->high_place) && (current_scorer > vals->third_place)){

                    vals->second_place = current_scorer;

                    vals->low_pair[1][0] = i;
                    vals->low_pair[1][1] = j;
                }else if((current_scorer < vals->second_place) && (current_scorer > vals->fourth_place)){

                    vals->third_place = current_scorer;

                    vals->low_pair[2][0] = i;
                    vals->low_pair[2][1] = j;
                }else if((current_scorer < vals->fourth_place) && (current_scorer > vals->low_place)){

                    vals->fourth_place = current_scorer;

                    vals->low_pair[3][0] = i;
                    vals->low_pair[3][1] = j;
                }else if((current_scorer < vals->fourth_place) && (current_scorer > low_min)){

                    vals->low_place = current_scorer;

                    vals->low_pair[4][0] = i;
                    vals->low_pair[4][1] = j;
                }else {
                    printf("Prohibitted action?\n");
                }

             }
          
        }else{

        }
         }
        }
    }
}


/*
//  If faves, set scores from read to faves, else set to grosses
//  Once values have been read, assign them to the current sel array
//  Use with MASTER
//  Bool selis (selection is) true for faves, false for grosses.
void set_score(bool selis, master_pairs *sets, high_duos *pairs){

    if(selis){

        sets->master_fave_pairs[0][0] = pairs->high_pair[0][0];
        sets->master_fave_pairs[0][1] = pairs->high_pair[0][1];
        sets->master_fav_one = pairs->high_place;

        sets->master_fave_pairs[1][0] = pairs->high_pair[1][0];
        sets->master_fave_pairs[1][1] = pairs->high_pair[1][1];
        sets->master_fav_two = pairs->second_place;

        sets->master_fave_pairs[2][0] = pairs->high_pair[2][0];
        sets->master_fave_pairs[2][1] = pairs->high_pair[2][1];
        sets->master_fav_three = pairs->third_place;

        sets->master_fave_pairs[3][0] = pairs->high_pair[3][0];
        sets->master_fave_pairs[3][1] = pairs->high_pair[3][1];
        sets->master_fav_four = pairs->fourth_place;

        sets->master_fave_pairs[4][0] = pairs->high_pair[4][0];
        sets->master_fave_pairs[4][1] = pairs->high_pair[4][1];
        sets->master_fav_five = pairs->low_place;
    
    }else {

        sets->master_gros_pairs[0][0] = pairs->low_pair[0][0];
        sets->master_gros_pairs[0][1] = pairs->low_pair[0][1];
        sets->master_gros_one = pairs->high_place;

        sets->master_gros_pairs[1][0] = pairs->low_pair[1][0];
        sets->master_gros_pairs[1][1] = pairs->low_pair[1][1];
        sets->master_gros_two = pairs->second_place;

        sets->master_gros_pairs[2][0] = pairs->low_pair[2][0];
        sets->master_gros_pairs[2][1] = pairs->low_pair[2][1];
        sets->master_fav_three = pairs->third_place;

        sets->master_gros_pairs[3][0] = pairs->low_pair[3][0];
        sets->master_gros_pairs[3][1] = pairs->low_pair[3][1];
        sets->master_gros_four = pairs->fourth_place;

        sets->master_gros_pairs[4][0] = pairs->low_pair[4][0];
        sets->master_gros_pairs[4][1] = pairs->low_pair[4][1];
        sets->master_gros_five = pairs->low_place;
    }

}
*/

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

    }else if(id->t_menu_id == 5) {
        printf("Entering DEBUG.\n");
        debug = true;
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
                          
    printf("Not valid to use for style_id.\n");

  }else if(current_sel == 1) {

    set_lcd_cursor(1,0);   
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->top_menu[(current_sel + 2)]);

  }else if(current_sel == 2){
  
    set_lcd_cursor(1,0);   
    print_lcd(names->top_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->top_menu[(current_sel + 1)]);

  }else if(current_sel == 3) {
    
    set_lcd_cursor(1,0);   
    print_lcd(names->top_menu[(current_sel - 2)]);
        
        
    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[(current_sel -1)]);
        
    set_lcd_cursor(3,0);
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);
    
  }else if(current_sel == 4) {

    set_lcd_cursor(1,0);   
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->top_menu[current_sel + 1]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);

  }else {

    set_lcd_cursor(1,0);   
    print_lcd(names->top_menu[current_sel - 1]);
        

    set_lcd_cursor(2,0);
        menu_highlighter(names->top_menu[current_sel]);
    print_lcd(names->top_menu[current_sel]);
        menu_dehighlighter(names->top_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);
    }

}


//  runs inside style menu while loop
void style_selection_menu(int_vars *level, char_vars *names, uint8_t current_sel){

    set_lcd_cursor(0,0);
    print_lcd(names->style_menu[0]);

  if(current_sel == 0){
                 
    printf("Zero is invalid for beer_id.\n");

  }else if(current_sel == 1){

    set_lcd_cursor(1,0);   
        menu_highlighter(names->style_menu[current_sel]);
    print_lcd(names->style_menu[current_sel]);
        menu_dehighlighter(names->style_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->style_menu[(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel + 2)]);

  }else if((current_sel > 1) && (current_sel < 12)) {
     
    set_lcd_cursor(1,0);   
    print_lcd(names->style_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->style_menu[current_sel]);
    print_lcd(names->style_menu[current_sel]);
        menu_dehighlighter(names->style_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel + 1)]);
    
  }else if(current_sel == 12){
                
    set_lcd_cursor(1,0);   
    print_lcd(names->style_menu[(current_sel - 2)]);
        
    set_lcd_cursor(2,0);
    print_lcd(names->style_menu[(current_sel - 1)]);
        
        menu_highlighter(names->style_menu[current_sel]);
    set_lcd_cursor(3,0);
    print_lcd(names->style_menu[(current_sel)]);
        menu_dehighlighter(names->style_menu[current_sel]);
    }else {
        printf("Vial witches of the northern kingdom!\n");

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
    print_lcd(names->beer_menu[menus->style_id][menus->beer_id]);

  if(current_sel == 0){

    set_lcd_cursor(1,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(2,0);
    print_lcd(names->data_menu[(current_sel + 1)]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 2)]);

  }else if(current_sel == 1){

    set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 1)]);

  }else if(current_sel == 2){

    set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 1)]);

    
  }else if(current_sel == 3){

        set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[(current_sel + 1)]);

  }else if(current_sel == 4){
    
            set_lcd_cursor(1,0);
    print_lcd(names->data_menu[(current_sel - 1)]);
        
    set_lcd_cursor(2,0);
        menu_highlighter(names->data_menu[current_sel]);
    print_lcd(names->data_menu[current_sel]);
        menu_dehighlighter(names->data_menu[current_sel]);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);
    
  }
}


//  runs inside sense menu (aka data entry)
void sense_menu(int_vars *menus, char_vars *names){

    //  Set cursor, and print out currently selected sense menu item (look, taste, smell, etc)
    set_lcd_cursor(0,0);
    print_lcd(names->data_menu[menus->sense_id]);
                    
    set_lcd_cursor(1,0);   
    sprintf(names->menu_input, "Enter Score: %i/20", rotary.rotary_total);
    print_lcd(names->menu_input);

    reset_menu_in(names);

    set_lcd_cursor(2,0);
    sprintf(names->menu_input, " Prev Score: %i/20", previous_score[my_vars.style_id][my_vars.beer_id][my_vars.sense_id][0]);
    print_lcd(names->menu_input);

    set_lcd_cursor(3,0);
    print_lcd(names->data_menu[5]);

}


void debug_display(char_vars *name, int_vars *levels){

    //  Set cursor, and print out currently selected sense menu item (look, taste, smell, etc)
    set_lcd_cursor(0,0);
    print_lcd(name->style_menu[levels->style_id]);
                    
    set_lcd_cursor(1,0);   
    print_lcd(name->beer_menu[levels->style_id][levels->beer_id]);

    set_lcd_cursor(2,0);
    print_lcd(name->data_menu[levels->sense_id]);

    set_lcd_cursor(3,0);
    sprintf(name->menu_input, "Current Input: %i.", previous_score[levels->style_id][levels->beer_id][levels->sense_id][1]);
    print_lcd(name->menu_input);

}

    // sprintf();

void display_faves(char_vars *names, ebit line, ebit style, ebit beer){

    set_lcd_cursor(line, 0);
    print_lcd(names->beer_menu[style][beer]);

}


void faves_menu(high_duos *hs, ebit cur_sel){

    uint8_t cur_sel;
    cur_sel = 0;
    cur_sel = menu_selection;

    if(!my_bools.nv_back){
        //  Enter isn't used in this function, so if it's pressed, we'll just reset it to false.
        if(my_bools.nv_enter){
            my_bools.nv_enter = false;
        }

    set_lcd_cursor(0,0);
    print_lcd("    Faves  Menu    ");

  if(cur_sel == 0){
                    
    set_lcd_cursor(1,0);
        sprintf(my_menus.menu_input, "Highest Score: %i.", hs->high_place);
    print_lcd(my_menus.menu_input);
        reset_menu_in(&my_menus);

    display_faves(&my_menus, 2, my_pairs.high_pair[0][0], my_pairs.high_pair[0][1]);

    set_lcd_cursor(3,0);
    print_lcd(my_menus.data_menu[5]);

  }else if(cur_sel == 1){

        set_lcd_cursor(1,0);
        sprintf(my_menus.menu_input, "Silver Medal: %i.", hs->second_place);
    print_lcd(my_menus.menu_input);
        reset_menu_in(&my_menus);

    display_faves(&my_menus, 2, my_pairs.high_pair[1][0], my_pairs.high_pair[1][1]);

    set_lcd_cursor(3,0);
    print_lcd(my_menus.data_menu[5]);

  }else if(cur_sel == 2){

            set_lcd_cursor(1,0);
        sprintf(my_menus.menu_input, "Bronze Medal: %i.", hs->third_place);
    print_lcd(my_menus.menu_input);
        reset_menu_in(&my_menus);

    display_faves(&my_menus, 2, my_pairs.high_pair[2][0], my_pairs.high_pair[2][1]);

    set_lcd_cursor(3,0);
    print_lcd(my_menus.data_menu[5]);

  }else if(cur_sel == 3){

        set_lcd_cursor(1,0);
        sprintf(my_menus.menu_input, "Participation: %i.", hs->fourth_place);
    print_lcd(my_menus.menu_input);
        reset_menu_in(&my_menus);

    display_faves(&my_menus, 2, my_pairs.high_pair[3][0], my_pairs.high_pair[3][1]);

    set_lcd_cursor(3,0);
    print_lcd(my_menus.data_menu[5]);

  }else if(cur_sel == 4){

        set_lcd_cursor(1,0);
        sprintf(my_menus.menu_input, "Mehstest Beer: %i.", hs->low_place);
    print_lcd(my_menus.menu_input);
        reset_menu_in(&my_menus);

    display_faves(&my_menus, 2, my_pairs.high_pair[4][0], my_pairs.high_pair[4][1]);

    set_lcd_cursor(3,0);
    print_lcd(my_menus.data_menu[5]);

  }else {

    printf("Error in menu_selection volatile bool section.\n");
  }

    }
}


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

        if(my_bools.top_menu){
            if(menu_selection < 6){
                menu_selection++;
            }else {
                printf("Bottom of t_menu.\n");
            }
        }else if(my_bools.styles_menu){
            if(menu_selection < 15){
                menu_selection++;
            }else{
                printf("Bottom of s_menu.\n");
            }
        }else if(my_bools.faves_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else{
                printf("Bottom of f_menu.\n");
            }
        }else if(my_bools.gross_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of g_menu.\n");
            }
        }else if(my_bools.beers_menu){
            if(menu_selection < 7){
                menu_selection++;
            }else {
                printf("Bottom of styles_menu.\n");
            }
        }else if(my_bools.data_menu){
            if(menu_selection < 5){
                menu_selection++;
            }else {
                printf("Bottom of styles_menu.\n");
            }
        }else if(my_bools.data_entry_menu){
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
        
        vol_boos.enter = true;
        my_bools.nv_enter = vol_boos.enter;

    }else if(gpio == enabled_buttons.button_four){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nRESET Button Pressed.\n");
        
        vol_boos.reset = true;
        my_bools.nv_reset = vol_boos.reset;

    }else if(gpio == enabled_buttons.button_five){
        gpio_acknowledge_irq(gpio, events);
        printf("\n\nBACK Button Pressed.\n");
        
        vol_boos.back = true;
        my_bools.nv_back = vol_boos.back;

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
void value_inits(boo_vars *bools, int_vars *i_vars, high_duos *arrs){

    printf("Top Menu set as true.\n");
    bools->top_menu = true;
        sleep_ms(100);

    printf("Styles Menu set as false.\n");
    bools->styles_menu = false;
        sleep_ms(100);

    printf("Faves Menu set as false.\n");
    bools->faves_menu = false;
        sleep_ms(100);

    printf("Grosses Menu set as false.\n");
    bools->gross_menu = false;
        sleep_ms(100);

    printf("About Menu set as false.\n");
    bools->menu_about = false;
        sleep_ms(100);

    printf("Beers Menu set as false.\n");
    bools->beers_menu = false;
        sleep_ms(100);

    printf("Data Menu set as false.\n");
    bools->data_menu = false;
        sleep_ms(100);

    printf("Data entry set as false.\n");
    bools->data_entry_menu = false;
        sleep_ms(100);

    printf("RESET set as FALSE.\n");
    bools->nv_reset = false;
        sleep_ms(100);

    printf("ENTER set as FALSE.\n");
    bools->nv_enter = false;
        sleep_ms(100);

    printf("BACK set as FALSE.\n");
    bools->nv_back = false;
        sleep_ms(100);

    printf("Setting all menu id's as 1.\n");
    i_vars->t_menu_id = 1;
        sleep_ms(2);
    i_vars->style_id = 1;
        sleep_ms(2);
    i_vars->beer_id = 1;
        sleep_ms(2);
    i_vars->sense_id= 1;
        sleep_ms(2);


    for(int i = 0; i < 15; i++){
      for(int j = 0; j < 7; j++){
          for(int k = 0; k < 5; k++){

        i_vars->current_scores[i][j][k][0] = 0;

            }
        }
    }

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 7; j++){
            for(int k = 0; k < 5; k++){

            i_vars->previous_scores[i][j][k][0] = 0;

            }
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            for(int k = 0; k < 2; k ++){
                i_vars->mri_status[i][j][k] = 0;
            }
        }
    }
 
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 2; j++){    

                arrs->high_pair[i][j] = 0;

        }
    }


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


//  This is a multi conditional function inside
//  It should ifdef between read/write, and faves/grosses
//Params are:
//  score: The current total score of a beer,
//  location: The current location of the beer
void master_to_slave(uint8_t score, uint16_t location, master_pairs *pairs){

    uint8_t separator;
    uint8_t com_stat;
    separator, com_stat = 0;

#ifdef M_Write

    //  Load transfer with the first byte of the location.
    separator = (location & 0xFF00);
    //  Perform transfer
    com_stat += i2c_master_write(&my_coms, separator);
    //  Wait a few ms.
    sleep_ms(20);
    //  Reset separator value to zero before next byte.
    separator = 0;

    //  Load separator with second byte of the location.
    separator = (location & 0x00FF);
    //  Perform transfer
    com_stat += i2c_master_write(&my_coms, separator);
    //  Wait a few ms.
    sleep_ms(20);
    //  Reset separator value to zero before sending score byte.
    separator = 0;

    //  Load separator with score byte.
    separator = score;
    //  Perform transfer
    com_stat += i2c_master_write(&my_coms, separator);
    //  Wait a few ms.
    sleep_ms(20);

    if(com_stat == 18){
        printf("I2C Master->Slave Write Success.\n");
    }else {
        printf("I2C Master->Slave Communication Error.\n");
    }

#endif

#ifdef M_Read
//  Reading multiple values back from slave.
#ifdef M_Faves

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[0][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[0][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_fav_one = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[1][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[1][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_fav_two = ar_to_ebit(my_coms.rx_buffer);
 
    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[2][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[2][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_fav_three = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[3][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[3][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_fav_four = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[4][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_fave_pairs[4][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_fav_five = ar_to_ebit(my_coms.rx_buffer);

#endif
#ifdef M_Grosses


    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[0][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[0][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_gros_one = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[1][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[1][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_gros_two = ar_to_ebit(my_coms.rx_buffer);
 
    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[2][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[2][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_gros_three = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[3][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[3][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_gros_four = ar_to_ebit(my_coms.rx_buffer);

    // Read from slave device
    i2c_master_read(&my_coms);
    //  Store first location of beer into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[4][0]);
    //  Read second location from slave device.
    i2c_master_read(&my_coms);
    //  Store second location into array
    ar_to_ar(my_coms.rx_buffer, pairs->master_gros_pairs[4][1]);
    //  Read total beer score value from slave device.
    i2c_master_read(&my_coms);
    //  Store total value into respective positions.
    pairs->master_gros_five = ar_to_ebit(my_coms.rx_buffer);

#endif
#endif
}


void slave_to_master(high_duos *pairs){

    uint8_t slave_status;
    slave_status = 0;
    uint8_t buf, buf_o, buf_t;
    buf, buf_o, buf_t = 0;

#ifdef S_Write
#ifdef S_Faves
    // Call top_fivers with true to generate highest rated beer values.
    top_fivers(true, pairs);

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[0][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[0][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->high_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[1][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[1][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->second_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[2][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[2][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->third_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[3][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[3][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->fourth_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[4][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->high_pair[4][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->low_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    if(slave_status == 90){
        printf("Slave Faves I2C data written.\n");
    }else {
        printf("Slave Faves I2C communication error.\n");
    }

#endif
#ifdef S_Grosses

    // Call top_fivers with true to generate highest rated beer values.
    top_fivers(false, pairs);

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[0][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[0][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->high_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[1][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[1][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->second_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[2][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[2][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->third_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[3][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[3][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->fourth_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    //  Copy current first byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[4][0];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current second byte of location to transfer_out
    pairs->transfer_out = pairs->low_pair[4][1];
    //  Perform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6
    //  Copy current high place score to transfer out.
    pairs->transfer_out = pairs->low_place;
    //  Peform transmit.
    slave_status += i2c_slave_write(&my_coms, pairs->transfer_out);     //+6

    if(slave_status == 90){
        printf("Slave Grosses I2C data written.\n");
    }else {
        printf("Slave Grosses I2C communication error.\n");
    }



#endif
#endif

#ifdef S_Read

    //  Read from master the first beer location data.
    i2c_slave_read(&my_coms);
    //  Convert sent array to uint8_t value.
    buf = ar_to_ebit(my_coms.rx_buffer);
    //  Wait a few ms.
    sleep_ms(20);

    //  Read from master the second beer location data.
    i2c_slave_read(&my_coms);
    //  Convert sent array to uint8_t value.
    buf_o = ar_to_ebit(my_coms.rx_buffer);
    //  Wait a few ms.
    sleep_ms(20);

    //  Read from master the total score data.
    i2c_slave_read(&my_coms);
    //  Convert sent array to uint8_t value.
    buf_t = ar_to_ebit(my_coms.rx_buffer);
    //  Wait a few ms.
    sleep_ms(20);

    printf("Current Read:\nStyle: %i\nBeer: %i\nScore: %i.\n\n", buf, buf_o, buf_t);

    beer_score[buf][buf_o][buf_t];

#endif

}


#endif