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
    SDL_Renderer* mRenderer;
    int mWidth;
    int mHeight;
};

void FreeTextures();

void LTextureInit(struct LTexture *texture);

void LTextureFree(struct LTexture *texture);

void LTextureSetBlendMode(struct LTexture *texture, SDL_BlendMode blending);

void LTextureSetAlpha(struct LTexture *texture, uint8_t alpha);

bool LTextureLoadFromFile(struct LTexture *texture, SDL_Renderer* gRenderer, char* path);

bool LTextureLoadFromRenderedText(struct LTexture *texture, SDL_Renderer* gRenderer, char *text, TTF_Font *font, SDL_Color textColor);

void LTextureRender(struct LTexture *texture, int x, int y, SDL_Rect* clip);

#endif //SDLGAME_LTEXTURE_H
