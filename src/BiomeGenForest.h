#ifndef BIOME_GEN_FOREST_H
#define BIOME_GEN_FOREST_H

#include "BiomeGenBase.h"

typedef struct BiomeGenForest {
    BiomeGenBase base;
} BiomeGenForest;

void BiomeGenForest_construct(BiomeGenForest *self);

#endif
