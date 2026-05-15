#ifndef PACKET39_ATTACH_ENTITY_H
#define PACKET39_ATTACH_ENTITY_H

#include "Packet.h"

struct Packet39AttachEntity {
    Packet base;
    int entityId;
    int vehicleEntityId;
};
typedef struct Packet39AttachEntity Packet39AttachEntity;

Packet *Packet39AttachEntity_create(void);
void Packet39AttachEntity_readPacketData(Packet *self, DataStream *var1);
void Packet39AttachEntity_writePacketData(Packet *self, DataStream *var1);
void Packet39AttachEntity_processPacket(Packet *self, NetHandler *var1);
int Packet39AttachEntity_getPacketSize(Packet *self);

#endif
