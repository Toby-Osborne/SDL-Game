//
// Created by tazukiswift on 10/02/24.
//

#include "LTexture.h"

#define MAX_TEXTURES 100

extern SDL_Renderer* gRenderer;

void LTextureInit(struct LTexture *texture)
{
    texture->mTexture = NULL;
    texture->mWidth = 0;
    texture->mHeight = 0;
}


struct LTexture * textures[MAX_TEXTURES] = {NULL};

bool LTextureLoadFromFile(struct LTexture *texture, char *path)
{
    // Register texture object
    for (int i = 0;i<MAX_TEXTURES+1;i++){
        if (i == MAX_TEXTURES)
        {
            return false;
        }

        if (texture == textures[i])
        {
            break;
        }
        if (textures[i] == 0)
        {
            textures[i] = texture;
            break;
        }
    }

    LTextureFree(texture);

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);

    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture==NULL){
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        else
        {
            texture->mWidth = loadedSurface->w;
            texture->mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture->mTexture = newTexture;
    return texture->mTexture != NULL;
}

void LTextureFree(struct LTexture *texture)
{
    if(texture->mTexture != NULL)
    {
        SDL_DestroyTexture(texture->mTexture);
        texture->mTexture = NULL;
        texture->mWidth = 0;
        texture->mHeight = 0;
    }
}

void LTextureRender(struct LTexture *texture, int x, int y, int w, int h,SDL_Rect* clip)
{
    // Here we create an arbitrary quad and map the texture onto it
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopy(gRenderer, texture->mTexture, clip, &renderQuad);
}

void LTextureRenderEx(struct LTexture *texture, int x, int y, int w, int h,SDL_Rect* clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopyEx(gRenderer, texture->mTexture, clip, &renderQuad, angle, center, flip);
}

void LTextureSetBlendMode(struct LTexture *texture, SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(texture->mTexture, blending);
}

void LTextureSetAlpha(struct LTexture *texture, uint8_t alpha)
{
    SDL_SetTextureAlphaMod( texture->mTexture, alpha);
}

void FreeTextures()
{
     for (int i = 0;i<MAX_TEXTURES;i++)
    {
        if (textures[i] != NULL) LTextureFree(textures[i]);
    }
}