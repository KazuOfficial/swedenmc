#include "EmptyChunk.h"

Chunk *EmptyChunk_create(World *var1, int var2, int var3) {
    Chunk *self = Chunk_create(var1, var2, var3);
    self->neverSave = 1;
    self->isEmptyChunk = 1;
    return self;
}

Chunk *EmptyChunk_create_with_blocks(World *var1, uint8_t *var2, int var3, int var4) {
    Chunk *self = Chunk_create_with_blocks(var1, var2, var3, var4);
    self->neverSave = 1;
    self->isEmptyChunk = 1;
    return self;
}
