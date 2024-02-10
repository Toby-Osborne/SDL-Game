//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "LTexture.h"

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

#define WALKING_ANIMATION_FRAMES 4
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];

struct LTexture gWalkingTexture;

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
    else
    {
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
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    // Load FOO Texture
    initLTexture(&gWalkingTexture);
    if (!loadLTextureFromFile(&gWalkingTexture, gRenderer, "../resources/walk.png"))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    };

    setLTextureBlendMode(&gWalkingTexture, SDL_BLENDMODE_BLEND);

    //Set sprite clips
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  64;
    gSpriteClips[ 0 ].h = 205;

    gSpriteClips[ 1 ].x =  64;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  64;
    gSpriteClips[ 1 ].h = 205;

    gSpriteClips[ 2 ].x = 128;
    gSpriteClips[ 2 ].y =   0;
    gSpriteClips[ 2 ].w =  64;
    gSpriteClips[ 2 ].h = 205;

    gSpriteClips[ 3 ].x = 192;
    gSpriteClips[ 3 ].y =   0;
    gSpriteClips[ 3 ].w =  64;
    gSpriteClips[ 3 ].h = 205;

    return success;
}

void closeGame() {

    freeLTexture(&gWalkingTexture);

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
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
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render top left sprite

                renderLTexture(&gWalkingTexture,100,100,&gSpriteClips[counter/4]);
                counter = (counter+1)%(4*WALKING_ANIMATION_FRAMES);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    closeGame();

    return 0;
}