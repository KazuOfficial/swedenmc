#ifndef PACKET29_DESTROY_ENTITY_H
#define PACKET29_DESTROY_ENTITY_H

#include "Packet.h"

struct Packet29DestroyEntity {
    Packet base;
    int entityId;
};
typedef struct Packet29DestroyEntity Packet29DestroyEntity;

Packet *Packet29DestroyEntity_create(void);
void Packet29DestroyEntity_readPacketData(Packet *self, DataStream *var1);
void Packet29DestroyEntity_writePacketData(Packet *self, DataStream *var1);
void Packet29DestroyEntity_processPacket(Packet *self, NetHandler *var1);
int Packet29DestroyEntity_getPacketSize(Packet *self);

#endif
