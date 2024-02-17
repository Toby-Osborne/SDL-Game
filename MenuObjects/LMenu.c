//
// Created by tazukiswift on 17/02/24.
//

#include "LMenu.h"
#include "SDL_FontCache.h"
#include "LButton.h"
#include "../LevelObjects/Player.h"

FC_Font* gFCFont;

SDL_Renderer* menuRenderer;

enum MenuMenus
{
    MENU_MAIN_MENU,
    MENU_PAUSE_MENU,
};

struct MainMenu
{
    struct LButton startButton;
    struct LButton quitButton;
} MainMenu;

struct PauseMenu
{
    struct LButton unpauseButton;
    struct LButton quitToTitleButton;
} PauseMenu;

enum MenuMenus menuCurrentMenu = MENU_MAIN_MENU;

void LMenuSetMainMenu() {menuCurrentMenu = MENU_MAIN_MENU;}

void LMenuSetPauseMenu() {menuCurrentMenu = MENU_PAUSE_MENU;}

enum GameStates LMenuHandleInput(SDL_Event *e)
{
    switch (menuCurrentMenu)
    {
        case MENU_MAIN_MENU:
            if (LButtonProcessButton(&MainMenu.startButton, e)) {
                PlayerUnpause();
                return GS_LEVEL;
            }
            if (LButtonProcessButton(&MainMenu.quitButton, e)) return GS_Quit;
            break;
        case MENU_PAUSE_MENU:
            if (LButtonProcessButton(&PauseMenu.unpauseButton, e)) {
                PlayerUnpause();
                return GS_LEVEL;
            }
            if (LButtonProcessButton(&PauseMenu.quitToTitleButton, e))
            {
                LMenuSetMainMenu();
                return GS_MENU;
            }
    }
}

void LMenuDrawMenu()
{
    switch (menuCurrentMenu)
    {
        case MENU_MAIN_MENU:
            LButtonRenderButtons((struct LButton*)(&MainMenu.startButton),2);
            break;
        case MENU_PAUSE_MENU:
            LButtonRenderButtons((struct LButton*)(&PauseMenu.unpauseButton),2);
    }
}

// CONFIGURE YOUR MENUS HERE
void LMenuInitMenu(SDL_Renderer* renderer)
{
    menuRenderer = renderer;

    // Load the Menu Font
    gFCFont = FC_CreateFont();
    FC_LoadFont(gFCFont, menuRenderer, "GameResources/Fonts/lazy.ttf", 25, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

    // Init Main Menu Objects
    SDL_Rect startButtonLocation = {(SCREEN_WIDTH-400)/2,(SCREEN_HEIGHT-80)/2-50,400,80};
    LButtonInitButton(&MainMenu.startButton, menuRenderer, startButtonLocation, "Begin the Game", gFCFont);

    SDL_Rect quitButtonLocation = {(SCREEN_WIDTH-400)/2,(SCREEN_HEIGHT-80)/2+50,400,80};
    LButtonInitButton(&MainMenu.quitButton, menuRenderer, quitButtonLocation, "Quit the Game", gFCFont);

    // Init Pause Menu Objects
    LButtonInitButton(&PauseMenu.unpauseButton, menuRenderer, startButtonLocation, "Resume Game", gFCFont);

    LButtonInitButton(&PauseMenu.quitToTitleButton, menuRenderer, quitButtonLocation, "Quit to Title", gFCFont);
}

void LMenuFreeMenus()
{
    //Free global gFCFont
    FC_FreeFont(gFCFont);
}