#include "Packet18Animation.h"
#include "NetHandler.h"
#include "Entity.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet18Animation_readPacketData,
    Packet18Animation_writePacketData,
    Packet18Animation_processPacket,
    Packet18Animation_getPacketSize,
};

Packet *Packet18Animation_create(void) {
    Packet18Animation *self = calloc(1, sizeof(Packet18Animation));
    self->base.vtable = &vtable;
    self->base.packetId = 18;
    return (Packet *)self;
}

Packet *Packet18Animation_createWith(Entity *var1, int var2) {
    Packet18Animation *self = (Packet18Animation *)Packet18Animation_create();
    self->entityId = var1->entityId;
    self->animate = var2;
    return (Packet *)self;
}

void Packet18Animation_readPacketData(Packet *p, DataStream *var1) {
    Packet18Animation *self = (Packet18Animation *)p;
    self->entityId = DataStream_readInt(var1);
    self->animate = (int)(int8_t)DataStream_readByte(var1);
}

void Packet18Animation_writePacketData(Packet *p, DataStream *var1) {
    Packet18Animation *self = (Packet18Animation *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeByte(var1, (uint8_t)self->animate);
}

void Packet18Animation_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleArmAnimation(var1, (Packet18Animation *)p);
}

int Packet18Animation_getPacketSize(Packet *p) {
    (void)p;
    return 5;
}
