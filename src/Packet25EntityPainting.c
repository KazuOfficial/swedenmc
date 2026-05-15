#include "Packet25EntityPainting.h"
#include "NetHandler.h"
#include "EnumArt.h"
#include "EntityPainting.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet25EntityPainting_readPacketData,
    Packet25EntityPainting_writePacketData,
    Packet25EntityPainting_processPacket,
    Packet25EntityPainting_getPacketSize,
};

Packet *Packet25EntityPainting_create(void) {
    Packet25EntityPainting *self = calloc(1, sizeof(Packet25EntityPainting));
    self->base.vtable = &vtable;
    self->base.packetId = 25;
    return (Packet *)self;
}

Packet *Packet25EntityPainting_createWith(EntityPainting *var1) {
    Packet25EntityPainting *self = (Packet25EntityPainting *)Packet25EntityPainting_create();
    self->entityId = var1->base.entityId;
    self->xPosition = var1->xPosition;
    self->yPosition = var1->yPosition;
    self->zPosition = var1->zPosition;
    self->direction = var1->direction;
    self->title = strdup(var1->art->title);
    return (Packet *)self;
}

void Packet25EntityPainting_readPacketData(Packet *p, DataStream *var1) {
    Packet25EntityPainting *self = (Packet25EntityPainting *)p;
    self->entityId = DataStream_readInt(var1);

    self->title = Packet_readString(var1, ENUM_ART_MAX_TITLE_LENGTH);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
    self->direction = DataStream_readInt(var1);
}

void Packet25EntityPainting_writePacketData(Packet *p, DataStream *var1) {
    Packet25EntityPainting *self = (Packet25EntityPainting *)p;
    DataStream_writeInt(var1, self->entityId);
    Packet_writeString(self->title, var1);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeInt(var1, self->direction);
}

void Packet25EntityPainting_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_21146_a(var1, (Packet25EntityPainting *)p);
}

int Packet25EntityPainting_getPacketSize(Packet *p) {
    (void)p;
    return 24;
}
