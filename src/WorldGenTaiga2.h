#ifndef WORLD_GEN_TAIGA2_H
#define WORLD_GEN_TAIGA2_H

#include "WorldGenerator.h"

typedef struct WorldGenTaiga2 {
    WorldGenerator base;
} WorldGenTaiga2;

WorldGenTaiga2 *WorldGenTaiga2_create(void);
void WorldGenTaiga2_free(WorldGenTaiga2 *self);

#endif
