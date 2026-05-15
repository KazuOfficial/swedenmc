#ifndef CHUNK_FILE_PATTERN_H
#define CHUNK_FILE_PATTERN_H

#include "ChunkFolderPattern.h"

typedef struct ChunkFilePattern {

} ChunkFilePattern;

void ChunkFilePattern_init(void);

void ChunkFilePattern_construct(ChunkFilePattern *self, Empty2 *var1);

int ChunkFilePattern_accept(const char *name);

int ChunkFilePattern_parse(const char *name, int *chunkX, int *chunkZ);

#endif
