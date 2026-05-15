#include "WorldProviderSky.h"
#include "WorldChunkManagerHell.h"
#include "ChunkProviderSky.h"
#include "BiomeGenBase.h"
#include "Block.h"
#include "Material.h"
#include "World.h"
#include <stdlib.h>
#include <math.h>

static void sky_registerWorldChunkManager(WorldProvider *self) {
    if (self->worldChunkMgr)
        WorldChunkManager_destroy(self->worldChunkMgr);
    self->worldChunkMgr = (WorldChunkManager *)WorldChunkManagerHell_create(
        &BiomeGenBase_biomes[BIOME_SKY], 0.5, 0.0);
    self->worldType = 1;
}

static void *sky_getChunkProvider(WorldProvider *self) {

    return (void *)ChunkProviderSky_create(self->worldObj,
                                           (long)World_getRandomSeed(self->worldObj));
}

static float sky_calculateCelestialAngle(WorldProvider *self, int64_t worldTime,
                                         float partialTick) {
    (void)self;
    (void)worldTime;
    (void)partialTick;
    return 0.0f;
}

static float *sky_calcSunriseSunsetColors(WorldProvider *self, float angle, float partialTick) {
    (void)self;
    (void)angle;
    (void)partialTick;
    return NULL;
}

static Vec3D *sky_func_4096_a(WorldProvider *self, float angle, float partialTick) {
    (void)self;
    (void)partialTick;
    int var3 = 8421536;
    float var4 = cosf(angle * (float)M_PI * 2.0f) * 2.0f + 0.5f;
    if (var4 < 0.0f)
        var4 = 0.0f;
    if (var4 > 1.0f)
        var4 = 1.0f;
    float var5 = (float)((var3 >> 16) & 255) / 255.0f;
    float var6 = (float)((var3 >> 8) & 255) / 255.0f;
    float var7 = (float)(var3 & 255) / 255.0f;
    var5 *= var4 * 0.94f + 0.06f;
    var6 *= var4 * 0.94f + 0.06f;
    var7 *= var4 * 0.91f + 0.09f;
    return Vec3D_createVector((double)var5, (double)var6, (double)var7);
}

static int sky_func_28112_c(WorldProvider *self) {
    (void)self;
    return 0;
}

static float sky_getCloudHeight(WorldProvider *self) {
    (void)self;
    return 8.0f;
}

static int sky_canCoordinateBeSpawn(WorldProvider *self, int x, int z) {

    int var3 = World_getFirstUncoveredBlock(self->worldObj, x, z);
    if (var3 == 0 || !Block_blocksList[var3])
        return 0;
    return Material_getIsSolid(Block_blocksList[var3]->blockMaterial);
}

WorldProviderVtable WorldProviderSky_vtable;

static void __attribute__((constructor)) WorldProviderSky_initVtable(void) {
    WorldProviderSky_vtable = WorldProvider_defaultVtable;
    WorldProviderSky_vtable.registerWorldChunkManager = sky_registerWorldChunkManager;
    WorldProviderSky_vtable.getChunkProvider = sky_getChunkProvider;
    WorldProviderSky_vtable.calculateCelestialAngle = sky_calculateCelestialAngle;
    WorldProviderSky_vtable.calcSunriseSunsetColors = sky_calcSunriseSunsetColors;
    WorldProviderSky_vtable.func_4096_a = sky_func_4096_a;
    WorldProviderSky_vtable.func_28112_c = sky_func_28112_c;
    WorldProviderSky_vtable.getCloudHeight = sky_getCloudHeight;
    WorldProviderSky_vtable.canCoordinateBeSpawn = sky_canCoordinateBeSpawn;
}

WorldProvider *WorldProviderSky_create(void) {
    WorldProvider *self = (WorldProvider *)calloc(1, sizeof(WorldProvider));
    self->vtable = &WorldProviderSky_vtable;
    return self;
}
