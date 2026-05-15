#pragma once
#include <pthread.h>
#include "NetHandler.h"
#include "Packet.h"

typedef struct NetworkManager NetworkManager;

NetworkManager *NetworkManager_create(const char *host, int port, NetHandler *netHandler);

void NetworkManager_processReadPackets(NetworkManager *self);

void NetworkManager_wakeThreads(NetworkManager *self);

void NetworkManager_addToSendQueue(NetworkManager *self, Packet *packet);

void NetworkManager_networkShutdown(NetworkManager *self, const char *reason);

void NetworkManager_networkShutdownWithArg(NetworkManager *self, const char *reason,
                                           const char *arg);

void NetworkManager_func_28142_c(NetworkManager *self);

int NetworkManager_isRunning(NetworkManager *var0);

pthread_t *NetworkManager_getReadThread(NetworkManager *var0);

pthread_t *NetworkManager_getWriteThread(NetworkManager *var0);

void NetworkManager_destroy(NetworkManager *self);
