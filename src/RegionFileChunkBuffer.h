#ifndef REGION_FILE_CHUNK_BUFFER_H
#define REGION_FILE_CHUNK_BUFFER_H

#include <stdint.h>

struct RegionFile;

typedef struct RegionFileChunkBuffer {

    uint8_t *buf;
    int count;
    int cap;

    struct RegionFile *field_22284_a;

    int field_22283_b;

    int field_22285_c;
} RegionFileChunkBuffer;

RegionFileChunkBuffer *RegionFileChunkBuffer_create(struct RegionFile *rf, int x, int z);

void RegionFileChunkBuffer_writeByte(RegionFileChunkBuffer *self, int b);

void RegionFileChunkBuffer_writeBytes(RegionFileChunkBuffer *self, const uint8_t *b, int off,
                                      int len);

void RegionFileChunkBuffer_close(RegionFileChunkBuffer *self);

void RegionFileChunkBuffer_destroy(RegionFileChunkBuffer *self);

#endif
