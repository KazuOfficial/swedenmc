#ifndef PACKET103_SET_SLOT_H
#define PACKET103_SET_SLOT_H

#include "Packet.h"
#include "ItemStack.h"

struct Packet103SetSlot {
    Packet base;
    int windowId;
    int itemSlot;
    ItemStack *myItemStack;
};
typedef struct Packet103SetSlot Packet103SetSlot;

Packet *Packet103SetSlot_create(void);
void Packet103SetSlot_readPacketData(Packet *self, DataStream *var1);
void Packet103SetSlot_writePacketData(Packet *self, DataStream *var1);
void Packet103SetSlot_processPacket(Packet *self, NetHandler *var1);
int Packet103SetSlot_getPacketSize(Packet *self);

#endif
