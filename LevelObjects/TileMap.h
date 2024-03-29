//
// Created by tazukiswift on 12/02/24.
//

#ifndef SDLGAME_TILEMAP_H
#define SDLGAME_TILEMAP_H

#include "../LTexture.h"
#include "LCamera.h"
#include <SDL.h>

////Tile constants
//#define TILE_WIDTH 64
//#define TILE_HEIGHT 64
//
//#define LEVEL_WIDTH_TILES 128
//#define LEVEL_HEIGHT_TILES 64
//
//#define LEVEL_WIDTH LEVEL_WIDTH_TILES*TILE_WIDTH
//#define LEVEL_HEIGHT LEVEL_HEIGHT_TILES*TILE_HEIGHT

// This function needs to be called every time the display changes resolution
void TileMapUpdateConstantsFromDisplay();

int TileMapGetTileHeight();

int TileMapGetTileWidth();

int TileMapGetWidth();

int TileMapGetHeight();


#define TILE_TEXTURE_WIDTH 16
#define TILE_TEXTURE_HEIGHT 16

#define TILE_TEXTURE_MAP_WIDTH_TILES 32
#define TILE_TEXTURE_MAP_HEIGHT_TILES 32

void TileMapInit();

struct LTexture *TileMapGetTexture();

uint32_t TileMapWhatIsAt(int x, int y);

void TileMapSetTile(int x, int y, int val);

void TileMapFillTiles(int x1, int y1, int x2, int y2, int val);

void TileMapRenderTiles();

void TileMapLoadTileMap(char* path);

void TileMapSaveTileMap(char* path);

void TileMapFree();

int TileMapXToScreenX(int xTile);

int TileMapYToScreenY(int yTile);

#endif //SDLGAME_TILEMAP_H
