#ifndef PACKET100_OPEN_WINDOW_H
#define PACKET100_OPEN_WINDOW_H

#include "Packet.h"

struct Packet100OpenWindow {
    Packet base;
    int windowId;
    int inventoryType;

    char *windowTitle;
    int slotsCount;
};
typedef struct Packet100OpenWindow Packet100OpenWindow;

Packet *Packet100OpenWindow_create(void);
void Packet100OpenWindow_readPacketData(Packet *self, DataStream *var1);
void Packet100OpenWindow_writePacketData(Packet *self, DataStream *var1);
void Packet100OpenWindow_processPacket(Packet *self, NetHandler *var1);
int Packet100OpenWindow_getPacketSize(Packet *self);

#endif
