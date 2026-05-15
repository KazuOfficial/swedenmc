#ifndef WORLD_GEN_LIQUIDS_H
#define WORLD_GEN_LIQUIDS_H

#include "WorldGenerator.h"

typedef struct WorldGenLiquids {
    WorldGenerator base;
    int field_36_a;
} WorldGenLiquids;

WorldGenLiquids *WorldGenLiquids_create(int blockId);
void WorldGenLiquids_free(WorldGenLiquids *self);

#endif
