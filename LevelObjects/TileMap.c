//
// Created by tazukiswift on 12/02/24.
//

#include "TileMap.h"

#define TILE_TEXTURE_WIDTH 16
#define TILE_TEXTURE_HEIGHT 16

#define TILE_TEXTURE_MAP_WIDTH_TILES 32
#define TILE_TEXTURE_MAP_HEIGHT_TILES 32

extern SDL_Rect camera;

SDL_Rect tileClip = {0, 0, TILE_TEXTURE_WIDTH, TILE_TEXTURE_HEIGHT};

uint32_t map[LEVEL_WIDTH_TILES*LEVEL_HEIGHT_TILES] = {0};

struct LTexture gTextureTile;

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
    for (int i = 0;i<LEVEL_WIDTH_TILES*LEVEL_HEIGHT_TILES;i++)
    {
        SDL_RWread( file, &map[i], sizeof(uint8_t), 1);
    }
    SDL_RWclose(file);
}

void TileMapSaveTileMap(char* path)
{
    SDL_RWops* file = SDL_RWFromFile( path, "w+b" );

    if(file==NULL)
    {
        printf("%s does not exist, creating tilemap", path);
    }
    for (int i = 0;i<LEVEL_WIDTH_TILES*LEVEL_HEIGHT_TILES;i++)
    {
        SDL_RWwrite(file, &map[i], sizeof(uint8_t), 1);
    }

    SDL_RWclose(file);
}

void TileMapSetTile(int x, int y, int val)
{
    map[INDEX(x / TILE_WIDTH, y / TILE_HEIGHT)] = val;
}

void TileMapFillTiles(int x1, int y1, int x2, int y2, int val)
{
    if (x1 > x2){
        int temp = x2; x2 = x1; x1 = temp;
    }
    if (y1 > y2){
        int temp = y2; y2 = y1; y1 = temp;
    }
    for (int xTile = x1/TILE_WIDTH; xTile <= x2/TILE_WIDTH; xTile++){
        for (int yTile = y1/TILE_HEIGHT; yTile <= y2/TILE_HEIGHT; yTile++)
        {
            map[INDEX(xTile,yTile)] = val;
        }
    }
}

uint8_t TileMapWhatIsAt(int x, int y)
{
    return map[INDEX(x / TILE_WIDTH, y / TILE_HEIGHT)];
}

int fastTileClamp(int tile, int clamp) {
    return tile < 0 ? 0 : tile > clamp ? clamp : tile;
}

// Probably need some function for converting a tile ID to a tileClip
void TileMapRenderTiles()
{
    for (int x_tile = camera.x/TILE_WIDTH;x_tile < fastTileClamp((camera.x+camera.w)/TILE_WIDTH+1,LEVEL_WIDTH_TILES);x_tile++) {
        for (int y_tile = camera.y/TILE_WIDTH;y_tile < fastTileClamp((camera.y+camera.h)/TILE_WIDTH+1,LEVEL_HEIGHT_TILES );y_tile++) {
            if (!map[INDEX(x_tile,y_tile)]) continue;
            tileClip.x = TILE_TEXTURE_WIDTH * (map[INDEX(x_tile,y_tile)]%TILE_TEXTURE_MAP_WIDTH_TILES);
            tileClip.y = TILE_TEXTURE_WIDTH * (map[INDEX(x_tile,y_tile)]/TILE_TEXTURE_MAP_WIDTH_TILES);
            LTextureRender(&gTextureTile,x_tile*TILE_WIDTH-camera.x,y_tile*TILE_HEIGHT-camera.y,TILE_WIDTH,TILE_HEIGHT,&tileClip);
        }
    }
}