//
// Created by tazukiswift on 17/02/24.
//

#ifndef SDLGAME_LMENU_H
#define SDLGAME_LMENU_H

#include "../main.h"
#include <SDL.h>

void LMenuInitMenu(SDL_Renderer* renderer);

enum GameStates LMenuHandleInput(SDL_Event *e);

void LMenuDrawMenu();

void LMenuFreeMenus();

// You should enter a menu tree from one of these functions
void LMenuSetMainMenu();

void LMenuSetPauseMenu();

#endif //SDLGAME_LMENU_H
