#ifndef WORLD_GEN_TREES_H
#define WORLD_GEN_TREES_H

#include "WorldGenerator.h"

typedef struct WorldGenTrees {
    WorldGenerator base;
} WorldGenTrees;

WorldGenTrees *WorldGenTrees_create(void);
void WorldGenTrees_free(WorldGenTrees *self);

#endif
