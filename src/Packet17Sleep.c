#include "Packet17Sleep.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet17Sleep_readPacketData,
    Packet17Sleep_writePacketData,
    Packet17Sleep_processPacket,
    Packet17Sleep_getPacketSize,
};

Packet *Packet17Sleep_create(void) {
    Packet17Sleep *self = calloc(1, sizeof(Packet17Sleep));
    self->base.vtable = &vtable;
    self->base.packetId = 17;
    return (Packet *)self;
}

void Packet17Sleep_readPacketData(Packet *p, DataStream *var1) {
    Packet17Sleep *self = (Packet17Sleep *)p;
    self->field_22045_a = DataStream_readInt(var1);

    self->field_22046_e = (int)(int8_t)DataStream_readByte(var1);
    self->field_22044_b = DataStream_readInt(var1);

    self->field_22048_c = (int)(int8_t)DataStream_readByte(var1);
    self->field_22047_d = DataStream_readInt(var1);
}

void Packet17Sleep_writePacketData(Packet *p, DataStream *var1) {
    Packet17Sleep *self = (Packet17Sleep *)p;
    DataStream_writeInt(var1, self->field_22045_a);
    DataStream_writeByte(var1, (uint8_t)self->field_22046_e);
    DataStream_writeInt(var1, self->field_22044_b);
    DataStream_writeByte(var1, (uint8_t)self->field_22048_c);
    DataStream_writeInt(var1, self->field_22047_d);
}

void Packet17Sleep_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_22186_a(var1, (Packet17Sleep *)p);
}

int Packet17Sleep_getPacketSize(Packet *p) {
    (void)p;
    return 14;
}
