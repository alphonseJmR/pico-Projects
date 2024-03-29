#ifndef BEER_SCORE_STORE_H
#define BEER_SCORE_STORE_H

#include <stdio.h>

uint8_t beer_score[15][6][5];
/*  Beer score array is as follows. (it's long)
    For styles that have less than seven total beers, enter their remaining array positions as 0.
--------------------------------------------------------------------------------------------
                        [0]: Head   [1]: Flavor   [2]: Look   [3]: Smell   [4]: Aftertaste
--------------------------------------------------------------------------------------------
Voodoo Ranger       = [0][0][0-5]
Snake Dog           = [0][1][0-5]
Big Little Fish     = [0][2][0-5]
Furious IPA         = [0][3][0-5]
Union Jack          = [0][4][0-5]
Harvest Ale         = [0][5][0-5]

Obsidian            = [1][0][0-5]
Milk Stout          = [1][1][0-5]
The Poet            = [1][2][0-5]
Xocoveze            = [1][3][0-5]
Guiness Extra       = [1][4][0-5]
Double Cream        = [1][5][0-5]
Burbon County       = [1][6][0-5]

Coffee Baltic       = [2][0][0-5]
Gravity Drop        = [2][1][0-5]
Sextus              = [2][2][0-5]
Beer Bash           = [2][3][0-5]
Baltic Style        = [2][4][0-5]
The Tarten          = [2][5][0-5]

Budweiser           = [3][0][0-5]
Miller Lite         = [3][1][0-5]
Heineken            = [3][2][0-5]
Stella Artois       = [3][3][0-5]
Corona              = [3][4][0-5]

Urquell             = [4][0][0-5]
Sts Pils            = [4][1][0-5]
Heater Allen        = [4][2][0-5]
Live Oak            = [4][3][0-5]
Jever               = [4][4][0-5]
Keller Pils         = [4][5][0-5]

Allagash            = [5][0][0-5]
Summer Ale          = [5][1][0-5]
Blood Orange        = [5][2][0-5]
Belgin White        = [5][3][0-5]
Lambic              = [5][4][0-5]
Stoopid Wit         = [5][5][0-5]

Fifteen Hundred     = [6][0][0-5]
Doggie Style        = [6][1][0-5]
Daisey Cutter       = [6][2][0-5]
Whirlpool           = [6][3][0-5]
Pale Ale            = [6][4][0-5]
Sweet Action        = [6][5][0-5]

Fat Tire            = [7][0][0-5]
Nugget Nector       = [7][1][0-5]
Hop Head Red        = [7][2][0-5]
Amber Ale           = [7][3][0-5]
Flipside            = [7][4][0-5]

Brown Ale           = [8][0][0-5]
Nut Brown           = [8][1][0-5]
Best Brown          = [8][2][0-5]
Brekles Brown       = [8][3][0-5]
Palo Santo          = [8][4][0-5]

Red One             = [9][0][0-5]
Blonde Ale          = [9][1][0-5]
Red Two             = [9][2][0-5]
Krick Lambic        = [9][3][0-5]
Duvel               = [9][4][0-5]

Abbey Ale           = [10][0][0-5]
Soest Road          = [10][1][0-5]
Pere Jacques        = [10][2][0-5]
Benedicition        = [10][3][0-5]
Wild Duck           = [10][4][0-5]
Dub Sack            = [10][5][0-5]

Saison Du Buff      = [11][0][0-5]
Prop Culture        = [11][1][0-5]
Saison 83           = [11][2][0-5]
Saison Du Swamp     = [11][3][0-5]
Eukaryote           = [11][4][0-5]
Stone Saison        = [11][5][0-5]

Miami Madness       = [12][0][0-5]
Supplication        = [12][1][0-5]
Le Terroin          = [12][2][0-5]
Duck Duck Goose     = [12][3][0-5]
Amorphia            = [12][4][0-5]
Rubaeus             = [12][5][0-5]

*/


#endif