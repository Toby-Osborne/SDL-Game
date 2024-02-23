//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_PLAYER_H
#define SDLGAME_PLAYER_H

#include <SDL.h>

#include "../LTexture.h"
#include "../main.h"

#define DEFAULT_PLAYER_WIDTH 0.8f
#define DEFAULT_PLAYER_HEIGHT 1.9f

#define PLAYER_TEXTURE_HEIGHT 64
#define PLAYER_TEXTURE_WIDTH 64

//Maximum axis velocity of the dot
static const float PLAYER_ACCELERATION = 0.003f;
static const float PLAYER_JUMP_ANTIGRAVITY = 0.001f;
static const float PLAYER_JUMP_VELOCITY = 0.05f;

static const float CROUCH_FACTOR = 0.005f;

static const float PLAYER_VELOCITY = .03f;

float PlayerGetX();

float PlayerGetY();

float PlayerGetW();

float PlayerGetH();

void PlayerPause();

void PlayerUnpause();

void PlayerRespawn(float x, float y);

void EditorSetCurrentTile(int tileVal);

void PlayerSetGameMode(enum GameStates state);

enum GameStates PlayerGetGameMode();

void PlayerInit(struct LTexture *texture);

enum GameStates PlayerHandleInput(SDL_Event *e);

void PlayerProcessMovement();

void EditorProcessMovement();

void PlayerRender(int camX, int camY);

#endif //SDLGAME_PLAYER_H
