#include "Packet15Place.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet15Place_readPacketData,
    Packet15Place_writePacketData,
    Packet15Place_processPacket,
    Packet15Place_getPacketSize,
};

Packet *Packet15Place_create(void) {
    Packet15Place *self = calloc(1, sizeof(Packet15Place));
    self->base.vtable = &vtable;
    self->base.packetId = 15;
    return (Packet *)self;
}

Packet *Packet15Place_createWith(int x, int y, int z, int direction, ItemStack *itemStack) {
    Packet15Place *self = (Packet15Place *)Packet15Place_create();
    self->xPosition = x;
    self->yPosition = y;
    self->zPosition = z;
    self->direction = direction;
    if (itemStack) {
        self->itemStack = calloc(1, sizeof(ItemStack));
        self->itemStack->itemID = itemStack->itemID;
        self->itemStack->stackSize = itemStack->stackSize;
        self->itemStack->itemDamage = itemStack->itemDamage;
    } else {
        self->itemStack = NULL;
    }
    return (Packet *)self;
}

void Packet15Place_readPacketData(Packet *p, DataStream *var1) {
    Packet15Place *self = (Packet15Place *)p;
    self->xPosition = DataStream_readInt(var1);

    self->yPosition = (int)DataStream_readByte(var1);
    self->zPosition = DataStream_readInt(var1);
    self->direction = (int)DataStream_readByte(var1);

    int16_t var2 = DataStream_readShort(var1);
    if (var2 >= 0) {
        int8_t var3 = (int8_t)DataStream_readByte(var1);
        int16_t var4 = DataStream_readShort(var1);
        self->itemStack = calloc(1, sizeof(ItemStack));
        self->itemStack->itemID = (int)var2;
        self->itemStack->stackSize = (int)var3;
        self->itemStack->itemDamage = (int)var4;
    } else {
        self->itemStack = NULL;
    }
}

void Packet15Place_writePacketData(Packet *p, DataStream *var1) {
    Packet15Place *self = (Packet15Place *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeByte(var1, (uint8_t)self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->direction);
    if (self->itemStack == NULL) {
        DataStream_writeShort(var1, (int16_t)-1);
    } else {
        DataStream_writeShort(var1, (int16_t)self->itemStack->itemID);
        DataStream_writeByte(var1, (uint8_t)self->itemStack->stackSize);
        DataStream_writeShort(var1, (int16_t)self->itemStack->itemDamage);
    }
}

void Packet15Place_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handlePlace(var1, (Packet15Place *)p);
}

int Packet15Place_getPacketSize(Packet *p) {
    (void)p;
    return 15;
}
