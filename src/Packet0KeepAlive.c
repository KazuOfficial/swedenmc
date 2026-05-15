#include "Packet0KeepAlive.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <time.h>

static Packet_vtable vtable = {
    Packet0KeepAlive_readPacketData,
    Packet0KeepAlive_writePacketData,
    Packet0KeepAlive_processPacket,
    Packet0KeepAlive_getPacketSize,
};

Packet *Packet0KeepAlive_create(void) {
    Packet0KeepAlive *self = calloc(1, sizeof(Packet0KeepAlive));
    self->base.vtable = &vtable;
    self->base.packetId = 0;
    return (Packet *)self;
}

void Packet0KeepAlive_readPacketData(Packet *self, DataStream *var1) {
    (void)self;
    (void)var1;
}
void Packet0KeepAlive_writePacketData(Packet *self, DataStream *var1) {
    (void)self;
    (void)var1;
}
void Packet0KeepAlive_processPacket(Packet *self, NetHandler *var1) {
    (void)self;
    (void)var1;
}
int Packet0KeepAlive_getPacketSize(Packet *self) {
    (void)self;
    return 0;
}
