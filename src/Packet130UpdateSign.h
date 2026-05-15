#ifndef PACKET130_UPDATE_SIGN_H
#define PACKET130_UPDATE_SIGN_H

#include "Packet.h"

struct Packet130UpdateSign {
    Packet base;
    int xPosition;
    int yPosition;
    int zPosition;

    char *signLines[4];
};
typedef struct Packet130UpdateSign Packet130UpdateSign;

Packet *Packet130UpdateSign_create(void);

Packet *Packet130UpdateSign_createWith(int x, int y, int z, char *lines[4]);
void Packet130UpdateSign_readPacketData(Packet *self, DataStream *var1);
void Packet130UpdateSign_writePacketData(Packet *self, DataStream *var1);
void Packet130UpdateSign_processPacket(Packet *self, NetHandler *var1);
int Packet130UpdateSign_getPacketSize(Packet *self);

#endif
