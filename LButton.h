//
// Created by tazukiswift on 15/02/24.
//

#ifndef SDLGAME_LBUTTON_H
#define SDLGAME_LBUTTON_H

#include "LTexture.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

enum LButtonState {
    BUTTON_HOVERED,
    BUTTON_UNHOVERED,
    BUTTON_CLICKED
};

struct LButton {
    void (*CBFunc)();
    SDL_Rect buttonLocation;
    char* buttonText;
    enum LButtonState buttonState;
    struct LTexture text;
};

void LButtonInitButton(struct LButton* button, SDL_Renderer *renderer, SDL_Rect button_location, char* button_text, TTF_Font* font, void (*cb_func)());

bool LButtonProcessButton(struct LButton* button, SDL_Event *e);

void LButtonProcessButtons(SDL_Event *e, struct LButton* buttons, int numButtons);

void LButtonRenderButton(struct LButton* button);

void LButtonRenderButtons(struct LButton* buttons, int numButtons);

#endif //SDLGAME_LBUTTON_H
