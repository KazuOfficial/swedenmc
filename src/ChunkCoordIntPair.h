#ifndef CHUNK_COORD_INT_PAIR_H
#define CHUNK_COORD_INT_PAIR_H

typedef struct {
    int chunkXPos;
    int chunkZPos;
} ChunkCoordIntPair;

ChunkCoordIntPair ChunkCoordIntPair_create(int x, int z);
int ChunkCoordIntPair_chunkXZ2Int(int x, int z);
int ChunkCoordIntPair_hashCode(ChunkCoordIntPair *self);
int ChunkCoordIntPair_equals(ChunkCoordIntPair *self, ChunkCoordIntPair *other);

#endif
