#include "Packet106Transaction.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet106Transaction_readPacketData,
    Packet106Transaction_writePacketData,
    Packet106Transaction_processPacket,
    Packet106Transaction_getPacketSize,
};

Packet *Packet106Transaction_create(void) {
    Packet106Transaction *self = calloc(1, sizeof(Packet106Transaction));
    self->base.vtable = &vtable;
    self->base.packetId = 106;
    return (Packet *)self;
}

Packet *Packet106Transaction_createWith(int windowId, int16_t var2, int var3) {
    Packet106Transaction *self = (Packet106Transaction *)Packet106Transaction_create();
    self->windowId = windowId;
    self->field_20028_b = var2;
    self->field_20030_c = var3;
    return (Packet *)self;
}

void Packet106Transaction_readPacketData(Packet *p, DataStream *var1) {
    Packet106Transaction *self = (Packet106Transaction *)p;
    self->windowId = (int)(int8_t)DataStream_readByte(var1);
    self->field_20028_b = DataStream_readShort(var1);

    self->field_20030_c = (int)(int8_t)DataStream_readByte(var1) != 0;
}

void Packet106Transaction_writePacketData(Packet *p, DataStream *var1) {
    Packet106Transaction *self = (Packet106Transaction *)p;
    DataStream_writeByte(var1, (uint8_t)self->windowId);
    DataStream_writeShort(var1, self->field_20028_b);
    DataStream_writeByte(var1, (uint8_t)(self->field_20030_c ? 1 : 0));
}

void Packet106Transaction_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20089_a(var1, (Packet106Transaction *)p);
}

int Packet106Transaction_getPacketSize(Packet *p) {
    (void)p;
    return 4;
}
