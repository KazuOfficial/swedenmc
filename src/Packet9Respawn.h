#ifndef PACKET9_RESPAWN_H
#define PACKET9_RESPAWN_H

#include "Packet.h"
#include <stdint.h>

struct Packet9Respawn {
    Packet base;

    int8_t field_28048_a;
};
typedef struct Packet9Respawn Packet9Respawn;

Packet *Packet9Respawn_create(void);
Packet *Packet9Respawn_createWith(int8_t var1);
void Packet9Respawn_readPacketData(Packet *self, DataStream *var1);
void Packet9Respawn_writePacketData(Packet *self, DataStream *var1);
void Packet9Respawn_processPacket(Packet *self, NetHandler *var1);
int Packet9Respawn_getPacketSize(Packet *self);

#endif
