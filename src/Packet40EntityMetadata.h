#ifndef PACKET40_ENTITY_METADATA_H
#define PACKET40_ENTITY_METADATA_H

#include "Packet.h"
#include "DataWatcher.h"

struct Packet40EntityMetadata {
    Packet base;
    int entityId;

    DataWatcher field_21048_b;
};
typedef struct Packet40EntityMetadata Packet40EntityMetadata;

Packet *Packet40EntityMetadata_create(void);
void Packet40EntityMetadata_readPacketData(Packet *self, DataStream *var1);
void Packet40EntityMetadata_writePacketData(Packet *self, DataStream *var1);
void Packet40EntityMetadata_processPacket(Packet *self, NetHandler *var1);
int Packet40EntityMetadata_getPacketSize(Packet *self);

DataWatcher *Packet40EntityMetadata_func_21047_b(Packet40EntityMetadata *self);

#endif
