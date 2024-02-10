//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_LBUTTON_H
#define SDLGAME_LBUTTON_H

#include <SDL.h>

#include "LTexture.h"

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

struct LButton{
    SDL_Rect mPosition;
    SDL_Renderer *renderer;
    struct LTexture* buttonTexture;
    enum LButtonSprite mCurrentSprite;
};

static const int BUTTON_DEFAULT_WIDTH = 300;
static const int BUTTON_DEFAULT_HEIGHT = 200;

void initLButtons();

void initLButton(struct LButton *button, SDL_Renderer *renderer, struct LTexture *buttonTexture);

void LButtonSetPosition(struct LButton* button, int x, int y);

void LButtonSetSize(struct LButton* button, int w, int h);

void LButtonHandleEvent(struct LButton* button, SDL_Event* e);

void LButtonRender(struct LButton* button);

#endif //SDLGAME_LBUTTON_H
