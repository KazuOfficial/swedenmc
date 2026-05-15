#ifndef ISAVE_HANDLER_H
#define ISAVE_HANDLER_H

#include "GameForward.h"

struct ISaveHandler;
struct WorldInfo;
struct WorldProvider;
struct IChunkLoader;
struct EntityPlayer;

typedef struct ISaveHandlerVtable {

    struct WorldInfo *(*loadWorldInfo)(struct ISaveHandler *self);

    void (*func_22150_b)(struct ISaveHandler *self);

    struct IChunkLoader *(*getChunkLoader)(struct ISaveHandler *self,
                                           struct WorldProvider *provider);

    void (*saveWorldInfoAndPlayer)(struct ISaveHandler *self, struct WorldInfo *info,
                                   struct EntityPlayer **players, int playerCount);

    void (*saveWorldInfo)(struct ISaveHandler *self, struct WorldInfo *info);

    char *(*func_28113_a)(struct ISaveHandler *self, const char *var1);

    void (*destroy)(struct ISaveHandler *self);
} ISaveHandlerVtable;

typedef struct ISaveHandler {
    const ISaveHandlerVtable *vtable;
} ISaveHandler;

static inline struct WorldInfo *ISaveHandler_loadWorldInfo(struct ISaveHandler *s) {
    return s->vtable->loadWorldInfo(s);
}
static inline void ISaveHandler_func_22150_b(struct ISaveHandler *s) { s->vtable->func_22150_b(s); }
static inline struct IChunkLoader *ISaveHandler_getChunkLoader(struct ISaveHandler *s,
                                                               struct WorldProvider *p) {
    return s->vtable->getChunkLoader(s, p);
}
static inline void ISaveHandler_saveWorldInfoAndPlayer(struct ISaveHandler *s,
                                                       struct WorldInfo *info,
                                                       struct EntityPlayer **players,
                                                       int playerCount) {
    s->vtable->saveWorldInfoAndPlayer(s, info, players, playerCount);
}
static inline void ISaveHandler_saveWorldInfo(struct ISaveHandler *s, struct WorldInfo *info) {
    s->vtable->saveWorldInfo(s, info);
}
static inline char *ISaveHandler_func_28113_a(struct ISaveHandler *s, const char *var1) {
    return s->vtable->func_28113_a(s, var1);
}
static inline void ISaveHandler_destroy(struct ISaveHandler *s) { s->vtable->destroy(s); }

#endif
