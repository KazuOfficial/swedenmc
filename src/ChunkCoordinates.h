#ifndef CHUNK_COORDINATES_H
#define CHUNK_COORDINATES_H

typedef struct {
    int x;
    int y;
    int z;
} ChunkCoordinates;

ChunkCoordinates ChunkCoordinates_create(int x, int y, int z);
ChunkCoordinates ChunkCoordinates_copy(ChunkCoordinates *src);
int ChunkCoordinates_equals(ChunkCoordinates *self, ChunkCoordinates *other);
int ChunkCoordinates_hashCode(ChunkCoordinates *self);
int ChunkCoordinates_compareChunkCoordinate(ChunkCoordinates *self, ChunkCoordinates *other);
double ChunkCoordinates_getSqDistanceTo(ChunkCoordinates *self, int x, int y, int z);

#endif
