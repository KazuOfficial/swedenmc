#ifndef PACKET255_KICK_DISCONNECT_H
#define PACKET255_KICK_DISCONNECT_H

#include "Packet.h"

struct Packet255KickDisconnect {
    Packet base;

    char *reason;
};
typedef struct Packet255KickDisconnect Packet255KickDisconnect;

Packet *Packet255KickDisconnect_create(void);
Packet *Packet255KickDisconnect_createWith(const char *reason);
void Packet255KickDisconnect_readPacketData(Packet *self, DataStream *var1);
void Packet255KickDisconnect_writePacketData(Packet *self, DataStream *var1);
void Packet255KickDisconnect_processPacket(Packet *self, NetHandler *var1);
int Packet255KickDisconnect_getPacketSize(Packet *self);

#endif
