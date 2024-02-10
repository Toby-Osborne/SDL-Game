//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "LTexture.h"
#include "LButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Renderer for textures
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

struct LTexture gTextTexture;

struct LTexture gButtonTextureMap;

#define TOTAL_BUTTONS 4
struct LButton buttons[TOTAL_BUTTONS];

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

    gFont = TTF_OpenFont("../resources/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        SDL_Color textColor = { 0, 0, 0 };
        if(!loadLTextureFromRenderedText(&gTextTexture, gRenderer, "The quick brown fox jumps over the lazy dog", gFont, textColor ))
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }

    initLTexture(&gButtonTextureMap);
    loadLTextureFromFile(&gButtonTextureMap, gRenderer, "../resources/button.png");

    setLTextureBlendMode(&gTextTexture, SDL_BLENDMODE_BLEND);

    for (int i = 0; i<TOTAL_BUTTONS;i++){
        initLButton(&buttons[i], gRenderer, &gButtonTextureMap);
    }


    //Set buttons in corners
    LButtonSetPosition(&buttons[0],0,0);
    LButtonSetPosition(&buttons[1],SCREEN_WIDTH - BUTTON_DEFAULT_WIDTH, 0);
    LButtonSetPosition(&buttons[2],0, SCREEN_HEIGHT - BUTTON_DEFAULT_HEIGHT);
    LButtonSetPosition(&buttons[3],SCREEN_WIDTH - BUTTON_DEFAULT_WIDTH, SCREEN_HEIGHT - BUTTON_DEFAULT_HEIGHT);

    return success;
}

void closeGame() {

    freeLTexture(&gButtonTextureMap);
    freeLTexture(&gTextTexture);

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
            int counter = 0;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    //Handle button events
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        LButtonHandleEvent(&buttons[i],&e);
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render top left sprite
//                renderLTexture(&gTextTexture, (SCREEN_WIDTH - gTextTexture.mWidth)/2, (SCREEN_HEIGHT - gTextTexture.mHeight)/2, NULL);
                for (int i = 0; i < TOTAL_BUTTONS;i++)
                {
                    LButtonRender(&buttons[i]);
                }
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    closeGame();

    return 0;
}