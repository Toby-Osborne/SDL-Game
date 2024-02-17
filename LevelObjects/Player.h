//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_PLAYER_H
#define SDLGAME_PLAYER_H

#include <SDL.h>

#include "../LTexture.h"
#include "../main.h"

//The dimensions of the dot
#define PLAYER_HEIGHT 128
#define PLAYER_WIDTH 64

#define PLAYER_TEXTURE_HEIGHT 64
#define PLAYER_TEXTURE_WIDTH 64

//Maximum axis velocity of the dot
static const float PLAYER_VELOCITY = 1.5f;

int PlayerGetX();

int PlayerGetY();

void PlayerPause();

void PlayerUnpause();

void PlayerRespawn();

void PlayerSetGameMode(enum GameStates state);

enum GameStates PlayerGetGameMode();

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture);

enum GameStates PlayerHandleInput(SDL_Event *e);

void PlayerProcessMovement();

void EditorProcessMovement();

void PlayerRender(int camX, int camY);

void PlayerRenderDab(int camX, int camY);


#endif //SDLGAME_PLAYER_H
