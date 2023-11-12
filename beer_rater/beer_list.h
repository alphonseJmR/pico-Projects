#ifndef BEERLIST
#define BEERLIST

#include <stdio.h>
#include "pico/stdlib.h"

typedef struct {
    char top_menu_one[16];
    char top_menu_two[16];
    char top_menu_three[16];
    char top_menu_four[16];
    char top_menu_five[16];
    char top_menu_six[16];
} top_menu;

top_menu t_menu = {
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
#endif