#include "Packet34EntityTeleport.h"
#include "NetHandler.h"
#include "Entity.h"
#include "MathHelper.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet34EntityTeleport_readPacketData,
    Packet34EntityTeleport_writePacketData,
    Packet34EntityTeleport_processPacket,
    Packet34EntityTeleport_getPacketSize,
};

Packet *Packet34EntityTeleport_create(void) {
    Packet34EntityTeleport *self = calloc(1, sizeof(Packet34EntityTeleport));
    self->base.vtable = &vtable;
    self->base.packetId = 34;
    return (Packet *)self;
}

Packet *Packet34EntityTeleport_createFromEntity(Entity *var1) {
    Packet34EntityTeleport *self = (Packet34EntityTeleport *)Packet34EntityTeleport_create();
    self->entityId = var1->entityId;
    self->xPosition = MathHelper_floor_double(var1->posX * 32.0);
    self->yPosition = MathHelper_floor_double(var1->posY * 32.0);
    self->zPosition = MathHelper_floor_double(var1->posZ * 32.0);
    self->yaw = (int8_t)((int)(var1->rotationYaw * 256.0F / 360.0F));
    self->pitch = (int8_t)((int)(var1->rotationPitch * 256.0F / 360.0F));
    return (Packet *)self;
}

void Packet34EntityTeleport_readPacketData(Packet *p, DataStream *var1) {
    Packet34EntityTeleport *self = (Packet34EntityTeleport *)p;
    self->entityId = DataStream_readInt(var1);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);

    self->yaw = (int8_t)(uint8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)(uint8_t)DataStream_readByte(var1);
}

void Packet34EntityTeleport_writePacketData(Packet *p, DataStream *var1) {
    Packet34EntityTeleport *self = (Packet34EntityTeleport *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);

    DataStream_writeByte(var1, (uint8_t)self->yaw);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
}

void Packet34EntityTeleport_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleEntityTeleport(var1, (Packet34EntityTeleport *)p);
}

int Packet34EntityTeleport_getPacketSize(Packet *p) {
    (void)p;
    return 34;
}
