#include "Packet27Position.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet27Position_readPacketData,
    Packet27Position_writePacketData,
    Packet27Position_processPacket,
    Packet27Position_getPacketSize,
};

Packet *Packet27Position_create(void) {
    Packet27Position *self = calloc(1, sizeof(Packet27Position));
    self->base.vtable = &vtable;
    self->base.packetId = 27;
    return (Packet *)self;
}

void Packet27Position_readPacketData(Packet *p, DataStream *var1) {
    Packet27Position *self = (Packet27Position *)p;
    self->field_22039_a = DataStream_readFloat(var1);
    self->field_22038_b = DataStream_readFloat(var1);
    self->field_22041_e = DataStream_readFloat(var1);
    self->field_22040_f = DataStream_readFloat(var1);

    self->field_22043_c = DataStream_readByte(var1) != 0;
    self->field_22042_d = DataStream_readByte(var1) != 0;
}

void Packet27Position_writePacketData(Packet *p, DataStream *var1) {
    Packet27Position *self = (Packet27Position *)p;
    DataStream_writeFloat(var1, self->field_22039_a);
    DataStream_writeFloat(var1, self->field_22038_b);
    DataStream_writeFloat(var1, self->field_22041_e);
    DataStream_writeFloat(var1, self->field_22040_f);
    DataStream_writeByte(var1, (uint8_t)(self->field_22043_c ? 1 : 0));
    DataStream_writeByte(var1, (uint8_t)(self->field_22042_d ? 1 : 0));
}

void Packet27Position_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_22185_a(var1, (Packet27Position *)p);
}

int Packet27Position_getPacketSize(Packet *p) {
    (void)p;
    return 18;
}
