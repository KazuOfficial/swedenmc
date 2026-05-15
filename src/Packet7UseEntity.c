#include "Packet7UseEntity.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet7UseEntity_readPacketData,
    Packet7UseEntity_writePacketData,
    Packet7UseEntity_processPacket,
    Packet7UseEntity_getPacketSize,
};

Packet *Packet7UseEntity_create(void) {
    Packet7UseEntity *self = calloc(1, sizeof(Packet7UseEntity));
    self->base.vtable = &vtable;
    self->base.packetId = 7;
    return (Packet *)self;
}

Packet *Packet7UseEntity_createWith(int playerEntityId, int targetEntity, int isLeftClick) {
    Packet7UseEntity *self = (Packet7UseEntity *)Packet7UseEntity_create();
    self->playerEntityId = playerEntityId;
    self->targetEntity = targetEntity;
    self->isLeftClick = isLeftClick;
    return (Packet *)self;
}

void Packet7UseEntity_readPacketData(Packet *p, DataStream *var1) {
    Packet7UseEntity *self = (Packet7UseEntity *)p;
    self->playerEntityId = DataStream_readInt(var1);
    self->targetEntity = DataStream_readInt(var1);

    self->isLeftClick = (int)(int8_t)DataStream_readByte(var1);
}

void Packet7UseEntity_writePacketData(Packet *p, DataStream *var1) {
    Packet7UseEntity *self = (Packet7UseEntity *)p;
    DataStream_writeInt(var1, self->playerEntityId);
    DataStream_writeInt(var1, self->targetEntity);
    DataStream_writeByte(var1, (uint8_t)self->isLeftClick);
}

void Packet7UseEntity_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleUseEntity(var1, (Packet7UseEntity *)p);
}

int Packet7UseEntity_getPacketSize(Packet *p) {
    (void)p;
    return 9;
}
