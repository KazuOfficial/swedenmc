#ifndef PACKET33_REL_ENTITY_MOVE_LOOK_H
#define PACKET33_REL_ENTITY_MOVE_LOOK_H

#include "Packet30Entity.h"

typedef Packet30Entity Packet33RelEntityMoveLook;

Packet *Packet33RelEntityMoveLook_create(void);
void Packet33RelEntityMoveLook_readPacketData(Packet *self, DataStream *var1);
void Packet33RelEntityMoveLook_writePacketData(Packet *self, DataStream *var1);
int Packet33RelEntityMoveLook_getPacketSize(Packet *self);

#endif
