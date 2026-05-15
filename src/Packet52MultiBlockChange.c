#include "Packet52MultiBlockChange.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet52MultiBlockChange_readPacketData,
    Packet52MultiBlockChange_writePacketData,
    Packet52MultiBlockChange_processPacket,
    Packet52MultiBlockChange_getPacketSize,
};

Packet *Packet52MultiBlockChange_create(void) {
    Packet52MultiBlockChange *self = calloc(1, sizeof(Packet52MultiBlockChange));
    self->base.vtable = &vtable;
    self->base.packetId = 52;

    self->base.isChunkDataPacket = 1;
    return (Packet *)self;
}

void Packet52MultiBlockChange_readPacketData(Packet *p, DataStream *var1) {
    Packet52MultiBlockChange *self = (Packet52MultiBlockChange *)p;
    self->xPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);

    self->size = (int)((uint16_t)DataStream_readShort(var1));
    self->coordinateArray = malloc((size_t)self->size * sizeof(int16_t));
    self->typeArray = malloc((size_t)self->size);
    self->metadataArray = malloc((size_t)self->size);
    for (int var2 = 0; var2 < self->size; ++var2) {
        self->coordinateArray[var2] = DataStream_readShort(var1);
    }
    DataStream_readBytes(var1, self->typeArray, (size_t)self->size);
    DataStream_readBytes(var1, self->metadataArray, (size_t)self->size);
}

void Packet52MultiBlockChange_writePacketData(Packet *p, DataStream *var1) {
    Packet52MultiBlockChange *self = (Packet52MultiBlockChange *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeShort(var1, (int16_t)self->size);
    for (int var2 = 0; var2 < self->size; ++var2) {
        DataStream_writeShort(var1, self->coordinateArray[var2]);
    }
    DataStream_writeBytes(var1, self->typeArray, (size_t)self->size);
    DataStream_writeBytes(var1, self->metadataArray, (size_t)self->size);
}

void Packet52MultiBlockChange_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleMultiBlockChange(var1, (Packet52MultiBlockChange *)p);
}

int Packet52MultiBlockChange_getPacketSize(Packet *p) {
    return 10 + ((Packet52MultiBlockChange *)p)->size * 4;
}
