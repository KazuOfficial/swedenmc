#ifndef PACKET131_MAP_DATA_H
#define PACKET131_MAP_DATA_H

#include "Packet.h"
#include <stdint.h>

struct Packet131MapData {
    Packet base;

    int16_t field_28055_a;

    int16_t field_28054_b;

    uint8_t *field_28056_c;
    int field_28056_c_len;
};
typedef struct Packet131MapData Packet131MapData;

Packet *Packet131MapData_create(void);
void Packet131MapData_readPacketData(Packet *self, DataStream *var1);
void Packet131MapData_writePacketData(Packet *self, DataStream *var1);
void Packet131MapData_processPacket(Packet *self, NetHandler *var1);
int Packet131MapData_getPacketSize(Packet *self);

#endif
