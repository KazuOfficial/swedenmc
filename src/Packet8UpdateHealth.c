#include "Packet8UpdateHealth.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet8UpdateHealth_readPacketData,
    Packet8UpdateHealth_writePacketData,
    Packet8UpdateHealth_processPacket,
    Packet8UpdateHealth_getPacketSize,
};

Packet *Packet8UpdateHealth_create(void) {
    Packet8UpdateHealth *self = calloc(1, sizeof(Packet8UpdateHealth));
    self->base.vtable = &vtable;
    self->base.packetId = 8;
    return (Packet *)self;
}

void Packet8UpdateHealth_readPacketData(Packet *p, DataStream *var1) {

    ((Packet8UpdateHealth *)p)->healthMP = (int)DataStream_readShort(var1);
}

void Packet8UpdateHealth_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeShort(var1, (int16_t)((Packet8UpdateHealth *)p)->healthMP);
}

void Packet8UpdateHealth_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleHealth(var1, (Packet8UpdateHealth *)p);
}

int Packet8UpdateHealth_getPacketSize(Packet *p) {
    (void)p;
    return 2;
}
