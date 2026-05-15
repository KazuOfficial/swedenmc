#include "Packet105UpdateProgressbar.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet105UpdateProgressbar_readPacketData,
    Packet105UpdateProgressbar_writePacketData,
    Packet105UpdateProgressbar_processPacket,
    Packet105UpdateProgressbar_getPacketSize,
};

Packet *Packet105UpdateProgressbar_create(void) {
    Packet105UpdateProgressbar *self = calloc(1, sizeof(Packet105UpdateProgressbar));
    self->base.vtable = &vtable;
    self->base.packetId = 105;
    return (Packet *)self;
}

void Packet105UpdateProgressbar_readPacketData(Packet *p, DataStream *var1) {
    Packet105UpdateProgressbar *self = (Packet105UpdateProgressbar *)p;
    self->windowId = (int)(int8_t)DataStream_readByte(var1);
    self->progressBar = (int)DataStream_readShort(var1);
    self->progressBarValue = (int)DataStream_readShort(var1);
}

void Packet105UpdateProgressbar_writePacketData(Packet *p, DataStream *var1) {
    Packet105UpdateProgressbar *self = (Packet105UpdateProgressbar *)p;
    DataStream_writeByte(var1, (uint8_t)self->windowId);
    DataStream_writeShort(var1, (int16_t)self->progressBar);
    DataStream_writeShort(var1, (int16_t)self->progressBarValue);
}

void Packet105UpdateProgressbar_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20090_a(var1, (Packet105UpdateProgressbar *)p);
}

int Packet105UpdateProgressbar_getPacketSize(Packet *p) {
    (void)p;
    return 5;
}
