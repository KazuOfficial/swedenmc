#ifndef PACKET27_POSITION_H
#define PACKET27_POSITION_H

#include "Packet.h"

struct Packet27Position {
    Packet base;

    float field_22039_a;
    float field_22038_b;

    int field_22043_c;
    int field_22042_d;

    float field_22041_e;
    float field_22040_f;
};
typedef struct Packet27Position Packet27Position;

Packet *Packet27Position_create(void);
void Packet27Position_readPacketData(Packet *self, DataStream *var1);
void Packet27Position_writePacketData(Packet *self, DataStream *var1);
void Packet27Position_processPacket(Packet *self, NetHandler *var1);
int Packet27Position_getPacketSize(Packet *self);

#endif
