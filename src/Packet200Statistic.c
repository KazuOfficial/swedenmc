#include "Packet200Statistic.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet200Statistic_readPacketData,
    Packet200Statistic_writePacketData,
    Packet200Statistic_processPacket,
    Packet200Statistic_getPacketSize,
};

Packet *Packet200Statistic_create(void) {
    Packet200Statistic *self = calloc(1, sizeof(Packet200Statistic));
    self->base.vtable = &vtable;
    self->base.packetId = 200;
    return (Packet *)self;
}

void Packet200Statistic_readPacketData(Packet *p, DataStream *var1) {
    Packet200Statistic *self = (Packet200Statistic *)p;
    self->field_27052_a = DataStream_readInt(var1);
    self->field_27051_b = (int)(int8_t)DataStream_readByte(var1);
}

void Packet200Statistic_writePacketData(Packet *p, DataStream *var1) {
    Packet200Statistic *self = (Packet200Statistic *)p;
    DataStream_writeInt(var1, self->field_27052_a);
    DataStream_writeByte(var1, (uint8_t)self->field_27051_b);
}

void Packet200Statistic_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_27245_a(var1, (Packet200Statistic *)p);
}

int Packet200Statistic_getPacketSize(Packet *p) {
    (void)p;
    return 6;
}
