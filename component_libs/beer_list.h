#ifndef BEERLIST
#define BEERLIST

#include <stdio.h>
#include "pico/stdlib.h"
#include "peri_header.h"
#include "lcd_16x2.h"

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
    char top_menu_error[16];
} top_menu;

top_menu t_menu = {
    .top_menu_one = {'*', '*', '*', 'M', 'E', 'N', 'U', '*', '*', '*', '*', '*', '*', '*', '*',},
    .top_menu_two = {'*', '*', 'S', 't', 'y', 'l', 'e', 's', '*', '*', '*', '*', '*', '*', '*',},
    .top_menu_three = {'*', '*', 'C', 'o', 'm', 'p', 'a', 'n', 'i', 'e', 's', '*', '*', '*', '*',},
    .top_menu_four = {'*', '*', 'F', 'a', 'v', 'o', 'r', 'i', 't', 'e', 's', '*', '*', '*', '*',},
    .top_menu_five = {'*', '*', 'D', 'i', 's', 'l', 'i', 'k', 'e', 's', '*', '*', '*', '*', '*',},
    .top_menu_six = {'*', '*', '*', '*', 'A', 'b', 'o', 'u', 't', '*', '*', '*', '*', '*', '*',},
    .top_menu_error = {'*', '*', '*', '*', '*', 'E', 'R', 'R', 'O', 'R', '*', '*', '*', '*', '*'}
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
    uint8_t top_menu_count;
    uint8_t style_menu_count;
    uint8_t company_menu_count;
}menu_increment_values;
menu_increment_values menu_values;

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
    char henieken[16];
    char stella_artois[16];
    char corona[16];
    
}style_lagers;

style_lagers lagers = {

    .budweiser = {'*', '*', 'B', 'u', 'd', 'w', 'e', 'i', 's', 'e', 'r', '*', '*', '*', '*'},
    .miller_lite = {'*', 'M', 'i', 'l', 'l', 'e', 'r', ' ', 'L', 'i', 't', 'e', '*', '*', '*'},
    .henieken = {'*', '*', '*', 'H', 'e', 'i', 'n', 'e', 'k', 'e', 'n', '*', '*', '*', '*', '*'},
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


volatile bool button_return;
volatile bool rotary_selected;

void menu_selection(bool selection, bool button_return, menu_increment_values *config, const LCDPins *pins){

    switch(config->top_menu_count){

        case 0:

          while(!selection){
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_two, 0, 0, NULL);
          }
          while(!button_return){
            style_selection(config, pins, selection, button_return);
          }
          write_display(pins, t_menu.top_menu_one, t_menu.top_menu_two, 0, 0, NULL);
            break;

        case 1:

          while(!selection){
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_three, 0, 0, NULL);
          }
          while(!button_return){
            company_selection(config, pins, selection, button_return);
          }
          write_display(pins, t_menu.top_menu_one, t_menu.top_menu_three, 0, 0, NULL);
            break;


        case 2:

          while(!selection){
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_four, 0, 0, NULL);
          }
          while(!button_return){
            company_selection(config, pins, selection, button_return);
          }
          write_display(pins, t_menu.top_menu_one, t_menu.top_menu_four, 0, 0, NULL);
            break;

        case 3:
            
          while(!selection){
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_five, 0, 0, NULL);
          }
          while(!button_return){
            favorites_selection(config, pins, selection, button_return);
          }
          write_display(pins, t_menu.top_menu_one, t_menu.top_menu_five, 0, 0, NULL);
            break;

        case 4:
            
          while(!selection){
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_six, 0, 0, NULL);
          }
          while(!button_return){
            dislikes_selection(config, pins, selection, button_return);
          }
          write_display(pins, t_menu.top_menu_one, t_menu.top_menu_six, 0, 0, NULL);
            break;

        case 5:
            
          while(!button_return){
            write_display(pins, about_top_line, about_bottom_line, 0, 0, NULL);
          }
            break;

        default:
            write_display(pins, t_menu.top_menu_one, t_menu.top_menu_error, 0, 0, NULL);
            break;
    }
    }

void style_selection(menu_increment_values *config, const LCDPins *pins, bool selection, bool button_return){

    switch(config->style_menu_count){
        case 0:

          while(!selection){
            write_display(pins, t_menu.top_menu_two, styles.IPA, 0, 0, NULL);
          }
          while(!button_return){
            write_display(pins, t_menu.top_menu_two, )
          }
            break;
        case 1:
            write_display(pins, t_menu.top_menu_two, styles.stout, 0, 0, NULL);
            break;
        case 2:
            write_display(pins, t_menu.top_menu_two, styles.porter, 0, 0, NULL);
            break;
        case 3:
            write_display(pins, t_menu.top_menu_two, styles.lager, 0, 0, NULL);
            break;
        case 4:
            write_display(pins, t_menu.top_menu_two, styles.pilsner, 0, 0, NULL);
            break;
        case 5:
            write_display(pins, t_menu.top_menu_two, styles.wheat_beer, 0, 0, NULL);
            break;
        case 6:
            write_display(pins, t_menu.top_menu_two, styles.pale_ale, 0, 0, NULL);
            break;
        case 7:
            write_display(pins, t_menu.top_menu_two, styles.amber_ale, 0, 0, NULL);
            break;
        case 8:
            write_display(pins, t_menu.top_menu_two, styles.brown_ale, 0, 0, NULL);
            break;
        case 9:
            write_display(pins, t_menu.top_menu_two, styles.belgium_ale, 0, 0, NULL);
            break;
        case 10:
            write_display(pins, t_menu.top_menu_two, styles.belgium_dubbel, 0, 0, NULL);
            break;
        case 11:
            write_display(pins, t_menu.top_menu_two, styles.tripel, 0, 0, NULL);
            break;
        case 12:
            write_display(pins, t_menu.top_menu_two, styles.saison, 0, 0, NULL);
            break;
        case 13:
            write_display(pins, t_menu.top_menu_two, styles.hefeweizen, 0, 0, NULL);
            break;
        case 14:
            write_display(pins, t_menu.top_menu_two, styles.barely_wine, 0, 0, NULL);
            break;
        case 15:
            write_display(pins, t_menu.top_menu_two, styles.sour_ale, 0, 0, NULL);
            break;
        default:
            write_display(pins, t_menu.top_menu_two, t_menu.top_menu_error, 0, 0, NULL);
            break;
    }
}

void company_selection(menu_increment_values *config, const LCDPins *pins, bool selection, bool button_return){

    switch(config->company_menu_count){
        case 0:
            write_display(pins, t_menu.top_menu_two, styles.IPA, 0, 0, NULL);
            break;
        case 1:
            write_display(pins, t_menu.top_menu_two, styles.stout, 0, 0, NULL);
            break;
        case 2:
            write_display(pins, t_menu.top_menu_two, styles.porter, 0, 0, NULL);
            break;
        case 3:
            write_display(pins, t_menu.top_menu_two, styles.lager, 0, 0, NULL);
            break;
        case 4:
            write_display(pins, t_menu.top_menu_two, styles.pilsner, 0, 0, NULL);
            break;
        case 5:
            write_display(pins, t_menu.top_menu_two, styles.wheat_beer, 0, 0, NULL);
            break;
        case 6:
            write_display(pins, t_menu.top_menu_two, styles.pale_ale, 0, 0, NULL);
            break;
        case 7:
            write_display(pins, t_menu.top_menu_two, styles.amber_ale, 0, 0, NULL);
            break;
        case 8:
            write_display(pins, t_menu.top_menu_two, styles.brown_ale, 0, 0, NULL);
            break;
        case 9:
            write_display(pins, t_menu.top_menu_two, styles.belgium_ale, 0, 0, NULL);
            break;
        case 10:
            write_display(pins, t_menu.top_menu_two, styles.belgium_dubbel, 0, 0, NULL);
            break;
        case 11:
            write_display(pins, t_menu.top_menu_two, styles.tripel, 0, 0, NULL);
            break;
        case 12:
            write_display(pins, t_menu.top_menu_two, styles.saison, 0, 0, NULL);
            break;
        case 13:
            write_display(pins, t_menu.top_menu_two, styles.hefeweizen, 0, 0, NULL);
            break;
        case 14:
            write_display(pins, t_menu.top_menu_two, styles.barely_wine, 0, 0, NULL);
            break;
        case 15:
            write_display(pins, t_menu.top_menu_two, styles.sour_ale, 0, 0, NULL);
            break;
        default:
            write_display(pins, t_menu.top_menu_two, t_menu.top_menu_error, 0, 0, NULL);
            break;
    }
}

void favorites_selection(menu_increment_values *config, const LCDPins *pins, bool selection, bool button_return){

}

void dislikes_selection(menu_increment_values *config, const LCDPins *pins, bool selection, bool button_return){

}

void beer_selection(){

}

typedef struct {
    uint8_t current_menu_level;
    uint8_t current_selection;
    uint8_t menu_selection;
    uint8_t previous_menu_zero;
    uint8_t previous_menu_one;
    uint8_t previous_menu_two;
    uint8_t previous_menu_three;
    uint8_t previous_menu_four;

}menu_values;

menu_values m_vals;

void menu_function(menu_values *values, LCDPins *pins, top_menu *menu, bool return_button, bool selection_button){
    
    if(values->current_menu_level == 0 && values->previous_menu_zero == NULL){

        values->current_selection = enabled_buttons.rotary_current_value;
        enabled_buttons.rotary_current_value = 0;

        switch(values->current_selection){
            case 0:
              write_display(pins, menu->top_menu_zero, menu->top_menu_one, 0, 0, NULL);
              break;
            case 1:
              write_display(pins, menu->top_menu_zero, menu->top_menu_two, 0, 0, NULL);
              break;
            case 2:
              write_display(pins, menu->top_menu_zero, menu->top_menu_three, 0, 0, NULL);
              break;
            case 3:
              write_display(pins, menu->top_menu_zero, menu->top_menu_four, 0, 0, NULL);
              break;
            case 4:
              write_display(pins, menu->top_menu_zero, menu->top_menu_five, 0, 0, NULL);
              break;
            default:
              write_display(pins, menu->top_menu_zero, menu->top_menu_error, 0, 0, NULL);
              break;
        }
    
        values->previous_menu_zero = values->current_selection;

        if(selection_button){
            values->current_menu_level = 1;
            values->current_selection = enabled_buttons.rotary_current_value;
        }
    }
    if(values->current_menu_level == 0 && values->previous_menu_zero > 0){

        values->current_selection = enabled_buttons.rotary_current_value 

        switch(values->current_selection){
            case 0:
              write_display(pins, menu->top_menu_zero, menu->top_menu_one, 0, 0, NULL);
              break;
            case 1:
              write_display(pins, menu->top_menu_zero, menu->top_menu_two, 0, 0, NULL);
              break;
            case 2:
              write_display(pins, menu->top_menu_zero, menu->top_menu_three, 0, 0, NULL);
              break;
            case 3:
              write_display(pins, menu->top_menu_zero, menu->top_menu_four, 0, 0, NULL);
              break;
            case 4:
              write_display(pins, menu->top_menu_zero, menu->top_menu_five, 0, 0, NULL);
              break;
            default:
              write_display(pins, menu->top_menu_zero, menu->top_menu_error, 0, 0, NULL);
              break;
        }

        values->previous_menu_zero = values->current_selection;

         if(selection_button){
            values->current_menu_level = 1;
            values->current_selection = enabled_buttons.rotary_current_value;
        }
    }
    if(values->current_menu_level == 1 && values->previous_menu_zero == NULL) {

      values->current_selection = 0;
      enabled_buttons.rotary_current_value = 0;

        if(!return_button){

            values->current_selection = enabled_buttons.rotary_current_value;

            switch(values->current_selection){

                case 0:
                  write_display(pins, menu->top_menu_two, styles.IPA, 0, 0, NULL);
                  break;
                case 1:
                  write_display(pins, menu->top_menu_two, styles.stout, 0, 0, NULL);
                  break;
                case 2:
                  write_display(pins, menu->top_menu_two, styles.porter, 0, 0, NULL);
                  break;
                case 3:
                  write_display(pins, menu->top_menu_two, styles.lager, 0, 0, NULL);
                  break;
                case 4:
                  write_display(pins, menu->top_menu_two, styles.pilsner, 0, 0, NULL);
                  break;
                case 5:
                  write_display(pins, menu->top_menu_two, styles.wheat_beer, 0, 0, NULL);
                  break;
                case 6:
                  write_display(pins, menu->top_menu_two, styles.pale_ale, 0, 0, NULL);
                  break;
                case 7:
                  write_display(pins, menu->top_menu_two, styles.amber_ale, 0, 0, NULL);
                  break;
                case 8:
                  write_display(pins, menu->top_menu_two, styles.brown_ale, 0, 0, NULL);
                  break;
                case 9:
                  write_display(pins, menu->top_menu_two, styles.belgium_ale, 0, 0, NULL);
                  break;
                case 10:
                  write_display(pins, menu->top_menu_two, styles.belgium_dubbel, 0, 0, NULL);
                  break;
                case 11:
                  write_display(pins, menu->top_menu_two, styles.tripel, 0, 0, NULL);
                  break;
                case 12:
                  write_display(pins, menu->top_menu_two, styles.saison, 0, 0, NULL);
                  break;
                case 13:
                  write_display(pins, menu->top_menu_two, styles.sour_ale, 0, 0, NULL);
                  break;
                  
            }
           values->previous_menu_two = values->current_selection;

         if(selection_button){
            values->current_menu_level = 2;
            values->current_selection = enabled_buttons.rotary_current_value;
        }       
      }
        values->menu_level -= 1;
    }
    if(values->current_menu_level == 1 && values->previous_menu_zero > 0) {

        if(!return_button){

        values->current_selection = values->previous_menu_two;

            switch(values->current_selection){

                case 0:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 1:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 2:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 3:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 4:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 5:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 6:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 7:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 8:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 9:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 10:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 11:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 12:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 13:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                  
            }
          if(selection_button){
            values->current_menu_level = 2;
            values->menu_selection = menu_one_value;
        }
            
        }
        values->menu_level -= 1;
    }
    if(values->current_menu_level == 2 && values->previous_menu_zero == NULL) {

      values->current_selection = 0;
      enabled_buttons.rotary_current_value = 0;

        if(!return_button){

          values->current_selection = enabled_buttons.rotary_current_value;

            switch(values->current_selection){

                case 0:
                  write_display(pins, menu->top_menu_two, styles.IPA, 0, 0, NULL);
                  break;
                case 1:
                  write_display(pins, menu->top_menu_two, styles.stout, 0, 0, NULL);
                  break;
                case 2:
                  write_display(pins, menu->top_menu_two, styles.porter, 0, 0, NULL);
                  break;
                case 3:
                  write_display(pins, menu->top_menu_two, styles.lager, 0, 0, NULL);
                  break;
                case 4:
                  write_display(pins, menu->top_menu_two, styles.pilsner, 0, 0, NULL);
                  break;
                case 5:
                  write_display(pins, menu->top_menu_two, styles.wheat_beer, 0, 0, NULL);
                  break;
                case 6:
                  write_display(pins, menu->top_menu_two, styles.pale_ale, 0, 0, NULL);
                  break;
                case 7:
                  write_display(pins, menu->top_menu_two, styles.amber_ale, 0, 0, NULL);
                  break;
                case 8:
                  write_display(pins, menu->top_menu_two, styles.brown_ale, 0, 0, NULL);
                  break;
                case 9:
                  write_display(pins, menu->top_menu_two, styles.belgium_ale, 0, 0, NULL);
                  break;
                case 10:
                  write_display(pins, menu->top_menu_two, styles.belgium_dubbel, 0, 0, NULL);
                  break;
                case 11:
                  write_display(pins, menu->top_menu_two, styles.tripel, 0, 0, NULL);
                  break;
                case 12:
                  write_display(pins, menu->top_menu_two, styles.saison, 0, 0, NULL);
                  break;
                case 13:
                  write_display(pins, menu->top_menu_two, styles.sour_ale, 0, 0, NULL);
                  break;
                  
            }
           values->previous_menu_three = values->current_selection;
       
       if(selection_button){
            values->current_menu_level = 3;
            values->current_selection = enabled_buttons.rotary_current_value;
        }
      }
        values->menu_level -= 1;
    }
    if(values->current_menu_level == 2 && values->previous_menu_zero > 0) {

        if(!return_button){

            values->current_selection = values->previous_menu_three

            switch(values->current_selection){

                case 0:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 1:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 2:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 3:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 4:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 5:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 6:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 7:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 8:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 9:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 10:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 11:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 12:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                case 13:
                  write_display(pins, top_line, bottom_line, 0, 0, NULL);
                  break;
                }
            }
          if(selection_button){
            values->current_menu_level = 3;
            values->current_selection = enabled_buttons.rotary_current_value;
        }
    }  
        values->current_menu_level -= 1;
    
}
#endif