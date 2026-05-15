#ifndef PACKET18_ANIMATION_H
#define PACKET18_ANIMATION_H

#include "Packet.h"
#include "GameForward.h"

struct Packet18Animation {
    Packet base;
    int entityId;
    int animate;
};
typedef struct Packet18Animation Packet18Animation;

Packet *Packet18Animation_create(void);

Packet *Packet18Animation_createWith(Entity *var1, int var2);
void Packet18Animation_readPacketData(Packet *self, DataStream *var1);
void Packet18Animation_writePacketData(Packet *self, DataStream *var1);
void Packet18Animation_processPacket(Packet *self, NetHandler *var1);
int Packet18Animation_getPacketSize(Packet *self);

#endif
