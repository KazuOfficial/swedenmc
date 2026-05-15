#ifndef PACKET6_SPAWN_POSITION_H
#define PACKET6_SPAWN_POSITION_H

#include "Packet.h"

struct Packet6SpawnPosition {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;
};
typedef struct Packet6SpawnPosition Packet6SpawnPosition;

Packet *Packet6SpawnPosition_create(void);
void Packet6SpawnPosition_readPacketData(Packet *self, DataStream *var1);
void Packet6SpawnPosition_writePacketData(Packet *self, DataStream *var1);
void Packet6SpawnPosition_processPacket(Packet *self, NetHandler *var1);
int Packet6SpawnPosition_getPacketSize(Packet *self);

#endif
