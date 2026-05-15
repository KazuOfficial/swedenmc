#include "Packet54PlayNoteBlock.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet54PlayNoteBlock_readPacketData,
    Packet54PlayNoteBlock_writePacketData,
    Packet54PlayNoteBlock_processPacket,
    Packet54PlayNoteBlock_getPacketSize,
};

Packet *Packet54PlayNoteBlock_create(void) {
    Packet54PlayNoteBlock *self = calloc(1, sizeof(Packet54PlayNoteBlock));
    self->base.vtable = &vtable;
    self->base.packetId = 54;
    return (Packet *)self;
}

void Packet54PlayNoteBlock_readPacketData(Packet *p, DataStream *var1) {
    Packet54PlayNoteBlock *self = (Packet54PlayNoteBlock *)p;
    self->xLocation = DataStream_readInt(var1);

    self->yLocation = (int)DataStream_readShort(var1);
    self->zLocation = DataStream_readInt(var1);
    self->instrumentType = (int)DataStream_readByte(var1);
    self->pitch = (int)DataStream_readByte(var1);
}

void Packet54PlayNoteBlock_writePacketData(Packet *p, DataStream *var1) {
    Packet54PlayNoteBlock *self = (Packet54PlayNoteBlock *)p;
    DataStream_writeInt(var1, self->xLocation);
    DataStream_writeShort(var1, (int16_t)self->yLocation);
    DataStream_writeInt(var1, self->zLocation);
    DataStream_writeByte(var1, (uint8_t)self->instrumentType);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
}

void Packet54PlayNoteBlock_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleNotePlay(var1, (Packet54PlayNoteBlock *)p);
}

int Packet54PlayNoteBlock_getPacketSize(Packet *p) {
    (void)p;
    return 12;
}
