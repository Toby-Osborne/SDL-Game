//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <SDL_FontCache.h>

#include "main.h"
#include "LTexture.h"
#include "LevelObjects/Player.h"
#include "LevelObjects/LCamera.h"
#include "MenuObjects/LMenu.h"
#include "LevelObjects/Level.h"
#include "GameWindow.h"


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Renderer for textures
SDL_Renderer* gRenderer = NULL;
extern SDL_Rect camera;
// Renderer for tiles onto a texture

enum GameStates gState = GS_MENU;

struct LTexture gTextureCharacter;



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

    if(!GameWindowInit())
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        gRenderer = SDL_CreateRenderer( GameWindowGetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    LTextureLoadFromFile(&gTextureCharacter, "GameResources/FatRunTexture.png");
    LTextureSetBlendMode(&gTextureCharacter, SDL_BLENDMODE_BLEND);

    LevelInitLevels();

    PlayerInit(&gTextureCharacter);

    // Init the menus
    LMenuInitMenu();

    return success;
}

void closeGame() {

    FreeTextures();

    LMenuFreeMenus();
    LevelFree();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    GameWindowFree();

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

            while (!quit) {
                // Handle Inputs
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    GameWindowHandleEvent(&e);
                    switch (gState)
                    {
                        case GS_MENU:
                        case GS_PAUSED:
                            gState = LMenuHandleInput(&e);
                            break;
                        case GS_LEVEL:
                        case GS_LEVEL_EDIT:
                            gState = PlayerHandleInput(&e);
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
                        LevelDrawLevel();

                        PlayerProcessMovement();
                        PlayerRender(camera.x, camera.y);
                        break;
                    case GS_LEVEL_EDIT:
                        LevelDrawLevel();

                        EditorProcessMovement();
                        PlayerRender(camera.x, camera.y);
                        break;
                    case GS_PAUSED:
                        LevelDrawLevel();
                        PlayerRender(camera.x, camera.y);
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