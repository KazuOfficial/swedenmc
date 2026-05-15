#include "ChunkPosition.h"

ChunkPosition ChunkPosition_create(int x, int y, int z) {
    ChunkPosition p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

int ChunkPosition_equals(ChunkPosition *self, ChunkPosition *other) {
    return self->x == other->x && self->y == other->y && self->z == other->z;
}

int ChunkPosition_hashCode(ChunkPosition *self) {
    return self->x * 8976890 + self->y * 981131 + self->z;
}
