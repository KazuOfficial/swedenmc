#ifndef THREAD_CLOSE_CONNECTION_H
#define THREAD_CLOSE_CONNECTION_H

#include <pthread.h>
#include "NetworkManager.h"

typedef struct ThreadCloseConnection {

    NetworkManager *field_28109_a;
    pthread_t thread;
} ThreadCloseConnection;

ThreadCloseConnection *ThreadCloseConnection_create(NetworkManager *var1);

void ThreadCloseConnection_destroy(ThreadCloseConnection *self);

#endif
