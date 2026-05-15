#ifndef PACKET53_BLOCK_CHANGE_H
#define PACKET53_BLOCK_CHANGE_H

#include "Packet.h"

struct Packet53BlockChange {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;
    int type;
    int metadata;
};
typedef struct Packet53BlockChange Packet53BlockChange;

Packet *Packet53BlockChange_create(void);
void Packet53BlockChange_readPacketData(Packet *self, DataStream *var1);
void Packet53BlockChange_writePacketData(Packet *self, DataStream *var1);
void Packet53BlockChange_processPacket(Packet *self, NetHandler *var1);
int Packet53BlockChange_getPacketSize(Packet *self);

#endif
