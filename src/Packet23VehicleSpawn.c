#include "Packet23VehicleSpawn.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet23VehicleSpawn_readPacketData,
    Packet23VehicleSpawn_writePacketData,
    Packet23VehicleSpawn_processPacket,
    Packet23VehicleSpawn_getPacketSize,
};

Packet *Packet23VehicleSpawn_create(void) {
    Packet23VehicleSpawn *self = calloc(1, sizeof(Packet23VehicleSpawn));
    self->base.vtable = &vtable;
    self->base.packetId = 23;
    return (Packet *)self;
}

void Packet23VehicleSpawn_readPacketData(Packet *p, DataStream *var1) {
    Packet23VehicleSpawn *self = (Packet23VehicleSpawn *)p;
    self->entityId = DataStream_readInt(var1);
    self->type = (int)(int8_t)DataStream_readByte(var1);
    self->xPosition = DataStream_readInt(var1);
    self->yPosition = DataStream_readInt(var1);
    self->zPosition = DataStream_readInt(var1);
    self->field_28044_i = DataStream_readInt(var1);
    if (self->field_28044_i > 0) {
        self->field_28047_e = (int)DataStream_readShort(var1);
        self->field_28046_f = (int)DataStream_readShort(var1);
        self->field_28045_g = (int)DataStream_readShort(var1);
    }
}

void Packet23VehicleSpawn_writePacketData(Packet *p, DataStream *var1) {
    Packet23VehicleSpawn *self = (Packet23VehicleSpawn *)p;
    DataStream_writeInt(var1, self->entityId);
    DataStream_writeByte(var1, (uint8_t)self->type);
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeInt(var1, self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeInt(var1, self->field_28044_i);
    if (self->field_28044_i > 0) {
        DataStream_writeShort(var1, (int16_t)self->field_28047_e);
        DataStream_writeShort(var1, (int16_t)self->field_28046_f);
        DataStream_writeShort(var1, (int16_t)self->field_28045_g);
    }
}

void Packet23VehicleSpawn_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleVehicleSpawn(var1, (Packet23VehicleSpawn *)p);
}

int Packet23VehicleSpawn_getPacketSize(Packet *p) {
    Packet23VehicleSpawn *self = (Packet23VehicleSpawn *)p;

    return (21 + self->field_28044_i) > 0 ? 6 : 0;
}
