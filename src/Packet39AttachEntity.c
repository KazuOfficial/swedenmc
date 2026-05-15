#include "Packet39AttachEntity.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet39AttachEntity_readPacketData,
    Packet39AttachEntity_writePacketData,
    Packet39AttachEntity_processPacket,
    Packet39AttachEntity_getPacketSize,
};

Packet *Packet39AttachEntity_create(void) {
    Packet39AttachEntity *self = calloc(1, sizeof(Packet39AttachEntity));
    self->base.vtable = &vtable;
    self->base.packetId = 39;
    return (Packet *)self;
}

void Packet39AttachEntity_readPacketData(Packet *p, DataStream *var1) {
    Packet39AttachEntity *self = (Packet39AttachEntity *)p;
    self->entityId = DataStream_readInt(var1);
    self->vehicleEntityId = DataStream_readInt(var1);
}

void Packet39AttachEntity_writePacketData(Packet *p, DataStream *var1) {
    Packet39AttachEntity *self = (Packet39AttachEntity *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeInt(var1, self->vehicleEntityId);
}

void Packet39AttachEntity_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_6497_a(var1, (Packet39AttachEntity *)p);
}

int Packet39AttachEntity_getPacketSize(Packet *p) {
    (void)p;
    return 8;
}
