#ifndef PACKET30_ENTITY_H
#define PACKET30_ENTITY_H

#include "Packet.h"
#include <stdint.h>

struct Packet30Entity {
    Packet base;
    int entityId;
    int8_t xPosition;
    int8_t yPosition;
    int8_t zPosition;
    int8_t yaw;
    int8_t pitch;
    int rotating;
};
typedef struct Packet30Entity Packet30Entity;

Packet *Packet30Entity_create(void);
void Packet30Entity_readPacketData(Packet *self, DataStream *var1);
void Packet30Entity_writePacketData(Packet *self, DataStream *var1);
void Packet30Entity_processPacket(Packet *self, NetHandler *var1);
int Packet30Entity_getPacketSize(Packet *self);

#endif
