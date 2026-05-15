#ifndef PACKET34_ENTITY_TELEPORT_H
#define PACKET34_ENTITY_TELEPORT_H

#include "Packet.h"
#include "GameForward.h"
#include <stdint.h>

struct Packet34EntityTeleport {
    Packet base;
    int entityId;
    int xPosition;
    int yPosition;
    int zPosition;
    int8_t yaw;
    int8_t pitch;
};
typedef struct Packet34EntityTeleport Packet34EntityTeleport;

Packet *Packet34EntityTeleport_create(void);

Packet *Packet34EntityTeleport_createFromEntity(Entity *var1);
void Packet34EntityTeleport_readPacketData(Packet *self, DataStream *var1);
void Packet34EntityTeleport_writePacketData(Packet *self, DataStream *var1);
void Packet34EntityTeleport_processPacket(Packet *self, NetHandler *var1);
int Packet34EntityTeleport_getPacketSize(Packet *self);

#endif
