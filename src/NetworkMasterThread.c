#include "NetworkMasterThread.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    NetworkManager *netManager;
} NMTArgs;

static void *NetworkMasterThread_run(void *arg) {
    free(arg);

    usleep(5000000);
    return NULL;
}

void NetworkMasterThread_start(NetworkManager *netManager) {
    NMTArgs *args = calloc(1, sizeof(NMTArgs));
    args->netManager = netManager;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &attr, NetworkMasterThread_run, args);
    pthread_attr_destroy(&attr);
}
