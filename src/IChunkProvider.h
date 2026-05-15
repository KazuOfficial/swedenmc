#ifndef ICHUNK_PROVIDER_H
#define ICHUNK_PROVIDER_H

#include "GameForward.h"
#include "IProgressUpdate.h"

struct IChunkProvider;

typedef struct IChunkProviderVtable {

    int (*chunkExists)(struct IChunkProvider *self, int x, int z);

    Chunk *(*provideChunk)(struct IChunkProvider *self, int x, int z);

    Chunk *(*prepareChunk)(struct IChunkProvider *self, int x, int z);

    void (*populate)(struct IChunkProvider *self, struct IChunkProvider *chunkProvider, int x,
                     int z);

    int (*saveChunks)(struct IChunkProvider *self, int saveAll, IProgressUpdate *progress);

    int (*unload100OldestChunks)(struct IChunkProvider *self);

    int (*canSave)(struct IChunkProvider *self);

    const char *(*makeString)(struct IChunkProvider *self);
} IChunkProviderVtable;

typedef struct IChunkProvider {
    const IChunkProviderVtable *vtable;
} IChunkProvider;

static inline int IChunkProvider_chunkExists(IChunkProvider *s, int x, int z) {
    return s->vtable->chunkExists(s, x, z);
}
static inline Chunk *IChunkProvider_provideChunk(IChunkProvider *s, int x, int z) {
    return s->vtable->provideChunk(s, x, z);
}
static inline Chunk *IChunkProvider_prepareChunk(IChunkProvider *s, int x, int z) {
    return s->vtable->prepareChunk(s, x, z);
}
static inline void IChunkProvider_populate(IChunkProvider *s, IChunkProvider *provider, int x,
                                           int z) {
    s->vtable->populate(s, provider, x, z);
}
static inline int IChunkProvider_saveChunks(IChunkProvider *s, int saveAll,
                                            IProgressUpdate *progress) {
    return s->vtable->saveChunks(s, saveAll, progress);
}
static inline int IChunkProvider_unload100OldestChunks(IChunkProvider *s) {
    return s->vtable->unload100OldestChunks(s);
}
static inline int IChunkProvider_canSave(IChunkProvider *s) { return s->vtable->canSave(s); }
static inline const char *IChunkProvider_makeString(IChunkProvider *s) {
    return s->vtable->makeString(s);
}

#endif
