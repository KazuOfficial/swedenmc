#include "Packet28EntityVelocity.h"
#include "NetHandler.h"
#include "Entity.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet28EntityVelocity_readPacketData,
    Packet28EntityVelocity_writePacketData,
    Packet28EntityVelocity_processPacket,
    Packet28EntityVelocity_getPacketSize,
};

Packet *Packet28EntityVelocity_create(void) {
    Packet28EntityVelocity *self = calloc(1, sizeof(Packet28EntityVelocity));
    self->base.vtable = &vtable;
    self->base.packetId = 28;
    return (Packet *)self;
}

Packet *Packet28EntityVelocity_createFromEntity(Entity *var1) {
    return Packet28EntityVelocity_createWith(var1->entityId, var1->motionX, var1->motionY,
                                             var1->motionZ);
}

Packet *Packet28EntityVelocity_createWith(int entityId, double vx, double vy, double vz) {
    Packet28EntityVelocity *self = (Packet28EntityVelocity *)Packet28EntityVelocity_create();
    self->entityId = entityId;
    double var8 = 3.9;
    if (vx < -var8)
        vx = -var8;
    if (vy < -var8)
        vy = -var8;
    if (vz < -var8)
        vz = -var8;
    if (vx > var8)
        vx = var8;
    if (vy > var8)
        vy = var8;
    if (vz > var8)
        vz = var8;
    self->motionX = (int)(vx * 8000.0);
    self->motionY = (int)(vy * 8000.0);
    self->motionZ = (int)(vz * 8000.0);
    return (Packet *)self;
}

void Packet28EntityVelocity_readPacketData(Packet *p, DataStream *var1) {
    Packet28EntityVelocity *self = (Packet28EntityVelocity *)p;
    self->entityId = DataStream_readInt(var1);
    self->motionX = (int)DataStream_readShort(var1);
    self->motionY = (int)DataStream_readShort(var1);
    self->motionZ = (int)DataStream_readShort(var1);
}

void Packet28EntityVelocity_writePacketData(Packet *p, DataStream *var1) {
    Packet28EntityVelocity *self = (Packet28EntityVelocity *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeShort(var1, (int16_t)self->motionX);
    DataStream_writeShort(var1, (int16_t)self->motionY);
    DataStream_writeShort(var1, (int16_t)self->motionZ);
}

void Packet28EntityVelocity_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_6498_a(var1, (Packet28EntityVelocity *)p);
}

int Packet28EntityVelocity_getPacketSize(Packet *p) {
    (void)p;
    return 10;
}
