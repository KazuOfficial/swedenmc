#ifndef PACKET15_PLACE_H
#define PACKET15_PLACE_H

#include "Packet.h"
#include "ItemStack.h"

struct Packet15Place {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;
    int direction;

    ItemStack *itemStack;
};
typedef struct Packet15Place Packet15Place;

Packet *Packet15Place_create(void);
Packet *Packet15Place_createWith(int x, int y, int z, int direction, ItemStack *itemStack);
void Packet15Place_readPacketData(Packet *self, DataStream *var1);
void Packet15Place_writePacketData(Packet *self, DataStream *var1);
void Packet15Place_processPacket(Packet *self, NetHandler *var1);
int Packet15Place_getPacketSize(Packet *self);

#endif
