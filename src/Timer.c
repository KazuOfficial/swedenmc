#include "Timer.h"
#include <time.h>

static int64_t sys_millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

static int64_t sys_nanos_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

void Timer_init(Timer *self, float ticksPerSecond) {
    self->ticksPerSecond = ticksPerSecond;
    self->timerSpeed = 1.0f;
    self->elapsedPartialTicks = 0.0f;
    self->timeSyncAdjustment = 1.0;
    self->lastSyncSysClock = sys_millis();
    self->lastSyncHRClock = sys_nanos_ms();
}

void Timer_updateTimer(Timer *self) {
    int64_t var1 = sys_millis();
    int64_t var3 = var1 - self->lastSyncSysClock;
    int64_t var5 = sys_nanos_ms();
    double var7 = (double)var5 / 1000.0;

    if (var3 > 1000L) {
        self->lastHRTime = var7;
    } else if (var3 < 0L) {
        self->lastHRTime = var7;
    } else {
        self->field_28132_i += var3;
        if (self->field_28132_i > 1000L) {
            int64_t var9 = var5 - self->lastSyncHRClock;
            double var11 = (double)self->field_28132_i / (double)var9;
            self->timeSyncAdjustment += (var11 - self->timeSyncAdjustment) * (double)0.2f;
            self->lastSyncHRClock = var5;
            self->field_28132_i = 0L;
        }
        if (self->field_28132_i < 0L) {
            self->lastSyncHRClock = var5;
        }
    }

    self->lastSyncSysClock = var1;
    double var13 = (var7 - self->lastHRTime) * self->timeSyncAdjustment;
    self->lastHRTime = var7;

    if (var13 < 0.0)
        var13 = 0.0;
    if (var13 > 1.0)
        var13 = 1.0;

    self->elapsedPartialTicks =
        (float)((double)self->elapsedPartialTicks +
                var13 * (double)self->timerSpeed * (double)self->ticksPerSecond);
    self->elapsedTicks = (int)self->elapsedPartialTicks;
    self->elapsedPartialTicks -= (float)self->elapsedTicks;
    if (self->elapsedTicks > 10)
        self->elapsedTicks = 10;
    self->renderPartialTicks = self->elapsedPartialTicks;
}
