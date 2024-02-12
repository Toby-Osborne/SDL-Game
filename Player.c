//
// Created by tazukiswift on 11/02/24.
//

#include "Player.h"
#include "main.h"
#include "physics.h"
#include "LTimer.h"
#include "LCamera.h"
#include "TileMap.h"
float mPosX, mPosY;

float mPosXPlus, mPosYPlus;

float mVelX, mVelY;

const float VEL_DECAY_CONSTANT = 0.4f;

SDL_Rect collision_box;

SDL_Renderer *pRenderer;

struct LTimer playerTimer;

struct LTexture *pTexture;

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture)
{
    LTimerInit(&playerTimer);
    LTimerAction(&playerTimer,START);

    pRenderer = renderer;
    pTexture = texture;
    collision_box.x = (int)mPosX;
    collision_box.y = (int)mPosY;
    collision_box.w = 100;
    collision_box.h = 100;
}

enum PlayerVControllerKeys {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NUM_CONTROLS = 5,
};

bool PlayerController[NUM_CONTROLS] = {0};

void PlayerHandleEvent(SDL_Event *e)
{
    //If a key was pressed
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: PlayerController[UP] = true; break;
            case SDLK_DOWN: PlayerController[DOWN] = true; break;
            case SDLK_LEFT: PlayerController[LEFT] = true; break;
            case SDLK_RIGHT: PlayerController[RIGHT] = true; break;
        }
    }
        //If a key was released
    else if( e->type == SDL_KEYUP && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: PlayerController[UP] = false; break;
            case SDLK_DOWN: PlayerController[DOWN] = false; break;
            case SDLK_LEFT: PlayerController[LEFT] = false; break;
            case SDLK_RIGHT: PlayerController[RIGHT] = false; break;
        }
    }
}

bool checkCollision (SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool PlayerCheckXCollision()
{
    // Use mPosXPlus
    if (mVelX < 0)
    {
        if (TileMapWhatIsAt((int)(mPosXPlus),(int)mPosY)) return true;
        if (TileMapWhatIsAt((int)(mPosXPlus),(int)mPosY+DOT_HEIGHT)) return true;
    } else {
        if (TileMapWhatIsAt((int)(mPosXPlus)+DOT_WIDTH,(int)mPosY)) return true;
        if (TileMapWhatIsAt((int)(mPosXPlus)+DOT_WIDTH,(int)mPosY+DOT_HEIGHT)) return true;
    }
    return false;
}

bool PlayerCheckYCollision()
{
    // Use mPosXPlus
    if (mVelY < 0)
    {
        if (TileMapWhatIsAt((int)mPosX,(int)mPosYPlus)) return true;
        if (TileMapWhatIsAt((int)mPosX+DOT_WIDTH,(int)mPosYPlus)) return true;
    } else {
        if (TileMapWhatIsAt((int)mPosX,(int)mPosYPlus+DOT_HEIGHT)) return true;
        if (TileMapWhatIsAt((int)mPosX+DOT_WIDTH,(int)mPosYPlus+DOT_HEIGHT)) return true;
    }
    return false;
}

const float terminal_velocity = 5.f;

void PlayerProcessMovement()
{

    switch(PlayerController[UP]+(PlayerController[DOWN]<<1))
    {
        case 0:
            break;
        case 1:
            // TODO: check ground collision here
            if (TileMapWhatIsAt((int)mPosX+DOT_WIDTH/2,(int)mPosY+DOT_HEIGHT+2)) mVelY -= 2.5;
            if ((int)mPosY+DOT_HEIGHT+2 > LEVEL_HEIGHT) mVelY -= 2.5;
            break;
        case 2:
            mVelY = DOT_VEL;
            break;
    }
    switch(PlayerController[LEFT]+(PlayerController[RIGHT]<<1))
    {
        case 0:
            mVelX *= VEL_DECAY_CONSTANT;
            break;
        case 1:
            mVelX = -DOT_VEL;
            break;
        case 2:
            mVelX = DOT_VEL;
            break;
    }

    uint32_t dt = LTimerStopwatch(&playerTimer);
    mVelY += (g*(float)dt);
    mPosXPlus = mPosX + (float)dt*mVelX;
    mPosYPlus = mPosY + (float)dt*mVelY;

    if (mVelY > terminal_velocity) mVelY = terminal_velocity;

    //If the dot went too far to the left or right
    if (mPosXPlus < 0) {
        mVelX = 0;
        mPosX = 0;
    } else if ( (int)mPosXPlus + DOT_WIDTH > LEVEL_WIDTH )
    {
        mVelX = 0;
        mPosX = LEVEL_WIDTH-DOT_WIDTH;
    }
    else if(PlayerCheckXCollision())
    {
        // Clip to nearest tile if not already
        if (mVelX < 0) {
            mPosX = (float)(TILE_WIDTH*((int)mPosX/TILE_WIDTH));
        } else {
            mPosX = (float)(TILE_WIDTH*(((int)mPosX+DOT_WIDTH)/TILE_WIDTH+1)-DOT_WIDTH-1);
        }
        mVelX = 0;
    }
    else
    {
        mPosX = mPosXPlus;
    }
    if(mPosYPlus < 0) {
        mVelY = 0;
        mPosY = 0;
    } else if ((int)mPosYPlus + DOT_HEIGHT > LEVEL_HEIGHT) {
        mVelY = 0;
        mPosY = LEVEL_HEIGHT-DOT_HEIGHT;
    } else if (PlayerCheckYCollision())
    {
        // Clip to nearest tile if not already
        if (mVelY < 0) {
            mPosY = (float)(TILE_HEIGHT*((int)mPosY/TILE_HEIGHT));
        } else {
            mPosY = (float)(TILE_HEIGHT*((((int)mPosY+DOT_HEIGHT)/TILE_HEIGHT)+1)-DOT_HEIGHT-1);
        }
        mVelY = 0;
    }
    else {
        mPosY = mPosYPlus;
    }

    collision_box.x = (int)mPosX;
    collision_box.y = (int)mPosY;

    LCameraProcessMovement();
}

void PlayerRender(int camX, int camY)
{
    //Show the dot
    LTextureRender(pTexture, (int)mPosX-camX, (int)mPosY-camY, collision_box.w, collision_box.h,NULL);
}

int PlayerGetX(){ return (int)mPosX; }

int PlayerGetY() { return (int)mPosY; }