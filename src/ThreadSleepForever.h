#ifndef THREAD_SLEEP_FOREVER_H
#define THREAD_SLEEP_FOREVER_H

#include <pthread.h>

struct Minecraft;

typedef struct ThreadSleepForever {
    struct Minecraft *mc;
    pthread_t thread;

} ThreadSleepForever;

ThreadSleepForever *ThreadSleepForever_create(struct Minecraft *var1, const char *var2);

void ThreadSleepForever_destroy(ThreadSleepForever *self);

#endif
