#include "WorldInfo.h"
#include "NBTTagCompound.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void WorldInfo_updateTagCompound(WorldInfo *self, NBTTagCompound *var1,
                                        NBTTagCompound *var2) {
    NBTTagCompound_setLong(var1, "RandomSeed", self->randomSeed);
    NBTTagCompound_setInteger(var1, "SpawnX", self->spawnX);
    NBTTagCompound_setInteger(var1, "SpawnY", self->spawnY);
    NBTTagCompound_setInteger(var1, "SpawnZ", self->spawnZ);
    NBTTagCompound_setLong(var1, "Time", self->worldTime);
    NBTTagCompound_setLong(var1, "SizeOnDisk", self->sizeOnDisk);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    int64_t nowMs = (int64_t)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
    NBTTagCompound_setLong(var1, "LastPlayed", nowMs);
    NBTTagCompound_setString(var1, "LevelName", self->levelName ? self->levelName : "");
    NBTTagCompound_setInteger(var1, "version", self->saveVersion);
    NBTTagCompound_setInteger(var1, "rainTime", self->rainTime);
    NBTTagCompound_setBoolean(var1, "raining", self->raining);
    NBTTagCompound_setInteger(var1, "thunderTime", self->thunderTime);
    NBTTagCompound_setBoolean(var1, "thundering", self->thundering);
    if (var2 != NULL) {
        NBTTagCompound_setCompoundTag(var1, "Player", var2);
    }
}

WorldInfo *WorldInfo_create_from_nbt(NBTTagCompound *tag) {
    WorldInfo *self = (WorldInfo *)calloc(1, sizeof(WorldInfo));
    self->randomSeed = NBTTagCompound_getLong(tag, "RandomSeed");
    self->spawnX = NBTTagCompound_getInteger(tag, "SpawnX");
    self->spawnY = NBTTagCompound_getInteger(tag, "SpawnY");
    self->spawnZ = NBTTagCompound_getInteger(tag, "SpawnZ");
    self->worldTime = NBTTagCompound_getLong(tag, "Time");
    self->lastTimePlayed = NBTTagCompound_getLong(tag, "LastPlayed");
    self->sizeOnDisk = NBTTagCompound_getLong(tag, "SizeOnDisk");
    const char *ln = NBTTagCompound_getString(tag, "LevelName");
    self->levelName = ln ? strdup(ln) : NULL;
    self->saveVersion = NBTTagCompound_getInteger(tag, "version");
    self->rainTime = NBTTagCompound_getInteger(tag, "rainTime");
    self->raining = NBTTagCompound_getBoolean(tag, "raining");
    self->thunderTime = NBTTagCompound_getInteger(tag, "thunderTime");
    self->thundering = NBTTagCompound_getBoolean(tag, "thundering");
    if (NBTTagCompound_hasKey(tag, "Player")) {
        self->playerTag = (void *)NBTTagCompound_getCompoundTag(tag, "Player");
        self->dimension = NBTTagCompound_getInteger((NBTTagCompound *)self->playerTag, "Dimension");
    }
    return self;
}

WorldInfo *WorldInfo_create_from_seed_name(int64_t randomSeed, const char *levelName) {
    WorldInfo *self = (WorldInfo *)calloc(1, sizeof(WorldInfo));
    self->randomSeed = randomSeed;
    self->levelName = levelName ? strdup(levelName) : NULL;
    return self;
}

WorldInfo *WorldInfo_create_copy(const WorldInfo *src) {
    WorldInfo *self = (WorldInfo *)calloc(1, sizeof(WorldInfo));
    *self = *src;
    self->levelName = src->levelName ? strdup(src->levelName) : NULL;

    self->nbtRoot = NULL;
    self->playerTag = NULL;
    return self;
}

void WorldInfo_free(WorldInfo *self) {
    if (!self)
        return;
    free(self->levelName);
    if (self->nbtRoot)
        NBT_free(self->nbtRoot);
    free(self);
}

int64_t WorldInfo_getRandomSeed(const WorldInfo *self) { return self->randomSeed; }
int WorldInfo_getSpawnX(const WorldInfo *self) { return self->spawnX; }
int WorldInfo_getSpawnY(const WorldInfo *self) { return self->spawnY; }
int WorldInfo_getSpawnZ(const WorldInfo *self) { return self->spawnZ; }
int64_t WorldInfo_getWorldTime(const WorldInfo *self) { return self->worldTime; }
int64_t WorldInfo_getSizeOnDisk(const WorldInfo *self) { return self->sizeOnDisk; }
void *WorldInfo_getPlayerNBTTagCompound(const WorldInfo *s) { return s->playerTag; }
int WorldInfo_getDimension(const WorldInfo *self) { return self->dimension; }
const char *WorldInfo_getWorldName(const WorldInfo *self) { return self->levelName; }
int WorldInfo_getSaveVersion(const WorldInfo *self) { return self->saveVersion; }
int64_t WorldInfo_getLastTimePlayed(const WorldInfo *self) { return self->lastTimePlayed; }
int WorldInfo_getThundering(const WorldInfo *self) { return self->thundering; }
int WorldInfo_getThunderTime(const WorldInfo *self) { return self->thunderTime; }
int WorldInfo_getRaining(const WorldInfo *self) { return self->raining; }
int WorldInfo_getRainTime(const WorldInfo *self) { return self->rainTime; }

void WorldInfo_setSpawnX(WorldInfo *self, int x) { self->spawnX = x; }
void WorldInfo_setSpawnY(WorldInfo *self, int y) { self->spawnY = y; }
void WorldInfo_setSpawnZ(WorldInfo *self, int z) { self->spawnZ = z; }
void WorldInfo_setSpawn(WorldInfo *self, int x, int y, int z) {
    self->spawnX = x;
    self->spawnY = y;
    self->spawnZ = z;
}
void WorldInfo_setWorldTime(WorldInfo *self, int64_t t) { self->worldTime = t; }
void WorldInfo_setSizeOnDisk(WorldInfo *self, int64_t s) { self->sizeOnDisk = s; }
void WorldInfo_setPlayerNBTTagCompound(WorldInfo *self, void *t) { self->playerTag = t; }
void WorldInfo_setWorldName(WorldInfo *self, const char *name) {
    free(self->levelName);
    self->levelName = name ? strdup(name) : NULL;
}
void WorldInfo_setSaveVersion(WorldInfo *self, int v) { self->saveVersion = v; }
void WorldInfo_setThundering(WorldInfo *self, int v) { self->thundering = v; }
void WorldInfo_setThunderTime(WorldInfo *self, int t) { self->thunderTime = t; }
void WorldInfo_setRaining(WorldInfo *self, int v) { self->raining = v; }
void WorldInfo_setRainTime(WorldInfo *self, int t) { self->rainTime = t; }

NBTTagCompound *WorldInfo_getNBTTagCompound(WorldInfo *self) {
    NBTTagCompound *var1 = NBTTagCompound_create();
    WorldInfo_updateTagCompound(self, var1, (NBTTagCompound *)self->playerTag);
    return var1;
}

NBTTagCompound *WorldInfo_getNBTTagCompoundWithPlayer(WorldInfo *self,
                                                      struct EntityPlayer **players,
                                                      int playerCount) {
    NBTTagCompound *var2 = NBTTagCompound_create();
    struct EntityPlayer *var3 = NULL;
    NBTTagCompound *var4 = NULL;
    if (playerCount > 0) {
        var3 = players[0];
    }
    if (var3 != NULL) {
        var4 = NBTTagCompound_create();
        Entity_writeToNBT((Entity *)var3, var4);
    }
    WorldInfo_updateTagCompound(self, var2, var4);
    return var2;
}
