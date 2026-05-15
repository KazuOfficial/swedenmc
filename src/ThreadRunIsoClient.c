#include "ThreadRunIsoClient.h"
#include <stdlib.h>
#include <time.h>

static void *ThreadRunIsoClient_run(void *arg) {
    ThreadRunIsoClient *self = (ThreadRunIsoClient *)arg;
    while (CanvasIsomPreview_isRunning(self->isoCanvas)) {
        CanvasIsomPreview_showNextBuffer(self->isoCanvas);
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1000000L;
        nanosleep(&ts, NULL);
    }
    return NULL;
}

ThreadRunIsoClient *ThreadRunIsoClient_create(CanvasIsomPreview *var1) {
    ThreadRunIsoClient *self = (ThreadRunIsoClient *)calloc(1, sizeof(ThreadRunIsoClient));
    self->isoCanvas = var1;
    pthread_create(&self->thread, NULL, ThreadRunIsoClient_run, self);
    return self;
}

void ThreadRunIsoClient_destroy(ThreadRunIsoClient *self) { free(self); }
