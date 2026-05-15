#ifndef EMPTY_CHUNK_H
#define EMPTY_CHUNK_H

#include "Chunk.h"

Chunk *EmptyChunk_create(World *var1, int var2, int var3);

Chunk *EmptyChunk_create_with_blocks(World *var1, uint8_t *var2, int var3, int var4);

#endif
