#include "Packet22Collect.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet22Collect_readPacketData,
    Packet22Collect_writePacketData,
    Packet22Collect_processPacket,
    Packet22Collect_getPacketSize,
};

Packet *Packet22Collect_create(void) {
    Packet22Collect *self = calloc(1, sizeof(Packet22Collect));
    self->base.vtable = &vtable;
    self->base.packetId = 22;
    return (Packet *)self;
}

void Packet22Collect_readPacketData(Packet *p, DataStream *var1) {
    Packet22Collect *self = (Packet22Collect *)p;
    self->collectedEntityId = DataStream_readInt(var1);
    self->collectorEntityId = DataStream_readInt(var1);
}

void Packet22Collect_writePacketData(Packet *p, DataStream *var1) {
    Packet22Collect *self = (Packet22Collect *)p;
    DataStream_writeInt(var1, self->collectedEntityId);
    DataStream_writeInt(var1, self->collectorEntityId);
}

void Packet22Collect_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleCollect(var1, (Packet22Collect *)p);
}

int Packet22Collect_getPacketSize(Packet *p) {
    (void)p;
    return 8;
}
