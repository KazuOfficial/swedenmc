#include "WorldProviderHell.h"
#include "WorldChunkManagerHell.h"
#include "ChunkProviderHell.h"
#include "BiomeGenBase.h"
#include "Block.h"
#include "World.h"
#include <stdlib.h>
#include <math.h>

static void hell_registerWorldChunkManager(WorldProvider *self) {
    if (self->worldChunkMgr)
        WorldChunkManager_destroy(self->worldChunkMgr);
    self->worldChunkMgr = (WorldChunkManager *)WorldChunkManagerHell_create(
        &BiomeGenBase_biomes[BIOME_HELL], 1.0, 0.0);
    self->isNether = 1;
    self->isHellWorld = 1;
    self->hasNoSky = 1;
    self->worldType = -1;
}

static Vec3D *hell_func_4096_a(WorldProvider *self, float angle, float partialTick) {
    (void)self;
    (void)angle;
    (void)partialTick;
    return Vec3D_createVector((double)0.2f, (double)0.03f, (double)0.03f);
}

static void hell_generateLightBrightnessTable(WorldProvider *self) {
    float var1 = 0.1f;
    for (int var2 = 0; var2 <= 15; ++var2) {
        float var3 = 1.0f - (float)var2 / 15.0f;
        self->lightBrightnessTable[var2] =
            (1.0f - var3) / (var3 * 3.0f + 1.0f) * (1.0f - var1) + var1;
    }
}

static void *hell_getChunkProvider(WorldProvider *self) {

    return (void *)ChunkProviderHell_create(self->worldObj,
                                            (long)World_getRandomSeed(self->worldObj));
}

static int hell_canCoordinateBeSpawn(WorldProvider *self, int x, int z) {

    int var3 = World_getFirstUncoveredBlock(self->worldObj, x, z);
    if (var3 == Block_bedrock->blockID)
        return 0;
    if (var3 == 0)
        return 0;
    return Block_opaqueCubeLookup[var3];
}

static float hell_calculateCelestialAngle(WorldProvider *self, int64_t worldTime,
                                          float partialTick) {
    (void)self;
    (void)worldTime;
    (void)partialTick;
    return 0.5f;
}

static int hell_canRespawnHere(WorldProvider *self) {
    (void)self;
    return 0;
}

WorldProviderVtable WorldProviderHell_vtable;

static void __attribute__((constructor)) WorldProviderHell_initVtable(void) {
    WorldProviderHell_vtable = WorldProvider_defaultVtable;
    WorldProviderHell_vtable.registerWorldChunkManager = hell_registerWorldChunkManager;
    WorldProviderHell_vtable.func_4096_a = hell_func_4096_a;
    WorldProviderHell_vtable.generateLightBrightnessTable = hell_generateLightBrightnessTable;
    WorldProviderHell_vtable.getChunkProvider = hell_getChunkProvider;
    WorldProviderHell_vtable.canCoordinateBeSpawn = hell_canCoordinateBeSpawn;
    WorldProviderHell_vtable.calculateCelestialAngle = hell_calculateCelestialAngle;
    WorldProviderHell_vtable.canRespawnHere = hell_canRespawnHere;
}

WorldProvider *WorldProviderHell_create(void) {
    WorldProvider *self = (WorldProvider *)calloc(1, sizeof(WorldProvider));
    self->vtable = &WorldProviderHell_vtable;
    return self;
}
