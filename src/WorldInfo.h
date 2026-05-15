#ifndef WORLD_INFO_H
#define WORLD_INFO_H

#include "NBTTagCompound.h"
#include <stdint.h>

struct EntityPlayer;

typedef struct WorldInfo {
    int64_t randomSeed;
    int spawnX;
    int spawnY;
    int spawnZ;
    int64_t worldTime;
    int64_t lastTimePlayed;
    int64_t sizeOnDisk;

    void *playerTag;
    NBTTagCompound *nbtRoot;
    int dimension;
    char *levelName;
    int saveVersion;
    int raining;
    int rainTime;
    int thundering;
    int thunderTime;
} WorldInfo;

WorldInfo *WorldInfo_create_from_nbt(NBTTagCompound *tag);

WorldInfo *WorldInfo_create_from_seed_name(int64_t randomSeed, const char *levelName);

WorldInfo *WorldInfo_create_copy(const WorldInfo *src);
void WorldInfo_free(WorldInfo *self);

NBTTagCompound *WorldInfo_getNBTTagCompound(WorldInfo *self);

NBTTagCompound *WorldInfo_getNBTTagCompoundWithPlayer(WorldInfo *self,
                                                      struct EntityPlayer **players,
                                                      int playerCount);

int64_t WorldInfo_getRandomSeed(const WorldInfo *self);
int WorldInfo_getSpawnX(const WorldInfo *self);
int WorldInfo_getSpawnY(const WorldInfo *self);
int WorldInfo_getSpawnZ(const WorldInfo *self);
int64_t WorldInfo_getWorldTime(const WorldInfo *self);
int64_t WorldInfo_getSizeOnDisk(const WorldInfo *self);
void *WorldInfo_getPlayerNBTTagCompound(const WorldInfo *self);
int WorldInfo_getDimension(const WorldInfo *self);
const char *WorldInfo_getWorldName(const WorldInfo *self);
int WorldInfo_getSaveVersion(const WorldInfo *self);
int64_t WorldInfo_getLastTimePlayed(const WorldInfo *self);
int WorldInfo_getThundering(const WorldInfo *self);
int WorldInfo_getThunderTime(const WorldInfo *self);
int WorldInfo_getRaining(const WorldInfo *self);
int WorldInfo_getRainTime(const WorldInfo *self);

void WorldInfo_setSpawnX(WorldInfo *self, int x);
void WorldInfo_setSpawnY(WorldInfo *self, int y);
void WorldInfo_setSpawnZ(WorldInfo *self, int z);
void WorldInfo_setSpawn(WorldInfo *self, int x, int y, int z);
void WorldInfo_setWorldTime(WorldInfo *self, int64_t time);
void WorldInfo_setSizeOnDisk(WorldInfo *self, int64_t size);
void WorldInfo_setPlayerNBTTagCompound(WorldInfo *self, void *tag);
void WorldInfo_setWorldName(WorldInfo *self, const char *name);
void WorldInfo_setSaveVersion(WorldInfo *self, int v);
void WorldInfo_setThundering(WorldInfo *self, int thundering);
void WorldInfo_setThunderTime(WorldInfo *self, int time);
void WorldInfo_setRaining(WorldInfo *self, int raining);
void WorldInfo_setRainTime(WorldInfo *self, int time);

#endif
