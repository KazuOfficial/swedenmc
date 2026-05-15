#ifndef CHUNK_PROVIDER_H
#define CHUNK_PROVIDER_H

#include "IChunkProvider.h"
#include "IChunkLoader.h"
#include "IProgressUpdate.h"
#include "GameForward.h"

typedef struct {
    int key;
    Chunk *chunk;
    int occupied;
} ChunkMapSlot;

typedef struct ChunkProvider {
    IChunkProvider base;

    Chunk *field_28064_b;
    IChunkProvider *chunkProvider;
    IChunkLoader *chunkLoader;
    World *field_28066_g;

    ChunkMapSlot *chunkMap;
    int chunkMapCap;
    int chunkMapCount;

    Chunk **chunkList;
    int chunkListCount;
    int chunkListCap;

    int *droppedChunksSet;
    int droppedChunksCount;
    int droppedChunksCap;
} ChunkProvider;

ChunkProvider *ChunkProvider_create(World *var1, IChunkLoader *var2, IChunkProvider *var3);
void ChunkProvider_destroy(ChunkProvider *self);

#endif
