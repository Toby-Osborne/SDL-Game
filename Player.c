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

const float VEL_DECAY_CONSTANT = 0.8f;

SDL_Rect collision_box;

SDL_Renderer *pRenderer;

struct LTimer playerTimer;

struct LTimer dabTimer;

struct LTexture *pTexture;

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture)
{
    LTimerInit(&playerTimer);
    LTimerAction(&playerTimer,START);
    LTimerInit(&dabTimer);

    pRenderer = renderer;
    pTexture = texture;
    collision_box.x = (int)mPosX;
    collision_box.y = (int)mPosY;
    collision_box.w = PLAYER_WIDTH;
    collision_box.h = PLAYER_HEIGHT;
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
        if (TileMapWhatIsAt((int)(mPosXPlus), (int)mPosY + PLAYER_HEIGHT)) return true;
    } else {
        if (TileMapWhatIsAt((int)(mPosXPlus) + PLAYER_WIDTH, (int)mPosY)) return true;
        if (TileMapWhatIsAt((int)(mPosXPlus) + PLAYER_WIDTH, (int)mPosY + PLAYER_HEIGHT)) return true;
    }
    return false;
}

bool PlayerCheckYCollision()
{
    // Use mPosXPlus
    if (mVelY < 0)
    {
        if (TileMapWhatIsAt((int)mPosX,(int)mPosYPlus)) return true;
        if (TileMapWhatIsAt((int)mPosX + PLAYER_WIDTH, (int)mPosYPlus)) return true;
    } else {
        if (TileMapWhatIsAt((int)mPosX, (int)mPosYPlus + PLAYER_HEIGHT)) return true;
        if (TileMapWhatIsAt((int)mPosX + PLAYER_WIDTH, (int)mPosYPlus + PLAYER_HEIGHT)) return true;
    }
    return false;
}

const float terminal_velocity = 5.f;

int playerDistX = 0;

void PlayerProcessMovement()
{

    switch(PlayerController[UP]+(PlayerController[DOWN]<<1))
    {
        case 0:
            break;
        case 1:
            // TODO: check ground collision here
            if (TileMapWhatIsAt((int)mPosX + PLAYER_WIDTH / 2, (int)mPosY + PLAYER_HEIGHT + 2)) mVelY -= 2.5;
            if ((int)mPosY + PLAYER_HEIGHT + 2 > LEVEL_HEIGHT) mVelY -= 2.5;
            break;
        case 2:
            mVelY = PLAYER_VELOCITY;
            break;
    }
    switch(PlayerController[LEFT]+(PlayerController[RIGHT]<<1))
    {
        case 0:
            mVelX *= VEL_DECAY_CONSTANT;
            break;
        case 1:
            mVelX = -PLAYER_VELOCITY;
            break;
        case 2:
            mVelX = PLAYER_VELOCITY;
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
    } else if ((int)mPosXPlus + PLAYER_WIDTH > LEVEL_WIDTH )
    {
        mVelX = 0;
        mPosX = LEVEL_WIDTH - PLAYER_WIDTH - 1;
    }
    else if(PlayerCheckXCollision())
    {
        // Clip to nearest tile if not already
        if (mVelX < 0) {
            mPosX = (float)(TILE_WIDTH*((int)mPosX/TILE_WIDTH));
        } else {
            mPosX = (float)(TILE_WIDTH*(((int)mPosX + PLAYER_WIDTH) / TILE_WIDTH + 1) - PLAYER_WIDTH - 1);
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
    } else if ((int)mPosYPlus + PLAYER_HEIGHT > LEVEL_HEIGHT) {
        mVelY = 0;
        mPosY = LEVEL_HEIGHT - PLAYER_HEIGHT - 1;
    } else if (PlayerCheckYCollision())
    {
        // Clip to nearest tile if not already
        if (mVelY < 0) {
            mPosY = (float)(TILE_HEIGHT*((int)mPosY/TILE_HEIGHT));
        } else {
            mPosY = (float)(TILE_HEIGHT*((((int)mPosY + PLAYER_HEIGHT) / TILE_HEIGHT) + 1) - PLAYER_HEIGHT - 1);
        }
        mVelY = 0;
    }
    else {
        mPosY = mPosYPlus;
    }

    if (mVelX != 0) {
        playerDistX += (int)(5.f*mVelX);
    }
    collision_box.x = (int)mPosX;
    collision_box.y = (int)mPosY;

    LCameraProcessMovement();
}

int animationCounter = 0;

bool dabFlag = false;
int dabTime = 5000;

void PlayerRender(int camX, int camY)
{
    if (PlayerController[DOWN] && !PlayerController[LEFT] && !PlayerController[RIGHT]) {
        animationCounter = 6;
    }
    else if (!TileMapWhatIsAt((int)mPosX,(int)mPosY + PLAYER_HEIGHT+2)) {    // If in the air
        animationCounter = 7;
    } else if ((mVelX > -0.1) && (mVelX < 0.1)) {
        if (!dabFlag) {
            animationCounter = 4;
            LTimerAction(&dabTimer, START);
            dabFlag = true;
        } else {
            if (LTimerGetTicks(&dabTimer) > dabTime) {
                animationCounter = 5;
                LTimerAction(&dabTimer, STOP);
            }
        }
    } else {
        dabFlag = false;
        if (playerDistX > 16) {
            playerDistX = 0;
            animationCounter = (animationCounter+1)%4;
        } else if (playerDistX < -16) {
            playerDistX = 0;
            animationCounter = (animationCounter+1)%4;
        }
    }
    int height_offset = 8;
    SDL_Rect clip = {64*animationCounter,0,PLAYER_TEXTURE_WIDTH,PLAYER_TEXTURE_HEIGHT};
    if (mVelX > 0) {
        LTextureRenderEx(pTexture, (int)mPosX-camX-(PLAYER_HEIGHT-PLAYER_WIDTH)/2, (int)mPosY-camY+height_offset, PLAYER_HEIGHT, PLAYER_HEIGHT, &clip, 0.0f, NULL, SDL_FLIP_NONE);
    } else {
        LTextureRenderEx(pTexture, (int)mPosX-camX-(PLAYER_HEIGHT-PLAYER_WIDTH)/2, (int)mPosY-camY+height_offset, PLAYER_HEIGHT, PLAYER_HEIGHT, &clip, 0.0f, NULL, SDL_FLIP_HORIZONTAL);
    }
}

int PlayerGetX(){ return (int)mPosX; }

int PlayerGetY() { return (int)mPosY; }