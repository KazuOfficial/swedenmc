#ifndef PACKET71_WEATHER_H
#define PACKET71_WEATHER_H

#include "Packet.h"
#include "GameForward.h"

struct Packet71Weather {
    Packet base;
    int field_27054_a;
    int field_27053_b;
    int field_27057_c;
    int field_27056_d;
    int field_27055_e;
};
typedef struct Packet71Weather Packet71Weather;

Packet *Packet71Weather_create(void);

Packet *Packet71Weather_createFromEntity(struct Entity *var1);
void Packet71Weather_readPacketData(Packet *self, DataStream *var1);
void Packet71Weather_writePacketData(Packet *self, DataStream *var1);
void Packet71Weather_processPacket(Packet *self, NetHandler *var1);
int Packet71Weather_getPacketSize(Packet *self);

#endif
