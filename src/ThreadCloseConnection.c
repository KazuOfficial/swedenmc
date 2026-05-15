#include "ThreadCloseConnection.h"
#include <stdlib.h>
#include <time.h>

static void *ThreadCloseConnection_run(void *arg) {
    ThreadCloseConnection *self = (ThreadCloseConnection *)arg;
    struct timespec ts;
    ts.tv_sec = 2;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
    if (NetworkManager_isRunning(self->field_28109_a)) {

        pthread_cancel(*NetworkManager_getWriteThread(self->field_28109_a));
        NetworkManager_networkShutdown(self->field_28109_a, "disconnect.closed");
    }
    return NULL;
}

ThreadCloseConnection *ThreadCloseConnection_create(NetworkManager *var1) {
    ThreadCloseConnection *self = (ThreadCloseConnection *)calloc(1, sizeof(ThreadCloseConnection));
    self->field_28109_a = var1;
    pthread_create(&self->thread, NULL, ThreadCloseConnection_run, self);
    return self;
}

void ThreadCloseConnection_destroy(ThreadCloseConnection *self) { free(self); }
