//
// Created by tazukiswift on 11/02/24.
//

#include "LCamera.h"
#include "Player.h"
#include "TileMap.h"
#include "../GameWindow.h"
#include "physics.h"

// All values normalized to tile width
SDLP_FloatRect camera;

void LCameraInitCamera()
{
    camera.w = ((float)GameWindowGetWindowWidth())/((float)TileMapGetTileWidth());
    camera.h = ((float)GameWindowGetWindowHeight())/((float)TileMapGetTileHeight());
}

void LCameraProcessMovement()
{
    //Center the camera over the dot
    camera.x = (PlayerGetX() + PlayerGetW() / 2 ) - camera.w / 2;
    camera.y = (PlayerGetY() + PlayerGetH() / 2 ) - camera.h / 2;

    //Keep the camera in bounds
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > TileMapGetWidth() - camera.w )
    {
        camera.x = TileMapGetWidth() - camera.w;
    }
    if( camera.y > TileMapGetHeight() - camera.h )
    {
        camera.y = TileMapGetHeight() - camera.h;
    }
}
