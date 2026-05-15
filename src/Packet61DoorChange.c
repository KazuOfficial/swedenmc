#include "Packet61DoorChange.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet61DoorChange_readPacketData,
    Packet61DoorChange_writePacketData,
    Packet61DoorChange_processPacket,
    Packet61DoorChange_getPacketSize,
};

Packet *Packet61DoorChange_create(void) {
    Packet61DoorChange *self = calloc(1, sizeof(Packet61DoorChange));
    self->base.vtable = &vtable;
    self->base.packetId = 61;
    return (Packet *)self;
}

void Packet61DoorChange_readPacketData(Packet *p, DataStream *var1) {
    Packet61DoorChange *self = (Packet61DoorChange *)p;
    self->field_28050_a = DataStream_readInt(var1);
    self->field_28053_c = DataStream_readInt(var1);

    self->field_28052_d = (int)(int8_t)DataStream_readByte(var1);
    self->field_28051_e = DataStream_readInt(var1);
    self->field_28049_b = DataStream_readInt(var1);
}

void Packet61DoorChange_writePacketData(Packet *p, DataStream *var1) {
    Packet61DoorChange *self = (Packet61DoorChange *)p;
    DataStream_writeInt(var1, self->field_28050_a);
    DataStream_writeInt(var1, self->field_28053_c);
    DataStream_writeByte(var1, (uint8_t)self->field_28052_d);
    DataStream_writeInt(var1, self->field_28051_e);
    DataStream_writeInt(var1, self->field_28049_b);
}

void Packet61DoorChange_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_28115_a(var1, (Packet61DoorChange *)p);
}

int Packet61DoorChange_getPacketSize(Packet *p) {
    (void)p;
    return 20;
}
