#include "Packet24MobSpawn.h"
#include "NetHandler.h"
#include "EntityLiving.h"
#include "EntityList.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet24MobSpawn_readPacketData,
    Packet24MobSpawn_writePacketData,
    Packet24MobSpawn_processPacket,
    Packet24MobSpawn_getPacketSize,
};

Packet *Packet24MobSpawn_create(void) {
    Packet24MobSpawn *self = calloc(1, sizeof(Packet24MobSpawn));
    self->base.vtable = &vtable;
    self->base.packetId = 24;
    DataWatcher_init(&self->receivedMetadata);
    return (Packet *)self;
}

Packet *Packet24MobSpawn_createWith(EntityLiving *var1) {
    Packet24MobSpawn *self = (Packet24MobSpawn *)Packet24MobSpawn_create();
    self->entityId = var1->base.entityId;
    self->type = (int8_t)EntityList_getEntityID((Entity *)var1);
    self->xPosition = MathHelper_floor_double(var1->base.posX * 32.0);
    self->yPosition = MathHelper_floor_double(var1->base.posY * 32.0);
    self->zPosition = MathHelper_floor_double(var1->base.posZ * 32.0);
    self->yaw = (int8_t)((int)(var1->base.rotationYaw * 256.0f / 360.0f));
    self->pitch = (int8_t)((int)(var1->base.rotationPitch * 256.0f / 360.0f));
    self->metaData = Entity_getDataWatcher((Entity *)var1);
    return (Packet *)self;
}

void Packet24MobSpawn_readPacketData(Packet *p, DataStream *var1) {
    Packet24MobSpawn *self = (Packet24MobSpawn *)p;
    self->entityId = DataStream_readInt(var1);
    self->type = (int8_t)DataStream_readByte(var1);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
    self->yaw = (int8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)DataStream_readByte(var1);

    DataWatcher_readFromStream(&self->receivedMetadata, var1);
}

void Packet24MobSpawn_writePacketData(Packet *p, DataStream *var1) {
    Packet24MobSpawn *self = (Packet24MobSpawn *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeByte(var1, (uint8_t)self->type);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->yaw);
    DataStream_writeByte(var1, (uint8_t)self->pitch);

    DataWatcher_writeToStream(self->metaData, var1);
}

void Packet24MobSpawn_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleMobSpawn(var1, (Packet24MobSpawn *)p);
}

int Packet24MobSpawn_getPacketSize(Packet *p) {
    (void)p;
    return 20;
}

DataWatcher *Packet24MobSpawn_getMetadata(Packet24MobSpawn *self) {
    return &self->receivedMetadata;
}
