#ifndef PACKET8_UPDATE_HEALTH_H
#define PACKET8_UPDATE_HEALTH_H

#include "Packet.h"

struct Packet8UpdateHealth {
    Packet base;

    int healthMP;
};
typedef struct Packet8UpdateHealth Packet8UpdateHealth;

Packet *Packet8UpdateHealth_create(void);
void Packet8UpdateHealth_readPacketData(Packet *self, DataStream *var1);
void Packet8UpdateHealth_writePacketData(Packet *self, DataStream *var1);
void Packet8UpdateHealth_processPacket(Packet *self, NetHandler *var1);
int Packet8UpdateHealth_getPacketSize(Packet *self);

#endif
