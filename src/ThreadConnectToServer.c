#include "ThreadConnectToServer.h"
#include "Minecraft.h"
#include "NetClientHandler.h"
#include "Packet2Handshake.h"
#include "GuiConnectFailed.h"
#include <errno.h>
#ifndef _WIN32
#include <netdb.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *ThreadConnectToServer_run(void *arg) {
    ThreadConnectToServer *self = (ThreadConnectToServer *)arg;
    NetClientHandler *nch = NetClientHandler_create(self->mc, self->hostName, self->port);
    if (nch != NULL) {
        GuiConnecting_setNetClientHandler(self->connectingGui, nch);
        if (GuiConnecting_isCancelled(self->connectingGui))
            return NULL;
        Packet *handshake = Packet2Handshake_createWith(self->mc->session->username);
        NetClientHandler_addToSendQueue(GuiConnecting_getNetClientHandler(self->connectingGui),
                                        handshake);
        return NULL;
    }

    if (GuiConnecting_isCancelled(self->connectingGui))
        return NULL;
    char detail[512];
#ifndef _WIN32
    if (h_errno != 0) {
#else
    if (0) {
#endif

        snprintf(detail, sizeof(detail), "Unknown host '%s'", self->hostName);
    } else if (errno == ECONNREFUSED || errno == ETIMEDOUT) {

        snprintf(detail, sizeof(detail), "%s", strerror(errno));
    } else {

        fprintf(stderr, "Exception in thread \"Client thread\" java.lang.Exception: %s\n",
                strerror(errno));
        snprintf(detail, sizeof(detail), "java.lang.Exception: %s", strerror(errno));
    }
    GuiConnectFailed *gui = (GuiConnectFailed *)calloc(1, sizeof(GuiConnectFailed));
    GuiConnectFailed_construct(gui, "connect.failed", "disconnect.genericReason", detail);
    Minecraft_displayGuiScreen(self->mc, (GuiScreen *)gui);
    return NULL;
}

ThreadConnectToServer *ThreadConnectToServer_create(GuiConnecting *var1, struct Minecraft *var2,
                                                    const char *var3, int var4) {
    ThreadConnectToServer *self = (ThreadConnectToServer *)calloc(1, sizeof(ThreadConnectToServer));
    self->connectingGui = var1;
    self->mc = var2;
    self->hostName = strdup(var3);
    self->port = var4;
    pthread_create(&self->thread, NULL, ThreadConnectToServer_run, self);
    return self;
}

void ThreadConnectToServer_destroy(ThreadConnectToServer *self) {
    free(self->hostName);
    free(self);
}
