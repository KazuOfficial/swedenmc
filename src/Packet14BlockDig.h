#ifndef PACKET14_BLOCK_DIG_H
#define PACKET14_BLOCK_DIG_H

#include "Packet.h"

struct Packet14BlockDig {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;
    int face;
    int status;
};
typedef struct Packet14BlockDig Packet14BlockDig;

Packet *Packet14BlockDig_create(void);
Packet *Packet14BlockDig_createWith(int status, int x, int y, int z, int face);
void Packet14BlockDig_readPacketData(Packet *self, DataStream *var1);
void Packet14BlockDig_writePacketData(Packet *self, DataStream *var1);
void Packet14BlockDig_processPacket(Packet *self, NetHandler *var1);
int Packet14BlockDig_getPacketSize(Packet *self);

#endif
