#include "Packet70Bed.h"
#include "NetHandler.h"
#include <stdlib.h>

const char *Packet70Bed_field_25020_a[3] = {"tile.bed.notValid", NULL, NULL};

static Packet_vtable vtable = {
    Packet70Bed_readPacketData,
    Packet70Bed_writePacketData,
    Packet70Bed_processPacket,
    Packet70Bed_getPacketSize,
};

Packet *Packet70Bed_create(void) {
    Packet70Bed *self = calloc(1, sizeof(Packet70Bed));
    self->base.vtable = &vtable;
    self->base.packetId = 70;
    return (Packet *)self;
}

void Packet70Bed_readPacketData(Packet *p, DataStream *var1) {

    ((Packet70Bed *)p)->field_25019_b = (int)(int8_t)DataStream_readByte(var1);
}

void Packet70Bed_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeByte(var1, (uint8_t)((Packet70Bed *)p)->field_25019_b);
}

void Packet70Bed_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_25118_a(var1, (Packet70Bed *)p);
}

int Packet70Bed_getPacketSize(Packet *p) {
    (void)p;
    return 1;
}
