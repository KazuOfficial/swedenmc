#ifndef PACKET102_WINDOW_CLICK_H
#define PACKET102_WINDOW_CLICK_H

#include "Packet.h"
#include "ItemStack.h"
#include <stdint.h>

struct Packet102WindowClick {
    Packet base;
    int window_Id;
    int inventorySlot;
    int mouseClick;
    int16_t action;
    ItemStack *itemStack;

    int field_27050_f;
};
typedef struct Packet102WindowClick Packet102WindowClick;

Packet *Packet102WindowClick_create(void);

Packet *Packet102WindowClick_createWith(int windowId, int slot, int mouseClick, int shiftHeld,
                                        ItemStack *stack, int16_t action);
void Packet102WindowClick_readPacketData(Packet *self, DataStream *var1);
void Packet102WindowClick_writePacketData(Packet *self, DataStream *var1);
void Packet102WindowClick_processPacket(Packet *self, NetHandler *var1);
int Packet102WindowClick_getPacketSize(Packet *self);

#endif
