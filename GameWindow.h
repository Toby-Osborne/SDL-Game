//
// Created by tazukiswift on 19/02/24.
//

#ifndef SDLGAME_GAMEWINDOW_H
#define SDLGAME_GAMEWINDOW_H
#include <stdbool.h>
#include <SDL.h>

bool GameWindowInit();
int GameWindowGetWindowWidth();
int GameWindowGetWindowHeight();
SDL_Window* GameWindowGetWindow();
void GameWindowHandleEvent(SDL_Event *e);
void GameWindowFree();



#endif //SDLGAME_GAMEWINDOW_H
