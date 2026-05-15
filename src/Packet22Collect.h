#ifndef PACKET22_COLLECT_H
#define PACKET22_COLLECT_H

#include "Packet.h"

struct Packet22Collect {
    Packet base;
    int collectedEntityId;
    int collectorEntityId;
};
typedef struct Packet22Collect Packet22Collect;

Packet *Packet22Collect_create(void);
void Packet22Collect_readPacketData(Packet *self, DataStream *var1);
void Packet22Collect_writePacketData(Packet *self, DataStream *var1);
void Packet22Collect_processPacket(Packet *self, NetHandler *var1);
int Packet22Collect_getPacketSize(Packet *self);

#endif
