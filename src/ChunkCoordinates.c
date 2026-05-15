#include "ChunkCoordinates.h"
#include <math.h>

ChunkCoordinates ChunkCoordinates_create(int x, int y, int z) {
    ChunkCoordinates c;
    c.x = x;
    c.y = y;
    c.z = z;
    return c;
}

ChunkCoordinates ChunkCoordinates_copy(ChunkCoordinates *src) {
    ChunkCoordinates c;
    c.x = src->x;
    c.y = src->y;
    c.z = src->z;
    return c;
}

int ChunkCoordinates_equals(ChunkCoordinates *self, ChunkCoordinates *other) {
    return self->x == other->x && self->y == other->y && self->z == other->z;
}

int ChunkCoordinates_hashCode(ChunkCoordinates *self) {
    return ((self->x + self->z) << (8 + self->y)) << 16;
}

int ChunkCoordinates_compareChunkCoordinate(ChunkCoordinates *self, ChunkCoordinates *other) {
    return self->y == other->y ? (self->z == other->z ? self->x - other->x : self->z - other->z)
                               : self->y - other->y;
}

double ChunkCoordinates_getSqDistanceTo(ChunkCoordinates *self, int x, int y, int z) {
    int dx = self->x - x;
    int dy = self->y - y;
    int dz = self->z - z;
    return sqrt((double)(dx * dx + dy * dy + dz * dz));
}
