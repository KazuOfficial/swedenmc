#include "ChunkProviderLoadOrGenerate.h"
#include "Chunk.h"
#include "World.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Chunk *func_542_c(ChunkProviderLoadOrGenerate *self, int var1, int var2);
static void saveChunk(ChunkProviderLoadOrGenerate *self, Chunk *var1);
static void saveExtraChunkData(ChunkProviderLoadOrGenerate *self, Chunk *var1);

static int cplg_chunkExists(IChunkProvider *iself, int var1, int var2) {
    ChunkProviderLoadOrGenerate *self = (ChunkProviderLoadOrGenerate *)iself;
    if (!ChunkProviderLoadOrGenerate_canChunkExist(self, var1, var2)) {
        return 0;
    }
    if (var1 == self->lastQueriedChunkXPos && var2 == self->lastQueriedChunkZPos &&
        self->lastQueriedChunk != NULL) {
        return 1;
    }
    int var3 = var1 & 31;
    int var4 = var2 & 31;
    int var5 = var3 + var4 * 32;
    return self->chunks[var5] != NULL && (self->chunks[var5] == self->blankChunk ||
                                          Chunk_isAtLocation(self->chunks[var5], var1, var2));
}

static Chunk *cplg_prepareChunk(IChunkProvider *iself, int var1, int var2) {
    return iself->vtable->provideChunk(iself, var1, var2);
}

static Chunk *cplg_provideChunk(IChunkProvider *iself, int var1, int var2) {
    ChunkProviderLoadOrGenerate *self = (ChunkProviderLoadOrGenerate *)iself;
    if (var1 == self->lastQueriedChunkXPos && var2 == self->lastQueriedChunkZPos &&
        self->lastQueriedChunk != NULL) {
        return self->lastQueriedChunk;
    }
    if (!self->worldObj->findingSpawnPoint &&
        !ChunkProviderLoadOrGenerate_canChunkExist(self, var1, var2)) {
        return self->blankChunk;
    }

    int var3 = var1 & 31;
    int var4 = var2 & 31;
    int var5 = var3 + var4 * 32;

    if (!cplg_chunkExists(iself, var1, var2)) {
        if (self->chunks[var5] != NULL) {
            Chunk_onChunkUnload(self->chunks[var5]);
            saveChunk(self, self->chunks[var5]);
            saveExtraChunkData(self, self->chunks[var5]);
        }

        Chunk *var6 = func_542_c(self, var1, var2);
        if (var6 == NULL) {
            if (self->chunkProvider == NULL) {
                var6 = self->blankChunk;
            } else {
                var6 = IChunkProvider_provideChunk(self->chunkProvider, var1, var2);
                Chunk_func_25124_i(var6);
            }
        }

        self->chunks[var5] = var6;
        Chunk_func_4143_d(var6);
        if (self->chunks[var5] != NULL) {
            Chunk_onChunkLoad(self->chunks[var5]);
        }

        if (!self->chunks[var5]->isTerrainPopulated &&
            cplg_chunkExists(iself, var1 + 1, var2 + 1) &&
            cplg_chunkExists(iself, var1, var2 + 1) && cplg_chunkExists(iself, var1 + 1, var2)) {
            iself->vtable->populate(iself, iself, var1, var2);
        }

        if (cplg_chunkExists(iself, var1 - 1, var2) &&
            !IChunkProvider_provideChunk(iself, var1 - 1, var2)->isTerrainPopulated &&
            cplg_chunkExists(iself, var1 - 1, var2 + 1) &&
            cplg_chunkExists(iself, var1, var2 + 1) && cplg_chunkExists(iself, var1 - 1, var2)) {
            iself->vtable->populate(iself, iself, var1 - 1, var2);
        }

        if (cplg_chunkExists(iself, var1, var2 - 1) &&
            !IChunkProvider_provideChunk(iself, var1, var2 - 1)->isTerrainPopulated &&
            cplg_chunkExists(iself, var1 + 1, var2 - 1) &&
            cplg_chunkExists(iself, var1, var2 - 1) && cplg_chunkExists(iself, var1 + 1, var2)) {
            iself->vtable->populate(iself, iself, var1, var2 - 1);
        }

        if (cplg_chunkExists(iself, var1 - 1, var2 - 1) &&
            !IChunkProvider_provideChunk(iself, var1 - 1, var2 - 1)->isTerrainPopulated &&
            cplg_chunkExists(iself, var1 - 1, var2 - 1) &&
            cplg_chunkExists(iself, var1, var2 - 1) && cplg_chunkExists(iself, var1 - 1, var2)) {
            iself->vtable->populate(iself, iself, var1 - 1, var2 - 1);
        }
    }

    self->lastQueriedChunkXPos = var1;
    self->lastQueriedChunkZPos = var2;
    self->lastQueriedChunk = self->chunks[var5];
    return self->chunks[var5];
}

static void cplg_populate(IChunkProvider *iself, IChunkProvider *var1, int var2, int var3) {
    ChunkProviderLoadOrGenerate *self = (ChunkProviderLoadOrGenerate *)iself;
    Chunk *var4 = IChunkProvider_provideChunk(iself, var2, var3);
    if (!var4->isTerrainPopulated) {
        var4->isTerrainPopulated = 1;
        if (self->chunkProvider != NULL) {
            IChunkProvider_populate(self->chunkProvider, var1, var2, var3);
            Chunk_setChunkModified(var4);
        }
    }
}

static int cplg_saveChunks(IChunkProvider *iself, int var1, IProgressUpdate *var2) {
    ChunkProviderLoadOrGenerate *self = (ChunkProviderLoadOrGenerate *)iself;
    int var3 = 0;
    int var4 = 0;
    int var5;

    if (var2 != NULL) {
        for (var5 = 0; var5 < CPLG_CACHE_SIZE; ++var5) {
            if (self->chunks[var5] != NULL && Chunk_needsSaving(self->chunks[var5], var1)) {
                ++var4;
            }
        }
    }

    var5 = 0;

    for (int var6 = 0; var6 < CPLG_CACHE_SIZE; ++var6) {
        if (self->chunks[var6] != NULL) {
            if (var1 && !self->chunks[var6]->neverSave) {
                saveExtraChunkData(self, self->chunks[var6]);
            }
            if (Chunk_needsSaving(self->chunks[var6], var1)) {
                saveChunk(self, self->chunks[var6]);
                self->chunks[var6]->isModified = 0;
                ++var3;
                if (var3 == 2 && !var1) {
                    return 0;
                }
                if (var2 != NULL) {
                    ++var5;
                    if (var5 % 10 == 0) {
                        var2->setLoadingProgress(var2, var5 * 100 / var4);
                    }
                }
            }
        }
    }

    if (var1) {
        if (self->chunkLoader == NULL) {
            return 1;
        }
        IChunkLoader_saveExtraData(self->chunkLoader);
    }

    return 1;
}

static int cplg_unload100OldestChunks(IChunkProvider *iself) {
    ChunkProviderLoadOrGenerate *self = (ChunkProviderLoadOrGenerate *)iself;
    if (self->chunkLoader != NULL) {
        IChunkLoader_func_814_a(self->chunkLoader);
    }
    return IChunkProvider_unload100OldestChunks(self->chunkProvider);
}

static int cplg_canSave(IChunkProvider *iself) {
    (void)iself;
    return 1;
}

static const char *cplg_makeString(IChunkProvider *iself) {
    (void)iself;
    return "ChunkCache: "
           "1024";
}

const IChunkProviderVtable ChunkProviderLoadOrGenerate_vtable = {
    .chunkExists = cplg_chunkExists,
    .provideChunk = cplg_provideChunk,
    .prepareChunk = cplg_prepareChunk,
    .populate = cplg_populate,
    .saveChunks = cplg_saveChunks,
    .unload100OldestChunks = cplg_unload100OldestChunks,
    .canSave = cplg_canSave,
    .makeString = cplg_makeString,
};

static Chunk *func_542_c(ChunkProviderLoadOrGenerate *self, int var1, int var2) {
    if (self->chunkLoader == NULL) {
        return self->blankChunk;
    }

    Chunk *var3 = IChunkLoader_loadChunk(self->chunkLoader, self->worldObj, var1, var2);
    if (var3 != NULL) {
        var3->lastSaveTime = World_getWorldTime(self->worldObj);
    }
    return var3;
}

static void saveExtraChunkData(ChunkProviderLoadOrGenerate *self, Chunk *var1) {
    if (self->chunkLoader != NULL) {
        IChunkLoader_saveExtraChunkData(self->chunkLoader, self->worldObj, var1);
    }
}

static void saveChunk(ChunkProviderLoadOrGenerate *self, Chunk *var1) {
    if (self->chunkLoader != NULL) {
        var1->lastSaveTime = World_getWorldTime(self->worldObj);
        IChunkLoader_saveChunk(self->chunkLoader, self->worldObj, var1);
    }
}

ChunkProviderLoadOrGenerate *ChunkProviderLoadOrGenerate_create(World *world, IChunkLoader *loader,
                                                                IChunkProvider *generator) {
    ChunkProviderLoadOrGenerate *self =
        (ChunkProviderLoadOrGenerate *)calloc(1, sizeof(ChunkProviderLoadOrGenerate));
    self->base.vtable = &ChunkProviderLoadOrGenerate_vtable;
    self->worldObj = world;
    self->chunkLoader = loader;
    self->chunkProvider = generator;
    self->blankChunk = Chunk_create(world, 0, 0);
    self->blankChunk->neverSave = 1;
    self->blankChunk->isTerrainPopulated = 1;
    return self;
}

void ChunkProviderLoadOrGenerate_destroy(ChunkProviderLoadOrGenerate *self) {
    Chunk_destroy(self->blankChunk);
    free(self);
}

void ChunkProviderLoadOrGenerate_setCurrentChunkOver(ChunkProviderLoadOrGenerate *self, int var1,
                                                     int var2) {
    self->curChunkX = var1;
    self->curChunkY = var2;
}

int ChunkProviderLoadOrGenerate_canChunkExist(ChunkProviderLoadOrGenerate *self, int var1,
                                              int var2) {
    int var3 = 15;
    return var1 >= self->curChunkX - var3 && var2 >= self->curChunkY - var3 &&
           var1 <= self->curChunkX + var3 && var2 <= self->curChunkY + var3;
}
