#ifndef CHUNK_FOLDER_PATTERN_H
#define CHUNK_FOLDER_PATTERN_H

typedef struct Empty2 {
    int _unused;
} Empty2;

typedef struct ChunkFolderPattern {

} ChunkFolderPattern;

void ChunkFolderPattern_construct(ChunkFolderPattern *self, struct Empty2 *var1);

int ChunkFolderPattern_accept(ChunkFolderPattern *self, const char *var1);

void ChunkFolderPattern_init(void);

#endif
