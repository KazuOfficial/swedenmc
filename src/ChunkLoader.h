#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "IChunkLoader.h"
#include "GameForward.h"
#include "NBTTagCompound.h"

typedef struct ChunkLoader {
    IChunkLoader base;
    char *saveDir;
    int createIfNecessary;
} ChunkLoader;

ChunkLoader *ChunkLoader_create(const char *saveDir, int var2);
void ChunkLoader_destroy(ChunkLoader *self);

char *ChunkLoader_chunkFileForXZ(ChunkLoader *self, int x, int z);

void ChunkLoader_storeChunkInCompound(Chunk *chunk, World *world, NBTTagCompound *tag);

Chunk *ChunkLoader_loadChunkIntoWorldFromCompound(World *world, NBTTagCompound *tag);

#endif
