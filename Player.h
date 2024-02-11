//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_PLAYER_H
#define SDLGAME_PLAYER_H

#include <SDL.h>

#include "LTexture.h"

//The dimensions of the dot
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
static const int DOT_VEL = 10;

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture);

void PlayerHandleEvent(SDL_Event *e);

void PlayerProcessMovement(SDL_Rect *wall);

void PlayerRender(int camX, int camY);

int PlayerGetX();

int PlayerGetY();

#endif //SDLGAME_PLAYER_H
