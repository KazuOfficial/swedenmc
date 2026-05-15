#include "Packet14BlockDig.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet14BlockDig_readPacketData,
    Packet14BlockDig_writePacketData,
    Packet14BlockDig_processPacket,
    Packet14BlockDig_getPacketSize,
};

Packet *Packet14BlockDig_create(void) {
    Packet14BlockDig *self = calloc(1, sizeof(Packet14BlockDig));
    self->base.vtable = &vtable;
    self->base.packetId = 14;
    return (Packet *)self;
}

Packet *Packet14BlockDig_createWith(int status, int x, int y, int z, int face) {
    Packet14BlockDig *self = (Packet14BlockDig *)Packet14BlockDig_create();
    self->status = status;
    self->xPosition = x;
    self->yPosition = y;
    self->zPosition = z;
    self->face = face;
    return (Packet *)self;
}

void Packet14BlockDig_readPacketData(Packet *p, DataStream *var1) {
    Packet14BlockDig *self = (Packet14BlockDig *)p;

    self->status = (int)DataStream_readByte(var1);
    self->xPosition = DataStream_readInt(var1);

    self->yPosition = (int)DataStream_readByte(var1);
    self->zPosition = DataStream_readInt(var1);
    self->face = (int)DataStream_readByte(var1);
}

void Packet14BlockDig_writePacketData(Packet *p, DataStream *var1) {
    Packet14BlockDig *self = (Packet14BlockDig *)p;
    DataStream_writeByte(var1, (uint8_t)self->status);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeByte(var1, (uint8_t)self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->face);
}

void Packet14BlockDig_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleBlockDig(var1, (Packet14BlockDig *)p);
}

int Packet14BlockDig_getPacketSize(Packet *p) {
    (void)p;
    return 11;
}
