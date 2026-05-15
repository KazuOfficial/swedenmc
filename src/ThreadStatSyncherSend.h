#ifndef THREAD_STAT_SYNCHER_SEND_H
#define THREAD_STAT_SYNCHER_SEND_H

#include "StatsSyncher.h"
#include "StatMap.h"
#include <pthread.h>

typedef struct ThreadStatSyncherSend {
    StatMap *field_27233_a;
    StatsSyncher *field_27232_b;
    pthread_t thread;
} ThreadStatSyncherSend;

ThreadStatSyncherSend *ThreadStatSyncherSend_create(StatsSyncher *var1, StatMap *var2);
void ThreadStatSyncherSend_start(ThreadStatSyncherSend *self);

#endif
