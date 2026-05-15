#ifndef PACKET200_STATISTIC_H
#define PACKET200_STATISTIC_H

#include "Packet.h"

struct Packet200Statistic {
    Packet base;
    int field_27052_a;
    int field_27051_b;
};
typedef struct Packet200Statistic Packet200Statistic;

Packet *Packet200Statistic_create(void);
void Packet200Statistic_readPacketData(Packet *self, DataStream *var1);
void Packet200Statistic_writePacketData(Packet *self, DataStream *var1);
void Packet200Statistic_processPacket(Packet *self, NetHandler *var1);
int Packet200Statistic_getPacketSize(Packet *self);

#endif
