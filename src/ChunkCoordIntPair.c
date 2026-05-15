#include "ChunkCoordIntPair.h"
#include <limits.h>

ChunkCoordIntPair ChunkCoordIntPair_create(int x, int z) {
    ChunkCoordIntPair p;
    p.chunkXPos = x;
    p.chunkZPos = z;
    return p;
}

int ChunkCoordIntPair_chunkXZ2Int(int x, int z) {
    return (x < 0 ? (int)0x80000000 : 0) | ((x & 0x7FFF) << 16) | (z < 0 ? 0x8000 : 0) |
           (z & 0x7FFF);
}

int ChunkCoordIntPair_hashCode(ChunkCoordIntPair *self) {
    return ChunkCoordIntPair_chunkXZ2Int(self->chunkXPos, self->chunkZPos);
}

int ChunkCoordIntPair_equals(ChunkCoordIntPair *self, ChunkCoordIntPair *other) {
    return self->chunkXPos == other->chunkXPos && self->chunkZPos == other->chunkZPos;
}
