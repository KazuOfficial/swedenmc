#include "ThreadSleepForever.h"
#include "Minecraft.h"
#include <stdlib.h>
#include <time.h>

static void *ThreadSleepForever_run(void *arg) {
    ThreadSleepForever *self = (ThreadSleepForever *)arg;
    while (self->mc->running) {
        struct timespec ts;

        ts.tv_sec = 2147483647L / 1000;
        ts.tv_nsec = (2147483647L % 1000) * 1000000L;
        nanosleep(&ts, NULL);
    }
    return NULL;
}

ThreadSleepForever *ThreadSleepForever_create(struct Minecraft *var1, const char *var2) {
    (void)var2;
    ThreadSleepForever *self = (ThreadSleepForever *)calloc(1, sizeof(ThreadSleepForever));
    self->mc = var1;

    pthread_create(&self->thread, NULL, ThreadSleepForever_run, self);
    pthread_detach(self->thread);
    return self;
}

void ThreadSleepForever_destroy(ThreadSleepForever *self) { free(self); }
