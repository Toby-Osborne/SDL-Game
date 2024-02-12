//
// Created by tazukiswift on 12/02/24.
//

#include "TileMap.h"

SDL_Renderer* tRenderer;
struct LTexture * textureMap;

SDL_Rect tileClip = {0, 0, TILE_TEXTURE_WIDTH, TILE_TEXTURE_HEIGHT};

uint8_t map[LEVEL_WIDTH_TILES*LEVEL_HEIGHT_TILES] = {0};

struct Tile tileMap[LEVEL_WIDTH_TILES*LEVEL_HEIGHT_TILES];

void TileMapInit(struct LTexture* texture, SDL_Renderer* renderer)
{
    tRenderer = renderer;
    textureMap = texture;

    for (int x_tile = 0;x_tile < LEVEL_WIDTH_TILES;x_tile++) {
        for (int y_tile = 0;y_tile < LEVEL_HEIGHT_TILES;y_tile++) {
            tileMap[INDEX(x_tile,y_tile)].mPos.x = x_tile*TILE_WIDTH;
            tileMap[INDEX(x_tile,y_tile)].mPos.y = y_tile*TILE_WIDTH;

            tileMap[INDEX(x_tile,y_tile)].mType = map[INDEX(x_tile,y_tile)];
        }
    }

    for (int x_tile = 0; x_tile < 10; x_tile ++) {
        map[INDEX(x_tile,5)] = 1;
        tileMap[INDEX(x_tile,5)].mType = 1;
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

void TileMapRenderTiles(SDL_Rect *camera)
{
    for (int x_tile = camera->x/TILE_WIDTH;x_tile < fastTileClamp((camera->x+camera->w)/TILE_WIDTH+1,LEVEL_WIDTH_TILES);x_tile++) {
        for (int y_tile = camera->y/TILE_WIDTH;y_tile < fastTileClamp((camera->y+camera->h)/TILE_WIDTH+1,LEVEL_HEIGHT_TILES );y_tile++) {
            if (!tileMap[INDEX(x_tile,y_tile)].mType) continue;
            int x = tileMap[INDEX(x_tile,y_tile)].mPos.x-camera->x;
            int y =tileMap[INDEX(x_tile,y_tile)].mPos.y-camera->y;
            LTextureRender(textureMap,x,y,128,128,&tileClip);
        }
    }
}