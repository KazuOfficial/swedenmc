#include "Packet2Handshake.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet2Handshake_readPacketData,
    Packet2Handshake_writePacketData,
    Packet2Handshake_processPacket,
    Packet2Handshake_getPacketSize,
};

Packet *Packet2Handshake_create(void) {
    Packet2Handshake *self = calloc(1, sizeof(Packet2Handshake));
    self->base.vtable = &vtable;
    self->base.packetId = 2;
    return (Packet *)self;
}

Packet *Packet2Handshake_createWith(const char *username) {
    Packet2Handshake *self = (Packet2Handshake *)Packet2Handshake_create();
    self->username = strdup(username);
    return (Packet *)self;
}

void Packet2Handshake_readPacketData(Packet *p, DataStream *var1) {
    Packet2Handshake *self = (Packet2Handshake *)p;

    self->username = Packet_readString(var1, 32);
}

void Packet2Handshake_writePacketData(Packet *p, DataStream *var1) {
    Packet2Handshake *self = (Packet2Handshake *)p;
    Packet_writeString(self->username, var1);
}

void Packet2Handshake_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleHandshake(var1, (Packet2Handshake *)p);
}

int Packet2Handshake_getPacketSize(Packet *p) {
    Packet2Handshake *self = (Packet2Handshake *)p;
    return 4 + (int)strlen(self->username) + 4;
}
