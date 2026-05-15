#ifndef PACKET38_ENTITY_STATUS_H
#define PACKET38_ENTITY_STATUS_H

#include "Packet.h"
#include <stdint.h>

struct Packet38EntityStatus {
    Packet base;
    int entityId;
    int8_t entityStatus;
};
typedef struct Packet38EntityStatus Packet38EntityStatus;

Packet *Packet38EntityStatus_create(void);
void Packet38EntityStatus_readPacketData(Packet *self, DataStream *var1);
void Packet38EntityStatus_writePacketData(Packet *self, DataStream *var1);
void Packet38EntityStatus_processPacket(Packet *self, NetHandler *var1);
int Packet38EntityStatus_getPacketSize(Packet *self);

#endif
