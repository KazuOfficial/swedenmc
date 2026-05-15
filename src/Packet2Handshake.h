#ifndef PACKET2_HANDSHAKE_H
#define PACKET2_HANDSHAKE_H

#include "Packet.h"

struct Packet2Handshake {
    Packet base;

    char *username;
};
typedef struct Packet2Handshake Packet2Handshake;

Packet *Packet2Handshake_create(void);
Packet *Packet2Handshake_createWith(const char *username);
void Packet2Handshake_readPacketData(Packet *self, DataStream *var1);
void Packet2Handshake_writePacketData(Packet *self, DataStream *var1);
void Packet2Handshake_processPacket(Packet *self, NetHandler *var1);
int Packet2Handshake_getPacketSize(Packet *self);

#endif
