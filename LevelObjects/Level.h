//
// Created by tazukiswift on 17/02/24.
//

#ifndef SDLGAME_LEVEL_H
#define SDLGAME_LEVEL_H

#include "../LTexture.h"

void LevelInitLevels();

void LevelLoadLevel(char * path);

void LevelSaveLevel();

void LevelDrawLevel();

void LevelFree();

#endif //SDLGAME_LEVEL_H
