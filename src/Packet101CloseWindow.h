#ifndef PACKET101_CLOSE_WINDOW_H
#define PACKET101_CLOSE_WINDOW_H

#include "Packet.h"

struct Packet101CloseWindow {
    Packet base;
    int windowId;
};
typedef struct Packet101CloseWindow Packet101CloseWindow;

Packet *Packet101CloseWindow_create(void);
Packet *Packet101CloseWindow_createWith(int windowId);
void Packet101CloseWindow_readPacketData(Packet *self, DataStream *var1);
void Packet101CloseWindow_writePacketData(Packet *self, DataStream *var1);
void Packet101CloseWindow_processPacket(Packet *self, NetHandler *var1);
int Packet101CloseWindow_getPacketSize(Packet *self);

#endif
