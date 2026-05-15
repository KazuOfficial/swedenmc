#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include "GameForward.h"

struct ImageBuffer;

typedef struct ImageBufferVtable {

    void *(*parseUserSkin)(struct ImageBuffer *self, void *var1);
} ImageBufferVtable;

typedef struct ImageBuffer {
    const ImageBufferVtable *vtable;
} ImageBuffer;

#endif
