#ifndef PACKET5_PLAYER_INVENTORY_H
#define PACKET5_PLAYER_INVENTORY_H

#include "Packet.h"

struct Packet5PlayerInventory {
    Packet base;

    int entityID;

    int slot;

    int itemID;

    int itemDamage;
};
typedef struct Packet5PlayerInventory Packet5PlayerInventory;

Packet *Packet5PlayerInventory_create(void);
void Packet5PlayerInventory_readPacketData(Packet *self, DataStream *var1);
void Packet5PlayerInventory_writePacketData(Packet *self, DataStream *var1);
void Packet5PlayerInventory_processPacket(Packet *self, NetHandler *var1);
int Packet5PlayerInventory_getPacketSize(Packet *self);

#endif
