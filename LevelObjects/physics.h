//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_PHYSICS_H
#define SDLGAME_PHYSICS_H

#include <stdbool.h>
#include <SDL.h>

static const float g = 0.0002f;

typedef struct {
    float x, y, w, h;
} SDLP_FloatRect ;

#endif //SDLGAME_PHYSICS_H
