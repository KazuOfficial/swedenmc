#ifndef CHUNK_PROVIDER_CLIENT_H
#define CHUNK_PROVIDER_CLIENT_H

#include "IChunkProvider.h"
#include "Chunk.h"
#include "ChunkCoordIntPair.h"
#include "World.h"
#include "IProgressUpdate.h"

#define CHUNK_MAP_BUCKETS 16

typedef struct ChunkMapEntry {
    ChunkCoordIntPair key;
    Chunk *value;
    struct ChunkMapEntry *next;
} ChunkMapEntry;

typedef struct {
    ChunkMapEntry *buckets[CHUNK_MAP_BUCKETS];
    int size;
} ChunkMap;

typedef struct {
    Chunk **entries;
    int count;
    int capacity;
} ChunkList;

typedef struct ChunkProviderClient {
    IChunkProvider base;

    Chunk *blankChunk;

    ChunkMap chunkMapping;

    ChunkList field_889_c;

    World *worldObj;
} ChunkProviderClient;

void ChunkProviderClient_construct(ChunkProviderClient *self, World *var1);

int ChunkProviderClient_chunkExists(ChunkProviderClient *self, int var1, int var2);

void ChunkProviderClient_func_539_c(ChunkProviderClient *self, int var1, int var2);

Chunk *ChunkProviderClient_prepareChunk(ChunkProviderClient *self, int var1, int var2);

Chunk *ChunkProviderClient_provideChunk(ChunkProviderClient *self, int var1, int var2);

int ChunkProviderClient_saveChunks(ChunkProviderClient *self, int var1, IProgressUpdate *var2);

int ChunkProviderClient_unload100OldestChunks(ChunkProviderClient *self);

int ChunkProviderClient_canSave(ChunkProviderClient *self);

void ChunkProviderClient_populate(ChunkProviderClient *self, IChunkProvider *var1, int var2,
                                  int var3);

const char *ChunkProviderClient_makeString(ChunkProviderClient *self);

extern const IChunkProviderVtable ChunkProviderClient_vtable;
void ChunkProviderClient_destroy(ChunkProviderClient *self);

#endif
