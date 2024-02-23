//
// Created by tazukiswift on 17/02/24.
//

#include "Level.h"
#include "TileMap.h"
#include "string.h"
#include "../GameWindow.h"
#include "physics.h"

struct LTexture gTextureBackground;

// Nobody tell them
extern SDL_Renderer* gRenderer;
extern SDLP_FloatRect camera;

char currentLevelPath[200];

void LevelInitLevels()
{
    TileMapInit();

    LTextureInit(&gTextureBackground);
    LTextureLoadFromFile(&gTextureBackground, "GameResources/bg-tiled.png");
}

void LevelLoadLevel(char * path)
{
    strcpy(currentLevelPath,path);
    TileMapLoadTileMap(path);
    TileMapUpdateConstantsFromDisplay();
}

void LevelSaveLevel()
{
    TileMapSaveTileMap(currentLevelPath);
}

void LevelDrawLevel()
{
    // The textures repeat every 512 pixels, so just snap back there
    SDL_Rect background_render = {((int)(camera.x*(float)TileMapGetTileWidth())/2)%512, ((int)(camera.y*(float)TileMapGetTileWidth())/2)%512, GameWindowGetWindowWidth()/2, GameWindowGetWindowHeight()/2};
    LTextureRender(&gTextureBackground,0,0,GameWindowGetWindowWidth(), GameWindowGetWindowHeight(), &background_render);
    TileMapRenderTiles();
}

void LevelFree()
{
    TileMapFree();
}