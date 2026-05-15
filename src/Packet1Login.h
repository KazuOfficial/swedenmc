#ifndef PACKET1_LOGIN_H
#define PACKET1_LOGIN_H

#include "Packet.h"
#include <stdint.h>

struct Packet1Login {
    Packet base;

    int32_t protocolVersion;

    char *username;

    int64_t mapSeed;

    int8_t dimension;
};
typedef struct Packet1Login Packet1Login;

Packet *Packet1Login_create(void);

Packet *Packet1Login_createWith(const char *username, int protocolVersion);
void Packet1Login_readPacketData(Packet *self, DataStream *var1);
void Packet1Login_writePacketData(Packet *self, DataStream *var1);
void Packet1Login_processPacket(Packet *self, NetHandler *var1);
int Packet1Login_getPacketSize(Packet *self);

#endif
