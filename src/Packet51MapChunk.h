#ifndef PACKET51_MAP_CHUNK_H
#define PACKET51_MAP_CHUNK_H

#include "Packet.h"
#include <stdint.h>

struct Packet51MapChunk {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;
    int xSize;
    int ySize;
    int zSize;

    uint8_t *chunk;

    int chunkSize;
};
typedef struct Packet51MapChunk Packet51MapChunk;

Packet *Packet51MapChunk_create(void);
void Packet51MapChunk_readPacketData(Packet *self, DataStream *var1);
void Packet51MapChunk_writePacketData(Packet *self, DataStream *var1);
void Packet51MapChunk_processPacket(Packet *self, NetHandler *var1);
int Packet51MapChunk_getPacketSize(Packet *self);

#endif
