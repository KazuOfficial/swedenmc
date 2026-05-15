#include "Packet1Login.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet1Login_readPacketData,
    Packet1Login_writePacketData,
    Packet1Login_processPacket,
    Packet1Login_getPacketSize,
};

Packet *Packet1Login_create(void) {
    Packet1Login *self = calloc(1, sizeof(Packet1Login));
    self->base.vtable = &vtable;
    self->base.packetId = 1;
    return (Packet *)self;
}

Packet *Packet1Login_createWith(const char *username, int protocolVersion) {
    Packet1Login *self = (Packet1Login *)Packet1Login_create();
    self->username = strdup(username);
    self->protocolVersion = protocolVersion;
    return (Packet *)self;
}

void Packet1Login_readPacketData(Packet *p, DataStream *var1) {
    Packet1Login *self = (Packet1Login *)p;

    self->protocolVersion = DataStream_readInt(var1);

    self->username = Packet_readString(var1, 16);

    self->mapSeed = DataStream_readLong(var1);

    self->dimension = (int8_t)DataStream_readByte(var1);
}

void Packet1Login_writePacketData(Packet *p, DataStream *var1) {
    Packet1Login *self = (Packet1Login *)p;
    DataStream_writeInt(var1, self->protocolVersion);
    Packet_writeString(self->username, var1);
    DataStream_writeLong(var1, self->mapSeed);
    DataStream_writeByte(var1, (uint8_t)self->dimension);
}

void Packet1Login_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleLogin(var1, (Packet1Login *)p);
}

int Packet1Login_getPacketSize(Packet *p) {
    Packet1Login *self = (Packet1Login *)p;
    return 4 + (int)strlen(self->username) + 4 + 5;
}
