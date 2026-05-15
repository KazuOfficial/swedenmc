#ifndef CHUNK_PROVIDER_LOAD_OR_GENERATE_H
#define CHUNK_PROVIDER_LOAD_OR_GENERATE_H

#include "IChunkProvider.h"
#include "IChunkLoader.h"
#include "IProgressUpdate.h"
#include "GameForward.h"

#define CPLG_CACHE_SIZE 1024

typedef struct ChunkProviderLoadOrGenerate {
    IChunkProvider base;

    Chunk *blankChunk;
    IChunkProvider *chunkProvider;
    IChunkLoader *chunkLoader;
    Chunk *chunks[CPLG_CACHE_SIZE];
    World *worldObj;

    int lastQueriedChunkXPos;
    int lastQueriedChunkZPos;
    Chunk *lastQueriedChunk;

    int curChunkX;
    int curChunkY;
} ChunkProviderLoadOrGenerate;

ChunkProviderLoadOrGenerate *ChunkProviderLoadOrGenerate_create(World *world, IChunkLoader *loader,
                                                                IChunkProvider *generator);
void ChunkProviderLoadOrGenerate_destroy(ChunkProviderLoadOrGenerate *self);

void ChunkProviderLoadOrGenerate_setCurrentChunkOver(ChunkProviderLoadOrGenerate *self, int var1,
                                                     int var2);

int ChunkProviderLoadOrGenerate_canChunkExist(ChunkProviderLoadOrGenerate *self, int var1,
                                              int var2);

extern const IChunkProviderVtable ChunkProviderLoadOrGenerate_vtable;

#endif
