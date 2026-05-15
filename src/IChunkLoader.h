#ifndef ICHUNK_LOADER_H
#define ICHUNK_LOADER_H

#include "GameForward.h"

struct IChunkLoader;

typedef struct IChunkLoaderVtable {

    Chunk *(*loadChunk)(struct IChunkLoader *self, World *world, int x, int z);

    void (*saveChunk)(struct IChunkLoader *self, World *world, Chunk *chunk);

    void (*saveExtraChunkData)(struct IChunkLoader *self, World *world, Chunk *chunk);

    void (*func_814_a)(struct IChunkLoader *self);

    void (*saveExtraData)(struct IChunkLoader *self);
} IChunkLoaderVtable;

typedef struct IChunkLoader {
    const IChunkLoaderVtable *vtable;
} IChunkLoader;

static inline Chunk *IChunkLoader_loadChunk(IChunkLoader *self, World *w, int x, int z) {
    return self->vtable->loadChunk(self, w, x, z);
}
static inline void IChunkLoader_saveChunk(IChunkLoader *self, World *w, Chunk *c) {
    self->vtable->saveChunk(self, w, c);
}
static inline void IChunkLoader_saveExtraChunkData(IChunkLoader *self, World *w, Chunk *c) {
    self->vtable->saveExtraChunkData(self, w, c);
}
static inline void IChunkLoader_func_814_a(IChunkLoader *self) { self->vtable->func_814_a(self); }
static inline void IChunkLoader_saveExtraData(IChunkLoader *self) {
    self->vtable->saveExtraData(self);
}

#endif
