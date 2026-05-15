#ifndef PACKET31_REL_ENTITY_MOVE_H
#define PACKET31_REL_ENTITY_MOVE_H

#include "Packet30Entity.h"

typedef Packet30Entity Packet31RelEntityMove;

Packet *Packet31RelEntityMove_create(void);
void Packet31RelEntityMove_readPacketData(Packet *self, DataStream *var1);
void Packet31RelEntityMove_writePacketData(Packet *self, DataStream *var1);
int Packet31RelEntityMove_getPacketSize(Packet *self);

#endif
