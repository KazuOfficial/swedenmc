#include "Packet21PickupSpawn.h"
#include "NetHandler.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "MathHelper.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet21PickupSpawn_readPacketData,
    Packet21PickupSpawn_writePacketData,
    Packet21PickupSpawn_processPacket,
    Packet21PickupSpawn_getPacketSize,
};

Packet *Packet21PickupSpawn_create(void) {
    Packet21PickupSpawn *self = calloc(1, sizeof(Packet21PickupSpawn));
    self->base.vtable = &vtable;
    self->base.packetId = 21;
    return (Packet *)self;
}

Packet *Packet21PickupSpawn_createWith(EntityItem *var1) {
    Packet21PickupSpawn *self = (Packet21PickupSpawn *)Packet21PickupSpawn_create();
    self->entityId = var1->base.entityId;
    self->itemID = var1->item.itemID;
    self->count = var1->item.stackSize;
    self->itemDamage = ItemStack_getItemDamage(&var1->item);
    self->xPosition = MathHelper_floor_double(var1->base.posX * 32.0);
    self->yPosition = MathHelper_floor_double(var1->base.posY * 32.0);
    self->zPosition = MathHelper_floor_double(var1->base.posZ * 32.0);
    self->rotation = (int8_t)((int)(var1->base.motionX * 128.0));
    self->pitch = (int8_t)((int)(var1->base.motionY * 128.0));
    self->roll = (int8_t)((int)(var1->base.motionZ * 128.0));
    return (Packet *)self;
}

void Packet21PickupSpawn_readPacketData(Packet *p, DataStream *var1) {
    Packet21PickupSpawn *self = (Packet21PickupSpawn *)p;
    self->entityId = DataStream_readInt(var1);
    self->itemID = (int)DataStream_readShort(var1);
    self->count = (int)(int8_t)DataStream_readByte(var1);
    self->itemDamage = (int)DataStream_readShort(var1);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
    self->rotation = (int8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)DataStream_readByte(var1);
    self->roll = (int8_t)DataStream_readByte(var1);
}

void Packet21PickupSpawn_writePacketData(Packet *p, DataStream *var1) {
    Packet21PickupSpawn *self = (Packet21PickupSpawn *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeShort(var1, (int16_t)self->itemID);
    DataStream_writeByte(var1, (uint8_t)self->count);
    DataStream_writeShort(var1, (int16_t)self->itemDamage);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->rotation);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
    DataStream_writeByte(var1, (uint8_t)self->roll);
}

void Packet21PickupSpawn_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handlePickupSpawn(var1, (Packet21PickupSpawn *)p);
}

int Packet21PickupSpawn_getPacketSize(Packet *p) {
    (void)p;
    return 24;
}
