#ifndef BEER_FUNCTIONS_H
#define BEER_FUNCTIONS_H

#include <stdio.h>
#include "beer_list.h"
#include "pin_manager.h"
#include "error_manager.h"
#define ebit uint8_t

volatile bool back, enter, reset, error;

//  Define SPI header packet parts.
#define Master_Write 0x80
#define Master_Read 0x40

//  Array data for beer array will be accessed by switching() defined with beers enums
//  Inner brackets will be bianry choice of current bias / previous bias.
//  The beers will be selected by hex values from 0x00 - 0x49
const uint8_t beer_array[80][2];

//  73 beers... jeez
typedef enum beers {
    voodoo_ranger = 0,
    snake_dog,
    big_little_fish,
    furious_ipa,
    union_jack,
    harvest_ale,
    obsidian,
    milk_stout,
    the_poet,
    xocoveza,
    guiness_extra,
    double_cream,
    burbon_county,
    coffee_baltic,
    gravity_drop,
    sextus,
    beer_bash,
    baltic_style,
    the_tarten,
    budweiser,
    miller_lite,
    heineken,
    stella_artois,
    corona,
    urquell,
    sts_pils,
    heater_allen,
    live_oak,
    keller_pils,
    allagash,
    summer_ale,
    blood_orange,
    belgin_white,
    lambic,
    stoopid_wit,
    fifteen_hundred,
    doggie_style,
    daisey_cutter,
    whirlpool,
    pale_ale,
    sweet_action,
    fat_tire,
    nugget_nector,
    hop_head_red,
    amber_ale,
    flipside,
    brown_ale,
    nut_brown,
    best_brown,
    brekles_brown,
    palo_santo,
    red_one,
    blonde_ale,
    red_two,
    krick_lambic,
    duvel,
    abbey_ale,
    soest_road,
    pere_jacques,
    benediction,
    wild_duck,
    dub_sack,
    saison_du_buff,
    prop_culture,
    saison_83,
    saison_du_swamp,
    eukaryote,
    stone_saison,
    miami_madness,
    supplication,
    le_terroin,
    duck_duck_goose,
    amorphia,
    rubaeus

};


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