#include "Packet104WindowItems.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet104WindowItems_readPacketData,
    Packet104WindowItems_writePacketData,
    Packet104WindowItems_processPacket,
    Packet104WindowItems_getPacketSize,
};

Packet *Packet104WindowItems_create(void) {
    Packet104WindowItems *self = calloc(1, sizeof(Packet104WindowItems));
    self->base.vtable = &vtable;
    self->base.packetId = 104;
    return (Packet *)self;
}

void Packet104WindowItems_readPacketData(Packet *p, DataStream *var1) {
    Packet104WindowItems *self = (Packet104WindowItems *)p;
    self->windowId = (int)(int8_t)DataStream_readByte(var1);
    int16_t var2 = DataStream_readShort(var1);
    self->itemStackLen = (int)var2;

    self->itemStack = calloc((size_t)var2, sizeof(ItemStack *));
    for (int var3 = 0; var3 < var2; ++var3) {
        int16_t var4 = DataStream_readShort(var1);
        if (var4 >= 0) {
            int8_t var5 = (int8_t)DataStream_readByte(var1);
            int16_t var6 = DataStream_readShort(var1);
            self->itemStack[var3] = calloc(1, sizeof(ItemStack));
            self->itemStack[var3]->itemID = (int)var4;
            self->itemStack[var3]->stackSize = (int)var5;
            self->itemStack[var3]->itemDamage = (int)var6;
        }
    }
}

void Packet104WindowItems_writePacketData(Packet *p, DataStream *var1) {
    Packet104WindowItems *self = (Packet104WindowItems *)p;
    DataStream_writeByte(var1, (uint8_t)self->windowId);
    DataStream_writeShort(var1, (int16_t)self->itemStackLen);
    for (int var2 = 0; var2 < self->itemStackLen; ++var2) {
        if (self->itemStack[var2] == NULL) {
            DataStream_writeShort(var1, (int16_t)-1);
        } else {
            DataStream_writeShort(var1, (int16_t)self->itemStack[var2]->itemID);
            DataStream_writeByte(var1, (uint8_t)self->itemStack[var2]->stackSize);
            DataStream_writeShort(var1, (int16_t)self->itemStack[var2]->itemDamage);
        }
    }
}

void Packet104WindowItems_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20094_a(var1, (Packet104WindowItems *)p);
}

int Packet104WindowItems_getPacketSize(Packet *p) {
    return 3 + ((Packet104WindowItems *)p)->itemStackLen * 5;
}
