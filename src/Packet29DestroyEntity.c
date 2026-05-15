#include "Packet29DestroyEntity.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet29DestroyEntity_readPacketData,
    Packet29DestroyEntity_writePacketData,
    Packet29DestroyEntity_processPacket,
    Packet29DestroyEntity_getPacketSize,
};

Packet *Packet29DestroyEntity_create(void) {
    Packet29DestroyEntity *self = calloc(1, sizeof(Packet29DestroyEntity));
    self->base.vtable = &vtable;
    self->base.packetId = 29;
    return (Packet *)self;
}

void Packet29DestroyEntity_readPacketData(Packet *p, DataStream *var1) {
    ((Packet29DestroyEntity *)p)->entityId = DataStream_readInt(var1);
}

void Packet29DestroyEntity_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeInt(var1, ((Packet29DestroyEntity *)p)->entityId);
}

void Packet29DestroyEntity_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleDestroyEntity(var1, (Packet29DestroyEntity *)p);
}

int Packet29DestroyEntity_getPacketSize(Packet *p) {
    (void)p;
    return 4;
}
