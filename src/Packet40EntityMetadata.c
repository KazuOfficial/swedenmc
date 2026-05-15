#include "Packet40EntityMetadata.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet40EntityMetadata_readPacketData,
    Packet40EntityMetadata_writePacketData,
    Packet40EntityMetadata_processPacket,
    Packet40EntityMetadata_getPacketSize,
};

Packet *Packet40EntityMetadata_create(void) {
    Packet40EntityMetadata *self = calloc(1, sizeof(Packet40EntityMetadata));
    self->base.vtable = &vtable;
    self->base.packetId = 40;
    DataWatcher_init(&self->field_21048_b);
    return (Packet *)self;
}

void Packet40EntityMetadata_readPacketData(Packet *p, DataStream *var1) {
    Packet40EntityMetadata *self = (Packet40EntityMetadata *)p;
    self->entityId = DataStream_readInt(var1);

    DataWatcher_readFromStream(&self->field_21048_b, var1);
}

void Packet40EntityMetadata_writePacketData(Packet *p, DataStream *var1) {
    Packet40EntityMetadata *self = (Packet40EntityMetadata *)p;
    DataStream_writeInt(var1, self->entityId);

    DataWatcher_writeToStream(&self->field_21048_b, var1);
}

void Packet40EntityMetadata_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_21148_a(var1, (Packet40EntityMetadata *)p);
}

int Packet40EntityMetadata_getPacketSize(Packet *p) {
    (void)p;
    return 5;
}

DataWatcher *Packet40EntityMetadata_func_21047_b(Packet40EntityMetadata *self) {
    return &self->field_21048_b;
}
