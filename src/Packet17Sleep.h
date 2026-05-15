#ifndef PACKET17_SLEEP_H
#define PACKET17_SLEEP_H

#include "Packet.h"

struct Packet17Sleep {
    Packet base;
    int field_22045_a;
    int field_22044_b;
    int field_22048_c;
    int field_22047_d;
    int field_22046_e;
};
typedef struct Packet17Sleep Packet17Sleep;

Packet *Packet17Sleep_create(void);
void Packet17Sleep_readPacketData(Packet *self, DataStream *var1);
void Packet17Sleep_writePacketData(Packet *self, DataStream *var1);
void Packet17Sleep_processPacket(Packet *self, NetHandler *var1);
int Packet17Sleep_getPacketSize(Packet *self);

#endif
