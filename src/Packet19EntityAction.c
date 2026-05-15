#include "Packet19EntityAction.h"
#include "NetHandler.h"
#include "Entity.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet19EntityAction_readPacketData,
    Packet19EntityAction_writePacketData,
    Packet19EntityAction_processPacket,
    Packet19EntityAction_getPacketSize,
};

Packet *Packet19EntityAction_create(void) {
    Packet19EntityAction *self = calloc(1, sizeof(Packet19EntityAction));
    self->base.vtable = &vtable;
    self->base.packetId = 19;
    return (Packet *)self;
}

Packet *Packet19EntityAction_createWith(Entity *var1, int var2) {
    Packet19EntityAction *self = (Packet19EntityAction *)Packet19EntityAction_create();
    self->entityId = var1->entityId;
    self->state = var2;
    return (Packet *)self;
}

void Packet19EntityAction_readPacketData(Packet *p, DataStream *var1) {
    Packet19EntityAction *self = (Packet19EntityAction *)p;
    self->entityId = DataStream_readInt(var1);
    self->state = (int)(int8_t)DataStream_readByte(var1);
}

void Packet19EntityAction_writePacketData(Packet *p, DataStream *var1) {
    Packet19EntityAction *self = (Packet19EntityAction *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeByte(var1, (uint8_t)self->state);
}

void Packet19EntityAction_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_21147_a(var1, (Packet19EntityAction *)p);
}

int Packet19EntityAction_getPacketSize(Packet *p) {
    (void)p;
    return 5;
}
