#include "Packet50PreChunk.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet50PreChunk_readPacketData,
    Packet50PreChunk_writePacketData,
    Packet50PreChunk_processPacket,
    Packet50PreChunk_getPacketSize,
};

Packet *Packet50PreChunk_create(void) {
    Packet50PreChunk *self = calloc(1, sizeof(Packet50PreChunk));
    self->base.vtable = &vtable;
    self->base.packetId = 50;

    self->base.isChunkDataPacket = 0;
    return (Packet *)self;
}

void Packet50PreChunk_readPacketData(Packet *p, DataStream *var1) {
    Packet50PreChunk *self = (Packet50PreChunk *)p;
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);

    self->mode = (int)DataStream_readByte(var1) != 0;
}

void Packet50PreChunk_writePacketData(Packet *p, DataStream *var1) {
    Packet50PreChunk *self = (Packet50PreChunk *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeByte(var1, (uint8_t)(self->mode ? 1 : 0));
}

void Packet50PreChunk_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handlePreChunk(var1, (Packet50PreChunk *)p);
}

int Packet50PreChunk_getPacketSize(Packet *p) {
    (void)p;
    return 9;
}
