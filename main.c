//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <SDL_FontCache.h>

#include "main.h"
#include "LTexture.h"
#include "LTimer.h"
#include "LevelObjects/Player.h"
#include "LevelObjects/LCamera.h"
#include "LevelObjects/TileMap.h"
#include "MenuObjects/LButton.h"
#include "MenuObjects/LMenu.h"


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Renderer for textures
SDL_Renderer* gRenderer = NULL;

// Renderer for tiles onto a texture

enum GameStates gState = GS_MENU;

struct LTexture gTextureCharacter;
struct LTexture gTextureBackground;
struct LTexture gTextureTile;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //Create window
    gWindow = SDL_CreateWindow( "Neon Grey", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

    // Init all stuff related to level and player
    LTextureInit(&gTextureCharacter);
    LTextureLoadFromFile(&gTextureCharacter, gRenderer, "GameResources/FatRunTexture.png");

    LTextureInit(&gTextureBackground);
    LTextureLoadFromFile(&gTextureBackground, gRenderer, "GameResources/bg-tiled.png");

    LTextureInit(&gTextureTile);
    LTextureLoadFromFile(&gTextureTile, gRenderer, "GameResources/dirt-tile-1.png");

    LTextureSetBlendMode(&gTextureCharacter, SDL_BLENDMODE_BLEND);

    TileMapInit(&gTextureTile);
    TileMapLoadTileMap("Levels/level-1.tm");

    PlayerInit(gRenderer, &gTextureCharacter);

    // Init the menus
    LMenuInitMenu(gRenderer);

    return success;
}

void closeGame() {

    FreeTextures();

    //Save Tilemap
    TileMapSaveTileMap("Levels/level-1.tm");

    LMenuFreeMenus();

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

            SDL_Rect *camera = LCameraGetCamera();
            while (!quit) {
                // Handle Inputs
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    switch (gState)
                    {
                        case GS_MENU:
                            gState = LMenuHandleInput(&e);
                            break;
                        case GS_LEVEL:
                            gState = PlayerHandleEvent(&e);
                            break;
                        case GS_PAUSED:
                            gState = LMenuHandleInput(&e);
                            break;
                        case GS_Quit:
                            quit = true;
                            break;
                    }
                }

                // Render Scene
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(gRenderer);

                // Draw whatever
                switch (gState)
                {
                    case GS_MENU:
                        LMenuDrawMenu();
                        break;
                    case GS_LEVEL:
                        PlayerProcessMovement();

                        SDL_Rect background_render = {camera->x%512, camera->y%512, camera->w, camera->h};
                        LTextureRender(&gTextureBackground,0,0,background_render.w, background_render.h, &background_render);

                        TileMapRenderTiles(camera);

                        PlayerRender(camera->x, camera->y);
                        break;
                    case GS_PAUSED:
                        LMenuDrawMenu();
                        break;
                    case GS_Quit:
                        printf("Closing...");
                        break;
                }

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    closeGame();

    return 0;
}