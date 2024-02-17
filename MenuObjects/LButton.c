//
// Created by tazukiswift on 15/02/24.
//

#include "LButton.h"

extern SDL_Renderer* gRenderer;


void LButtonInitButton(struct LButton *button, SDL_Rect button_location, char *button_text, FC_Font *font)
{
    button->buttonLocation = button_location;
    button->buttonState = BUTTON_UNHOVERED;
    button->buttonText = button_text;
    button->buttonFont = font;
}

int x, y;

bool LButtonProcessButton(struct LButton* button, SDL_Event *e)
{
    if ((e->type == SDL_MOUSEMOTION)||(e->type == SDL_MOUSEBUTTONDOWN)) {
        SDL_GetMouseState(&x, &y);
        if ((button->buttonLocation.x < x) && (x < button->buttonLocation.x + button->buttonLocation.w)
            && (button->buttonLocation.y < y) && (y < button->buttonLocation.y + button->buttonLocation.h)) {
            // Mouse is over button
            switch (e->type)
            {
                case SDL_MOUSEMOTION:
                    button->buttonState = BUTTON_HOVERED;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    return true;
                    break;
            }
        } else {
            // Mouse moved and not on button, hi-light
            button->buttonState = BUTTON_UNHOVERED;
        }
    }
    return false;
}

void LButtonRenderButton(struct LButton* button)
{
    switch (button->buttonState)
    {
        case BUTTON_HOVERED:
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
            break;
        case BUTTON_UNHOVERED:
            SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 0xFF);
            break;
    }

    SDL_RenderFillRect(gRenderer, &button->buttonLocation);
    FC_DrawAlign(button->buttonFont, gRenderer, button->buttonLocation.x+(button->buttonLocation.w)/2,button->buttonLocation.y+(button->buttonLocation.h)/2, FC_ALIGN_CENTER,button->buttonText, NULL);
}

void LButtonRenderButtons(struct LButton* buttons, int numButtons)
{
    for (int i = 0;i<numButtons;i++) LButtonRenderButton(&(buttons[i]));
}