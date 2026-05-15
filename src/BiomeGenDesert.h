#ifndef BIOME_GEN_DESERT_H
#define BIOME_GEN_DESERT_H

#include "BiomeGenBase.h"

typedef struct BiomeGenDesert {
    BiomeGenBase base;
} BiomeGenDesert;

void BiomeGenDesert_construct(BiomeGenDesert *self);

#endif
