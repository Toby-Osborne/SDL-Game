//
// Created by tazukiswift on 10/02/24.
//

#ifndef SDLGAME_LTEXTURE_H
#define SDLGAME_LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdbool.h>

struct LTexture
{
    SDL_Texture* mTexture;
    bool mScaled;
    int mWidth;
    int mHeight;
};

void FreeTextures();

void LTextureInit(struct LTexture *texture);

void LTextureFree(struct LTexture *texture);

void LTextureSetBlendMode(struct LTexture *texture, SDL_BlendMode blending);

void LTextureSetAlpha(struct LTexture *texture, uint8_t alpha);

bool LTextureLoadFromFile(struct LTexture *texture, char *path);

void LTextureRender(struct LTexture *texture, int x, int y, int w, int h,SDL_Rect* clip);

void LTextureRenderEx(struct LTexture *texture, int x, int y, int w, int h,SDL_Rect* clip, double angle, SDL_Point *center, SDL_RendererFlip flip);

#endif //SDLGAME_LTEXTURE_H
