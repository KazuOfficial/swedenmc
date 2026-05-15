#include "WorldProvider.h"
#include "ChunkProviderGenerate.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>
#include <math.h>

WorldProvider *WorldProviderSurface_create(void);
WorldProvider *WorldProviderHell_create(void);
WorldProvider *WorldProviderSky_create(void);

static void def_registerWorldChunkManager(WorldProvider *self) {

    if (self->worldChunkMgr)
        WorldChunkManager_destroy(self->worldChunkMgr);
    self->worldChunkMgr = WorldChunkManager_create_world(self->worldObj);
}

static void def_generateLightBrightnessTable(WorldProvider *self) {

    float var1 = 0.05f;
    for (int var2 = 0; var2 <= 15; ++var2) {
        float var3 = 1.0f - (float)var2 / 15.0f;
        self->lightBrightnessTable[var2] =
            (1.0f - var3) / (var3 * 3.0f + 1.0f) * (1.0f - var1) + var1;
    }
}

static void *def_getChunkProvider(WorldProvider *self) {

    return (void *)ChunkProviderGenerate_create(self->worldObj,
                                                (long)World_getRandomSeed(self->worldObj));
}

static int def_canCoordinateBeSpawn(WorldProvider *self, int x, int z) {

    int var3 = World_getFirstUncoveredBlock(self->worldObj, x, z);
    return var3 == Block_sand->blockID;
}

static float def_calculateCelestialAngle(WorldProvider *self, int64_t worldTime,
                                         float partialTick) {
    (void)self;
    int var4 = (int)(worldTime % 24000LL);
    float var5 = ((float)var4 + partialTick) / 24000.0f - 0.25f;
    if (var5 < 0.0f)
        ++var5;
    if (var5 > 1.0f)
        --var5;
    float var6 = var5;
    var5 = 1.0f - (float)((cos((double)var5 * M_PI) + 1.0) / 2.0);
    var5 = var6 + (var5 - var6) / 3.0f;
    return var5;
}

static float *def_calcSunriseSunsetColors(WorldProvider *self, float angle, float partialTick) {
    (void)partialTick;
    float var3 = 0.4f;
    float var4 = cosf(angle * (float)M_PI * 2.0f) - 0.0f;
    float var5 = -0.0f;
    if (var4 >= var5 - var3 && var4 <= var5 + var3) {
        float var6 = (var4 - var5) / var3 * 0.5f + 0.5f;
        float var7 = 1.0f - (1.0f - sinf(var6 * (float)M_PI)) * 0.99f;
        var7 *= var7;
        self->colorsSunriseSunset[0] = var6 * 0.3f + 0.7f;
        self->colorsSunriseSunset[1] = var6 * var6 * 0.7f + 0.2f;
        self->colorsSunriseSunset[2] = var6 * var6 * 0.0f + 0.2f;
        self->colorsSunriseSunset[3] = var7;
        return self->colorsSunriseSunset;
    } else {
        return NULL;
    }
}

static Vec3D *def_func_4096_a(WorldProvider *self, float angle, float partialTick) {
    (void)self;
    (void)partialTick;
    float var3 = cosf(angle * (float)M_PI * 2.0f) * 2.0f + 0.5f;
    if (var3 < 0.0f)
        var3 = 0.0f;
    if (var3 > 1.0f)
        var3 = 1.0f;
    float var4 = 192.0f / 255.0f;
    float var5 = 216.0f / 255.0f;
    float var6 = 1.0f;
    var4 *= var3 * 0.94f + 0.06f;
    var5 *= var3 * 0.94f + 0.06f;
    var6 *= var3 * 0.91f + 0.09f;
    return Vec3D_createVector((double)var4, (double)var5, (double)var6);
}

static int def_canRespawnHere(WorldProvider *self) {
    (void)self;
    return 1;
}

static float def_getCloudHeight(WorldProvider *self) {
    (void)self;
    return 108.0f;
}

static int def_func_28112_c(WorldProvider *self) {
    (void)self;
    return 1;
}

static void def_destroy(WorldProvider *self) {
    if (self->worldChunkMgr)
        WorldChunkManager_destroy(self->worldChunkMgr);
    free(self);
}

WorldProviderVtable WorldProvider_defaultVtable = {
    def_registerWorldChunkManager,
    def_generateLightBrightnessTable,
    def_getChunkProvider,
    def_canCoordinateBeSpawn,
    def_calculateCelestialAngle,
    def_calcSunriseSunsetColors,
    def_func_4096_a,
    def_canRespawnHere,
    def_getCloudHeight,
    def_func_28112_c,
    def_destroy,
};

void WorldProvider_registerWorld(WorldProvider *self, World *world) {
    self->worldObj = world;
    self->vtable->registerWorldChunkManager(self);
    self->vtable->generateLightBrightnessTable(self);
}

WorldProvider *WorldProvider_getProviderForDimension(int dim) {
    if (dim == -1)
        return WorldProviderHell_create();
    if (dim == 0)
        return WorldProviderSurface_create();
    if (dim == 1)
        return WorldProviderSky_create();
    return NULL;
}
