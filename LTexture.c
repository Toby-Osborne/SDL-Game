//
// Created by tazukiswift on 10/02/24.
//

#include "LTexture.h"

void initLTexture(struct LTexture *texture)
{
    texture->mTexture = NULL;
    texture->mWidth = 0;
    texture->mHeight = 0;
}

bool loadLTexture(struct LTexture *texture, SDL_Renderer* gRenderer, char* path)
{
    freeLTexture(texture);

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);

    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture==NULL){
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        else
        {
            texture->mRenderer = gRenderer;
            texture->mWidth = loadedSurface->w;
            texture->mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture->mTexture = newTexture;
    return texture->mTexture != NULL;
}

void freeLTexture(struct LTexture *texture)
{
    if(texture->mTexture != NULL)
    {
        SDL_DestroyTexture(texture->mTexture);
        texture->mTexture = NULL;
        texture->mWidth = 0;
        texture->mHeight = 0;
    }
}

void renderLTexture(struct LTexture *texture, int x, int y)
{
    // Here we create an arbitrary quad and map the texture onto it
    SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
    SDL_RenderCopy(texture->mRenderer, texture->mTexture, NULL, &renderQuad);
}

