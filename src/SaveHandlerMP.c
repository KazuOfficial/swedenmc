#include "SaveHandlerMP.h"
#include "WorldInfo.h"
#include "WorldProvider.h"
#include "IChunkLoader.h"
#include <stdlib.h>

static struct WorldInfo *shmp_loadWorldInfo(ISaveHandler *self) {
    (void)self;
    return NULL;
}

static void shmp_func_22150_b(ISaveHandler *self) { (void)self; }

static struct IChunkLoader *shmp_getChunkLoader(ISaveHandler *self, struct WorldProvider *var1) {
    (void)self;
    (void)var1;
    return NULL;
}

static void shmp_saveWorldInfoAndPlayer(ISaveHandler *self, struct WorldInfo *var1,
                                        struct EntityPlayer **var2, int playerCount) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)playerCount;
}

static void shmp_saveWorldInfo(ISaveHandler *self, struct WorldInfo *var1) {
    (void)self;
    (void)var1;
}

static char *shmp_func_28113_a(ISaveHandler *self, const char *var1) {
    (void)self;
    (void)var1;
    return NULL;
}

static void shmp_destroy(ISaveHandler *self) { free(self); }

static const ISaveHandlerVtable SaveHandlerMP_vtable = {
    shmp_loadWorldInfo, shmp_func_22150_b, shmp_getChunkLoader, shmp_saveWorldInfoAndPlayer,
    shmp_saveWorldInfo, shmp_func_28113_a, shmp_destroy,
};

SaveHandlerMP *SaveHandlerMP_create(void) {
    SaveHandlerMP *self = (SaveHandlerMP *)calloc(1, sizeof(SaveHandlerMP));
    self->base.vtable = &SaveHandlerMP_vtable;
    return self;
}
