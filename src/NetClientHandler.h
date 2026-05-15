#pragma once
#include "NetHandler.h"
#include "MapStorage.h"
#include "JavaRandom.h"

typedef struct Minecraft Minecraft;
typedef struct WorldClient WorldClient;
typedef struct NetworkManager NetworkManager;

typedef struct NetClientHandler {
    NetHandler base;

    int disconnected;

    NetworkManager *netManager;

    char *field_1209_a;

    Minecraft *mc;

    WorldClient *worldClient;

    int field_1210_g;

    MapStorage field_28118_b;

    JavaRandom rand;
} NetClientHandler;

NetClientHandler *NetClientHandler_create(Minecraft *var1, const char *var2, int var3);

void NetClientHandler_processReadPackets(NetClientHandler *self);

void NetClientHandler_addToSendQueue(NetClientHandler *self, Packet *var1);

void NetClientHandler_func_28117_a(NetClientHandler *self, Packet *var1);

void NetClientHandler_disconnect(NetClientHandler *self);

int NetClientHandler_isServerHandler(NetClientHandler *self);

void NetClientHandler_destroy(NetClientHandler *self);
