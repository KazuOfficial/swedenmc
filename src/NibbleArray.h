#ifndef NIBBLE_ARRAY_H
#define NIBBLE_ARRAY_H

#include <stdint.h>

typedef struct {
    uint8_t *data;
    int size;
    int owned;
} NibbleArray;

NibbleArray *NibbleArray_create(int nibbleCount);
NibbleArray *NibbleArray_wrap(uint8_t *data, int bytes);
void NibbleArray_free(NibbleArray *self);

static inline int NibbleArray_getNibble(NibbleArray *self, int x, int y, int z) {
    int idx = (x << 11) | (z << 7) | y;
    int byte = idx >> 1;
    return (idx & 1) == 0 ? (self->data[byte] & 0x0f) : ((self->data[byte] >> 4) & 0x0f);
}

void NibbleArray_setNibble(NibbleArray *self, int x, int y, int z, int val);
int NibbleArray_isValid(NibbleArray *self);

#endif
