//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_PHYSICS_H
#define SDLGAME_PHYSICS_H

#include <stdbool.h>
#include <SDL.h>

bool checkCollision (SDL_Rect a, SDL_Rect b);

static const float g = 0.01;

#endif //SDLGAME_PHYSICS_H
