//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_MAIN_H
#define SDLGAME_MAIN_H

#include <stdbool.h>

//Screen dimension constants
static const int SCREEN_WIDTH = 2560;
static const int SCREEN_HEIGHT = 1440;

static const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
enum tileSprites {
    TILE_RED = 0,
    TILE_GREEN = 1,
    TILE_BLUE = 2,
    TILE_CENTER = 3,
    TILE_TOP = 4,
    TILE_TOPRIGHT = 5,
    TILE_RIGHT = 6,
    TILE_BOTTOMRIGHT = 7,
    TILE_BOTTOM = 8,
    TILE_BOTTOMLEFT = 9,
    TILE_LEFT = 10,
    TILE_TOPLEFT = 11
};

#endif //SDLGAME_MAIN_H
