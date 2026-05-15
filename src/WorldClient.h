#ifndef WORLD_CLIENT_H
#define WORLD_CLIENT_H

#include "World.h"
#include "ChunkProviderClient.h"
#include <stdint.h>

struct NetClientHandler;
struct Packet255KickDisconnect;

#include "WorldBlockPositionType.h"

typedef struct {
    WorldBlockPositionType *entries;
    int count;
    int capacity;
} WorldBlockPositionList;

#include "MCHash.h"

#define MCHASH_INITIAL_SIZE MC_HASH_INITIAL_SLOTS

typedef struct {
    struct Entity **entries;
    int count;
    int capacity;
} EntitySet;

typedef struct WorldClient {
    World base;

    WorldBlockPositionList field_1057_z;

    struct NetClientHandler *sendQueue;

    ChunkProviderClient *field_20915_C;

    MCHash field_1055_D;

    EntitySet field_20914_E;

    EntitySet field_1053_F;
} WorldClient;

void MCHash_init(MCHash *self);
void MCHash_destroy(MCHash *self);
void *MCHash_lookup(MCHash *self, int var1);
void MCHash_addKey(MCHash *self, int var1, void *var2);
void *MCHash_removeObject(MCHash *self, int var1);

void EntitySet_init(EntitySet *self);
void EntitySet_destroy(EntitySet *self);
void EntitySet_add(EntitySet *self, struct Entity *e);
void EntitySet_remove(EntitySet *self, struct Entity *e);
int EntitySet_contains(const EntitySet *self, struct Entity *e);
int EntitySet_isEmpty(const EntitySet *self);
struct Entity *EntitySet_first(const EntitySet *self);

void WorldBlockPositionList_init(WorldBlockPositionList *self);
void WorldBlockPositionList_destroy(WorldBlockPositionList *self);
void WorldBlockPositionList_add(WorldBlockPositionList *self, WorldBlockPositionType item);
void WorldBlockPositionList_removeAt(WorldBlockPositionList *self, int idx);

void WorldClient_construct(WorldClient *self, struct NetClientHandler *var1, int64_t var2,
                           int var4);
WorldClient *WorldClient_create(struct NetClientHandler *var1, int64_t var2, int var4);

void WorldClient_tick(WorldClient *self);
void WorldClient_func_711_c(WorldClient *self, int var1, int var2, int var3, int var4, int var5,
                            int var6);

ChunkProviderClient *WorldClient_getChunkProvider(WorldClient *self);
void WorldClient_setSpawnLocation(WorldClient *self);

void WorldClient_updateBlocksAndPlayCaveSounds(WorldClient *self);

void WorldClient_scheduleBlockUpdate(WorldClient *self, int var1, int var2, int var3, int var4,
                                     int var5);

int WorldClient_TickUpdates(WorldClient *self, int var1);
void WorldClient_doPreChunk(WorldClient *self, int var1, int var2, int var3);
int WorldClient_entityJoinedWorld(WorldClient *self, struct Entity *var1);
void WorldClient_setEntityDead(WorldClient *self, struct Entity *var1);

void WorldClient_func_712_a(WorldClient *self, int var1, struct Entity *var2);
struct Entity *WorldClient_func_709_b(WorldClient *self, int var1);
struct Entity *WorldClient_removeEntityFromWorld(WorldClient *self, int var1);
int WorldClient_setBlockMetadata(WorldClient *self, int var1, int var2, int var3, int var4);
int WorldClient_setBlockAndMetadata(WorldClient *self, int var1, int var2, int var3, int var4,
                                    int var5);
int WorldClient_setBlock(WorldClient *self, int var1, int var2, int var3, int var4);
int WorldClient_func_714_c(WorldClient *self, int var1, int var2, int var3, int var4, int var5);
void WorldClient_sendQuittingDisconnectingPacket(WorldClient *self);

#endif
