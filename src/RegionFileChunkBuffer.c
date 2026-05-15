#include "RegionFileChunkBuffer.h"
#include "RegionFile.h"
#include <stdlib.h>
#include <string.h>

RegionFileChunkBuffer *RegionFileChunkBuffer_create(RegionFile *rf, int x, int z) {
    RegionFileChunkBuffer *self = (RegionFileChunkBuffer *)calloc(1, sizeof(RegionFileChunkBuffer));
    self->cap = 8096;
    self->buf = (uint8_t *)malloc(self->cap);
    self->count = 0;
    self->field_22284_a = rf;
    self->field_22283_b = x;
    self->field_22285_c = z;
    return self;
}

void RegionFileChunkBuffer_writeByte(RegionFileChunkBuffer *self, int b) {
    if (self->count >= self->cap) {
        int nc = self->cap * 2;
        self->buf = (uint8_t *)realloc(self->buf, nc);
        self->cap = nc;
    }
    self->buf[self->count++] = (uint8_t)(b & 0xFF);
}

void RegionFileChunkBuffer_writeBytes(RegionFileChunkBuffer *self, const uint8_t *b, int off,
                                      int len) {
    while (self->count + len > self->cap) {
        int nc = self->cap * 2;
        self->buf = (uint8_t *)realloc(self->buf, nc);
        self->cap = nc;
    }
    memcpy(self->buf + self->count, b + off, len);
    self->count += len;
}

void RegionFileChunkBuffer_close(RegionFileChunkBuffer *self) {
    RegionFile_write(self->field_22284_a, self->field_22283_b, self->field_22285_c, self->buf,
                     self->count);
}

void RegionFileChunkBuffer_destroy(RegionFileChunkBuffer *self) {
    free(self->buf);
    free(self);
}
