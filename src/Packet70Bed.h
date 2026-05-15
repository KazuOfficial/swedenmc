#ifndef PACKET70_BED_H
#define PACKET70_BED_H

#include "Packet.h"

struct Packet70Bed {
    Packet base;

    int field_25019_b;
};
typedef struct Packet70Bed Packet70Bed;

extern const char *Packet70Bed_field_25020_a[3];

Packet *Packet70Bed_create(void);
void Packet70Bed_readPacketData(Packet *self, DataStream *var1);
void Packet70Bed_writePacketData(Packet *self, DataStream *var1);
void Packet70Bed_processPacket(Packet *self, NetHandler *var1);
int Packet70Bed_getPacketSize(Packet *self);

#endif
