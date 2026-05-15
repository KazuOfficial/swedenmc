#ifndef PACKET104_WINDOW_ITEMS_H
#define PACKET104_WINDOW_ITEMS_H

#include "Packet.h"
#include "ItemStack.h"

struct Packet104WindowItems {
    Packet base;
    int windowId;

    ItemStack **itemStack;
    int itemStackLen;
};
typedef struct Packet104WindowItems Packet104WindowItems;

Packet *Packet104WindowItems_create(void);
void Packet104WindowItems_readPacketData(Packet *self, DataStream *var1);
void Packet104WindowItems_writePacketData(Packet *self, DataStream *var1);
void Packet104WindowItems_processPacket(Packet *self, NetHandler *var1);
int Packet104WindowItems_getPacketSize(Packet *self);

#endif
