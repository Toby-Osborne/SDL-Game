//
// Created by tazukiswift on 19/02/24.
//

#include "GameWindow.h"
#include "LevelObjects/LCamera.h"
#include "MenuObjects/LMenu.h"

extern SDL_Renderer *gRenderer;

struct WindowStruct
{
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mMinimized;
    bool mFullScreen;
    int mWidth;
    int mHeight;
    SDL_Window *mWindow;
} WindowStruct = {true,true,false,false,1920,1080,NULL};



bool GameWindowInit()
{
    WindowStruct.mWindow = SDL_CreateWindow( "Neon Gray", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowStruct.mWidth, WindowStruct.mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if (WindowStruct.mWindow == NULL) return false;
    LMenuInitMenu();
    LCameraInitCamera();
    return true;
}

SDL_Window* GameWindowGetWindow()
{
    return WindowStruct.mWindow;
}

int GameWindowGetWindowWidth()
{
    return WindowStruct.mWidth;
}

int GameWindowGetWindowHeight()
{
    return WindowStruct.mHeight;
}

void GameWindowHandleEvent(SDL_Event *e)
{
    if(e->type == SDL_WINDOWEVENT)
    {
        switch (e->window.event)
        {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                WindowStruct.mWidth = e->window.data1;
                WindowStruct.mHeight = e->window.data2;
                LMenuInitMenu();
                LCameraInitCamera();
                SDL_RenderPresent(gRenderer);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(gRenderer);
                break;
            case SDL_WINDOWEVENT_ENTER:
                WindowStruct.mMouseFocus = true;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                WindowStruct.mMouseFocus = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                WindowStruct.mKeyboardFocus = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                WindowStruct.mKeyboardFocus = false;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                WindowStruct.mMinimized = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                WindowStruct.mMinimized = false;
                break;
        }
    }
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_f)
    {
        if (WindowStruct.mFullScreen)
        {
            SDL_SetWindowFullscreen(WindowStruct.mWindow,0);
            WindowStruct.mFullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen(WindowStruct.mWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
            WindowStruct.mFullScreen = true;
            WindowStruct.mMinimized = false;
        }
        LMenuInitMenu();
        LCameraInitCamera();
    }
}

void GameWindowFree()
{
    SDL_DestroyWindow(WindowStruct.mWindow);
}