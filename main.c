//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "Player.h"


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Renderer for textures
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

struct LTexture gTextureCharacter;
struct LTexture gTextureBackground;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //Create window
    gWindow = SDL_CreateWindow( "C Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (gRenderer == NULL)
        {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    gFont = TTF_OpenFont("resources/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    LTextureInit(&gTextureCharacter);
    LTextureLoadFromFile(&gTextureCharacter, gRenderer, "resources/character.png");

    LTextureInit(&gTextureBackground);
    LTextureLoadFromFile(&gTextureBackground, gRenderer, "resources/background.png");

    LTextureSetBlendMode(&gTextureCharacter, SDL_BLENDMODE_BLEND);

    return success;
}

void closeGame() {

    FreeTextures();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    if (!init()) {
        printf("Failed to init \n");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Cant load shit\n");
        }
        else
        {
            bool quit = false;
            SDL_Event e;

            struct LTimer fpsTimer;
            struct LTimer capTimer;

            LTimerInit(&fpsTimer);
            LTimerInit(&capTimer);
            int countedFrames = 0;

            LTimerAction(&fpsTimer,START);

            PlayerInit(gRenderer, &gTextureCharacter);

            while (!quit) {
                LTimerAction(&capTimer,START);
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    PlayerHandleEvent(&e);
                }

                float avgFPS = (float)countedFrames / ((float)LTimerGetTicks(&fpsTimer)/1000.f);
                if(avgFPS > 144)
                {
                    avgFPS = 0.f;
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                PlayerProcessMovement();

                LTextureRender(&gTextureBackground,0,0,NULL);
//                LTextureRender(&gTextureCharacter, 100,100,NULL);
                PlayerRender();
                SDL_RenderPresent(gRenderer);
                countedFrames++;
            }
        }
    }
    closeGame();

    return 0;
}