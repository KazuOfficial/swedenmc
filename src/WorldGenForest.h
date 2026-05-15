#ifndef WORLD_GEN_FOREST_H
#define WORLD_GEN_FOREST_H

#include "WorldGenerator.h"

typedef struct WorldGenForest {
    WorldGenerator base;
} WorldGenForest;

WorldGenForest *WorldGenForest_create(void);
void WorldGenForest_free(WorldGenForest *self);

#endif
