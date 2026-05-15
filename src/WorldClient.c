#include "WorldClient.h"
#include "MCHash.h"
#include "ChunkProviderClient.h"
#include "WorldProvider.h"
#include "SaveHandlerMP.h"
#include "WorldInfo.h"
#include "Entity.h"
#include "ChunkCoordinates.h"
#include "IWorldAccess.h"
#include "MapStorage.h"
#include "NetClientHandler.h"
#include "Minecraft.h"
#include "Packet255KickDisconnect.h"
#include <stdlib.h>
#include <string.h>

void EntitySet_init(EntitySet *self) {
    self->entries = NULL;
    self->count = 0;
    self->capacity = 0;
}

void EntitySet_destroy(EntitySet *self) {
    free(self->entries);
    self->entries = NULL;
    self->count = 0;
    self->capacity = 0;
}

void EntitySet_add(EntitySet *self, struct Entity *e) {
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i] == e)
            return;
    }
    if (self->count >= self->capacity) {
        self->capacity = self->capacity ? self->capacity * 2 : 8;
        self->entries =
            (struct Entity **)realloc(self->entries, self->capacity * sizeof(struct Entity *));
    }
    self->entries[self->count++] = e;
}

void EntitySet_remove(EntitySet *self, struct Entity *e) {
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i] == e) {
            self->entries[i] = self->entries[--self->count];
            return;
        }
    }
}

int EntitySet_contains(const EntitySet *self, struct Entity *e) {
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i] == e)
            return 1;
    }
    return 0;
}

int EntitySet_isEmpty(const EntitySet *self) { return self->count == 0; }

struct Entity *EntitySet_first(const EntitySet *self) {
    return self->count > 0 ? self->entries[0] : NULL;
}

void WorldBlockPositionList_init(WorldBlockPositionList *self) {
    self->entries = NULL;
    self->count = 0;
    self->capacity = 0;
}

void WorldBlockPositionList_destroy(WorldBlockPositionList *self) {
    free(self->entries);
    self->entries = NULL;
    self->count = 0;
    self->capacity = 0;
}

void WorldBlockPositionList_add(WorldBlockPositionList *self, WorldBlockPositionType item) {
    if (self->count >= self->capacity) {
        self->capacity = self->capacity ? self->capacity * 2 : 8;
        self->entries = (WorldBlockPositionType *)realloc(
            self->entries, self->capacity * sizeof(WorldBlockPositionType));
    }
    self->entries[self->count++] = item;
}

void WorldBlockPositionList_removeAt(WorldBlockPositionList *self, int idx) {
    memmove(&self->entries[idx], &self->entries[idx + 1],
            (self->count - idx - 1) * sizeof(WorldBlockPositionType));
    --self->count;
}

static void wc_vtable_obtainEntitySkin(World *self, struct Entity *var1);
static void wc_vtable_releaseEntitySkin(World *self, struct Entity *var1);
static void wc_vtable_updateWeather(World *self);
static IChunkProvider *wc_vtable_getChunkProvider(World *self);
static void wc_vtable_tick(World *self);
static void wc_vtable_setSpawnLocation(World *self);

void WorldClient_construct(WorldClient *self, struct NetClientHandler *var1, int64_t var2,
                           int var4) {

    static WorldVtable wc_vtable;
    static int wc_vtable_init = 0;
    if (!wc_vtable_init) {
        wc_vtable = World_defaultVtable;
        wc_vtable.getChunkProvider = wc_vtable_getChunkProvider;
        wc_vtable.obtainEntitySkin = wc_vtable_obtainEntitySkin;
        wc_vtable.releaseEntitySkin = wc_vtable_releaseEntitySkin;
        wc_vtable.updateWeather = wc_vtable_updateWeather;
        wc_vtable.tick = wc_vtable_tick;
        wc_vtable.setSpawnLocation = wc_vtable_setSpawnLocation;
        wc_vtable_init = 1;
    }
    ((World *)self)->vtable = &wc_vtable;

    SaveHandlerMP *saveHandler = SaveHandlerMP_create();
    WorldProvider *provider = WorldProvider_getProviderForDimension(var4);
    World_init_with_save_and_provider((World *)self, (ISaveHandler *)saveHandler, "MpServer",
                                      provider, var2);
    WorldBlockPositionList_init(&self->field_1057_z);
    self->sendQueue = var1;
    MCHash_init(&self->field_1055_D);
    EntitySet_init(&self->field_20914_E);
    EntitySet_init(&self->field_1053_F);
    ChunkCoordinates spawnPos;
    spawnPos.x = 8;
    spawnPos.y = 64;
    spawnPos.z = 8;
    World_setSpawnPoint((World *)self, &spawnPos);

    if (((World *)self)->field_28108_z) {
        MapStorage_destroy(((World *)self)->field_28108_z);
        free(((World *)self)->field_28108_z);
    }
    ((World *)self)->field_28108_z = &var1->field_28118_b;
}

WorldClient *WorldClient_create(struct NetClientHandler *var1, int64_t var2, int var4) {
    WorldClient *self = (WorldClient *)calloc(1, sizeof(WorldClient));
    WorldClient_construct(self, var1, var2, var4);
    return self;
}

void WorldClient_tick(WorldClient *self) {
    World *w = (World *)self;
    World_setWorldTime(w, World_getWorldTime(w) + 1L);
    int var1 = World_calculateSkylightSubtracted(w, 1.0F);
    int var2;
    if (var1 != w->skylightSubtracted) {
        w->skylightSubtracted = var1;
        for (var2 = 0; var2 < w->numWorldAccesses; ++var2) {
            IWorldAccess *wa = (IWorldAccess *)w->worldAccesses[var2];
            wa->vtable->updateAllRenderers(wa);
        }
    }

    for (var2 = 0; var2 < 10 && !EntitySet_isEmpty(&self->field_1053_F); ++var2) {
        struct Entity *var3 = EntitySet_first(&self->field_1053_F);

        if (var3->isDead) {
            EntitySet_remove(&self->field_1053_F, var3);
            EntitySet_remove(&self->field_20914_E, var3);
            continue;
        }

        int found = 0;
        for (int i = 0; i < w->numEntities; i++) {
            if (w->loadedEntityList[i] == var3) {
                found = 1;
                break;
            }
        }
        if (!found) {
            WorldClient_entityJoinedWorld(self, var3);
        }
    }

    NetClientHandler_processReadPackets(self->sendQueue);

    if (self->sendQueue == NULL || self->sendQueue->mc->theWorld != w)
        return;

    for (var2 = 0; var2 < self->field_1057_z.count; ++var2) {
        WorldBlockPositionType *var4 = &self->field_1057_z.entries[var2];
        if (--var4->field_1206_d == 0) {

            World_setBlockAndMetadata(w, var4->field_1202_a, var4->field_1201_b, var4->field_1207_c,
                                      var4->field_1205_e, var4->field_1204_f);

            World_markBlockNeedsUpdate(w, var4->field_1202_a, var4->field_1201_b,
                                       var4->field_1207_c);
            WorldBlockPositionList_removeAt(&self->field_1057_z, var2--);
        }
    }
}

void WorldClient_func_711_c(WorldClient *self, int var1, int var2, int var3, int var4, int var5,
                            int var6) {
    for (int var7 = 0; var7 < self->field_1057_z.count; ++var7) {
        WorldBlockPositionType *var8 = &self->field_1057_z.entries[var7];
        if (var8->field_1202_a >= var1 && var8->field_1201_b >= var2 &&
            var8->field_1207_c >= var3 && var8->field_1202_a <= var4 &&
            var8->field_1201_b <= var5 && var8->field_1207_c <= var6) {
            WorldBlockPositionList_removeAt(&self->field_1057_z, var7--);
        }
    }
}

ChunkProviderClient *WorldClient_getChunkProvider(WorldClient *self) {
    self->field_20915_C = (ChunkProviderClient *)calloc(1, sizeof(ChunkProviderClient));
    ChunkProviderClient_construct(self->field_20915_C, (World *)self);
    return self->field_20915_C;
}

static IChunkProvider *wc_vtable_getChunkProvider(World *self) {
    return (IChunkProvider *)WorldClient_getChunkProvider((WorldClient *)self);
}

void WorldClient_setSpawnLocation(WorldClient *self) {
    ChunkCoordinates spawnPos;
    spawnPos.x = 8;
    spawnPos.y = 64;
    spawnPos.z = 8;
    World_setSpawnPoint((World *)self, &spawnPos);
}

void WorldClient_updateBlocksAndPlayCaveSounds(WorldClient *self) { (void)self; }

void WorldClient_scheduleBlockUpdate(WorldClient *self, int var1, int var2, int var3, int var4,
                                     int var5) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    (void)var5;
}

int WorldClient_TickUpdates(WorldClient *self, int var1) {
    (void)self;
    (void)var1;
    return 0;
}

void WorldClient_doPreChunk(WorldClient *self, int var1, int var2, int var3) {
    if (var3) {
        ChunkProviderClient_prepareChunk(self->field_20915_C, var1, var2);
    } else {
        ChunkProviderClient_func_539_c(self->field_20915_C, var1, var2);
    }
    if (!var3) {
        World_markBlocksDirty((World *)self, var1 * 16, 0, var2 * 16, var1 * 16 + 15, 128,
                              var2 * 16 + 15);
    }
}

int WorldClient_entityJoinedWorld(WorldClient *self, struct Entity *var1) {
    int var2 = World_entityJoinedWorld((World *)self, var1);
    EntitySet_add(&self->field_20914_E, var1);
    if (!var2) {
        EntitySet_add(&self->field_1053_F, var1);
    }
    return var2;
}

void WorldClient_setEntityDead(WorldClient *self, struct Entity *var1) {
    World_setEntityDead((World *)self, var1);
    EntitySet_remove(&self->field_20914_E, var1);
}

static void WorldClient_obtainEntitySkin(WorldClient *self, struct Entity *var1) {
    World_obtainEntitySkin((World *)self, var1);
    if (EntitySet_contains(&self->field_1053_F, var1)) {
        EntitySet_remove(&self->field_1053_F, var1);
    }
}

static void WorldClient_releaseEntitySkin(WorldClient *self, struct Entity *var1) {
    World_releaseEntitySkin((World *)self, var1);

    MCHash_removeObject(&self->field_1055_D, var1->entityId);
    if (EntitySet_contains(&self->field_20914_E, var1)) {
        EntitySet_add(&self->field_1053_F, var1);
    }
}

void WorldClient_func_712_a(WorldClient *self, int var1, struct Entity *var2) {
    struct Entity *var3 = WorldClient_func_709_b(self, var1);
    if (var3 != NULL) {
        WorldClient_setEntityDead(self, var3);
    }
    EntitySet_add(&self->field_20914_E, var2);
    var2->entityId = var1;
    if (!WorldClient_entityJoinedWorld(self, var2)) {
        EntitySet_add(&self->field_1053_F, var2);
    }
    MCHash_addKey(&self->field_1055_D, var1, var2);
}

struct Entity *WorldClient_func_709_b(WorldClient *self, int var1) {
    return (struct Entity *)MCHash_lookup(&self->field_1055_D, var1);
}

struct Entity *WorldClient_removeEntityFromWorld(WorldClient *self, int var1) {
    struct Entity *var2 = (struct Entity *)MCHash_removeObject(&self->field_1055_D, var1);
    if (var2 != NULL) {
        EntitySet_remove(&self->field_20914_E, var2);
        WorldClient_setEntityDead(self, var2);
    }
    return var2;
}

int WorldClient_setBlockMetadata(WorldClient *self, int var1, int var2, int var3, int var4) {
    World *w = (World *)self;
    int var5 = World_getBlockId(w, var1, var2, var3);
    int var6 = World_getBlockMetadata(w, var1, var2, var3);
    if (World_setBlockMetadata(w, var1, var2, var3, var4)) {
        WorldBlockPositionType item;
        item.field_1202_a = var1;
        item.field_1201_b = var2;
        item.field_1207_c = var3;
        item.field_1206_d = 80;
        item.field_1205_e = var5;
        item.field_1204_f = var6;
        WorldBlockPositionList_add(&self->field_1057_z, item);
        return 1;
    }
    return 0;
}

int WorldClient_setBlockAndMetadata(WorldClient *self, int var1, int var2, int var3, int var4,
                                    int var5) {
    World *w = (World *)self;
    int var6 = World_getBlockId(w, var1, var2, var3);
    int var7 = World_getBlockMetadata(w, var1, var2, var3);
    if (World_setBlockAndMetadata(w, var1, var2, var3, var4, var5)) {
        WorldBlockPositionType item;
        item.field_1202_a = var1;
        item.field_1201_b = var2;
        item.field_1207_c = var3;
        item.field_1206_d = 80;
        item.field_1205_e = var6;
        item.field_1204_f = var7;
        WorldBlockPositionList_add(&self->field_1057_z, item);
        return 1;
    }
    return 0;
}

int WorldClient_setBlock(WorldClient *self, int var1, int var2, int var3, int var4) {
    World *w = (World *)self;
    int var5 = World_getBlockId(w, var1, var2, var3);
    int var6 = World_getBlockMetadata(w, var1, var2, var3);
    if (World_setBlock(w, var1, var2, var3, var4)) {
        WorldBlockPositionType item;
        item.field_1202_a = var1;
        item.field_1201_b = var2;
        item.field_1207_c = var3;
        item.field_1206_d = 80;
        item.field_1205_e = var5;
        item.field_1204_f = var6;
        WorldBlockPositionList_add(&self->field_1057_z, item);
        return 1;
    }
    return 0;
}

int WorldClient_func_714_c(WorldClient *self, int var1, int var2, int var3, int var4, int var5) {
    WorldClient_func_711_c(self, var1, var2, var3, var1, var2, var3);
    if (World_setBlockAndMetadata((World *)self, var1, var2, var3, var4, var5)) {
        World_notifyBlockChange((World *)self, var1, var2, var3, var4);
        return 1;
    }
    return 0;
}

void WorldClient_sendQuittingDisconnectingPacket(WorldClient *self) {
    NetClientHandler_func_28117_a(self->sendQueue, Packet255KickDisconnect_createWith("Quitting"));
}

static void WorldClient_updateWeather(WorldClient *self) {
    World *w = (World *)self;
    if (!w->worldProvider->hasNoSky) {
        if (w->field_27168_F > 0) {
            --w->field_27168_F;
        }

        w->prevRainingStrength = w->rainingStrength;
        if (WorldInfo_getRaining(w->worldInfo)) {
            w->rainingStrength = (float)((double)w->rainingStrength + 0.01);
        } else {
            w->rainingStrength = (float)((double)w->rainingStrength - 0.01);
        }
        if (w->rainingStrength < 0.0F) {
            w->rainingStrength = 0.0F;
        }
        if (w->rainingStrength > 1.0F) {
            w->rainingStrength = 1.0F;
        }

        w->prevThunderingStrength = w->thunderingStrength;
        if (WorldInfo_getThundering(w->worldInfo)) {
            w->thunderingStrength = (float)((double)w->thunderingStrength + 0.01);
        } else {
            w->thunderingStrength = (float)((double)w->thunderingStrength - 0.01);
        }
        if (w->thunderingStrength < 0.0F) {
            w->thunderingStrength = 0.0F;
        }
        if (w->thunderingStrength > 1.0F) {
            w->thunderingStrength = 1.0F;
        }
    }
}

static void wc_vtable_obtainEntitySkin(World *self, struct Entity *var1) {
    WorldClient_obtainEntitySkin((WorldClient *)self, var1);
}
static void wc_vtable_releaseEntitySkin(World *self, struct Entity *var1) {
    WorldClient_releaseEntitySkin((WorldClient *)self, var1);
}
static void wc_vtable_updateWeather(World *self) { WorldClient_updateWeather((WorldClient *)self); }
static void wc_vtable_tick(World *self) { WorldClient_tick((WorldClient *)self); }
static void wc_vtable_setSpawnLocation(World *self) {
    WorldClient_setSpawnLocation((WorldClient *)self);
}
