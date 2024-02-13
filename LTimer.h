//
// Created by tazukiswift on 11/02/24.
//

#ifndef SDLGAME_LTIMER_H
#define SDLGAME_LTIMER_H

#include <stdint.h>
#include <stdbool.h>

struct LTimer {
    uint32_t mStartTicks;
    uint32_t mPausedTicks;
    bool mPaused;
    bool mStarted;
};

void LTimerInit(struct LTimer* timer);

enum LTimerAction {
    TIMER_START,
    TIMER_STOP,
    TIMER_PAUSE,
    TIMER_UNPAUSE
};

void LTimerAction(struct LTimer* timer, enum LTimerAction action);

uint32_t LTimerGetTicks(struct LTimer* timer);

uint32_t LTimerStopwatch(struct LTimer* timer);

static bool LTimerStarted(struct LTimer* timer) {return timer->mStarted;}

static bool LTimerPaused(struct LTimer* timer) {return timer->mPaused;}

#endif //SDLGAME_LTIMER_H
