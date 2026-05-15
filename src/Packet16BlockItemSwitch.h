#ifndef PACKET16_BLOCK_ITEM_SWITCH_H
#define PACKET16_BLOCK_ITEM_SWITCH_H

#include "Packet.h"

struct Packet16BlockItemSwitch {
    Packet base;

    int id;
};
typedef struct Packet16BlockItemSwitch Packet16BlockItemSwitch;

Packet *Packet16BlockItemSwitch_create(void);
Packet *Packet16BlockItemSwitch_createWith(int id);
void Packet16BlockItemSwitch_readPacketData(Packet *self, DataStream *var1);
void Packet16BlockItemSwitch_writePacketData(Packet *self, DataStream *var1);
void Packet16BlockItemSwitch_processPacket(Packet *self, NetHandler *var1);
int Packet16BlockItemSwitch_getPacketSize(Packet *self);

#endif
