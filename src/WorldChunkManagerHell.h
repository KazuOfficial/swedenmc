#ifndef WORLD_CHUNK_MANAGER_HELL_H
#define WORLD_CHUNK_MANAGER_HELL_H

#include "WorldChunkManager.h"
#include "BiomeGenBase.h"

typedef struct WorldChunkManagerHell {
    WorldChunkManager base;

    BiomeGenBase *field_4201_e;

    double field_4200_f;

    double field_4199_g;
} WorldChunkManagerHell;

WorldChunkManagerHell *WorldChunkManagerHell_create(BiomeGenBase *biome, double temperature,
                                                    double humidity);

extern WorldChunkManagerVtable WorldChunkManagerHell_vtable;

#endif
