#include "Packet6SpawnPosition.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet6SpawnPosition_readPacketData,
    Packet6SpawnPosition_writePacketData,
    Packet6SpawnPosition_processPacket,
    Packet6SpawnPosition_getPacketSize,
};

Packet *Packet6SpawnPosition_create(void) {
    Packet6SpawnPosition *self = calloc(1, sizeof(Packet6SpawnPosition));
    self->base.vtable = &vtable;
    self->base.packetId = 6;
    return (Packet *)self;
}

void Packet6SpawnPosition_readPacketData(Packet *p, DataStream *var1) {
    Packet6SpawnPosition *self = (Packet6SpawnPosition *)p;
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
}

void Packet6SpawnPosition_writePacketData(Packet *p, DataStream *var1) {
    Packet6SpawnPosition *self = (Packet6SpawnPosition *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
}

void Packet6SpawnPosition_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleSpawnPosition(var1, (Packet6SpawnPosition *)p);
}

int Packet6SpawnPosition_getPacketSize(Packet *p) {
    (void)p;
    return 12;
}
