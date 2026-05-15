#ifndef PACKET106_TRANSACTION_H
#define PACKET106_TRANSACTION_H

#include "Packet.h"
#include <stdint.h>

struct Packet106Transaction {
    Packet base;
    int windowId;
    int16_t field_20028_b;

    int field_20030_c;
};
typedef struct Packet106Transaction Packet106Transaction;

Packet *Packet106Transaction_create(void);
Packet *Packet106Transaction_createWith(int windowId, int16_t var2, int var3);
void Packet106Transaction_readPacketData(Packet *self, DataStream *var1);
void Packet106Transaction_writePacketData(Packet *self, DataStream *var1);
void Packet106Transaction_processPacket(Packet *self, NetHandler *var1);
int Packet106Transaction_getPacketSize(Packet *self);

#endif
