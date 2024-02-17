//
// Created by tazukiswift on 11/02/24.
//

#include "Player.h"
#include "physics.h"
#include "../LTimer.h"
#include "LCamera.h"
#include "TileMap.h"
#include "../MenuObjects/LMenu.h"
float mPosX, mPosY;

float mPosXPlus, mPosYPlus;

float mVelX, mVelY;

const float VEL_DECAY_CONSTANT = 0.8f;

SDL_Rect collision_box;

extern SDL_Rect camera;

struct LTimer playerTimer;

struct LTimer dabTimer;

struct LTexture *pTexture;

enum GameStates gameMode;

void PlayerRespawn()
{
    LTimerStopwatch(&playerTimer);
    mPosX = 0;
    mPosY = 0;
    mVelX = 0;
    mVelY = 0;
}

void PlayerPause() {
    LTimerAction(&playerTimer, TIMER_PAUSE);
}

void PlayerUnpause() {
    LTimerAction(&playerTimer, TIMER_UNPAUSE);
}

void PlayerSetGameMode(enum GameStates state)
{
    if ((state != GS_LEVEL_EDIT)&&(state != GS_LEVEL))
    {
        printf("Tried to set invalid game mode");
        return;
    }
    gameMode = state;
}

enum GameStates PlayerGetGameMode() {return gameMode;}

void PlayerInit(struct LTexture *texture)
{
    LTimerInit(&playerTimer);
    LTimerAction(&playerTimer, TIMER_START);
    LTimerInit(&dabTimer);

    pTexture = texture;
    collision_box.x = (int)mPosX;
    collision_box.y = (int)mPosY;
    collision_box.w = PLAYER_WIDTH;
    collision_box.h = PLAYER_HEIGHT;

    PlayerPause();
}

enum PlayerVControllerKeys {
    PLAYER_UP,
    PLAYER_DOWN,
    PLAYER_LEFT,
    PLAYER_RIGHT,
    PLAYER_SHIFT,
    PLAYER_CTRL,
    NUM_CONTROLS,
};

bool PlayerController[NUM_CONTROLS] = {0};

int dragStartX,dragStartY;

int currentPlacingTile = 1;

void EditorSetCurrentTile(int tileVal) {currentPlacingTile = tileVal;}

void EditorMapInteract(SDL_Event *e)
{
    int x, y = {0};

    switch(e->type)
    {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_GetMouseState(&x, &y);
            TileMapSetTile(x + camera.x, y + camera.y,
                           e->button.button == SDL_BUTTON_LEFT ? currentPlacingTile : 0);
            if (PlayerController[PLAYER_SHIFT]) {
                dragStartX = x + camera.x;
                dragStartY = y + camera.y;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
            if (PlayerController[PLAYER_SHIFT]) {
                SDL_GetMouseState(&x, &y);
                TileMapFillTiles(x + camera.x,y + camera.y,dragStartX,dragStartY,e->button.button == SDL_BUTTON_LEFT ? currentPlacingTile : 0);
            }
            break;
    }

}

void PlayerMapInteract(SDL_Event *e)
{
    // HAHA ADVENTURE MODE
}

enum GameStates PlayerHandleInput(SDL_Event *e)
{
    if (e->key.repeat) return gameMode;
    //If a key was pressed
    switch (e->type)
    {
        case SDL_KEYDOWN:
            switch( e->key.keysym.sym )
            {
                case SDLK_w: PlayerController[PLAYER_UP] = true; break;
                case SDLK_s: PlayerController[PLAYER_DOWN] = true; break;
                case SDLK_a: PlayerController[PLAYER_LEFT] = true; break;
                case SDLK_d: PlayerController[PLAYER_RIGHT] = true; break;
                case SDLK_e: PlayerPause(); LMenuSetInventoryMenu(); return GS_PAUSED;
                case SDLK_ESCAPE: PlayerPause(); LMenuSetPauseMenu(); return GS_PAUSED;
                case SDLK_LSHIFT: PlayerController[PLAYER_SHIFT] = true; break;
                case SDLK_LCTRL: PlayerController[PLAYER_CTRL] = true; break;
                // Edit Specific

            }
            break;
        case SDL_KEYUP:
            //Adjust the velocity
            switch( e->key.keysym.sym )
            {
                case SDLK_w: PlayerController[PLAYER_UP] = false; break;
                case SDLK_s: PlayerController[PLAYER_DOWN] = false; break;
                case SDLK_a: PlayerController[PLAYER_LEFT] = false; break;
                case SDLK_d: PlayerController[PLAYER_RIGHT] = false; break;
                case SDLK_LSHIFT: PlayerController[PLAYER_SHIFT] = false; break;
                case SDLK_LCTRL: PlayerController[PLAYER_CTRL] = false; break;
            }
            break;
    }
    if (gameMode == GS_LEVEL_EDIT){
        EditorMapInteract(e);
    } else {
        PlayerMapInteract(e);
    }
    return gameMode;
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
    switch(PlayerController[PLAYER_UP] + (PlayerController[PLAYER_DOWN] << 1))
    {
        case 0:
            break;
        case 1:
            // TODO: check ground collision here
            if (TileMapWhatIsAt((int)mPosX + PLAYER_WIDTH / 2, (int)mPosY + PLAYER_HEIGHT + 1)) mVelY -= 2.5;
            if ((int)mPosY + PLAYER_HEIGHT + 1 > LEVEL_HEIGHT) mVelY -= 2.5;
            break;
        case 2:
            mVelY = PLAYER_VELOCITY;
            break;
    }
    switch(PlayerController[PLAYER_LEFT] + (PlayerController[PLAYER_RIGHT] << 1))
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

void EditorProcessMovement()
{
    int horizontalMoveConst = PlayerController[PLAYER_LEFT] + (PlayerController[PLAYER_RIGHT] << 1);
    int verticalMoveConst = PlayerController[PLAYER_UP] + (PlayerController[PLAYER_DOWN] << 1);
    switch (horizontalMoveConst)
    {
        case 1:
            if ((int)mPosX - TILE_WIDTH > 0) mPosX -= TILE_WIDTH; break;
        case 2:
            if ((int)mPosX + TILE_WIDTH < LEVEL_WIDTH) mPosX += TILE_WIDTH; break;
        default:
            break;
    }
    switch (verticalMoveConst)
    {
        case 1:
            if ((int)mPosY - TILE_HEIGHT > 0) mPosY -= TILE_HEIGHT; break;
        case 2:
            if ((int)mPosY + TILE_HEIGHT < LEVEL_HEIGHT) mPosY += TILE_HEIGHT; break;
        default:
            break;
    }
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
    if (gameMode == GS_LEVEL){
        int inputPos = PlayerController[PLAYER_UP] + (PlayerController[PLAYER_DOWN] << 1) + (PlayerController[PLAYER_LEFT] << 2) + (PlayerController[PLAYER_RIGHT] << 3);
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
                else if (PlayerController[PLAYER_UP])
                {
                    PlayerState = PJUMP;
                }
                else if (PlayerController[PLAYER_DOWN])
                {
                    PlayerState = PCROUCH;
                }
                else if (PlayerController[PLAYER_LEFT] || PlayerController[PLAYER_RIGHT])
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
                if (!PlayerController[PLAYER_LEFT] && !PlayerController[PLAYER_RIGHT] && PlayerController[PLAYER_DOWN])
                {
                    PlayerState = PCROUCH;
                }
                else if (PlayerController[PLAYER_UP]) {
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
                if (PlayerController[PLAYER_LEFT] || PlayerController[PLAYER_RIGHT]) {
                    PlayerState = PRUN;
                }
                else if (!PlayerController[PLAYER_DOWN])
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
                if ((TileMapWhatIsAt((int)mPosX,(int)mPosY+PLAYER_HEIGHT+2))||(TileMapWhatIsAt((int)mPosX+PLAYER_WIDTH,(int)mPosY+PLAYER_HEIGHT+2))) {
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
    } else {
        AnimationFrame = KDAB;
    }

    int height_offset = 8;
    SDL_Rect clip = {64*(int)AnimationFrame,0,PLAYER_TEXTURE_WIDTH,PLAYER_TEXTURE_HEIGHT};
    LTextureRenderEx(pTexture, (int)mPosX-camX-(PLAYER_HEIGHT-PLAYER_WIDTH)/2, (int)mPosY-camY+height_offset, PLAYER_HEIGHT, PLAYER_HEIGHT, &clip, 0.0f, NULL, mVelX > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

int PlayerGetX(){ return (int)mPosX; }

int PlayerGetY() { return (int)mPosY; }