//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_MAIN_H
#define SDLGAME_MAIN_H

#include <stdbool.h>

//Screen dimension constants
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

static const int LEVEL_WIDTH = 8192;
static const int LEVEL_HEIGHT = 4096;

//Tile constants
static const int TILE_WIDTH = 80;
static const int TILE_HEIGHT = 80;
static const int TOTAL_TILES = 192;
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
