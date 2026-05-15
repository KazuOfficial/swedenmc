#ifndef PACKET10_FLYING_H
#define PACKET10_FLYING_H

#include "Packet.h"

struct Packet10Flying {
    Packet base;
    double xPosition;
    double yPosition;
    double zPosition;
    double stance;
    float yaw;
    float pitch;
    int onGround;
    int moving;
    int rotating;
};
typedef struct Packet10Flying Packet10Flying;

Packet *Packet10Flying_create(void);
Packet *Packet10Flying_createWith(int onGround);
void Packet10Flying_readPacketData(Packet *self, DataStream *var1);
void Packet10Flying_writePacketData(Packet *self, DataStream *var1);
void Packet10Flying_processPacket(Packet *self, NetHandler *var1);
int Packet10Flying_getPacketSize(Packet *self);

#endif
