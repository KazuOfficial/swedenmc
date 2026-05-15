#include "PacketCounter.h"

void PacketCounter_init(PacketCounter *self) {
    self->totalPackets = 0;
    self->totalBytes = 0;
}

void PacketCounter_addPacket(PacketCounter *self, int var1) {
    ++self->totalPackets;
    self->totalBytes += (int64_t)var1;
}
