#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct Timer {
    float ticksPerSecond;
    double lastHRTime;
    int elapsedTicks;
    float renderPartialTicks;
    float timerSpeed;
    float elapsedPartialTicks;
    int64_t lastSyncSysClock;
    int64_t lastSyncHRClock;
    int64_t field_28132_i;
    double timeSyncAdjustment;
} Timer;

void Timer_init(Timer *self, float ticksPerSecond);
void Timer_updateTimer(Timer *self);

#endif
