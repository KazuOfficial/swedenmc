#include "WorldChunkManagerHell.h"
#include <stdlib.h>
#include <string.h>

static BiomeGenBase *hell_getBiomeGenAt(WorldChunkManager *base, int x, int z) {
    (void)x;
    (void)z;
    return ((WorldChunkManagerHell *)base)->field_4201_e;
}

static BiomeGenBase **hell_loadBiomeData(WorldChunkManager *base, BiomeGenBase **var1, int var2,
                                         int var3, int var4, int var5) {
    (void)var2;
    (void)var3;
    WorldChunkManagerHell *self = (WorldChunkManagerHell *)base;
    int len = var4 * var5;

    if (var1 == NULL) {
        var1 = (BiomeGenBase **)malloc((size_t)len * sizeof(BiomeGenBase *));
    }

    if (base->scratch_size < len) {
        free(base->temperature);
        free(base->humidity);
        base->temperature = (double *)malloc((size_t)len * sizeof(double));
        base->humidity = (double *)malloc((size_t)len * sizeof(double));
        base->scratch_size = len;
    }

    for (int i = 0; i < len; i++) {
        var1[i] = self->field_4201_e;
        base->humidity[i] = self->field_4199_g;
        base->temperature[i] = self->field_4200_f;
    }

    return var1;
}

static double hell_getTemperature(WorldChunkManager *base, int x, int z) {
    (void)x;
    (void)z;
    return ((WorldChunkManagerHell *)base)->field_4200_f;
}

static double *hell_getTemperatures(WorldChunkManager *base, double *var1, int var2, int var3,
                                    int var4, int var5) {
    (void)var2;
    (void)var3;
    WorldChunkManagerHell *self = (WorldChunkManagerHell *)base;
    int len = var4 * var5;
    if (var1 == NULL) {
        var1 = (double *)malloc((size_t)len * sizeof(double));
    }
    for (int i = 0; i < len; i++) {
        var1[i] = self->field_4200_f;
    }
    return var1;
}

static BiomeGenBase **hell_func_4069_a(WorldChunkManager *base, int x, int z, int sizeX,
                                       int sizeZ) {
    int len = sizeX * sizeZ;
    if (base->field_4195_d_size < len) {
        free(base->field_4195_d);
        base->field_4195_d = (BiomeGenBase **)malloc((size_t)len * sizeof(BiomeGenBase *));
        base->field_4195_d_size = len;
    }
    base->field_4195_d = hell_loadBiomeData(base, base->field_4195_d, x, z, sizeX, sizeZ);
    return base->field_4195_d;
}

static void hell_destroy(WorldChunkManager *base) {
    free(base->temperature);
    free(base->humidity);
    free(base->field_4196_c);
    free(base->field_4195_d);
    free(base);
}

WorldChunkManagerVtable WorldChunkManagerHell_vtable = {
    hell_getBiomeGenAt,   hell_loadBiomeData, hell_getTemperature,
    hell_getTemperatures, hell_func_4069_a,   hell_destroy,
};

WorldChunkManagerHell *WorldChunkManagerHell_create(BiomeGenBase *biome, double temperature,
                                                    double humidity) {
    WorldChunkManagerHell *self = (WorldChunkManagerHell *)calloc(1, sizeof(WorldChunkManagerHell));
    self->base.vtable = &WorldChunkManagerHell_vtable;
    self->field_4201_e = biome;
    self->field_4200_f = temperature;
    self->field_4199_g = humidity;
    return self;
}
