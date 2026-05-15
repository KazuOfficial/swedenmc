#include "WorldChunkManager.h"
#include "World.h"
#include <stdlib.h>
#include <string.h>

static BiomeGenBase *base_getBiomeGenAt(WorldChunkManager *self, int x, int z);
static BiomeGenBase **base_loadBiomeData(WorldChunkManager *self, BiomeGenBase **buf, int x, int z,
                                         int sizeX, int sizeZ);
static double base_getTemperature(WorldChunkManager *self, int x, int z);
static double *base_getTemperatures(WorldChunkManager *self, double *buf, int x, int z, int sizeX,
                                    int sizeZ);
static BiomeGenBase **base_func_4069_a(WorldChunkManager *self, int x, int z, int sizeX, int sizeZ);
static void base_destroy(WorldChunkManager *self);

WorldChunkManagerVtable WorldChunkManager_defaultVtable = {
    base_getBiomeGenAt,   base_loadBiomeData, base_getTemperature,
    base_getTemperatures, base_func_4069_a,   base_destroy,
};

WorldChunkManager *WorldChunkManager_create(void) {
    WorldChunkManager *self = (WorldChunkManager *)calloc(1, sizeof(WorldChunkManager));
    self->vtable = &WorldChunkManager_defaultVtable;
    return self;
}

WorldChunkManager *WorldChunkManager_create_world(World *world) {
    WorldChunkManager *self = WorldChunkManager_create();
    int64_t seed = (int64_t)World_getRandomSeed(world);
    JavaRandom r;
    JavaRandom_init(&r, (int64_t)((uint64_t)seed * 9871ULL));
    NoiseGeneratorOctaves2_init(&self->field_4194_e, &r, 4);
    JavaRandom_init(&r, (int64_t)((uint64_t)seed * 39811ULL));
    NoiseGeneratorOctaves2_init(&self->field_4193_f, &r, 4);
    JavaRandom_init(&r, (int64_t)((uint64_t)seed * 543321ULL));
    NoiseGeneratorOctaves2_init(&self->field_4192_g, &r, 2);
    return self;
}

void WorldChunkManager_destroy(WorldChunkManager *self) {
    if (!self)
        return;
    self->vtable->destroy(self);
}

static void ensure_scratch(WorldChunkManager *self, int len) {
    if (self->scratch_size < len) {
        free(self->temperature);
        free(self->humidity);
        free(self->field_4196_c);
        self->temperature = (double *)malloc((size_t)len * sizeof(double));
        self->humidity = (double *)malloc((size_t)len * sizeof(double));
        self->field_4196_c = (double *)malloc((size_t)len * sizeof(double));
        self->scratch_size = len;
    }
}

BiomeGenBase *WorldChunkManager_getBiomeGenAt(WorldChunkManager *self, int x, int z) {
    return self->vtable->getBiomeGenAt(self, x, z);
}

BiomeGenBase **WorldChunkManager_loadBiomeData(WorldChunkManager *self, BiomeGenBase **buf, int x,
                                               int z, int sizeX, int sizeZ) {
    return self->vtable->loadBiomeData(self, buf, x, z, sizeX, sizeZ);
}

double WorldChunkManager_getTemperature(WorldChunkManager *self, int x, int z) {
    return self->vtable->getTemperature(self, x, z);
}

double *WorldChunkManager_getTemperatures(WorldChunkManager *self, double *buf, int x, int z,
                                          int sizeX, int sizeZ) {
    return self->vtable->getTemperatures(self, buf, x, z, sizeX, sizeZ);
}

BiomeGenBase **WorldChunkManager_func_4069_a(WorldChunkManager *self, int x, int z, int sizeX,
                                             int sizeZ) {
    return self->vtable->func_4069_a(self, x, z, sizeX, sizeZ);
}

static BiomeGenBase *base_getBiomeGenAt(WorldChunkManager *self, int x, int z) {
    return self->vtable->func_4069_a(self, x, z, 1, 1)[0];
}

static BiomeGenBase **base_loadBiomeData(WorldChunkManager *self, BiomeGenBase **var1, int var2,
                                         int var3, int var4, int var5) {
    int len = var4 * var5;
    if (var1 == NULL) {
        var1 = (BiomeGenBase **)malloc((size_t)len * sizeof(BiomeGenBase *));
    }
    ensure_scratch(self, len);

    self->temperature = NoiseGeneratorOctaves2_func_4112_a(&self->field_4194_e, self->temperature,
                                                           (double)var2, (double)var3, var4, var4,
                                                           (double)0.025f, (double)0.025f, 0.25);

    self->humidity = NoiseGeneratorOctaves2_func_4112_a(&self->field_4193_f, self->humidity,
                                                        (double)var2, (double)var3, var4, var4,
                                                        (double)0.05f, (double)0.05f, 1.0 / 3.0);

    self->field_4196_c = NoiseGeneratorOctaves2_func_4112_a(&self->field_4192_g, self->field_4196_c,
                                                            (double)var2, (double)var3, var4, var4,
                                                            0.25, 0.25, 0.5882352941176471);

    int var6 = 0;
    for (int var7 = 0; var7 < var4; ++var7) {
        for (int var8 = 0; var8 < var5; ++var8) {
            double var9 = self->field_4196_c[var6] * 1.1 + 0.5;
            double var11 = 0.01;
            double var13 = 1.0 - var11;
            double var15 = (self->temperature[var6] * 0.15 + 0.7) * var13 + var9 * var11;
            var11 = 0.002;
            var13 = 1.0 - var11;
            double var17 = (self->humidity[var6] * 0.15 + 0.5) * var13 + var9 * var11;
            var15 = 1.0 - (1.0 - var15) * (1.0 - var15);
            if (var15 < 0.0)
                var15 = 0.0;
            if (var17 < 0.0)
                var17 = 0.0;
            if (var15 > 1.0)
                var15 = 1.0;
            if (var17 > 1.0)
                var17 = 1.0;
            self->temperature[var6] = var15;
            self->humidity[var6] = var17;
            var1[var6++] = BiomeGenBase_getBiomeFromLookup(var15, var17);
        }
    }

    return var1;
}

static double base_getTemperature(WorldChunkManager *self, int x, int z) {
    ensure_scratch(self, 1);
    self->temperature =
        NoiseGeneratorOctaves2_func_4112_a(&self->field_4194_e, self->temperature, (double)x,
                                           (double)z, 1, 1, (double)0.025f, (double)0.025f, 0.5);
    return self->temperature[0];
}

static double *base_getTemperatures(WorldChunkManager *self, double *var1, int var2, int var3,
                                    int var4, int var5) {
    int len = var4 * var5;
    if (var1 == NULL) {
        var1 = (double *)malloc((size_t)len * sizeof(double));
    }
    ensure_scratch(self, len);

    var1 = NoiseGeneratorOctaves2_func_4112_a(&self->field_4194_e, var1, (double)var2, (double)var3,
                                              var4, var5, (double)0.025f, (double)0.025f, 0.25);
    self->field_4196_c = NoiseGeneratorOctaves2_func_4112_a(&self->field_4192_g, self->field_4196_c,
                                                            (double)var2, (double)var3, var4, var5,
                                                            0.25, 0.25, 0.5882352941176471);

    int var6 = 0;
    for (int var7 = 0; var7 < var4; ++var7) {
        for (int var8 = 0; var8 < var5; ++var8) {
            double var9 = self->field_4196_c[var6] * 1.1 + 0.5;
            double var11 = 0.01;
            double var13 = 1.0 - var11;
            double var15 = (var1[var6] * 0.15 + 0.7) * var13 + var9 * var11;
            var15 = 1.0 - (1.0 - var15) * (1.0 - var15);
            if (var15 < 0.0)
                var15 = 0.0;
            if (var15 > 1.0)
                var15 = 1.0;
            var1[var6] = var15;
            ++var6;
        }
    }

    return var1;
}

static BiomeGenBase **base_func_4069_a(WorldChunkManager *self, int x, int z, int sizeX,
                                       int sizeZ) {
    int len = sizeX * sizeZ;
    if (self->field_4195_d_size < len) {
        free(self->field_4195_d);
        self->field_4195_d = (BiomeGenBase **)malloc((size_t)len * sizeof(BiomeGenBase *));
        self->field_4195_d_size = len;
    }
    self->field_4195_d = self->vtable->loadBiomeData(self, self->field_4195_d, x, z, sizeX, sizeZ);
    return self->field_4195_d;
}

static void base_destroy(WorldChunkManager *self) {
    NoiseGeneratorOctaves2_free(&self->field_4194_e);
    NoiseGeneratorOctaves2_free(&self->field_4193_f);
    NoiseGeneratorOctaves2_free(&self->field_4192_g);
    free(self->temperature);
    free(self->humidity);
    free(self->field_4196_c);
    free(self->field_4195_d);
    free(self);
}

BiomeGenBase *WorldChunkManager_getBiomeGenAtChunkCoord(WorldChunkManager *self, int chunkX,
                                                        int chunkZ) {
    return WorldChunkManager_getBiomeGenAt(self, (int)((unsigned int)chunkX << 4),
                                           (int)((unsigned int)chunkZ << 4));
}

void WorldChunkManager_getSingleBiomeData(WorldChunkManager *self, int x, int z,
                                          double *out_temperature, double *out_humidity) {
    double temp_noise = 0.0, hum_noise = 0.0, field_c = 0.0;

    NoiseGeneratorOctaves2_func_4112_a(&self->field_4194_e, &temp_noise, (double)x, (double)z, 1, 1,
                                       (double)0.025f, (double)0.025f, 0.25);
    NoiseGeneratorOctaves2_func_4112_a(&self->field_4193_f, &hum_noise, (double)x, (double)z, 1, 1,
                                       (double)0.05f, (double)0.05f, 1.0 / 3.0);
    NoiseGeneratorOctaves2_func_4112_a(&self->field_4192_g, &field_c, (double)x, (double)z, 1, 1,
                                       0.25, 0.25, 0.5882352941176471);

    double var9 = field_c * 1.1 + 0.5;
    double var11 = 0.01;
    double var13 = 1.0 - var11;
    double temperature = (temp_noise * 0.15 + 0.7) * var13 + var9 * var11;
    var11 = 0.002;
    var13 = 1.0 - var11;
    double humidity = (hum_noise * 0.15 + 0.5) * var13 + var9 * var11;
    temperature = 1.0 - (1.0 - temperature) * (1.0 - temperature);
    if (temperature < 0.0)
        temperature = 0.0;
    if (humidity < 0.0)
        humidity = 0.0;
    if (temperature > 1.0)
        temperature = 1.0;
    if (humidity > 1.0)
        humidity = 1.0;

    *out_temperature = temperature;
    *out_humidity = humidity;
}
