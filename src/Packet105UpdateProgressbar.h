#ifndef PACKET105_UPDATE_PROGRESSBAR_H
#define PACKET105_UPDATE_PROGRESSBAR_H

#include "Packet.h"

struct Packet105UpdateProgressbar {
    Packet base;
    int windowId;
    int progressBar;
    int progressBarValue;
};
typedef struct Packet105UpdateProgressbar Packet105UpdateProgressbar;

Packet *Packet105UpdateProgressbar_create(void);
void Packet105UpdateProgressbar_readPacketData(Packet *self, DataStream *var1);
void Packet105UpdateProgressbar_writePacketData(Packet *self, DataStream *var1);
void Packet105UpdateProgressbar_processPacket(Packet *self, NetHandler *var1);
int Packet105UpdateProgressbar_getPacketSize(Packet *self);

#endif
