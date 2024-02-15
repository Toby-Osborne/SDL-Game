//
// Created by tazukiswift on 15/02/24.
//

#include "LButton.h"

SDL_Renderer* buttonRenderer = NULL;


void LButtonInitButton(struct LButton* button, SDL_Renderer *renderer, SDL_Rect button_location, char* button_text, TTF_Font* font, void (*cb_func)())
{
    if (buttonRenderer == NULL) buttonRenderer = renderer;
    button->buttonLocation = button_location;
    button->buttonState = BUTTON_UNHOVERED;
    button->CBFunc = cb_func;
    button->buttonText = button_text;
    LTextureInit(&button->text);
    SDL_Color text_color = {0,0,0,0xFF};
    LTextureLoadFromRenderedText(&button->text, buttonRenderer, button_text, font, text_color);
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
                    if (button->CBFunc != NULL) (*button->CBFunc)();   // Callback function to do something
                    break;
            }
        } else {
            // Mouse moved and not on button, hi-light
            button->buttonState = BUTTON_UNHOVERED;
        }
    }
}

void LButtonProcessButtons(SDL_Event *e, struct LButton* buttons, int numButtons)
{
    for (int i = 0;i<numButtons;i++) LButtonProcessButton(&(buttons[i]),e);
}

void LButtonRenderButton(struct LButton* button)
{
    switch (button->buttonState)
    {
        case BUTTON_HOVERED:
            SDL_SetRenderDrawColor(buttonRenderer, 255, 255, 255, 0xFF);
            break;
        case BUTTON_UNHOVERED:
            SDL_SetRenderDrawColor(buttonRenderer, 200, 200, 200, 0xFF);
            break;
    }

    SDL_RenderFillRect(buttonRenderer, &button->buttonLocation);
    LTextureRender(&button->text,button->buttonLocation.x+(button->buttonLocation.w-button->text.mWidth)/2,button->buttonLocation.y+(button->buttonLocation.h-button->text.mHeight)/2,button->text.mWidth,button->text.mHeight,NULL);
}

void LButtonRenderButtons(struct LButton* buttons, int numButtons)
{
    for (int i = 0;i<numButtons;i++) LButtonRenderButton(&(buttons[i]));
}