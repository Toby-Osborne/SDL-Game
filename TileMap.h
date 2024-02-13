//
// Created by tazukiswift on 12/02/24.
//

#ifndef SDLGAME_TILEMAP_H
#define SDLGAME_TILEMAP_H

#include "LTexture.h"
#include "LCamera.h"
#include <SDL.h>

//Tile constants
#define TILE_WIDTH 128
#define TILE_HEIGHT 128

#define TILE_TEXTURE_WIDTH 512
#define TILE_TEXTURE_HEIGHT 512

#define LEVEL_WIDTH_TILES 64
#define LEVEL_HEIGHT_TILES 32

#define LEVEL_WIDTH LEVEL_WIDTH_TILES*TILE_WIDTH
#define LEVEL_HEIGHT LEVEL_HEIGHT_TILES*TILE_HEIGHT

#define INDEX(x,y) (LEVEL_WIDTH_TILES * (y) + (x))

struct Tile{
    int mType;
};

void TileMapInit(struct LTexture* texture, SDL_Renderer* renderer);

uint8_t TileMapWhatIsAt(int x, int y);

void TileMapRenderTiles(SDL_Rect *camera);

#endif //SDLGAME_TILEMAP_H
