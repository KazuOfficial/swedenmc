#ifndef THREAD_RUN_ISO_CLIENT_H
#define THREAD_RUN_ISO_CLIENT_H

#include <pthread.h>
#include "CanvasIsomPreview.h"

typedef struct ThreadRunIsoClient {
    CanvasIsomPreview *isoCanvas;
    pthread_t thread;
} ThreadRunIsoClient;

ThreadRunIsoClient *ThreadRunIsoClient_create(CanvasIsomPreview *var1);

void ThreadRunIsoClient_destroy(ThreadRunIsoClient *self);

#endif
