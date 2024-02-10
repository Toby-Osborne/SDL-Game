//
// Created by tazukiswift on 11/02/24.
//

#include "LTimer.h"
#include <SDL.h>

void LTimerInit(struct LTimer* timer)
{
    timer->mPausedTicks = 0;
    timer->mPaused = false;

    timer->mStartTicks = 0;
    timer->mStarted = false;
}

void LTimerAction(struct LTimer* timer, enum LTimerAction action)
{
    switch (action)
    {
        case START:
            timer->mStarted = true;
            timer->mStartTicks = SDL_GetTicks();
            timer->mPaused = false;
            timer->mPausedTicks = 0;
            break;
        case STOP:
            timer->mStarted = false;
            timer->mPaused = false;
            timer->mStartTicks = 0;
            timer->mPausedTicks = 0;
            break;
        case PAUSE:
            if( timer->mStarted && !timer->mPaused )
            {
                timer->mPaused = true;
                timer->mPausedTicks = SDL_GetTicks() - timer->mStartTicks;
                timer->mStartTicks = 0;
            }
            break;
        case UNPAUSE:
            if( timer->mStarted && timer->mPaused )
            {
                timer->mPaused = false;
                timer->mStartTicks = SDL_GetTicks() - timer->mPausedTicks;
                timer->mPausedTicks = 0;
            }
            break;
    }
}

uint32_t LTimerGetTicks(struct LTimer* timer)
{
    if (timer->mStarted)
    {
        return timer->mPaused ? timer->mPausedTicks:SDL_GetTicks() - timer->mStartTicks;
    }
    else
    {
        return SDL_GetTicks() - timer->mStartTicks;
    }
}