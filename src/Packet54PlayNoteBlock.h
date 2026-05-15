#ifndef PACKET54_PLAY_NOTE_BLOCK_H
#define PACKET54_PLAY_NOTE_BLOCK_H

#include "Packet.h"

struct Packet54PlayNoteBlock {
    Packet base;
    int xLocation;
    int yLocation;
    int zLocation;
    int instrumentType;
    int pitch;
};
typedef struct Packet54PlayNoteBlock Packet54PlayNoteBlock;

Packet *Packet54PlayNoteBlock_create(void);
void Packet54PlayNoteBlock_readPacketData(Packet *self, DataStream *var1);
void Packet54PlayNoteBlock_writePacketData(Packet *self, DataStream *var1);
void Packet54PlayNoteBlock_processPacket(Packet *self, NetHandler *var1);
int Packet54PlayNoteBlock_getPacketSize(Packet *self);

#endif
