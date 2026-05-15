#ifndef PACKET12_PLAYER_LOOK_H
#define PACKET12_PLAYER_LOOK_H

#include "Packet10Flying.h"

typedef Packet10Flying Packet12PlayerLook;

Packet *Packet12PlayerLook_create(void);
Packet *Packet12PlayerLook_createWith(float yaw, float pitch, int onGround);
void Packet12PlayerLook_readPacketData(Packet *self, DataStream *var1);
void Packet12PlayerLook_writePacketData(Packet *self, DataStream *var1);
int Packet12PlayerLook_getPacketSize(Packet *self);

#endif
