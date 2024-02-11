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
    LTextureLoadFromFile(&gTextureBackground, gRenderer, "resources/bg.png");

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

            // Wall
            SDL_Rect wall;
            wall.x = 300;
            wall.y = 40;
            wall.w = 40;
            wall.h = 400;

            SDL_Rect camera = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

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

                camera.x = ( PlayerGetX() + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
                camera.y = ( PlayerGetY() + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

                //Center the camera over the dot
                camera.x = ( PlayerGetX() + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
                camera.y = ( PlayerGetY() + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

                //Keep the camera in bounds
                if( camera.x < 0 )
                {
                    camera.x = 0;
                }
                if( camera.y < 0 )
                {
                    camera.y = 0;
                }
                if( camera.x > LEVEL_WIDTH - camera.w )
                {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if( camera.y > LEVEL_HEIGHT - camera.h )
                {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }

                SDL_Rect wall_render = {wall.x - camera.x, wall.y - camera.y, wall.w, wall.h};

                PlayerProcessMovement(&wall);

                LTextureRender(&gTextureBackground,0,0,&camera);
                SDL_SetRenderDrawColor(gRenderer, 0x0,0x0,0x0,0xFF);
                SDL_RenderDrawRect(gRenderer, &wall_render);

                PlayerRender(camera.x, camera.y);
                SDL_RenderPresent(gRenderer);
                countedFrames++;
            }
        }
    }
    closeGame();

    return 0;
}