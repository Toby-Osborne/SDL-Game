//
// Created by tazukiswift on 11/02/24.
//

#include "Player.h"
#include "main.h"
#include "physics.h"
#include "LCamera.h"
int mPosX, mPosY;

int mVelX, mVelY;

SDL_Rect collision_box;

SDL_Renderer *pRenderer;

struct LTexture *pTexture;

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture)
{
    pRenderer = renderer;
    pTexture = texture;
    collision_box.x = mPosX;
    collision_box.y = mPosY;
    collision_box.w = 100;
    collision_box.h = 100;
}

void PlayerHandleEvent(SDL_Event *e)
{
    //If a key was pressed
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
        //If a key was released
    else if( e->type == SDL_KEYUP && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void PlayerProcessMovement()
{
    //Move the dot left or right
    mPosX += mVelX;
    mPosY += mVelY;

    collision_box.x = mPosX;
    collision_box.y = mPosY;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ))
    {
        //Move back
        mPosX -= mVelX;
    }
    //Move the dot up or down

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ))
    {
        //Move back
        mPosY -= mVelY;
    }
    LCameraProcessMovement();
}

void PlayerRender(int camX, int camY)
{
    //Show the dot
    LTextureRender(pTexture, mPosX-camX, mPosY-camY, NULL);
}

int PlayerGetX(){ return mPosX; }

int PlayerGetY() { return mPosY; }