#ifndef BEERLIST
#define BEERLIST

#include <stdio.h>
#include "peri_header.h"
#include "lcd_16x2.h"
#include "pico/stdlib.h"

volatile bool back;

char intro_top_line[16] = {'*', '*', 'B', 'e', 'e', 'r', ' ', 'R', 'a', 't', 'e', 'r', '*', '*', '*'};
char intro_bottom_line[16] = {'F', 'o', 'r', ' ', 'B', 'e', 'e', 'r', ' ', 'D', 'u', 'd', 'e', 's', '*'};
char about_top_line[16] = {'B', 'y', ':', ' ', 'A', 'l', 'p', 'h', 'o', 'n', 's', 'e', 'J', 'M', 'R'};
char about_bottom_line[16] = {'*', 'V', 'e', 'r', 's', 'i', 'o', 'n', ':', ' ', '1', '.', '0', '0', '*'};

typedef struct {
    char top_menu_one[16];
    char top_menu_two[16];
    char top_menu_three[16];
    char top_menu_four[16];
    char top_menu_five[16];
    char top_menu_six[16];
} top_menu_selection;

top_menu_selection t_menu = {
    .top_menu_one = {'*', '*', '*', 'M', 'E', 'N', 'U', '*', '*', '*', '*', '*', '*', '*', '*',},
    .top_menu_two = {'*', '*', 'S', 't', 'y', 'l', 'e', 's', '*', '*', '*', '*', '*', '*', '*',},
    .top_menu_three = {'*', '*', 'C', 'o', 'm', 'p', 'a', 'n', 'i', 'e', 's', '*', '*', '*', '*',},
    .top_menu_four = {'*', '*', 'F', 'a', 'v', 'o', 'r', 'i', 't', 'e', 's', '*', '*', '*', '*',},
    .top_menu_five = {'*', '*', 'D', 'i', 's', 'l', 'i', 'k', 'e', 's', '*', '*', '*', '*', '*',},
    .top_menu_six = {'*', '*', '*', '*', 'A', 'b', 'o', 'u', 't', '*', '*', '*', '*', '*', '*',},
};

typedef struct {
    char IPA[16];
    char stout[16];
    char porter[16];
    char lager[16];
    char pilsner[16];
    char wheat_beer[16];
    char pale_ale[16];
    char amber_ale[16];
    char brown_ale[16];
    char belgium_ale[16];
    char belgium_dubbel[16];
    char tripel[16];
    char saison[16];
    char hefeweizen[16];
    char barely_wine[16];
    char sour_ale[16];
} beer_styles;

beer_styles styles = {
    .IPA = {'*', '*', '*', '*', '*', 'I', 'p', 'a', '`', 'S', '*', '*', '*', '*', '*'},
    .stout = {'*', '*', '*', '*', 'S', 't', 'o', 'u', 't', '`', 's', '*', '*', '*', '*'},
    .porter = {'*', '*', '*', '*', 'P', 'o', 'r', 't', 'e', 'r', '`', 's', '*', '*', '*'},
    .lager = {'*', '*', '*', '*', 'L', 'a', 'g', 'e', 'r', '`', 's', '*', '*', '*', '*'},
    .pilsner = {'*', '*', '*', 'P', 'i', 'l', 's', 'n', 'e', 'r', '`', 's', '*', '*', '*'},
    .wheat_beer = {'*', '*', 'W', 'h', 'e', 'a', 't', ' ', 'B', 'e', 'e', 'r', '*', '*', '*'},
    .pale_ale = {'*', '*', '*', 'P', 'a', 'l', 'e', ' ', 'A', 'l', 'e', '`', 's', '*', '*'},
    .amber_ale = {'*', '*', 'A', 'm', 'b', 'e', 'r', ' ', 'A', 'l', 'e', '`', 's', '*', '*'},
    .brown_ale = {'*', '*', 'B', 'r', 'o', 'w', 'n', ' ', 'A', 'l', 'e', '`', 's', '*', '*'},
    .belgium_ale = {'*', 'B', 'e', 'l', 'g', 'i', 'u', 'm', ' ', 'A', 'l', 'e', '`', 's', '*'},
    .belgium_dubbel = {'B', 'e', 'l', 'g', 'i', 'u', 'm', ' ', 'D', 'u', 'b', 'b', 'e', 'l', '*'},
    .tripel = {'*', '*', '*', '*', 'T', 'r', 'i', 'p', 'e', 'l', '*', '*', '*', '*', '*'},
    .saison = {'*', '*', '*', 'S', 'a', 'i', 's', 'o', 'n', '*', '*', '*', '*', '*', '*'},
    .hefeweizen = {'*', '*', 'H', 'e', 'f', 'e', 'w', 'e', 'i', 'z', 'e', 'n', '*', '*', '*'},
    .barely_wine = {'*', 'B', 'a', 'r', 'l', 'e', 'y', ' ', 'W', 'i', 'n', 'e', '*', '*', '*'},
    .sour_ale = {'*', '*', '*', 'S', 'o', 'u', 'r', ' ', 'A', 'l', 'e', '`', 's', '*', '*'},
};

typedef struct {
    char anheuser_busch[16];
    char heineken[16];
    char constellation_brands[16];
    char molson_coors_brewing[16];
    char sierra_nevada[16];
    char stone_brewing[16];
    char dog_fish_head[16];
    char new_belgium[16];
    char guiness[16];
    char chimay[16];
    char duval_moorgat[16];
    char founders_brewing[16];
} beer_companies;

beer_companies companies = {
    .anheuser_busch = {'A', 'n', 'h', 'e', 'u', 's', 'e', 'r', '-', 'B', 'u', 's', 'c', 'h', '*',},
    .heineken = {'*', '*', 'H', 'e', 'i', 'n', 'e', 'k', 'e', 'n', '*', '*', '*', '*', '*',},
    .constellation_brands = {'*', 'C', 'o', 'n', 's', 't', 'e', 'l', 'l', 'a', 't', 'i', 'o', 'n', '*',},
    .molson_coors_brewing = {'*', '*', 'M', 'o', 'l', 's', 'o', 'n', '-', 'C', 'o', 'o', 'r', 's', '*',},
    .sierra_nevada = {'*', 'S', 'i', 'e', 'r', 'r', 'a', ' ', 'N', 'e', 'v', 'a', 'd', 'a', '*',},
    .stone_brewing = {'*', 'S', 't', 'o', 'n', 'e', ' ', 'B', 'r', 'e', 'w', 'i', 'n', 'g', '*',},
    .dog_fish_head = {'*', 'D', 'o', 'g', ' ', 'F', 'i', 's', 'h', ' ', 'H', 'e', 'a', 'd', '*',},
    .new_belgium = {'*', '*', 'N', 'e', 'w', ' ', 'B', 'e', 'l', 'g', 'i', 'u', 'm', '*', '*',},
    .guiness = {'*', '*', '*', 'G', 'u', 'i', 'n', 'e', 's', 's', '*', '*', '*', '*', '*',},
    .chimay = {'*', '*', '*', '*', 'C', 'h', 'i', 'm', 'a', 'y', '*', '*', '*', '*', '*',},
    .duval_moorgat = {'*', 'D', 'u', 'v', 'a', 'l', ' ', 'M', 'o', 'o', 'g', 'r', 'a', 't', '*',},
    .founders_brewing = {'F', 'o', 'u', 'n', 'd', 'e', 'r', 's', ' ', 'B', 'r', 'e', 'w', 'i', 'n',},
};

typedef struct {
    char voodoo_ranger[16];
    char snake_dog[16];
    char big_little_fish[16];
    char furious_ipa[16];
    char union_jack[16];
    char harvest_ale[16];
}style_ipa;

style_ipa ipas = {
    .voodoo_ranger = {'*', 'V', 'o', 'o', 'd', 'o', 'o', ' ', 'R', 'a', 'n', 'g', 'e', 'r', '*'},
    .snake_dog = {'*', '*', '*', 'S', 'n', 'a', 'k', 'e', ' ', 'D', 'o', 'g', '*', '*', '*'},
    .big_little_fish = {'B', 'i', 'g', ' ', 'L', 'i', 't', 't', 'l', 'e', ' ', 'F', 'i', 's', 'h', '*'},
    .furious_ipa = {'*', '*', 'F', 'u', 'r', 'i', 'o', 'u', 's', ' ', 'I', 'P', 'A', '*', '*', '*'},
    .union_jack = {'*', '*', 'U', 'n', 'i', 'o', 'n', ' ', 'J', 'a', 'c', 'k', '*', '*', '*'},
    .harvest_ale = {'*', 'H', 'a', 'r', 'v', 'e', 's', 't', ' ', 'A', 'l', 'e', '*', '*'}
};


typedef struct {
    char obsidian[16];
    char milk_stout[16];
    char the_poet[16];
    char xocoveza[16];
    char guiness_extra[16];
    char double_cream[16];
    char burbon_county[16];
}style_stouts;

style_stouts stouts = {
    .obsidian = {'*', '*', '*', '*', 'O', 'b', 's', 'i', 'd', 'i', 'a', 'n', '*', '*', '*'},
    .milk_stout = {'*', '*', '*', 'M', 'i', 'l', 'k', ' ', 'S', 't', 'o', 'u', 't', '*', '*'},
    .the_poet = {'*', '*', '*', 'T', 'h', 'e', ' ', 'P', 'o', 'e', 't', '*', '*', '*', '*', '*'},
    .xocoveza = {'*', '*', '*', '*', 'X', 'o', 'c', 'o', 'v', 'e', 'z', 'a','*', '*', '*', '*'},
    .guiness_extra = {'*', 'G', 'u', 'i', 'n', 'e', 's', 's', ' ', 'E', 'x', 't', 'r', 'a', '*'},
    .double_cream = {'*', 'D', 'o', 'u', 'b', 'l', 'e', ' ', 'C', 'r', 'e', 'a', 'm', '*'},
    .burbon_county = {'*', 'B', 'u', 'r', 'b', 'o', 'n', ' ', 'C', 'o', 'u', 'n', 't', '*'}
};


typedef struct {

    char coffee_baltic[16];
    char gravity_drop[16];
    char sextus[16];
    char beer_bash[16];
    char baltic_style[16];
    char the_tarten[16];

}style_porters;

style_porters porters = {

    .coffee_baltic = {'*', 'C', 'o', 'f', 'f', 'e', 'e', ' ', 'B', 'a', 'l', 't', 'i', 'c', '*'},
    .gravity_drop = {'*', 'G', 'r', 'a', 'v', 'i', 't', 'y', ' ', 'D', 'r', 'o', 'p', '*', '*'},
    .sextus = {'*', '*', '*', '*', 'S', 'e', 'x', 't', 'u', 's', '*', '*', '*', '*', '*', '*'},
    .beer_bash = {'*', '*', 'B', 'e', 'e', 'r', ' ', 'B', 'a', 's', 'h', '*','*', '*', '*', '*'},
    .baltic_style = {'*', 'B', 'a', 'l', 't', 'i', 'c', ' ', 'S', 't', 'y', 'l', 'e', '*', '*'},
    .the_tarten = {'*', '*', 'T', 'h', 'e', ' ', 'T', 'a', 'r', 't', 'e', 'n', '*', '*'}
    
};


typedef struct {

    char budweiser[16];
    char miller_lite[16];
    char heineken[16];
    char stella_artois[16];
    char corona[16];
    
}style_lagers;

style_lagers lagers = {

    .budweiser = {'*', '*', 'B', 'u', 'd', 'w', 'e', 'i', 's', 'e', 'r', '*', '*', '*', '*'},
    .miller_lite = {'*', 'M', 'i', 'l', 'l', 'e', 'r', ' ', 'L', 'i', 't', 'e', '*', '*', '*'},
    .heineken = {'*', '*', '*', 'H', 'e', 'i', 'n', 'e', 'k', 'e', 'n', '*', '*', '*', '*', '*'},
    .stella_artois = {'*', 'S', 't', 'e', 'l', 'l', 'a', ' ', 'A', 'r', 't', 'o', 'i', 's', '*', '*'},
    .corona = {'*', '*', '*', '*', 'C', 'o', 'r', 'o', 'n', 'a', '*', '*', '*', '*', '*'}
    
};


typedef struct {

    char urquell[16];
    char sts_pils[16];
    char heater_allen[16];
    char live_oak[16];
    char jever[16];
    char keller_pils[16];
    
}style_pilsners;

style_pilsners pilsners = {
    
    .urquell = {'*', '*', '*', 'U', 'r', 'q', 'u', 'e', 'l', 'l', '*', '*', '*', '*', '*'},
    .sts_pils = {'*', '*', '*', 'S', 'T', 'S', ' ', 'P', 'i', 'l', 's', '*', '*', '*', '*'},
    .heater_allen = {'*', 'H', 'e', 'a', 't', 'e', 'r', ' ', 'A', 'l', 'l', 'e', 'n', '*', '*', '*'},
    .live_oak = {'*', '*', '*', 'L', 'i', 'v', 'e', ' ', 'O', 'a', 'k', '*', '*', '*', '*', '*'},
    .jever = {'*', '*', '*', '*', '*', 'J', 'e', 'v', 'e', 'r', '*', '*', '*', '*', '*'},
    .keller_pils = {'*', '*', 'K', 'e', 'l', 'l', 'e', 'r', ' ', 'P', 'i', 'l', 's', '*', '*'}
    
};


typedef struct {

    char allagash[16];
    char summer_ale[16];
    char blood_orange[16];
    char belgin_white[16];
    char lambic[16];
    char stoopid_wit[16];
    
}style_wheat_beers;

style_wheat_beers wheat_beers = {
    
    .allagash = {'*', '*', 'A', 'l', 'l', 'a', 'g', 'a', 's', 'h', '*', '*', '*', '*', '*'},
    .summer_ale = {'*', '*', 'S', 'u', 'm', 'm', 'e', 'r', ' ', 'A', 'l', 'e', 's', '*', '*'},
    .blood_orange = {'*', '*', 'B', 'l', 'o', 'o', 'd', ' ', 'O', 'r', 'a', 'n', 'g', 'e', '*', '*'},
    .belgin_white = {'*', '*', 'B', 'e', 'l', 'g', 'i', 'n', ' ', 'W', 'h', 'i', 't', 'e', '*', '*'},
    .lambic = {'*', '*', '*', '*', '*', 'L', 'a', 'm', 'b', 'i', 'c', '*', '*', '*', '*'},
    .stoopid_wit = {'*', '*', 'S', 't', 'o', 'o', 'p', 'i', 'd', ' ', 'W', 'i', 't', '*', '*'}
    
};


typedef struct {

    char fifteen_hundred[16];
    char doggie_style[16];
    char daisey_cutter[16];
    char whirlpool[16];
    char pale_ale[16];
    char sweet_action[16];
    
}style_pale_ales;

style_pale_ales pale_ales = {
    
    .fifteen_hundred = {'*', '*', '*', '*', '1', '5', '0', '0', '*', '*', '*', '*', '*', '*', '*'},
    .doggie_style = {'*', '*', 'D', 'o', 'g', 'g', 'i', 'e', ' ', 'S', 't', 'y', 'l', 'e', '*'},
    .daisey_cutter = {'*', '*', 'D', 'a', 'i', 's', 'e', 'y', ' ', 'C', 'u', 't', 't', 'e', 'r', '*'},
    .whirlpool = {'*', '*', '*', 'W', 'h', 'i', 'r', 'l', 'p', 'o', 'o', 'l', '*', '*', '*', '*'},
    .pale_ale = {'*', '*', '*', '*', 'P', 'a', 'l', 'e', ' ', 'A', 'l', 'e', '*', '*', '*'},
    .sweet_action = {'*', '*', 'S', 'w', 'e', 'e', 't', ' ', 'A', 'c', 't', 'i', 'o', 'n', '*'}
    
};


typedef struct {

    char fat_tire[16];
    char nugget_nector[16];
    char hop_head_red[16];
    char amber_ale[16];
    char flipside[16];
    
}style_amber_ales;

style_amber_ales amber_ales = {
    
    .fat_tire = {'*', '*', '*', 'F', 'a', 't', ' ', 'T', 'i', 'r', 'e', '*', '*', '*', '*'},
    .nugget_nector = {'*', 'N', 'u', 'g', 'g', 'e', 't', ' ', 'N', 'e', 'c', 't', 'o', 'r', '*'},
    .hop_head_red = {'*', 'H', 'o', 'p', ' ', 'H', 'e', 'a', 'd', ' ', 'R', 'e', 'd', '*', '*', '*'},
    .amber_ale = {'*', '*', '*', 'A', 'm', 'b', 'e', 'r', ' ', 'A', 'l', 'e', '*', '*', '*', '*'},
    .flipside = {'*', '*', '*', '*', 'F', 'l', 'i', 'p', 's', 'i', 'd', 'e', '*', '*', '*'}
    
};

typedef struct {

    char brown_ale[16];
    char nut_brown[16];
    char best_brown[16];
    char brekles_brown[16];
    char palo_santo[16];
    
}style_brown_ales;

style_brown_ales brown_ales = {
    
    .brown_ale = {'*', '*', 'B', 'r', 'o', 'w', 'n', ' ', 'A', 'l', 'e', '*', '*', '*', '*'},
    .nut_brown = {'*', 'N', 'u', 't', ' ', 'B', 'r', 'o', 'w', 'n', '*', '*', '*', '*', '*'},
    .best_brown = {'*', 'B', 'e', 's', 't', ' ', 'B', 'r', 'o', 'w', 'n', '*', '*', '*', '*', '*'},
    .brekles_brown = {'*', 'B', 'r', 'e', 'k', 'l', 'e', 's', ' ', 'B', 'r', 'o', 'w', 'n', '*', '*'},
    .palo_santo = {'*', '*', 'P', 'a', 'l', 'o', ' ', 'S', 'a', 'n', 't', 'o', '*', '*', '*'}
    
};


typedef struct {

    char red_one[16];
    char blonde_ale[16];
    char red_two[16];
    char krick_lambic[16];
    char duvel[16];
    
}style_belgium_ales;

style_belgium_ales belgium_ales = {
    
    .red_one = {'*', '*', '*', '*', 'R', 'e', 'd', '*', '*', '*', '*', '*', '*', '*', '*'},
    .blonde_ale = {'*', '*', 'B', 'l', 'o', 'n', 'd', 'e', ' ', 'A', 'l', 'e', '*', '*', '*'},
    .red_two = {'*', '*', '*', '*', '*', 'R', 'e', 'd', '*', '*', '*', '*', '*', '*', '*', '*'},
    .krick_lambic = {'*', 'K', 'r', 'i', 'c', 'k', ' ', 'L', 'a', 'm', 'b', 'i', 'c', '*', '*', '*'},
    .duvel = {'*', '*', '*', '*', '*', 'D', 'u', 'v', 'e', 'l', '*', '*', '*', '*', '*'}
    
};


typedef struct {

    char abbey_ale[16];
    char soest_road[16];
    char pere_jacques[16];
    char benedicition[16];
    char wild_duck[16];
    char dub_sack[16];
    
}style_belgium_dubbel;

style_belgium_dubbel belgium_dubbel = {
    
    .abbey_ale = {'*', '*', '*', 'A', 'b', 'b', 'e', 'y', ' ', 'A', 'l', 'e', '*', '*', '*'},
    .soest_road = {'*', '*', 'S', 'o', 'e', 's', 't', ' ', 'R', 'o', 'a', 'd', '*', '*', '*'},
    .pere_jacques = {'*', '*', 'P', 'e', 'r', 'e', ' ', 'J', 'a', 'c', 'q', 'u', 'e', 's', '*', '*'},
    .benedicition = {'*', '*', 'B', 'e', 'n', 'e', 'd', 'i', 'c', 't', 'i', 'i', 'o', 'n', '*', '*'},
    .wild_duck = {'*', '*', '*', 'W', 'i', 'l', 'd', ' ', 'D', 'u', 'c', 'k', '*', '*', '*'},
    .dub_sack = {'*', '*', '*', 'D', 'u', 'b', ' ', 'S', 'a', 'c', 'k', '*', '*', '*', '*'}
    
};


typedef struct {

    char saison_du_buff[16];
    char prop_culture[16];
    char saison_83[16];
    char saison_du_swamp[16];
    char eukaryote[16];
    char stone_saison[16];
    
}style_saison;

style_saison saisons = {
    
    .saison_du_buff = {'*', 'S', 'a', 'i', 's', 'o', 'n', ' ', 'D', 'u', ' ', 'B', 'u', 'f', 'f'},
    .prop_culture = {'*', '*', 'P', 'r', 'o', 'p', ' ', 'C', 'u', 'l', 't', 'u', 'r', 'e', '*'},
    .saison_83 = {'*', '*', 'S', 'a', 'i', 's', 'o', 'n', ' ', '8', '3', '*', '*', '*', '*', '*'},
    .saison_du_swamp = {'*', 'S', 'a', 'i', 's', 'o', 'n', ' ', 'D', 'u', ' ', 'S', 'w', 'a', 'm', 'p'},
    .eukaryote = {'*', '*', '*', 'E', 'u', 'k', 'a', 'r', 'y', 'o', 't', 'e', '*', '*', '*'},
    .stone_saison = {'*', '*', 'S', 't', 'o', 'n', 'e', ' ', 'S', 'a', 'i', 's', 'o', 'n', '*'}
    
};


typedef struct {

    char miami_madness[16];
    char supplication[16];
    char le_terroin[16];
    char duck_duck_goose[16];
    char amorphia[16];
    char rubaeus[16];
    
}style_sour_ales;

style_sour_ales sour_ales = {
    
    .miami_madness = {'*', 'M', 'i', 'a', 'm', 'i', ' ', 'M', 'a', 'd', 'n', 'e', 's', 's', '*'},
    .supplication = {'*', 'S', 'u', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', '*', '*'},
    .le_terroin = {'*', '*', 'L', 'e', ' ', 'T', 'e', 'r', 'r', 'o', 'i', 'n', '*', '*', '*', '*'},
    .duck_duck_goose = {'D', 'u', 'c', 'k', ' ', 'D', 'u', 'c', 'k', ' ', 'G', 'o', 'o', 's', 'e', '*'},
    .amorphia = {'*', '*', '*', 'A', 'm', 'o', 'r', 'p', 'h', 'i', 'a', '*', '*', '*', '*'},
    .rubaeus = {'*', '*', '*', '*', 'R', 'u', 'b', 'a', 'e', 'u', 's', '*', '*', '*', '*'}
    
};

typedef struct {
    int top_menu_value;
    int second_menu_value;
    int third_menu_value;
    int fourth_menu_value;
    int menu_level;
}menu_incrementer_values;
menu_incrementer_values menu_values;


void company_beers(LCDPins *pins, menu_incrementer_values *values){
}

void style_beer_selection(LCDPins *pins, menu_incrementer_values *values){

    values->third_menu_value = 0;
    values->third_menu_value = enabled_buttons.rotary_current_value;
  
  switch(values->third_menu_value) {

    case 0:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(values->third_menu_value == 0){
        write_display(pins, styles.IPA, ipas.voodoo_ranger, 0, 0);
        }
        if(values->third_menu_value == 1){
        write_display(pins, styles.IPA, ipas.snake_dog, 0, 0);
        }
        if(values->third_menu_value == 2){
        write_display(pins, styles.IPA, ipas.big_little_fish, 0, 0);
        }
        if(values->third_menu_value == 3){
        write_display(pins, styles.IPA, ipas.furious_ipa, 0, 0);
        }
        if(values->third_menu_value == 4){
        write_display(pins, styles.IPA, ipas.union_jack, 0, 0);
        }
        if(values->third_menu_value == 5){
        write_display(pins, styles.IPA, ipas.harvest_ale, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 1:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.stout, stouts.obsidian, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.stout, stouts.milk_stout, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.stout, stouts.the_poet, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.stout, stouts.xocoveza, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.stout, stouts.guiness_extra, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.stout, stouts.double_cream, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 6){
        write_display(pins, styles.stout, stouts.burbon_county, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 2:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
        

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.porter, porters.coffee_baltic, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.porter, porters.gravity_drop, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.porter, porters.sextus, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.porter, porters.beer_bash, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.porter, porters.baltic_style, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.porter, porters.the_tarten, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 3:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.lager, lagers.budweiser, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.lager, lagers.miller_lite, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.lager, lagers.heineken, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.lager, lagers.stella_artois, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.lager, lagers.corona, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 4:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.pilsner, pilsners.urquell, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.pilsner, pilsners.sts_pils, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.pilsner, pilsners.heater_allen, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.pilsner, pilsners.live_oak, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.pilsner, pilsners.jever, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.pilsner, pilsners.keller_pils, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 5:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.wheat_beer, wheat_beers.allagash, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.wheat_beer, wheat_beers.summer_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.wheat_beer, wheat_beers.blood_orange, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.wheat_beer, wheat_beers.belgin_white, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.wheat_beer, wheat_beers.lambic, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.wheat_beer, wheat_beers.stoopid_wit, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 6:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.pale_ale, pale_ales.fifteen_hundred, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.pale_ale, pale_ales.doggie_style, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.pale_ale, pale_ales.daisey_cutter, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.pale_ale, pale_ales.whirlpool, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.pale_ale, pale_ales.pale_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.pale_ale, pale_ales.sweet_action, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 7:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.amber_ale, amber_ales.fat_tire, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.amber_ale, amber_ales.nugget_nector, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.amber_ale, amber_ales.hop_head_red, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.amber_ale, amber_ales.amber_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.amber_ale, amber_ales.flipside, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 8:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.brown_ale, brown_ales.brown_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.brown_ale, brown_ales.nut_brown, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.brown_ale, brown_ales.best_brown, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.brown_ale, brown_ales.brekles_brown, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.brown_ale, brown_ales.palo_santo, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 9:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.belgium_ale, belgium_ales.red_one, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.belgium_ale, belgium_ales.blonde_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.belgium_ale, belgium_ales.red_two, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.belgium_ale, belgium_ales.krick_lambic, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.belgium_ale, belgium_ales.duvel, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 10:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.abbey_ale, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.soest_road, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.pere_jacques, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.benedicition, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.wild_duck, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.belgium_dubbel, belgium_dubbel.dub_sack, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 11:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.saison, saisons.saison_du_buff, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.saison, saisons.prop_culture, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.saison, saisons.saison_83, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.saison, saisons.saison_du_swamp, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.saison, saisons.eukaryote, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.saison, saisons.stone_saison, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    case 12:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){

        if(enabled_buttons.rotary_current_value == 0){
        write_display(pins, styles.sour_ale, sour_ales.miami_madness, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 1){
        write_display(pins, styles.sour_ale, sour_ales.supplication, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 2){
        write_display(pins, styles.sour_ale, sour_ales.le_terroin, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 3){
        write_display(pins, styles.sour_ale, sour_ales.duck_duck_goose, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 4){
        write_display(pins, styles.sour_ale, sour_ales.amorphia, 0, 0);
        }
        if(enabled_buttons.rotary_current_value == 5){
        write_display(pins, styles.sour_ale, sour_ales.rubaeus, 0, 0);
        }
      } else {
        styles_menu(pins, values);
      }
    }
    break;

    }
  }

void company_menu(LCDPins *pins, menu_incrementer_values *values){
       
    if(values->second_menu_value >= 0 && values->second_menu_value <= 12){

 switch(values->second_menu_value) {
    case 0:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.anheuser_busch, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 0;
    }
    break;

    case 1:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.heineken, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 1;
    }
    break;

    case 2:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.constellation_brands, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 2;
    }
    break;

    case 3:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.molson_coors_brewing, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 3;
    }
    break;

    case 4:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.sierra_nevada, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 4;
    }
    break;

    case 5:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.stone_brewing, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 5;
    }
    break;

    case 6:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.dog_fish_head, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 6;
    }
    break;

    case 7:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.new_belgium, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 7;
    }
    break;

    case 8:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.guiness, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 8;
    }
    break;

    case 9:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.chimay, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 9;

    }
    break;

    case 10:

      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.duval_moorgat, 0, 0);
    } else {
          company_beers(pins, values);
        }
    values->second_menu_value = 0;
    values->third_menu_value = 10;
    }
    
    break;

    case 11:

      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_three, companies.founders_brewing, 0, 0);
    } else {
          company_beers(pins, values);
        }
      values->second_menu_value = 0;
      values->third_menu_value = 11;
        
    }
    break;

    default:
        printf("Company Menu Selection Error.\n\n");

    }
  }
}

void styles_menu(LCDPins *pins, menu_incrementer_values *values){

    values->third_menu_value = 0;
    
    if(values->second_menu_value >= 0 && values->second_menu_value <= 15){
      if(values->menu_level == 2){

 switch(values->second_menu_value) {
    case 0:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.IPA, 0, 0);
        }else {
          top_menu(pins, values);
        }
    values->third_menu_value = 0;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 1:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.stout, 0, 0);
        } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 1;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 2:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.porter, 0, 0);
      } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 2;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 3:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.lager, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 3;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 4:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.pilsner, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 4;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 5:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.wheat_beer, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 5;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 6:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.pale_ale, 0, 0);
      } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 6;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 7:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.amber_ale, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 7;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 8:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.brown_ale, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 8;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 9:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.belgium_ale, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 9;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 10:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.belgium_dubbel, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 10;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 11:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.tripel, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 11;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 12:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.saison, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 12;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 13:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.hefeweizen, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 13;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 14:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status){
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.hefeweizen, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 14;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    case 15:
      if(!buttons_onoff.rotary_button_status){
        if(!buttons_onoff.button_one_status) {
          values->second_menu_value = values->second_menu_value;
          write_display(pins, t_menu.top_menu_two, styles.sour_ale, 0, 0);
    } else {
          top_menu(pins, values);
        }
    values->third_menu_value = 15;
    values->menu_level = 2;
        style_beer_selection(pins, values);
    }
    break;

    default:
        printf("Style Menu Error.\n\n");
      }
    }
  }
}

void top_menu(LCDPins *pins, menu_incrementer_values *values) {

  if(enabled_buttons.rotary_current_value != (int)NULL){
   values->top_menu_value = last_status.basic_rotary_value++;
  }else {
    values->top_menu_value = enabled_buttons.rotary_current_value;
  }

    if(values->top_menu_value >= 0 && values->top_menu_value <= 5){

 switch(values->top_menu_value) {
    case 0:
      if(!buttons_onoff.rotary_button_status){
        values->top_menu_value = values->top_menu_value;
        write_display(pins, t_menu.top_menu_one, t_menu.top_menu_two, 0, 0);
      }else{
    values->second_menu_value = 0;
    values->menu_level = 1;
        styles_menu(pins, values);
      }
    break;

    case 1:
      if(!buttons_onoff.rotary_button_status){
        values->top_menu_value = values->top_menu_value;
        write_display(pins, t_menu.top_menu_one, t_menu.top_menu_three, 0, 0);
      }else {
    values->second_menu_value = 1;
    values->menu_level = 1;
        company_menu(pins, values);
      }
    break;

    case 2:
      if(!buttons_onoff.rotary_button_status){
        values->top_menu_value = values->top_menu_value;
        write_display(pins, t_menu.top_menu_one, t_menu.top_menu_four, 0, 0);
      }else {
    values->second_menu_value = 2;
    values->menu_level = 1;
        company_menu(pins, values);
      }
    break;

    case 3:
      if(!buttons_onoff.rotary_button_status){
        values->top_menu_value = values->top_menu_value;
        write_display(pins, t_menu.top_menu_one, t_menu.top_menu_five, 0, 0);
      }else {
    values->second_menu_value = 3;
    values->menu_level = 1;
        company_menu(pins, values);
      }
    break;

    default:
      printf("Top Menu Error.\n\n");
    }
  }
}


#endif