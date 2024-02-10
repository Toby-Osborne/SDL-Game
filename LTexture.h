//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_LTEXTURE_H
#define SDLGAME_LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

struct LTexture
{
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

void initLTexture(struct LTexture *texture);

void freeLTexture(struct LTexture *texture);

bool loadLTexture(struct LTexture *texture, SDL_Renderer* gRenderer, char* path);

void renderLTexture(struct LTexture *texture, SDL_Renderer* gRenderer, int x, int y);

#endif //SDLGAME_LTEXTURE_H
