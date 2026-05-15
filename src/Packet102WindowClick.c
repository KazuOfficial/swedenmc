#include "Packet102WindowClick.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet102WindowClick_readPacketData,
    Packet102WindowClick_writePacketData,
    Packet102WindowClick_processPacket,
    Packet102WindowClick_getPacketSize,
};

Packet *Packet102WindowClick_create(void) {
    Packet102WindowClick *self = calloc(1, sizeof(Packet102WindowClick));
    self->base.vtable = &vtable;
    self->base.packetId = 102;
    return (Packet *)self;
}

Packet *Packet102WindowClick_createWith(int windowId, int slot, int mouseClick, int shiftHeld,
                                        ItemStack *stack, int16_t action) {
    Packet102WindowClick *self = (Packet102WindowClick *)Packet102WindowClick_create();
    self->window_Id = windowId;
    self->inventorySlot = slot;
    self->mouseClick = mouseClick;
    self->action = action;
    self->field_27050_f = shiftHeld;

    if (stack) {
        self->itemStack = calloc(1, sizeof(ItemStack));
        self->itemStack->itemID = stack->itemID;
        self->itemStack->stackSize = stack->stackSize;
        self->itemStack->itemDamage = stack->itemDamage;
    } else {
        self->itemStack = NULL;
    }
    return (Packet *)self;
}

void Packet102WindowClick_readPacketData(Packet *p, DataStream *var1) {
    Packet102WindowClick *self = (Packet102WindowClick *)p;
    self->window_Id = (int)(int8_t)DataStream_readByte(var1);
    self->inventorySlot = (int)DataStream_readShort(var1);
    self->mouseClick = (int)(int8_t)DataStream_readByte(var1);
    self->action = DataStream_readShort(var1);

    self->field_27050_f = DataStream_readByte(var1) != 0;
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

void Packet102WindowClick_writePacketData(Packet *p, DataStream *var1) {
    Packet102WindowClick *self = (Packet102WindowClick *)p;
    DataStream_writeByte(var1, (uint8_t)self->window_Id);
    DataStream_writeShort(var1, (int16_t)self->inventorySlot);
    DataStream_writeByte(var1, (uint8_t)self->mouseClick);
    DataStream_writeShort(var1, self->action);
    DataStream_writeByte(var1, (uint8_t)(self->field_27050_f ? 1 : 0));
    if (self->itemStack == NULL) {
        DataStream_writeShort(var1, (int16_t)-1);
    } else {
        DataStream_writeShort(var1, (int16_t)self->itemStack->itemID);
        DataStream_writeByte(var1, (uint8_t)self->itemStack->stackSize);
        DataStream_writeShort(var1, (int16_t)self->itemStack->itemDamage);
    }
}

void Packet102WindowClick_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20091_a(var1, (Packet102WindowClick *)p);
}

int Packet102WindowClick_getPacketSize(Packet *p) {
    (void)p;
    return 11;
}
