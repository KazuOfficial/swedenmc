#ifndef PACKET52_MULTI_BLOCK_CHANGE_H
#define PACKET52_MULTI_BLOCK_CHANGE_H

#include "Packet.h"
#include <stdint.h>

struct Packet52MultiBlockChange {
    Packet base;
    int xPosition;
    int zPosition;

    int16_t *coordinateArray;

    uint8_t *typeArray;

    uint8_t *metadataArray;
    int size;
};
typedef struct Packet52MultiBlockChange Packet52MultiBlockChange;

Packet *Packet52MultiBlockChange_create(void);
void Packet52MultiBlockChange_readPacketData(Packet *self, DataStream *var1);
void Packet52MultiBlockChange_writePacketData(Packet *self, DataStream *var1);
void Packet52MultiBlockChange_processPacket(Packet *self, NetHandler *var1);
int Packet52MultiBlockChange_getPacketSize(Packet *self);

#endif
