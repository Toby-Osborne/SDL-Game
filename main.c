//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

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

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed image
SDL_Surface* gMeForReal = NULL;

//Renderer for textures
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

//Texture to render to window
SDL_Texture* loadTexture( char* path);

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
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {

            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
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
                else
                {
                    //Get window surface
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
                }
            }
        }
    }

    return success;
}

SDL_Texture* loadTexture( char* path )
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path );

    if (loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);
        if(newTexture==NULL)
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    gTexture = loadTexture("../resources/mefr.png");
    if( gTexture == NULL )
    {
        printf( "Failed to load texture from image!\n");
        success = false;
    }

    return success;
}

void closeGame() {

    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

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

            SDL_Event e;

            SDL_Rect topLeftViewport;
            topLeftViewport.x = 0;
            topLeftViewport.y = 0;
            topLeftViewport.h = SCREEN_HEIGHT/2;
            topLeftViewport.w = SCREEN_WIDTH/2;

            SDL_Rect topRightViewport;
            topRightViewport.x = SCREEN_WIDTH/2;
            topRightViewport.y = 0;
            topRightViewport.h = SCREEN_HEIGHT/2;
            topRightViewport.w = SCREEN_WIDTH/2;

            SDL_Rect bottomViewport;
            bottomViewport.x = 0;
            bottomViewport.y = SCREEN_HEIGHT/2;
            bottomViewport.w = SCREEN_WIDTH;
            bottomViewport.h = SCREEN_HEIGHT/2;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                SDL_RenderClear(gRenderer);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                SDL_RenderSetViewport(gRenderer, &topLeftViewport);
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);

                SDL_RenderSetViewport(gRenderer, &topRightViewport);
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);

                SDL_RenderSetViewport(gRenderer, &bottomViewport);
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);

                SDL_RenderPresent( gRenderer );

            }
        }
    }
    closeGame();

    return 0;
}