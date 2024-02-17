//
// Created by tazukiswift on 15/02/24.
//

#ifndef SDLGAME_LBUTTON_H
#define SDLGAME_LBUTTON_H

#include "../LTexture.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>

enum LButtonState {
    BUTTON_HOVERED,
    BUTTON_UNHOVERED,
    BUTTON_CLICKED
};

struct LButton {
    SDL_Rect buttonLocation;
    char* buttonText;
    FC_Font* buttonFont;
    enum LButtonState buttonState;
};

void LButtonInitButton(struct LButton *button, SDL_Rect button_location, char *button_text, FC_Font *font);

bool LButtonProcessButton(struct LButton* button, SDL_Event *e);

void LButtonRenderButton(struct LButton* button);

void LButtonRenderButtons(struct LButton* buttons, int numButtons);

#endif //SDLGAME_LBUTTON_H
