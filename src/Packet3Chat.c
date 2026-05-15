#include "Packet3Chat.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet3Chat_readPacketData,
    Packet3Chat_writePacketData,
    Packet3Chat_processPacket,
    Packet3Chat_getPacketSize,
};

Packet *Packet3Chat_create(void) {
    Packet3Chat *self = calloc(1, sizeof(Packet3Chat));
    self->base.vtable = &vtable;
    self->base.packetId = 3;
    return (Packet *)self;
}

Packet *Packet3Chat_createWith(const char *message) {
    Packet3Chat *self = (Packet3Chat *)Packet3Chat_create();

    size_t len = strlen(message);
    if (len > 119)
        len = 119;
    self->message = malloc(len + 1);
    memcpy(self->message, message, len);
    self->message[len] = '\0';
    return (Packet *)self;
}

void Packet3Chat_readPacketData(Packet *p, DataStream *var1) {
    Packet3Chat *self = (Packet3Chat *)p;
    self->message = Packet_readString(var1, 119);
}

void Packet3Chat_writePacketData(Packet *p, DataStream *var1) {
    Packet3Chat *self = (Packet3Chat *)p;
    Packet_writeString(self->message, var1);
}

void Packet3Chat_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleChat(var1, (Packet3Chat *)p);
}

int Packet3Chat_getPacketSize(Packet *p) {
    Packet3Chat *self = (Packet3Chat *)p;
    return (int)strlen(self->message);
}
