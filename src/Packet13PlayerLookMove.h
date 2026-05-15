#ifndef PACKET13_PLAYER_LOOK_MOVE_H
#define PACKET13_PLAYER_LOOK_MOVE_H

#include "Packet10Flying.h"

typedef Packet10Flying Packet13PlayerLookMove;

Packet *Packet13PlayerLookMove_create(void);
Packet *Packet13PlayerLookMove_createWith(double x, double y, double stance, double z, float yaw,
                                          float pitch, int onGround);
void Packet13PlayerLookMove_readPacketData(Packet *self, DataStream *var1);
void Packet13PlayerLookMove_writePacketData(Packet *self, DataStream *var1);
int Packet13PlayerLookMove_getPacketSize(Packet *self);

#endif
