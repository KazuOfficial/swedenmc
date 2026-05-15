#ifndef PACKET_COUNTER_H
#define PACKET_COUNTER_H

#include <stdint.h>

typedef struct {

    int totalPackets;

    int64_t totalBytes;
} PacketCounter;

void PacketCounter_init(PacketCounter *self);

void PacketCounter_addPacket(PacketCounter *self, int var1);

#endif
