//
// Created by tazukiswift on 11/02/24.
//

#include "Player.h"
#include "../main.h"
#include "physics.h"
#include "../LTimer.h"
#include "LCamera.h"
#include "TileMap.h"
float mPosX, mPosY;

float mPosXPlus, mPosYPlus;

float mVelX, mVelY;

const float VEL_DECAY_CONSTANT = 0.8f;

SDL_Rect collision_box;

SDL_Rect *pCamera;

struct LTimer playerTimer;

struct LTimer dabTimer;

struct LTexture *pTexture;

bool paused = true;

void PlayerPause() {paused = true;}

void PlayerUnpause() {
    paused = false;
    LTimerStopwatch(&playerTimer);
}

void PlayerInit(SDL_Renderer *renderer, struct LTexture* texture)
{
    LTimerInit(&playerTimer);
    LTimerAction(&playerTimer, TIMER_START);
    LTimerInit(&dabTimer);

    pCamera = LCameraGetCamera();
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
            case SDLK_w: PlayerController[UP] = true; break;
            case SDLK_s: PlayerController[DOWN] = true; break;
            case SDLK_a: PlayerController[LEFT] = true; break;
            case SDLK_d: PlayerController[RIGHT] = true; break;
        }
    }
        //If a key was released
    else if( e->type == SDL_KEYUP && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_w: PlayerController[UP] = false; break;
            case SDLK_s: PlayerController[DOWN] = false; break;
            case SDLK_a: PlayerController[LEFT] = false; break;
            case SDLK_d: PlayerController[RIGHT] = false; break;
        }
    }
    else if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        int x,y;
        SDL_GetMouseState( &x, &y );
        TileMapSetTile(x+pCamera->x,y+pCamera->y,TileMapWhatIsAt(x+pCamera->x, y+pCamera->y) == 0 ? 1 : 0);
    }
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

const int dabTime = 5000;

enum PlayerSprites {
    KRUN1,
    KRUN2,
    KRUN3,
    KRUN4,
    KSTOP,
    KDAB,
    KCROUCH,
    KJUMP,
    KHANG
} AnimationFrame;

enum PlayerAnimationState {
    PRUN,
    PSTOP,
    PDAB,
    PCROUCH,
    PJUMP,
    PHANG
} PlayerState = PSTOP;

const int RunFrames = 4;
const int RunFrameCap = 4*RunFrames;
int RunFrameCounter;

void PlayerRender(int camX, int camY)
{
    int inputPos = PlayerController[UP]+(PlayerController[DOWN]<<1)+(PlayerController[LEFT]<<2)+(PlayerController[RIGHT]<<3);
    // The greatest state machine in the world...
    // Tribute...
    switch (PlayerState)
    {
        case PSTOP:
            AnimationFrame = KSTOP;
            if (!LTimerStarted(&dabTimer)) LTimerAction(&dabTimer, TIMER_START);
            if (LTimerGetTicks(&dabTimer) > dabTime) {
                PlayerState = PDAB;
            }
            else if (PlayerController[UP])
            {
                PlayerState = PJUMP;
            }
            else if (PlayerController[DOWN])
            {
                PlayerState = PCROUCH;
            }
            else if (PlayerController[LEFT] || PlayerController[RIGHT])
            {
                PlayerState = PRUN;
            }
            else
            {
                // Set animation
                AnimationFrame = KSTOP;
                break;
            }
            LTimerAction(&dabTimer,TIMER_STOP); // If we didn't break above, then we left STOP state
            break;
        case PRUN:
            if (!PlayerController[LEFT]&&!PlayerController[RIGHT]&&PlayerController[DOWN])
            {
                PlayerState = PCROUCH;
            }
            else if (PlayerController[UP]) {
                PlayerState = PJUMP;
            }
            else if (!inputPos)
            {
                PlayerState = PSTOP;
            }
            else
            {
                RunFrameCounter = (RunFrameCounter + 1) % RunFrameCap;
                AnimationFrame = RunFrameCounter/(RunFrameCap/RunFrames);
                break;
            }
            RunFrameCounter = 0;
            break;
        case PCROUCH:
            if (PlayerController[LEFT] || PlayerController[RIGHT]) {
                PlayerState = PRUN;
            }
            else if (!PlayerController[DOWN])
            {
                if (!TileMapWhatIsAt((int)mPosX,(int)mPosY+PLAYER_HEIGHT+1))
                {
                    PlayerState = PJUMP;
                }
                else
                {
                    PlayerState = PSTOP;
                }
            }
            AnimationFrame = KCROUCH;
            break;
        case PJUMP:
            // We only exit jump if something below
            if (TileMapWhatIsAt((int)mPosX,(int)mPosY+PLAYER_HEIGHT+2)) {
                PlayerState = PRUN;
            }
            AnimationFrame = KJUMP;
            break;

        case PDAB:
            if (inputPos) {
                PlayerState = PSTOP;
            }
            AnimationFrame = KDAB;
            break;
    }

    int height_offset = 8;
    SDL_Rect clip = {64*(int)AnimationFrame,0,PLAYER_TEXTURE_WIDTH,PLAYER_TEXTURE_HEIGHT};
    LTextureRenderEx(pTexture, (int)mPosX-camX-(PLAYER_HEIGHT-PLAYER_WIDTH)/2, (int)mPosY-camY+height_offset, PLAYER_HEIGHT, PLAYER_HEIGHT, &clip, 0.0f, NULL, mVelX > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

int PlayerGetX(){ return (int)mPosX; }

int PlayerGetY() { return (int)mPosY; }