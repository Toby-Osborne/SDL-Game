//
// Created by tazukiswift on 18/02/24.
//

#include "Inventory.h"
#include <SDL.h>
#include "../LevelObjects/TileMap.h"

#define INVENTORY_COLUMNS 32
#define INVENTORY_ROWS 32

#define INVENTORY_BOX_SIZE 32
#define INVENTORY_BOX_SEPARATION 8
#define INVENTORY_X_OFFSET 128
#define INVENTORY_Y_OFFSET 128

extern SDL_Renderer* gRenderer;

SDL_Rect inventorySlotBoxes[INVENTORY_COLUMNS*INVENTORY_ROWS] = {};
SDL_Vertex inventoryQuads[4*INVENTORY_COLUMNS*INVENTORY_ROWS] = {};
int inventoryQuadIndices[6*INVENTORY_COLUMNS*INVENTORY_ROWS] = {};

#define INV_INDEX(x,y) ((x) + (y) * INVENTORY_COLUMNS)

struct InventoryItemStack inventoryItems[INVENTORY_COLUMNS*INVENTORY_ROWS] = {0};

void inventoryUpdateSlotWithItem(int xSlot, int ySlot, int itemID)
{
    inventoryItems[INV_INDEX(xSlot,ySlot)].id = itemID;

    // Our system assumes that the item ids are the same as the tile ids, therefore
    // the item texture map should be identical to the tile texture map

    int tileTextureX = itemID % TILE_TEXTURE_MAP_WIDTH_TILES;
    int tileTextureY = itemID / TILE_TEXTURE_MAP_WIDTH_TILES;

    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2)].tex_coord.x = (float)tileTextureX / 32.f;
    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2)].tex_coord.y = (float)tileTextureY / 32.f;

    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 1].tex_coord.x = (float)(tileTextureX + 1) / 32.f;
    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 1].tex_coord.y = (float)tileTextureY / 32.f;

    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 2].tex_coord.x = (float)tileTextureX / 32.f;
    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 2].tex_coord.y = (float)(tileTextureY + 1) / 32.f;

    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 3].tex_coord.x = (float)(tileTextureX + 1) / 32.f;
    inventoryQuads[(INV_INDEX(xSlot,ySlot) << 2) + 3].tex_coord.y = (float)(tileTextureY + 1) / 32.f;
}

int InventoryGetIDFromScreenCoordinates(int x, int y)
{
    if ((x-INVENTORY_X_OFFSET)%(INVENTORY_BOX_SIZE+INVENTORY_BOX_SEPARATION) < INVENTORY_BOX_SIZE) {
        if ((y-INVENTORY_Y_OFFSET)%(INVENTORY_BOX_SIZE+INVENTORY_BOX_SEPARATION) < INVENTORY_BOX_SIZE) {
            int xSlot = (x-INVENTORY_X_OFFSET)/(INVENTORY_BOX_SIZE+INVENTORY_BOX_SEPARATION);
            int ySlot = (y-INVENTORY_Y_OFFSET)/(INVENTORY_BOX_SIZE+INVENTORY_BOX_SEPARATION);
            return inventoryItems[INV_INDEX(xSlot,ySlot)].id;
        }
    }
    return 0;
}

bool InventoryInit()
{
    SDL_Color drawColor = { 0xff,0xff,0xff,0xff };
    int indexTemplate[6] = {0 ,1 ,2 ,2 ,1 ,3};
    for (int ySlot = 0;ySlot < INVENTORY_ROWS;ySlot++)
    {
        for (int xSlot = 0;xSlot < INVENTORY_COLUMNS;xSlot++)
        {
            inventoryUpdateSlotWithItem(xSlot,ySlot,INV_INDEX(xSlot,ySlot));

            inventorySlotBoxes[xSlot + ySlot * INVENTORY_COLUMNS].x = INVENTORY_X_OFFSET + xSlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);
            inventorySlotBoxes[xSlot + ySlot * INVENTORY_COLUMNS].y = INVENTORY_X_OFFSET + ySlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);
            inventorySlotBoxes[xSlot + ySlot * INVENTORY_COLUMNS].w = INVENTORY_BOX_SIZE;
            inventorySlotBoxes[xSlot + ySlot * INVENTORY_COLUMNS].h = INVENTORY_BOX_SIZE;

            // Top Left
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2)].position.x = INVENTORY_X_OFFSET + xSlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2)].position.y = INVENTORY_X_OFFSET + ySlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);

            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2)].color = drawColor;

            // Top Right
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 1].position.x = INVENTORY_X_OFFSET + xSlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION) + INVENTORY_BOX_SIZE;
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 1].position.y = INVENTORY_X_OFFSET + ySlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 1].color = drawColor;

            // Bottom Left
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 2].position.x = INVENTORY_X_OFFSET + xSlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION);
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 2].position.y = INVENTORY_X_OFFSET + ySlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION) + INVENTORY_BOX_SIZE;
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 2].color = drawColor;

            // Bottom Right
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 3].position.x = INVENTORY_X_OFFSET + xSlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION) + INVENTORY_BOX_SIZE;
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 3].position.y = INVENTORY_X_OFFSET + ySlot * (INVENTORY_BOX_SIZE + INVENTORY_BOX_SEPARATION) + INVENTORY_BOX_SIZE;
            inventoryQuads[((xSlot + ySlot * INVENTORY_COLUMNS) << 2) + 3].color = drawColor;
            memcpy(&inventoryQuadIndices[((xSlot + ySlot * INVENTORY_COLUMNS) * 6)],&indexTemplate,6*sizeof(indexTemplate[0]));
            for (int i = 0;i<6;i++) {indexTemplate[i] += 4;}
        }
    }
}

void InventoryRender()
{
    SDL_RenderDrawRects(gRenderer, inventorySlotBoxes, INVENTORY_COLUMNS * INVENTORY_ROWS);
    SDL_RenderGeometry(gRenderer,TileMapGetTexture()->mTexture,inventoryQuads,4*INVENTORY_ROWS*INVENTORY_COLUMNS,inventoryQuadIndices,6*INVENTORY_ROWS*INVENTORY_COLUMNS);
}