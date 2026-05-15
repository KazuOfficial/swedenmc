#include "Packet5PlayerInventory.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet5PlayerInventory_readPacketData,
    Packet5PlayerInventory_writePacketData,
    Packet5PlayerInventory_processPacket,
    Packet5PlayerInventory_getPacketSize,
};

Packet *Packet5PlayerInventory_create(void) {
    Packet5PlayerInventory *self = calloc(1, sizeof(Packet5PlayerInventory));
    self->base.vtable = &vtable;
    self->base.packetId = 5;
    return (Packet *)self;
}

void Packet5PlayerInventory_readPacketData(Packet *p, DataStream *var1) {
    Packet5PlayerInventory *self = (Packet5PlayerInventory *)p;
    self->entityID = DataStream_readInt(var1);
    self->slot = (int)DataStream_readShort(var1);
    self->itemID = (int)DataStream_readShort(var1);
    self->itemDamage = (int)DataStream_readShort(var1);
}

void Packet5PlayerInventory_writePacketData(Packet *p, DataStream *var1) {
    Packet5PlayerInventory *self = (Packet5PlayerInventory *)p;
    DataStream_writeInt(var1, self->entityID);
    DataStream_writeShort(var1, (int16_t)self->slot);
    DataStream_writeShort(var1, (int16_t)self->itemID);
    DataStream_writeShort(var1, (int16_t)self->itemDamage);
}

void Packet5PlayerInventory_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handlePlayerInventory(var1, (Packet5PlayerInventory *)p);
}

int Packet5PlayerInventory_getPacketSize(Packet *p) {
    (void)p;
    return 8;
}
