#include "NibbleArray.h"
#include <stdlib.h>
#include <string.h>

NibbleArray *NibbleArray_create(int nibbleCount) {
    NibbleArray *self = (NibbleArray *)calloc(1, sizeof(NibbleArray));
    self->size = nibbleCount >> 1;
    self->data = (uint8_t *)calloc(self->size, 1);
    self->owned = 1;
    return self;
}

NibbleArray *NibbleArray_wrap(uint8_t *data, int bytes) {
    NibbleArray *self = (NibbleArray *)calloc(1, sizeof(NibbleArray));
    self->data = data;
    self->size = bytes;
    self->owned = 0;
    return self;
}

void NibbleArray_free(NibbleArray *self) {
    if (self->owned)
        free(self->data);
    free(self);
}

void NibbleArray_setNibble(NibbleArray *self, int x, int y, int z, int val) {
    int idx = (x << 11) | (z << 7) | y;
    int byte = idx >> 1;
    int half = idx & 1;
    if (half == 0)
        self->data[byte] = (uint8_t)((self->data[byte] & 0xf0) | (val & 0x0f));
    else
        self->data[byte] = (uint8_t)((self->data[byte] & 0x0f) | ((val & 0x0f) << 4));
}

int NibbleArray_isValid(NibbleArray *self) { return self->data != NULL; }
