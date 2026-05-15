#ifndef GL_ALLOCATION_H
#define GL_ALLOCATION_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>

static inline GLuint glGenLists(GLsizei n) {
    (void)n;
    static GLuint s = 1;
    return s++;
}
static inline void glDeleteLists(GLuint l, GLsizei n) {
    (void)l;
    (void)n;
}
#else
#include "glad/gl.h"
#endif
#include <stdlib.h>

int GLAllocation_generateDisplayLists(int count);
void GLAllocation_generateTextureNames(GLuint *buf, int count);

void GLAllocation_func_28194_b(int base);

void GLAllocation_deleteTexturesAndDisplayLists(void);

static inline unsigned char *GLAllocation_createDirectByteBuffer(int capacity) {
    return (unsigned char *)calloc(capacity, 1);
}
static inline int *GLAllocation_createDirectIntBuffer(int capacity) {
    return (int *)calloc(capacity, sizeof(int));
}
static inline float *GLAllocation_createDirectFloatBuffer(int capacity) {
    return (float *)calloc(capacity, sizeof(float));
}

#endif
