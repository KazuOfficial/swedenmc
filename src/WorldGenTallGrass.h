#ifndef WORLD_GEN_TALL_GRASS_H
#define WORLD_GEN_TALL_GRASS_H

#include "WorldGenerator.h"

typedef struct WorldGenTallGrass {
    WorldGenerator base;
    int field_44_a;
    int field_43_b;
} WorldGenTallGrass;

WorldGenTallGrass *WorldGenTallGrass_create(int blockId, int meta);
void WorldGenTallGrass_free(WorldGenTallGrass *self);

#endif
