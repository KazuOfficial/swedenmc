#ifndef CHUNK_FILE_H
#define CHUNK_FILE_H

typedef struct ChunkFile {
    char *field_22326_a;
    int field_22325_b;
    int field_22327_c;
} ChunkFile;

ChunkFile *ChunkFile_create(const char *path);
void ChunkFile_destroy(ChunkFile *self);

int ChunkFile_func_22322_a(const ChunkFile *self, const ChunkFile *var1);

char *ChunkFile_func_22324_a(const ChunkFile *self);

int ChunkFile_func_22323_b(const ChunkFile *self);

int ChunkFile_func_22321_c(const ChunkFile *self);

int ChunkFile_compareTo(const ChunkFile *self, void *var1);

#endif
