#include "Packet38EntityStatus.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet38EntityStatus_readPacketData,
    Packet38EntityStatus_writePacketData,
    Packet38EntityStatus_processPacket,
    Packet38EntityStatus_getPacketSize,
};

Packet *Packet38EntityStatus_create(void) {
    Packet38EntityStatus *self = calloc(1, sizeof(Packet38EntityStatus));
    self->base.vtable = &vtable;
    self->base.packetId = 38;
    return (Packet *)self;
}

void Packet38EntityStatus_readPacketData(Packet *p, DataStream *var1) {
    Packet38EntityStatus *self = (Packet38EntityStatus *)p;
    self->entityId = DataStream_readInt(var1);
    self->entityStatus = (int8_t)DataStream_readByte(var1);
}

void Packet38EntityStatus_writePacketData(Packet *p, DataStream *var1) {
    Packet38EntityStatus *self = (Packet38EntityStatus *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeByte(var1, (uint8_t)self->entityStatus);
}

void Packet38EntityStatus_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_9447_a(var1, (Packet38EntityStatus *)p);
}

int Packet38EntityStatus_getPacketSize(Packet *p) {
    (void)p;
    return 5;
}
