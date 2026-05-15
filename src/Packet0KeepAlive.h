#ifndef PACKET0_KEEP_ALIVE_H
#define PACKET0_KEEP_ALIVE_H

#include "Packet.h"

struct Packet0KeepAlive {
    Packet base;
};
typedef struct Packet0KeepAlive Packet0KeepAlive;

Packet *Packet0KeepAlive_create(void);
void Packet0KeepAlive_readPacketData(Packet *self, DataStream *var1);
void Packet0KeepAlive_writePacketData(Packet *self, DataStream *var1);
void Packet0KeepAlive_processPacket(Packet *self, NetHandler *var1);
int Packet0KeepAlive_getPacketSize(Packet *self);

#endif
