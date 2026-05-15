#include "Packet20NamedEntitySpawn.h"
#include "NetHandler.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet20NamedEntitySpawn_readPacketData,
    Packet20NamedEntitySpawn_writePacketData,
    Packet20NamedEntitySpawn_processPacket,
    Packet20NamedEntitySpawn_getPacketSize,
};

Packet *Packet20NamedEntitySpawn_create(void) {
    Packet20NamedEntitySpawn *self = calloc(1, sizeof(Packet20NamedEntitySpawn));
    self->base.vtable = &vtable;
    self->base.packetId = 20;
    return (Packet *)self;
}

Packet *Packet20NamedEntitySpawn_createWith(EntityPlayer *var1) {
    Packet20NamedEntitySpawn *self = (Packet20NamedEntitySpawn *)Packet20NamedEntitySpawn_create();
    self->entityId = var1->base.base.entityId;
    self->name = strdup(var1->username);
    self->xPosition = MathHelper_floor_double(var1->base.base.posX * 32.0);
    self->yPosition = MathHelper_floor_double(var1->base.base.posY * 32.0);
    self->zPosition = MathHelper_floor_double(var1->base.base.posZ * 32.0);
    self->rotation = (int8_t)((int)(var1->base.base.rotationYaw * 256.0f / 360.0f));
    self->pitch = (int8_t)((int)(var1->base.base.rotationPitch * 256.0f / 360.0f));
    ItemStack *var2 = InventoryPlayer_getCurrentItem(&var1->inventory);
    self->currentItem = var2 == NULL ? 0 : var2->itemID;
    return (Packet *)self;
}

void Packet20NamedEntitySpawn_readPacketData(Packet *p, DataStream *var1) {
    Packet20NamedEntitySpawn *self = (Packet20NamedEntitySpawn *)p;
    self->entityId = DataStream_readInt(var1);
    self->name = Packet_readString(var1, 16);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
    self->rotation = (int8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)DataStream_readByte(var1);
    self->currentItem = (int)DataStream_readShort(var1);
}

void Packet20NamedEntitySpawn_writePacketData(Packet *p, DataStream *var1) {
    Packet20NamedEntitySpawn *self = (Packet20NamedEntitySpawn *)p;
    DataStream_writeInt(var1, self->entityId);
    Packet_writeString(self->name, var1);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->rotation);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
    DataStream_writeShort(var1, (int16_t)self->currentItem);
}

void Packet20NamedEntitySpawn_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleNamedEntitySpawn(var1, (Packet20NamedEntitySpawn *)p);
}

int Packet20NamedEntitySpawn_getPacketSize(Packet *p) {
    (void)p;
    return 28;
}
