#ifndef PACKET24_MOB_SPAWN_H
#define PACKET24_MOB_SPAWN_H

#include "Packet.h"
#include "DataWatcher.h"
#include <stdint.h>

struct Packet24MobSpawn {
    Packet base;
    int entityId;
    int8_t type;
    int xPosition;
    int yPosition;
    int zPosition;
    int8_t yaw;
    int8_t pitch;

    DataWatcher *metaData;

    DataWatcher receivedMetadata;
};
typedef struct Packet24MobSpawn Packet24MobSpawn;

Packet *Packet24MobSpawn_create(void);

typedef struct EntityLiving EntityLiving;
Packet *Packet24MobSpawn_createWith(EntityLiving *var1);
void Packet24MobSpawn_readPacketData(Packet *self, DataStream *var1);
void Packet24MobSpawn_writePacketData(Packet *self, DataStream *var1);
void Packet24MobSpawn_processPacket(Packet *self, NetHandler *var1);
int Packet24MobSpawn_getPacketSize(Packet *self);

DataWatcher *Packet24MobSpawn_getMetadata(Packet24MobSpawn *self);

#endif
