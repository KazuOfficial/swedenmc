#ifndef PACKET61_DOOR_CHANGE_H
#define PACKET61_DOOR_CHANGE_H

#include "Packet.h"

struct Packet61DoorChange {
    Packet base;
    int field_28050_a;
    int field_28049_b;
    int field_28053_c;
    int field_28052_d;
    int field_28051_e;
};
typedef struct Packet61DoorChange Packet61DoorChange;

Packet *Packet61DoorChange_create(void);
void Packet61DoorChange_readPacketData(Packet *self, DataStream *var1);
void Packet61DoorChange_writePacketData(Packet *self, DataStream *var1);
void Packet61DoorChange_processPacket(Packet *self, NetHandler *var1);
int Packet61DoorChange_getPacketSize(Packet *self);

#endif
