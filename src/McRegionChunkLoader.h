#ifndef MC_REGION_CHUNK_LOADER_H
#define MC_REGION_CHUNK_LOADER_H

#include "IChunkLoader.h"
#include "GameForward.h"

typedef struct McRegionChunkLoader {
    IChunkLoader base;
    char *worldDir;
} McRegionChunkLoader;

McRegionChunkLoader *McRegionChunkLoader_create(const char *worldDir);
void McRegionChunkLoader_destroy(McRegionChunkLoader *self);

#endif
