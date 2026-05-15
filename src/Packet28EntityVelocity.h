#ifndef PACKET28_ENTITY_VELOCITY_H
#define PACKET28_ENTITY_VELOCITY_H

#include "Packet.h"
#include "GameForward.h"

struct Packet28EntityVelocity {
    Packet base;
    int entityId;
    int motionX;
    int motionY;
    int motionZ;
};
typedef struct Packet28EntityVelocity Packet28EntityVelocity;

Packet *Packet28EntityVelocity_create(void);

Packet *Packet28EntityVelocity_createFromEntity(Entity *var1);

Packet *Packet28EntityVelocity_createWith(int entityId, double vx, double vy, double vz);
void Packet28EntityVelocity_readPacketData(Packet *self, DataStream *var1);
void Packet28EntityVelocity_writePacketData(Packet *self, DataStream *var1);
void Packet28EntityVelocity_processPacket(Packet *self, NetHandler *var1);
int Packet28EntityVelocity_getPacketSize(Packet *self);

#endif
