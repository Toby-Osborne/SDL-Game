//
// Created by tazukiswift on 17/02/24.
//

#include "Level.h"
#include "TileMap.h"
#include "string.h"

struct LTexture gTextureBackground;

// Nobody tell them
extern SDL_Renderer* gRenderer;
extern SDL_Rect camera;

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
}

void LevelSaveLevel()
{
    TileMapSaveTileMap(currentLevelPath);
}

void LevelDrawLevel()
{
    // The textures repeat every 512 pixels, so just snap back there
    SDL_Rect background_render = {camera.x%512, camera.y%512, camera.w, camera.h};
    LTextureRender(&gTextureBackground,0,0,background_render.w, background_render.h, &background_render);
    TileMapRenderTiles();
}

void LevelFree()
{

}