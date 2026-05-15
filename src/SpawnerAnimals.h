#ifndef SPAWNER_ANIMALS_H
#define SPAWNER_ANIMALS_H

#include "World.h"
#include "EntityLiving.h"
#include "EnumCreatureType.h"
#include "ChunkPosition.h"
#include "SpawnListEntry.h"

extern const EntitySpawnType SpawnerAnimals_nightSpawnEntities[3];

ChunkPosition SpawnerAnimals_getRandomSpawningPointInChunk(World *var0, int var1, int var2);

int SpawnerAnimals_performSpawning(World *var0, int var1, int var2);

int SpawnerAnimals_canCreatureTypeSpawnAtLocation(EnumCreatureType var0, World *var1, int var2,
                                                  int var3, int var4);

void SpawnerAnimals_creatureSpecificInit(EntityLiving *var0, World *var1, float var2, float var3,
                                         float var4);

int SpawnerAnimals_performSleepSpawning(World *var0, struct EntityPlayer **var1, int var1Count);

#endif
