//
// Created by tazukiswift on 11/02/24.
//

#include "Player.h"
#include "physics.h"
#include "../LTimer.h"
#include "LCamera.h"
#include "TileMap.h"
#include "../MenuObjects/LMenu.h"
const float VEL_DECAY_CONSTANT = 0.9f;

extern SDLP_FloatRect camera;

struct LTimer playerTimer;

struct LTimer dabTimer;

struct LTexture *pTexture;

enum GameStates gameMode;

enum PlayerState {
    PRUN,
    PSTOP,
    PDAB,
    PCROUCH,
    PJUMP,
    PHANG
} PlayerState = PSTOP;

struct Player {
    enum PlayerState playerState;
    float mPosX, mPosY;
    float mPosXPlus, mPosYPlus;
    float mVelX, mVelY;
    float playerWidth, playerHeight;
} Player;

void PlayerRespawn(float x, float y)
{
    LTimerStopwatch(&playerTimer);
    Player.mPosX = x;
    Player.mPosY = y;
    Player.mVelX = 0;
    Player.mVelY = 0;
    Player.playerWidth = DEFAULT_PLAYER_WIDTH;
    Player.playerHeight = DEFAULT_PLAYER_HEIGHT;
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
    if (Player.mVelX < 0)
    {
        if (TileMapWhatIsAt((int)(Player.mPosXPlus),(int)(Player.mPosY))) return true;
        if (TileMapWhatIsAt((int)(Player.mPosXPlus), (int)((Player.mPosY) + (0.5f*(Player.playerHeight))))) return true;
        if (TileMapWhatIsAt((int)(Player.mPosXPlus), (int)((Player.mPosY) + Player.playerHeight))) return true;
    } else {
        if (TileMapWhatIsAt((int)(Player.mPosXPlus + Player.playerWidth), (int)(Player.mPosY))) return true;
        if (TileMapWhatIsAt((int)(Player.mPosXPlus + Player.playerWidth), (int)(Player.mPosY + (0.5*Player.playerHeight)))) return true;
        if (TileMapWhatIsAt((int)(Player.mPosXPlus + Player.playerWidth), (int)(Player.mPosY + Player.playerHeight))) return true;
    }
    return false;
}


bool PlayerCheckYCollision()
{
    // Use mPosXPlus
    if (Player.mVelY < 0)
    {
        if (TileMapWhatIsAt((int)(Player.mPosX),(int)Player.mPosYPlus)) return true;
        if (TileMapWhatIsAt((int)(Player.mPosX + Player.playerWidth), (int)Player.mPosYPlus)) return true;
    } else {
        if (TileMapWhatIsAt((int)(Player.mPosX), (int)(Player.mPosYPlus + Player.playerHeight))) return true;
        if (TileMapWhatIsAt((int)(Player.mPosX + Player.playerWidth), (int)(Player.mPosYPlus + Player.playerHeight))) return true;
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

            if ((TileMapWhatIsAt(Player.mPosX, Player.mPosY + Player.playerHeight+0.1f))||((TileMapWhatIsAt(Player.mPosX + Player.playerWidth, Player.mPosY + Player.playerHeight + 0.1f))))
            {
                Player.mVelY = -PLAYER_JUMP_VELOCITY;
            }
            else
            {
                Player.mVelY -= PLAYER_JUMP_ANTIGRAVITY;
            }
            if (Player.mPosY + Player.playerHeight + 1 > TileMapGetHeight()) Player.mVelY = 0;
            break;
        case 2:
            break;
    }

        if (PlayerController[PLAYER_LEFT] && !PlayerController[PLAYER_RIGHT] && Player.mVelX < 0.01)
        {
            Player.mVelX -= PLAYER_ACCELERATION;
            if (Player.mVelX <  (PlayerState == PCROUCH ? -CROUCH_FACTOR*PLAYER_VELOCITY:-PLAYER_VELOCITY)) Player.mVelX = (PlayerState == PCROUCH ? -CROUCH_FACTOR*PLAYER_VELOCITY:-PLAYER_VELOCITY);
        }
        else if (!PlayerController[PLAYER_LEFT] && PlayerController[PLAYER_RIGHT] && Player.mVelX > -0.01)
        {
            Player.mVelX += PLAYER_ACCELERATION;
            if (Player.mVelX > (PlayerState == PCROUCH ? CROUCH_FACTOR*PLAYER_VELOCITY:PLAYER_VELOCITY)) Player.mVelX = (PlayerState == PCROUCH ? CROUCH_FACTOR*PLAYER_VELOCITY:PLAYER_VELOCITY);
        }
        else
        {
            Player.mVelX = 0;
        }


    uint32_t dt = LTimerStopwatch(&playerTimer);
    Player.mVelY += (g*(float)dt);

    Player.mPosXPlus = Player.mPosX + (float)dt*Player.mVelX;
    Player.mPosYPlus = Player.mPosY + (float)dt*Player.mVelY;

    if (Player.mVelY > terminal_velocity) Player.mVelY = terminal_velocity;

    //If the dot went too far to the left or right
    if (Player.mPosXPlus < 0) {
        Player.mVelX = 0;
        Player.mPosX = 0;
    } else if ((int)Player.mPosXPlus + Player.playerWidth > TileMapGetWidth())
    {
        Player.mVelX = 0;
        Player.mPosX = TileMapGetWidth() - Player.playerWidth - 1;
    }
    else if(PlayerCheckXCollision())
    {
        // Clip to nearest tile if not already
        if (Player.mVelX < 0) {
            Player.mPosX = (float)(((int)Player.mPosX));
        } else {
            Player.mPosX = (float)((int)(Player.mPosX + Player.playerWidth))+0.99-Player.playerWidth;
        }
        Player.mVelX = 0;
    }
    else
    {
        Player.mPosX = Player.mPosXPlus;
    }
    if(Player.mPosYPlus < 0) {
        Player.mVelY = 0;
        Player.mPosY = 0;
    } else if ((int)Player.mPosYPlus + Player.playerHeight > TileMapGetHeight()) {
        Player.mVelY = 0;
        Player.mPosY = TileMapGetHeight() - Player.playerHeight;
    } else if (PlayerCheckYCollision())
    {
        // Clip to nearest tile if not already
        if (Player.mVelY < 0) {
            Player.mPosY = (float)((int)Player.mPosY);
        } else {
            Player.mPosY = (float)((int)(Player.mPosY + Player.playerHeight))+0.99-Player.playerHeight;
        }
        Player.mVelY = 0;
    }
    else {
        Player.mPosY = Player.mPosYPlus;
    }

    if (Player.mVelX != 0) {
        playerDistX += (int)(5.f*Player.mVelX);
    }
//    printf("xVel: %.2f     yVel: %.2f     mPosX: %.2f    mPosY: %.2f\n",Player.mVelX,Player.mVelY,Player.mPosX,Player.mPosY);
    LCameraProcessMovement();
}

float editorMoveSpeed = 0.5f;

void EditorProcessMovement()
{
    int horizontalMoveConst = PlayerController[PLAYER_LEFT] + (PlayerController[PLAYER_RIGHT] << 1);
    int verticalMoveConst = PlayerController[PLAYER_UP] + (PlayerController[PLAYER_DOWN] << 1);
    switch (horizontalMoveConst)
    {
        case 1:
            if (Player.mPosX - editorMoveSpeed > 0) Player.mPosX -= editorMoveSpeed; break;
        case 2:
            if (Player.mPosX + editorMoveSpeed < TileMapGetWidth()) Player.mPosX += editorMoveSpeed; break;
        default:
            break;
    }
    switch (verticalMoveConst)
    {
        case 1:
            if (Player.mPosY - editorMoveSpeed > 0) Player.mPosY -= editorMoveSpeed; break;
        case 2:
            if (Player.mPosY + editorMoveSpeed < TileMapGetHeight()) Player.mPosY += editorMoveSpeed; break;
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

const int RunFrames = 4;
const int RunFrameCap = 16*RunFrames;
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
                    Player.playerHeight = 1.f; Player.mPosY += 1.f;
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
                if (PlayerController[PLAYER_DOWN])
                {
                    Player.playerHeight = 1.f; Player.mPosY += 1.f;
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
                if (!PlayerController[PLAYER_DOWN] && !TileMapWhatIsAt(Player.mPosX,Player.mPosY-0.5f) && !TileMapWhatIsAt(Player.mPosX+Player.playerWidth,(Player.mPosY-0.5f)))
                {
                    if (PlayerController[PLAYER_LEFT] || PlayerController[PLAYER_RIGHT]) {
                        PlayerState = PRUN;
                    }
                    else
                    {
                        if (!TileMapWhatIsAt(Player.mPosX, Player.mPosY + Player.playerHeight))
                        {
                            PlayerState = PJUMP;
                        }
                        else
                        {
                            PlayerState = PSTOP;
                        }
                    }
                    Player.playerHeight = 2.f; Player.mPosY -= 1.f;
                }

                AnimationFrame = KCROUCH;
                break;
            case PJUMP:
                // We only exit jump if something below
                if ((TileMapWhatIsAt(Player.mPosX, Player.mPosY + Player.playerHeight + 0.1f)) || (TileMapWhatIsAt(Player.mPosX + Player.playerWidth, Player.mPosY + Player.playerHeight+0.1f))) {
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

    SDL_Rect clip = {64*(int)AnimationFrame,0,PLAYER_TEXTURE_WIDTH,PLAYER_TEXTURE_HEIGHT};
    LTextureRenderEx(pTexture, (Player.mPosX-camera.x-0.6*Player.playerWidth)*TileMapGetTileWidth(), (Player.mPosY - 1.f*(PlayerState==PCROUCH)-camera.y)*TileMapGetTileHeight(), TileMapGetTileWidth()*2, TileMapGetTileHeight()*2, &clip, 0.0f, NULL, PlayerController[PLAYER_RIGHT] > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

float PlayerGetX(){return Player.mPosX;}

float PlayerGetY() {return Player.mPosY;}

float PlayerGetW() {return Player.playerWidth;}

float PlayerGetH() {return Player.playerHeight;}