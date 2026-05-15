#ifndef PACKET50_PRE_CHUNK_H
#define PACKET50_PRE_CHUNK_H

#include "Packet.h"

struct Packet50PreChunk {
    Packet base;
    int xPosition;
    int yPosition;

    int mode;
};
typedef struct Packet50PreChunk Packet50PreChunk;

Packet *Packet50PreChunk_create(void);
void Packet50PreChunk_readPacketData(Packet *self, DataStream *var1);
void Packet50PreChunk_writePacketData(Packet *self, DataStream *var1);
void Packet50PreChunk_processPacket(Packet *self, NetHandler *var1);
int Packet50PreChunk_getPacketSize(Packet *self);

#endif
