//
// Created by tazukiswift on 12/02/24.
//

#include "TileMap.h"
#include "../GameWindow.h"
#include "physics.h"

SDL_Rect tileClip = {0, 0, TILE_TEXTURE_WIDTH, TILE_TEXTURE_HEIGHT};
extern SDLP_FloatRect camera;
uint32_t *mapTiles;

static struct TileConstants {
    int tileWidth;
    int tileHeight;
    float levelRenderHeightTiles;
} TileConstants;

static struct MapHeader {
    int LevelWidthTiles;
    int LevelHeightTiles;
    int LevelNumEntities;
} MapHeader;

struct LTexture gTextureTile;
void TileMapUpdateConstantsFromDisplay()
{
    // Change this number to change the render distance
    TileConstants.levelRenderHeightTiles = 22.5f;
    int WindowHeight = GameWindowGetWindowHeight();
    TileConstants.tileHeight = (int)(((float)WindowHeight)/TileConstants.levelRenderHeightTiles);
    TileConstants.tileWidth = TileConstants.tileHeight;
    LCameraInitCamera();
}

int TileMapGetWidth() {
    return MapHeader.LevelWidthTiles;
}

int TileMapGetHeight() {
    return MapHeader.LevelHeightTiles;
}

int TileMapGetTileHeight() {
    return TileConstants.tileHeight;
}

int TileMapGetTileWidth() {
    return TileConstants.tileWidth;
}

int Index(int x, int y) {
    return x+y*MapHeader.LevelWidthTiles;
}

void TileMapInit()
{
    LTextureInit(&gTextureTile);
    LTextureLoadFromFile(&gTextureTile, "GameResources/Textures-16.png");
}

void TileMapLoadTileMap(char* path)
{
    SDL_RWops* file = SDL_RWFromFile( path, "r+b" );

    if (file==NULL)
    {
        printf("%s does not exist, cannot load tilemap", path);
        return;
    }
    SDL_RWread(file,&MapHeader,sizeof(MapHeader),1);
    free(mapTiles);
    mapTiles = malloc(sizeof(*mapTiles)*MapHeader.LevelWidthTiles*MapHeader.LevelHeightTiles);
    SDL_RWread(file, mapTiles, sizeof(mapTiles[0]), MapHeader.LevelWidthTiles * MapHeader.LevelHeightTiles);
    SDL_RWclose(file);
}

void TileMapFree()
{
    free(mapTiles);
}

struct LTexture *TileMapGetTexture()
{
    return &gTextureTile;
}

void TileMapSaveTileMap(char* path)
{
    SDL_RWops* file = SDL_RWFromFile( path, "w+b" );

    if(file==NULL)
    {
        printf("%s does not exist, creating tilemap", path);
    }
    // First, we write the mapTiles header
    SDL_RWwrite(file, &MapHeader, sizeof(MapHeader),1);

    // Second, write the tile layer of the mapTiles
    SDL_RWwrite(file, mapTiles, sizeof(mapTiles[0]), MapHeader.LevelWidthTiles * MapHeader.LevelHeightTiles);

    // Third, write the background layer of the mapTiles

    // Fourth, write entities

    SDL_RWclose(file);
}

void TileMapSetTile(int x, int y, int val)
{
    mapTiles[Index(x, y)] = val;
}

void TileMapFillTiles(int x1, int y1, int x2, int y2, int val)
{
    if (x1 > x2){
        int temp = x2; x2 = x1; x1 = temp;
    }
    if (y1 > y2){
        int temp = y2; y2 = y1; y1 = temp;
    }
    for (int xTile = x1; xTile <= x2; xTile++){
        for (int yTile = y1; yTile <= y2; yTile++)
        {
            mapTiles[Index(xTile, yTile)] = val;
        }
    }
}

uint32_t TileMapWhatIsAt(int x, int y)
{
    return mapTiles[Index(x, y)];
}

int fastTileClamp(int tile, int clamp) {
    return tile < 0 ? 0 : tile > clamp ? clamp : tile;
}

int TileMapXToScreenX(int xTile) {
    return ((float)xTile-camera.x)*TileConstants.tileWidth;
}
int TileMapYToScreenY(int yTile) {
    return ((float)yTile-camera.y)*TileConstants.tileHeight;
}

// Probably need some function for converting a tile ID to a tileClip
void TileMapRenderTiles()
{
    for (int x_tile = camera.x;x_tile < fastTileClamp(camera.x+camera.w+1,MapHeader.LevelWidthTiles);x_tile++) {
        for (int y_tile = camera.y;y_tile < fastTileClamp(camera.y+camera.h+1,MapHeader.LevelHeightTiles );y_tile++) {
            if (!mapTiles[Index(x_tile, y_tile)]) continue;
            // TODO: Maybe bake a lookup table for this for speed?
            tileClip.x = TILE_TEXTURE_WIDTH * (mapTiles[Index(x_tile, y_tile)] % TILE_TEXTURE_MAP_WIDTH_TILES);
            tileClip.y = TILE_TEXTURE_WIDTH * (mapTiles[Index(x_tile, y_tile)] / TILE_TEXTURE_MAP_WIDTH_TILES);
            LTextureRender(&gTextureTile,TileMapXToScreenX(x_tile),TileMapYToScreenY(y_tile),TileConstants.tileWidth,TileConstants.tileHeight,&tileClip);
        }
    }
}