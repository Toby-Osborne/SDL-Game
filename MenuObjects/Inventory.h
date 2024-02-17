//
// Created by tazukiswift on 18/02/24.
//

#ifndef SDLGAME_INVENTORY_H
#define SDLGAME_INVENTORY_H

#include <stdbool.h>

struct InventoryItemStack {
    int id;
    int count;
};

bool InventoryInit();
void InventoryRender();
int InventoryGetIDFromScreenCoordinates(int x, int y);
#endif //SDLGAME_INVENTORY_H
