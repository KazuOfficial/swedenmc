#ifndef PACKET23_VEHICLE_SPAWN_H
#define PACKET23_VEHICLE_SPAWN_H

#include "Packet.h"

struct Packet23VehicleSpawn {
    Packet base;
    int entityId;
    int xPosition;
    int yPosition;
    int zPosition;
    int field_28047_e;
    int field_28046_f;
    int field_28045_g;
    int type;
    int field_28044_i;
};
typedef struct Packet23VehicleSpawn Packet23VehicleSpawn;

Packet *Packet23VehicleSpawn_create(void);
void Packet23VehicleSpawn_readPacketData(Packet *self, DataStream *var1);
void Packet23VehicleSpawn_writePacketData(Packet *self, DataStream *var1);
void Packet23VehicleSpawn_processPacket(Packet *self, NetHandler *var1);
int Packet23VehicleSpawn_getPacketSize(Packet *self);

#endif
