//
// Created by tazukiswift on 10/02/24.
//

#include "LTexture.h"

#define MAX_TEXTURES 5

void LTextureInit(struct LTexture *texture)
{
    texture->mTexture = NULL;
    texture->mWidth = 0;
    texture->mHeight = 0;
}


struct LTexture * textures[MAX_TEXTURES] = {NULL};

bool LTextureLoadFromFile(struct LTexture *texture, SDL_Renderer* gRenderer, char* path)
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
            texture->mRenderer = gRenderer;
            texture->mWidth = loadedSurface->w;
            texture->mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture->mTexture = newTexture;
    return texture->mTexture != NULL;
}

bool LTextureLoadFromRenderedText(struct LTexture *texture, SDL_Renderer* gRenderer, char *text, TTF_Font *font, SDL_Color textColor)
{
//Get rid of preexisting texture
    LTextureFree(texture);

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        texture->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( texture->mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            texture->mRenderer = gRenderer;
            texture->mWidth = textSurface->w;
            texture->mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
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

void LTextureRender(struct LTexture *texture, int x, int y, SDL_Rect* clip)
{
    // Here we create an arbitrary quad and map the texture onto it
    SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
    if(clip!=NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(texture->mRenderer, texture->mTexture, clip, &renderQuad);
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