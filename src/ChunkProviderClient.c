#include "ChunkProviderClient.h"
#include "EmptyChunk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int ChunkMap_bucket(ChunkCoordIntPair *key) {
    int h = ChunkCoordIntPair_hashCode(key);

    h ^= (int)((unsigned int)h >> 16);
    return ((unsigned int)h) % CHUNK_MAP_BUCKETS;
}

static void ChunkMap_put(ChunkMap *map, ChunkCoordIntPair *key, Chunk *value) {
    int b = ChunkMap_bucket(key);
    ChunkMapEntry *e = map->buckets[b];
    while (e != NULL) {
        if (ChunkCoordIntPair_equals(&e->key, key)) {
            if (e->value && e->value != value)
                Chunk_destroy(e->value);
            e->value = value;
            return;
        }
        e = e->next;
    }
    ChunkMapEntry *entry = (ChunkMapEntry *)calloc(1, sizeof(ChunkMapEntry));
    entry->key = *key;
    entry->value = value;
    entry->next = map->buckets[b];
    map->buckets[b] = entry;
    map->size++;
}

static Chunk *ChunkMap_get(ChunkMap *map, ChunkCoordIntPair *key) {
    int b = ChunkMap_bucket(key);
    ChunkMapEntry *e = map->buckets[b];
    while (e != NULL) {
        if (ChunkCoordIntPair_equals(&e->key, key)) {
            return e->value;
        }
        e = e->next;
    }
    return NULL;
}

static int ChunkMap_containsKey(ChunkMap *map, ChunkCoordIntPair *key) {
    return ChunkMap_get(map, key) != NULL;
}

static void ChunkMap_remove(ChunkMap *map, ChunkCoordIntPair *key) {
    int b = ChunkMap_bucket(key);
    ChunkMapEntry **ep = &map->buckets[b];
    while (*ep != NULL) {
        if (ChunkCoordIntPair_equals(&(*ep)->key, key)) {
            ChunkMapEntry *dead = *ep;
            *ep = dead->next;
            free(dead);
            map->size--;
            return;
        }
        ep = &(*ep)->next;
    }
}

static void ChunkList_remove(ChunkList *list, Chunk *entry) {
    int i;
    for (i = 0; i < list->count; ++i) {
        if (list->entries[i] == entry) {
            memmove(list->entries + i, list->entries + i + 1,
                    (list->count - i - 1) * sizeof(Chunk *));
            list->count--;
            return;
        }
    }
}

static int vtable_chunkExists(IChunkProvider *self, int x, int z) {
    return ChunkProviderClient_chunkExists((ChunkProviderClient *)self, x, z);
}
static Chunk *vtable_provideChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderClient_provideChunk((ChunkProviderClient *)self, x, z);
}
static Chunk *vtable_prepareChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderClient_prepareChunk((ChunkProviderClient *)self, x, z);
}
static void vtable_populate(IChunkProvider *self, IChunkProvider *var1, int var2, int var3) {
    ChunkProviderClient_populate((ChunkProviderClient *)self, var1, var2, var3);
}
static int vtable_saveChunks(IChunkProvider *self, int var1, IProgressUpdate *var2) {
    return ChunkProviderClient_saveChunks((ChunkProviderClient *)self, var1, var2);
}
static int vtable_unload100OldestChunks(IChunkProvider *self) {
    return ChunkProviderClient_unload100OldestChunks((ChunkProviderClient *)self);
}
static int vtable_canSave(IChunkProvider *self) {
    return ChunkProviderClient_canSave((ChunkProviderClient *)self);
}
static const char *vtable_makeString(IChunkProvider *self) {
    return ChunkProviderClient_makeString((ChunkProviderClient *)self);
}

const IChunkProviderVtable ChunkProviderClient_vtable = {
    vtable_chunkExists, vtable_provideChunk,          vtable_prepareChunk, vtable_populate,
    vtable_saveChunks,  vtable_unload100OldestChunks, vtable_canSave,      vtable_makeString};

void ChunkProviderClient_construct(ChunkProviderClient *self, World *var1) {
    self->base.vtable = &ChunkProviderClient_vtable;
    self->blankChunk = EmptyChunk_create(var1, 0, 0);
    memset(&self->chunkMapping, 0, sizeof(ChunkMap));
    memset(&self->field_889_c, 0, sizeof(ChunkList));
    self->worldObj = var1;
}

int ChunkProviderClient_chunkExists(ChunkProviderClient *self, int var1, int var2) {
    if (self != NULL) {
        return 1;
    } else {
        ChunkCoordIntPair var3 = ChunkCoordIntPair_create(var1, var2);
        return ChunkMap_containsKey(&self->chunkMapping, &var3);
    }
}

void ChunkProviderClient_func_539_c(ChunkProviderClient *self, int var1, int var2) {
    Chunk *var3 = ChunkProviderClient_provideChunk(self, var1, var2);
    if (!Chunk_func_21167_h(var3)) {
        Chunk_onChunkUnload(var3);
    }
    ChunkCoordIntPair key = ChunkCoordIntPair_create(var1, var2);
    ChunkMap_remove(&self->chunkMapping, &key);
    ChunkList_remove(&self->field_889_c, var3);
    if (var3 != self->blankChunk)
        Chunk_destroy(var3);
}

Chunk *ChunkProviderClient_prepareChunk(ChunkProviderClient *self, int var1, int var2) {
    ChunkCoordIntPair var3 = ChunkCoordIntPair_create(var1, var2);

    Chunk *var5 = Chunk_create(self->worldObj, var1, var2);

    memset(var5->skylightMap->data, 0xFF, (size_t)var5->skylightMap->size);
    ChunkMap_put(&self->chunkMapping, &var3, var5);
    var5->isChunkLoaded = 1;
    return var5;
}

Chunk *ChunkProviderClient_provideChunk(ChunkProviderClient *self, int var1, int var2) {
    ChunkCoordIntPair var3 = ChunkCoordIntPair_create(var1, var2);
    Chunk *var4 = ChunkMap_get(&self->chunkMapping, &var3);
    return var4 == NULL ? self->blankChunk : var4;
}

int ChunkProviderClient_saveChunks(ChunkProviderClient *self, int var1, IProgressUpdate *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 1;
}

int ChunkProviderClient_unload100OldestChunks(ChunkProviderClient *self) {
    (void)self;
    return 0;
}

int ChunkProviderClient_canSave(ChunkProviderClient *self) {
    (void)self;
    return 0;
}

void ChunkProviderClient_populate(ChunkProviderClient *self, IChunkProvider *var1, int var2,
                                  int var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
}

const char *ChunkProviderClient_makeString(ChunkProviderClient *self) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "MultiplayerChunkCache: %d", self->chunkMapping.size);
    return buf;
}

void ChunkProviderClient_destroy(ChunkProviderClient *self) {
    for (int b = 0; b < CHUNK_MAP_BUCKETS; ++b) {
        ChunkMapEntry *e = self->chunkMapping.buckets[b];
        while (e) {
            ChunkMapEntry *next = e->next;
            if (e->value)
                Chunk_destroy(e->value);
            free(e);
            e = next;
        }
    }
    free(self->field_889_c.entries);
    Chunk_destroy(self->blankChunk);
    free(self);
}
