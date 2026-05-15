#include "Packet4UpdateTime.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet4UpdateTime_readPacketData,
    Packet4UpdateTime_writePacketData,
    Packet4UpdateTime_processPacket,
    Packet4UpdateTime_getPacketSize,
};

Packet *Packet4UpdateTime_create(void) {
    Packet4UpdateTime *self = calloc(1, sizeof(Packet4UpdateTime));
    self->base.vtable = &vtable;
    self->base.packetId = 4;
    return (Packet *)self;
}

void Packet4UpdateTime_readPacketData(Packet *p, DataStream *var1) {
    ((Packet4UpdateTime *)p)->time = DataStream_readLong(var1);
}

void Packet4UpdateTime_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeLong(var1, ((Packet4UpdateTime *)p)->time);
}

void Packet4UpdateTime_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleUpdateTime(var1, (Packet4UpdateTime *)p);
}

int Packet4UpdateTime_getPacketSize(Packet *p) {
    (void)p;
    return 8;
}
