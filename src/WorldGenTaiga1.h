#ifndef WORLD_GEN_TAIGA1_H
#define WORLD_GEN_TAIGA1_H

#include "WorldGenerator.h"

typedef struct WorldGenTaiga1 {
    WorldGenerator base;
} WorldGenTaiga1;

WorldGenTaiga1 *WorldGenTaiga1_create(void);
void WorldGenTaiga1_free(WorldGenTaiga1 *self);

#endif
