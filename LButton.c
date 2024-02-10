//
// Created by tazukiswift on 10/02/24.
//

#include "LButton.h"

#include <stdbool.h>


SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

bool buttons_are_init = false;

void initLButtons()
{
    for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
    {
        gSpriteClips[ i ].x = 0;
        gSpriteClips[ i ].y = i * 200;
        gSpriteClips[ i ].w = BUTTON_DEFAULT_WIDTH;
        gSpriteClips[ i ].h = BUTTON_DEFAULT_HEIGHT;
    }
    buttons_are_init = true;
}


void initLButton(struct LButton *button, SDL_Renderer *renderer, struct LTexture *buttonTexture)
{
    if (!buttons_are_init) initLButtons();

    button->buttonTexture = buttonTexture;
    button->renderer = renderer;
    button->mPosition.x = 0;
    button->mPosition.y = 0;
    button->mPosition.w = BUTTON_DEFAULT_WIDTH;
    button->mPosition.h = BUTTON_DEFAULT_HEIGHT;
    button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButtonSetPosition(struct LButton* button, int x, int y)
{
    button->mPosition.x = x;
    button->mPosition.y = y;
}

void LButtonSetSize(struct LButton* button, int w, int h)
{
    button->mPosition.w = w;
    button->mPosition.h = h;
}

void LButtonHandleEvent(struct LButton* button, SDL_Event* e)
{
    if (e-> type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;

        if (x < button->mPosition.x)
        {
            inside = false;
        }
        else if (x > button->mPosition.x + button->mPosition.w)
        {
            inside = false;
        }
        else if (y < button->mPosition.y)
        {
            inside = false;
        }
        else if (y > button->mPosition.y + button->mPosition.h)
        {
            inside = false;
        }
        if (!inside)
        {
            button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        else
        {
            switch(e->type)
            {
                case SDL_MOUSEMOTION:
                    button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    button->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    button->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
            }
        }
    }
}

void LButtonRender(struct LButton* button)
{
    renderLTexture(button->buttonTexture,button->mPosition.x, button->mPosition.y, &gSpriteClips[button->mCurrentSprite]);
}