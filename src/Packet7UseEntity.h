#ifndef PACKET7_USE_ENTITY_H
#define PACKET7_USE_ENTITY_H

#include "Packet.h"

struct Packet7UseEntity {
    Packet base;

    int playerEntityId;

    int targetEntity;

    int isLeftClick;
};
typedef struct Packet7UseEntity Packet7UseEntity;

Packet *Packet7UseEntity_create(void);
Packet *Packet7UseEntity_createWith(int playerEntityId, int targetEntity, int isLeftClick);
void Packet7UseEntity_readPacketData(Packet *self, DataStream *var1);
void Packet7UseEntity_writePacketData(Packet *self, DataStream *var1);
void Packet7UseEntity_processPacket(Packet *self, NetHandler *var1);
int Packet7UseEntity_getPacketSize(Packet *self);

#endif
