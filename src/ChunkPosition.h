#ifndef CHUNK_POSITION_H
#define CHUNK_POSITION_H

typedef struct {
    int x;
    int y;
    int z;
} ChunkPosition;

ChunkPosition ChunkPosition_create(int x, int y, int z);
int ChunkPosition_equals(ChunkPosition *self, ChunkPosition *other);
int ChunkPosition_hashCode(ChunkPosition *self);

#endif
