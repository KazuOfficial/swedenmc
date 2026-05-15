#ifndef PACKET60_EXPLOSION_H
#define PACKET60_EXPLOSION_H

#include "Packet.h"
#include "ChunkPosition.h"
#include <stddef.h>

struct Packet60Explosion {
    Packet base;
    double explosionX;
    double explosionY;
    double explosionZ;
    float explosionSize;

    ChunkPosition *destroyedBlocks;
    int destroyedBlockCount;
};
typedef struct Packet60Explosion Packet60Explosion;

Packet *Packet60Explosion_create(void);
void Packet60Explosion_readPacketData(Packet *self, DataStream *var1);
void Packet60Explosion_writePacketData(Packet *self, DataStream *var1);
void Packet60Explosion_processPacket(Packet *self, NetHandler *var1);
int Packet60Explosion_getPacketSize(Packet *self);

#endif
