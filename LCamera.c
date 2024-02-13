//
// Created by tazukiswift on 11/02/24.
//

#include "LCamera.h"
#include "Player.h"
#include "TileMap.h"
#include "main.h"

SDL_Rect camera = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

void LCameraProcessMovement()
{
    camera.x = (PlayerGetX() + PLAYER_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = (PlayerGetY() + PLAYER_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Center the camera over the dot
    camera.x = (PlayerGetX() + PLAYER_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = (PlayerGetY() + PLAYER_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

SDL_Rect *LCameraGetCamera()
{
    return &camera;
}
