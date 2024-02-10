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
    SDL_Renderer* mRenderer;
    int mWidth;
    int mHeight;
};

void initLTexture(struct LTexture *texture);

void freeLTexture(struct LTexture *texture);

void setLTextureBlendMode(struct LTexture *texture, SDL_BlendMode blending);

void setLTextureAlpha(struct LTexture *texture, uint8_t alpha);

bool loadLTexture(struct LTexture *texture, SDL_Renderer* gRenderer, char* path);

void renderLTexture(struct LTexture *texture, int x, int y, SDL_Rect* clip);

#endif //SDLGAME_LTEXTURE_H
