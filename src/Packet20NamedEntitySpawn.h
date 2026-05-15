#ifndef PACKET20_NAMED_ENTITY_SPAWN_H
#define PACKET20_NAMED_ENTITY_SPAWN_H

#include "Packet.h"
#include <stdint.h>

struct Packet20NamedEntitySpawn {
    Packet base;
    int entityId;
    char *name;
    int xPosition;
    int yPosition;
    int zPosition;
    int8_t rotation;
    int8_t pitch;
    int currentItem;
};
typedef struct Packet20NamedEntitySpawn Packet20NamedEntitySpawn;

Packet *Packet20NamedEntitySpawn_create(void);

typedef struct EntityPlayer EntityPlayer;
Packet *Packet20NamedEntitySpawn_createWith(EntityPlayer *var1);
void Packet20NamedEntitySpawn_readPacketData(Packet *self, DataStream *var1);
void Packet20NamedEntitySpawn_writePacketData(Packet *self, DataStream *var1);
void Packet20NamedEntitySpawn_processPacket(Packet *self, NetHandler *var1);
int Packet20NamedEntitySpawn_getPacketSize(Packet *self);

#endif
