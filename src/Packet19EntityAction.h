#ifndef PACKET19_ENTITY_ACTION_H
#define PACKET19_ENTITY_ACTION_H

#include "Packet.h"
#include "GameForward.h"

struct Packet19EntityAction {
    Packet base;
    int entityId;
    int state;
};
typedef struct Packet19EntityAction Packet19EntityAction;

Packet *Packet19EntityAction_create(void);

Packet *Packet19EntityAction_createWith(Entity *var1, int var2);
void Packet19EntityAction_readPacketData(Packet *self, DataStream *var1);
void Packet19EntityAction_writePacketData(Packet *self, DataStream *var1);
void Packet19EntityAction_processPacket(Packet *self, NetHandler *var1);
int Packet19EntityAction_getPacketSize(Packet *self);

#endif
