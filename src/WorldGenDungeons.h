#ifndef WORLD_GEN_DUNGEONS_H
#define WORLD_GEN_DUNGEONS_H

#include "WorldGenerator.h"

typedef struct WorldGenDungeons {
    WorldGenerator base;
} WorldGenDungeons;

WorldGenDungeons *WorldGenDungeons_create(void);
void WorldGenDungeons_free(WorldGenDungeons *self);

#endif
