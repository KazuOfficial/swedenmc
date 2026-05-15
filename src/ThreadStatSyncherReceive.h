#ifndef THREAD_STAT_SYNCHER_RECEIVE_H
#define THREAD_STAT_SYNCHER_RECEIVE_H

#include "StatsSyncher.h"
#include <pthread.h>

typedef struct ThreadStatSyncherReceive {

    StatsSyncher *field_27231_a;
    pthread_t thread;
} ThreadStatSyncherReceive;

ThreadStatSyncherReceive *ThreadStatSyncherReceive_create(StatsSyncher *var1);

void ThreadStatSyncherReceive_start(ThreadStatSyncherReceive *self);

#endif
