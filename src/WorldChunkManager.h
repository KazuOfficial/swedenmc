#ifndef WORLD_CHUNK_MANAGER_H
#define WORLD_CHUNK_MANAGER_H

#include "NoiseGeneratorOctaves2.h"
#include "BiomeGenBase.h"
#include "JavaRandom.h"
#include "GameForward.h"

typedef struct WorldChunkManager WorldChunkManager;

typedef struct WorldChunkManagerVtable {
    BiomeGenBase *(*getBiomeGenAt)(WorldChunkManager *self, int x, int z);
    BiomeGenBase **(*loadBiomeData)(WorldChunkManager *self, BiomeGenBase **buf, int x, int z,
                                    int sizeX, int sizeZ);
    double (*getTemperature)(WorldChunkManager *self, int x, int z);
    double *(*getTemperatures)(WorldChunkManager *self, double *buf, int x, int z, int sizeX,
                               int sizeZ);
    BiomeGenBase **(*func_4069_a)(WorldChunkManager *self, int x, int z, int sizeX, int sizeZ);
    void (*destroy)(WorldChunkManager *self);
} WorldChunkManagerVtable;

struct WorldChunkManager {
    const WorldChunkManagerVtable *vtable;

    NoiseGeneratorOctaves2 field_4194_e;
    NoiseGeneratorOctaves2 field_4193_f;
    NoiseGeneratorOctaves2 field_4192_g;

    double *temperature;
    double *humidity;
    double *field_4196_c;
    int scratch_size;

    BiomeGenBase **field_4195_d;
    int field_4195_d_size;
};

extern WorldChunkManagerVtable WorldChunkManager_defaultVtable;

WorldChunkManager *WorldChunkManager_create(void);

WorldChunkManager *WorldChunkManager_create_world(World *world);
void WorldChunkManager_destroy(WorldChunkManager *self);
static inline void WorldChunkManager_free(WorldChunkManager *self) {
    WorldChunkManager_destroy(self);
}

BiomeGenBase *WorldChunkManager_getBiomeGenAt(WorldChunkManager *self, int x, int z);
BiomeGenBase **WorldChunkManager_loadBiomeData(WorldChunkManager *self, BiomeGenBase **buf, int x,
                                               int z, int sizeX, int sizeZ);
double WorldChunkManager_getTemperature(WorldChunkManager *self, int x, int z);
double *WorldChunkManager_getTemperatures(WorldChunkManager *self, double *buf, int x, int z,
                                          int sizeX, int sizeZ);
BiomeGenBase **WorldChunkManager_func_4069_a(WorldChunkManager *self, int x, int z, int sizeX,
                                             int sizeZ);

BiomeGenBase *WorldChunkManager_getBiomeGenAtChunkCoord(WorldChunkManager *self, int chunkX,
                                                        int chunkZ);

void WorldChunkManager_getSingleBiomeData(WorldChunkManager *self, int x, int z,
                                          double *out_temperature, double *out_humidity);

#endif
