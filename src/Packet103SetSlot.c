#include "Packet103SetSlot.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet103SetSlot_readPacketData,
    Packet103SetSlot_writePacketData,
    Packet103SetSlot_processPacket,
    Packet103SetSlot_getPacketSize,
};

Packet *Packet103SetSlot_create(void) {
    Packet103SetSlot *self = calloc(1, sizeof(Packet103SetSlot));
    self->base.vtable = &vtable;
    self->base.packetId = 103;
    return (Packet *)self;
}

void Packet103SetSlot_readPacketData(Packet *p, DataStream *var1) {
    Packet103SetSlot *self = (Packet103SetSlot *)p;
    self->windowId = (int)(int8_t)DataStream_readByte(var1);
    self->itemSlot = (int)DataStream_readShort(var1);
    int16_t var2 = DataStream_readShort(var1);
    if (var2 >= 0) {
        int8_t var3 = (int8_t)DataStream_readByte(var1);
        int16_t var4 = DataStream_readShort(var1);
        self->myItemStack = calloc(1, sizeof(ItemStack));
        self->myItemStack->itemID = (int)var2;
        self->myItemStack->stackSize = (int)var3;
        self->myItemStack->itemDamage = (int)var4;
    } else {
        self->myItemStack = NULL;
    }
}

void Packet103SetSlot_writePacketData(Packet *p, DataStream *var1) {
    Packet103SetSlot *self = (Packet103SetSlot *)p;
    DataStream_writeByte(var1, (uint8_t)self->windowId);
    DataStream_writeShort(var1, (int16_t)self->itemSlot);
    if (self->myItemStack == NULL) {
        DataStream_writeShort(var1, (int16_t)-1);
    } else {
        DataStream_writeShort(var1, (int16_t)self->myItemStack->itemID);
        DataStream_writeByte(var1, (uint8_t)self->myItemStack->stackSize);
        DataStream_writeShort(var1, (int16_t)self->myItemStack->itemDamage);
    }
}

void Packet103SetSlot_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20088_a(var1, (Packet103SetSlot *)p);
}

int Packet103SetSlot_getPacketSize(Packet *p) {
    (void)p;
    return 8;
}
