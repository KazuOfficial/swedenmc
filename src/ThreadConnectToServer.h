#ifndef THREAD_CONNECT_TO_SERVER_H
#define THREAD_CONNECT_TO_SERVER_H

#include <pthread.h>
#include "GuiConnecting.h"

struct Minecraft;

typedef struct ThreadConnectToServer {

    struct Minecraft *mc;

    char *hostName;

    int port;

    GuiConnecting *connectingGui;
    pthread_t thread;
} ThreadConnectToServer;

ThreadConnectToServer *ThreadConnectToServer_create(GuiConnecting *var1, struct Minecraft *var2,
                                                    const char *var3, int var4);

void ThreadConnectToServer_destroy(ThreadConnectToServer *self);

#endif
