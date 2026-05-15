#ifndef PACKET11_PLAYER_POSITION_H
#define PACKET11_PLAYER_POSITION_H

#include "Packet10Flying.h"

typedef Packet10Flying Packet11PlayerPosition;

Packet *Packet11PlayerPosition_create(void);
Packet *Packet11PlayerPosition_createWith(double x, double y, double stance, double z,
                                          int onGround);
void Packet11PlayerPosition_readPacketData(Packet *self, DataStream *var1);
void Packet11PlayerPosition_writePacketData(Packet *self, DataStream *var1);
int Packet11PlayerPosition_getPacketSize(Packet *self);

#endif
