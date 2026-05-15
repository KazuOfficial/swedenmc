#include "Packet9Respawn.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet9Respawn_readPacketData,
    Packet9Respawn_writePacketData,
    Packet9Respawn_processPacket,
    Packet9Respawn_getPacketSize,
};

Packet *Packet9Respawn_create(void) {
    Packet9Respawn *self = calloc(1, sizeof(Packet9Respawn));
    self->base.vtable = &vtable;
    self->base.packetId = 9;
    return (Packet *)self;
}

Packet *Packet9Respawn_createWith(int8_t var1) {
    Packet9Respawn *self = (Packet9Respawn *)Packet9Respawn_create();
    self->field_28048_a = var1;
    return (Packet *)self;
}

void Packet9Respawn_readPacketData(Packet *p, DataStream *var1) {
    ((Packet9Respawn *)p)->field_28048_a = (int8_t)DataStream_readByte(var1);
}

void Packet9Respawn_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeByte(var1, (uint8_t)((Packet9Respawn *)p)->field_28048_a);
}

void Packet9Respawn_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_9448_a(var1, (Packet9Respawn *)p);
}

int Packet9Respawn_getPacketSize(Packet *p) {
    (void)p;
    return 1;
}
