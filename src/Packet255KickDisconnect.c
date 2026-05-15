#include "Packet255KickDisconnect.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet255KickDisconnect_readPacketData,
    Packet255KickDisconnect_writePacketData,
    Packet255KickDisconnect_processPacket,
    Packet255KickDisconnect_getPacketSize,
};

Packet *Packet255KickDisconnect_create(void) {
    Packet255KickDisconnect *self = calloc(1, sizeof(Packet255KickDisconnect));
    self->base.vtable = &vtable;
    self->base.packetId = 255;
    return (Packet *)self;
}

Packet *Packet255KickDisconnect_createWith(const char *reason) {
    Packet255KickDisconnect *self = (Packet255KickDisconnect *)Packet255KickDisconnect_create();
    self->reason = strdup(reason);
    return (Packet *)self;
}

void Packet255KickDisconnect_readPacketData(Packet *p, DataStream *var1) {
    ((Packet255KickDisconnect *)p)->reason = Packet_readString(var1, 100);
}

void Packet255KickDisconnect_writePacketData(Packet *p, DataStream *var1) {
    Packet_writeString(((Packet255KickDisconnect *)p)->reason, var1);
}

void Packet255KickDisconnect_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleKickDisconnect(var1, (Packet255KickDisconnect *)p);
}

int Packet255KickDisconnect_getPacketSize(Packet *p) {
    return (int)strlen(((Packet255KickDisconnect *)p)->reason);
}
