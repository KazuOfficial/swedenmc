#ifndef PACKET4_UPDATE_TIME_H
#define PACKET4_UPDATE_TIME_H

#include "Packet.h"
#include <stdint.h>

struct Packet4UpdateTime {
    Packet base;

    int64_t time;
};
typedef struct Packet4UpdateTime Packet4UpdateTime;

Packet *Packet4UpdateTime_create(void);
void Packet4UpdateTime_readPacketData(Packet *self, DataStream *var1);
void Packet4UpdateTime_writePacketData(Packet *self, DataStream *var1);
void Packet4UpdateTime_processPacket(Packet *self, NetHandler *var1);
int Packet4UpdateTime_getPacketSize(Packet *self);

#endif
