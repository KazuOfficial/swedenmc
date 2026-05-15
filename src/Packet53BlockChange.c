#include "Packet53BlockChange.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet53BlockChange_readPacketData,
    Packet53BlockChange_writePacketData,
    Packet53BlockChange_processPacket,
    Packet53BlockChange_getPacketSize,
};

Packet *Packet53BlockChange_create(void) {
    Packet53BlockChange *self = calloc(1, sizeof(Packet53BlockChange));
    self->base.vtable = &vtable;
    self->base.packetId = 53;

    self->base.isChunkDataPacket = 1;
    return (Packet *)self;
}

void Packet53BlockChange_readPacketData(Packet *p, DataStream *var1) {
    Packet53BlockChange *self = (Packet53BlockChange *)p;
    self->xPosition = DataStream_readInt(var1);

    self->yPosition = (int)DataStream_readByte(var1);
    self->zPosition = DataStream_readInt(var1);
    self->type = (int)DataStream_readByte(var1);
    self->metadata = (int)DataStream_readByte(var1);
}

void Packet53BlockChange_writePacketData(Packet *p, DataStream *var1) {
    Packet53BlockChange *self = (Packet53BlockChange *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeByte(var1, (uint8_t)self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->type);
    DataStream_writeByte(var1, (uint8_t)self->metadata);
}

void Packet53BlockChange_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleBlockChange(var1, (Packet53BlockChange *)p);
}

int Packet53BlockChange_getPacketSize(Packet *p) {
    (void)p;
    return 11;
}
