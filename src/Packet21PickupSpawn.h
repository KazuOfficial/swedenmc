#ifndef PACKET21_PICKUP_SPAWN_H
#define PACKET21_PICKUP_SPAWN_H

#include "Packet.h"
#include <stdint.h>

struct Packet21PickupSpawn {
    Packet base;
    int entityId;
    int xPosition;
    int yPosition;
    int zPosition;
    int8_t rotation;
    int8_t pitch;
    int8_t roll;
    int itemID;
    int count;
    int itemDamage;
};
typedef struct Packet21PickupSpawn Packet21PickupSpawn;

Packet *Packet21PickupSpawn_create(void);

typedef struct EntityItem EntityItem;
Packet *Packet21PickupSpawn_createWith(EntityItem *var1);
void Packet21PickupSpawn_readPacketData(Packet *self, DataStream *var1);
void Packet21PickupSpawn_writePacketData(Packet *self, DataStream *var1);
void Packet21PickupSpawn_processPacket(Packet *self, NetHandler *var1);
int Packet21PickupSpawn_getPacketSize(Packet *self);

#endif
