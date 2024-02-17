//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_MAIN_H
#define SDLGAME_MAIN_H

#include <stdbool.h>

//Screen dimension constants
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

enum GameStates
{
    GS_MENU,
    GS_LEVEL,
    GS_LEVEL_EDIT,
    GS_PAUSED,
    GS_Quit
};
#endif //SDLGAME_MAIN_H
