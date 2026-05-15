#ifndef PACKET32_ENTITY_LOOK_H
#define PACKET32_ENTITY_LOOK_H

#include "Packet30Entity.h"

typedef Packet30Entity Packet32EntityLook;

Packet *Packet32EntityLook_create(void);
void Packet32EntityLook_readPacketData(Packet *self, DataStream *var1);
void Packet32EntityLook_writePacketData(Packet *self, DataStream *var1);
int Packet32EntityLook_getPacketSize(Packet *self);

#endif
