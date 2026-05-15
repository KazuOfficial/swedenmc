#include "ChunkProvider.h"
#include "EmptyChunk.h"
#include "Chunk.h"
#include "World.h"
#include "ChunkCoordIntPair.h"
#include "ChunkProviderGenerate.h"
#include "ChunkProviderHell.h"
#include "McRegionChunkLoader.h"
#include <stdlib.h>
#include <string.h>

static int cp_map_hash(int key) {
    unsigned int h = (unsigned int)key;
    h ^= (h >> 20) ^ (h >> 12);
    return (int)(h ^ (h >> 7) ^ (h >> 4));
}

static int cp_map_find(ChunkProvider *self, int key) {
    int mask = self->chunkMapCap - 1;
    int idx = cp_map_hash(key) & mask;
    for (int i = 0; i < self->chunkMapCap; i++) {
        int slot = (idx + i) & mask;
        if (!self->chunkMap[slot].occupied)
            return -1;
        if (self->chunkMap[slot].key == key)
            return slot;
    }
    return -1;
}

static void cp_map_put_raw(ChunkMapSlot *table, int cap, int key, Chunk *chunk) {
    int mask = cap - 1;
    int idx = cp_map_hash(key) & mask;
    for (int i = 0; i < cap; i++) {
        int slot = (idx + i) & mask;
        if (!table[slot].occupied) {
            table[slot].key = key;
            table[slot].chunk = chunk;
            table[slot].occupied = 1;
            return;
        }
        if (table[slot].key == key) {
            table[slot].chunk = chunk;
            return;
        }
    }
}

static void cp_map_grow(ChunkProvider *self) {
    int newCap = self->chunkMapCap * 2;
    ChunkMapSlot *newTable = (ChunkMapSlot *)calloc(newCap, sizeof(ChunkMapSlot));
    for (int i = 0; i < self->chunkMapCap; i++) {
        if (self->chunkMap[i].occupied)
            cp_map_put_raw(newTable, newCap, self->chunkMap[i].key, self->chunkMap[i].chunk);
    }
    free(self->chunkMap);
    self->chunkMap = newTable;
    self->chunkMapCap = newCap;
}

static void cp_map_put(ChunkProvider *self, int key, Chunk *chunk) {

    if (self->chunkMapCount * 4 >= self->chunkMapCap * 3)
        cp_map_grow(self);
    int mask = self->chunkMapCap - 1;
    int idx = cp_map_hash(key) & mask;
    for (int i = 0; i < self->chunkMapCap; i++) {
        int slot = (idx + i) & mask;
        if (!self->chunkMap[slot].occupied) {
            self->chunkMap[slot].key = key;
            self->chunkMap[slot].chunk = chunk;
            self->chunkMap[slot].occupied = 1;
            self->chunkMapCount++;
            return;
        }
        if (self->chunkMap[slot].key == key) {
            self->chunkMap[slot].chunk = chunk;
            return;
        }
    }
}

static Chunk *cp_map_get(ChunkProvider *self, int key) {
    int slot = cp_map_find(self, key);
    return slot >= 0 ? self->chunkMap[slot].chunk : NULL;
}

static int cp_map_contains(ChunkProvider *self, int key) { return cp_map_find(self, key) >= 0; }

static void cp_map_remove(ChunkProvider *self, int key) {
    int slot = cp_map_find(self, key);
    if (slot < 0)
        return;
    self->chunkMap[slot].occupied = 0;
    self->chunkMap[slot].chunk = NULL;
    self->chunkMapCount--;

    int mask = self->chunkMapCap - 1;
    for (int i = 1; i < self->chunkMapCap; i++) {
        int next = (slot + i) & mask;
        if (!self->chunkMap[next].occupied)
            break;
        ChunkMapSlot saved = self->chunkMap[next];
        self->chunkMap[next].occupied = 0;
        self->chunkMapCount--;
        cp_map_put(self, saved.key, saved.chunk);
    }
}

static void cp_list_add(ChunkProvider *self, Chunk *chunk) {
    if (self->chunkListCount >= self->chunkListCap) {
        int nc = self->chunkListCap ? self->chunkListCap * 2 : 64;
        self->chunkList = realloc(self->chunkList, nc * sizeof(Chunk *));
        self->chunkListCap = nc;
    }
    self->chunkList[self->chunkListCount++] = chunk;
}

static void cp_list_remove(ChunkProvider *self, Chunk *chunk) {
    for (int i = 0; i < self->chunkListCount; i++) {
        if (self->chunkList[i] == chunk) {
            self->chunkList[i] = self->chunkList[--self->chunkListCount];
            return;
        }
    }
}

static void cp_set_remove(ChunkProvider *self, int key) {
    for (int i = 0; i < self->droppedChunksCount; i++) {
        if (self->droppedChunksSet[i] == key) {
            self->droppedChunksSet[i] = self->droppedChunksSet[--self->droppedChunksCount];
            return;
        }
    }
}

static int cp_set_pop(ChunkProvider *self) {
    int val = self->droppedChunksSet[0];
    self->droppedChunksSet[0] = self->droppedChunksSet[--self->droppedChunksCount];
    return val;
}

static Chunk *loadChunkFromFile(ChunkProvider *self, int var1, int var2) {
    if (self->chunkLoader == NULL)
        return NULL;
    Chunk *var3 = IChunkLoader_loadChunk(self->chunkLoader, self->field_28066_g, var1, var2);
    if (var3 != NULL) {
        var3->lastSaveTime = World_getWorldTime(self->field_28066_g);
    }
    return var3;
}

static void func_28063_a(ChunkProvider *self, Chunk *var1) {
    if (self->chunkLoader != NULL) {
        IChunkLoader_saveExtraChunkData(self->chunkLoader, self->field_28066_g, var1);
    }
}

static void func_28062_b(ChunkProvider *self, Chunk *var1) {
    if (self->chunkLoader != NULL) {
        var1->lastSaveTime = World_getWorldTime(self->field_28066_g);
        IChunkLoader_saveChunk(self->chunkLoader, self->field_28066_g, var1);
    }
}

static int cp_chunkExists(IChunkProvider *iself, int var1, int var2) {
    ChunkProvider *self = (ChunkProvider *)iself;
    return cp_map_contains(self, ChunkCoordIntPair_chunkXZ2Int(var1, var2));
}

static Chunk *cp_prepareChunk(IChunkProvider *iself, int var1, int var2) {
    ChunkProvider *self = (ChunkProvider *)iself;
    int var3 = ChunkCoordIntPair_chunkXZ2Int(var1, var2);
    cp_set_remove(self, var3);
    Chunk *var4 = cp_map_get(self, var3);
    if (var4 == NULL) {
        var4 = loadChunkFromFile(self, var1, var2);
        if (var4 == NULL) {
            if (self->chunkProvider == NULL) {
                var4 = self->field_28064_b;
            } else {
                var4 = IChunkProvider_provideChunk(self->chunkProvider, var1, var2);
            }
        }

        cp_map_put(self, var3, var4);
        cp_list_add(self, var4);
        if (var4 != NULL) {
            Chunk_func_4143_d(var4);
            Chunk_onChunkLoad(var4);
        }

        if (!var4->isTerrainPopulated && cp_chunkExists(iself, var1 + 1, var2 + 1) &&
            cp_chunkExists(iself, var1, var2 + 1) && cp_chunkExists(iself, var1 + 1, var2)) {
            iself->vtable->populate(iself, iself, var1, var2);
        }

        if (cp_chunkExists(iself, var1 - 1, var2) &&
            !IChunkProvider_provideChunk(iself, var1 - 1, var2)->isTerrainPopulated &&
            cp_chunkExists(iself, var1 - 1, var2 + 1) && cp_chunkExists(iself, var1, var2 + 1) &&
            cp_chunkExists(iself, var1 - 1, var2)) {
            iself->vtable->populate(iself, iself, var1 - 1, var2);
        }

        if (cp_chunkExists(iself, var1, var2 - 1) &&
            !IChunkProvider_provideChunk(iself, var1, var2 - 1)->isTerrainPopulated &&
            cp_chunkExists(iself, var1 + 1, var2 - 1) && cp_chunkExists(iself, var1, var2 - 1) &&
            cp_chunkExists(iself, var1 + 1, var2)) {
            iself->vtable->populate(iself, iself, var1, var2 - 1);
        }

        if (cp_chunkExists(iself, var1 - 1, var2 - 1) &&
            !IChunkProvider_provideChunk(iself, var1 - 1, var2 - 1)->isTerrainPopulated &&
            cp_chunkExists(iself, var1 - 1, var2 - 1) && cp_chunkExists(iself, var1, var2 - 1) &&
            cp_chunkExists(iself, var1 - 1, var2)) {
            iself->vtable->populate(iself, iself, var1 - 1, var2 - 1);
        }
    }

    return var4;
}

static Chunk *cp_provideChunk(IChunkProvider *iself, int var1, int var2) {
    ChunkProvider *self = (ChunkProvider *)iself;
    Chunk *var3 = cp_map_get(self, ChunkCoordIntPair_chunkXZ2Int(var1, var2));
    return var3 == NULL ? cp_prepareChunk(iself, var1, var2) : var3;
}

static void cp_populate(IChunkProvider *iself, IChunkProvider *var1, int var2, int var3) {
    ChunkProvider *self = (ChunkProvider *)iself;
    Chunk *var4 = cp_provideChunk(iself, var2, var3);
    if (!var4->isTerrainPopulated) {
        var4->isTerrainPopulated = 1;
        if (self->chunkProvider != NULL) {
            IChunkProvider_populate(self->chunkProvider, var1, var2, var3);
            Chunk_setChunkModified(var4);
        }
    }
}

static int cp_saveChunks(IChunkProvider *iself, int var1, IProgressUpdate *var2) {
    ChunkProvider *self = (ChunkProvider *)iself;
    int var3 = 0;

    for (int var4 = 0; var4 < self->chunkListCount; ++var4) {
        Chunk *var5 = self->chunkList[var4];
        if (var1 && !var5->neverSave) {
            func_28063_a(self, var5);
        }
        if (Chunk_needsSaving(var5, var1)) {
            func_28062_b(self, var5);
            var5->isModified = 0;
            ++var3;
            if (var3 == 24 && !var1) {
                return 0;
            }
        }
    }

    if (var1) {
        if (self->chunkLoader == NULL)
            return 1;
        IChunkLoader_saveExtraData(self->chunkLoader);
    }

    (void)var2;
    return 1;
}

static int cp_unload100OldestChunks(IChunkProvider *iself) {
    ChunkProvider *self = (ChunkProvider *)iself;
    for (int var1 = 0; var1 < 100; ++var1) {
        if (self->droppedChunksCount == 0)
            break;
        int var2 = cp_set_pop(self);
        Chunk *var3 = cp_map_get(self, var2);
        Chunk_onChunkUnload(var3);
        func_28062_b(self, var3);
        func_28063_a(self, var3);
        cp_map_remove(self, var2);
        cp_list_remove(self, var3);
    }

    if (self->chunkLoader != NULL) {
        IChunkLoader_func_814_a(self->chunkLoader);
    }

    return IChunkProvider_unload100OldestChunks(self->chunkProvider);
}

static int cp_canSave(IChunkProvider *iself) {
    (void)iself;
    return 1;
}

static const char *cp_makeString(IChunkProvider *iself) {
    ChunkProvider *self = (ChunkProvider *)iself;
    static char buf[64];
    snprintf(buf, sizeof(buf), "ServerChunkCache: %d Drop: %d", self->chunkMapCount,
             self->droppedChunksCount);
    return buf;
}

static const IChunkProviderVtable ChunkProvider_vtable = {
    .chunkExists = cp_chunkExists,
    .provideChunk = cp_provideChunk,
    .prepareChunk = cp_prepareChunk,
    .populate = cp_populate,
    .saveChunks = cp_saveChunks,
    .unload100OldestChunks = cp_unload100OldestChunks,
    .canSave = cp_canSave,
    .makeString = cp_makeString,
};

ChunkProvider *ChunkProvider_create(World *var1, IChunkLoader *var2, IChunkProvider *var3) {
    ChunkProvider *self = (ChunkProvider *)calloc(1, sizeof(ChunkProvider));
    self->base.vtable = &ChunkProvider_vtable;

    self->field_28064_b = EmptyChunk_create(var1, 0, 0);
    self->field_28066_g = var1;
    self->chunkLoader = var2;
    self->chunkProvider = var3;
    self->chunkMapCap = 256;
    self->chunkMap = (ChunkMapSlot *)calloc(self->chunkMapCap, sizeof(ChunkMapSlot));
    return self;
}

void ChunkProvider_destroy(ChunkProvider *self) {

    for (int i = 0; i < self->chunkMapCap; i++) {
        if (self->chunkMap[i].occupied && self->chunkMap[i].chunk)
            Chunk_destroy(self->chunkMap[i].chunk);
    }
    Chunk_destroy(self->field_28064_b);

    if (self->chunkProvider) {
        if (self->chunkProvider->vtable == &ChunkProviderGenerate_vtable)
            ChunkProviderGenerate_free((ChunkProviderGenerate *)self->chunkProvider);
        else if (self->chunkProvider->vtable == &ChunkProviderHell_vtable)
            ChunkProviderHell_free((ChunkProviderHell *)self->chunkProvider);
        else
            free(self->chunkProvider);
    }
    if (self->chunkLoader)
        McRegionChunkLoader_destroy((McRegionChunkLoader *)self->chunkLoader);
    free(self->chunkMap);
    free(self->chunkList);
    free(self->droppedChunksSet);
    free(self);
}
