#include "ChunkLoader.h"
#include "Chunk.h"
#include "ChunkBlockMap.h"
#include "World.h"
#include "NibbleArray.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "EntityList.h"
#include "TileEntity.h"
#include "Entity.h"
#include "CompressedStreamTools.h"
#include "WorldInfo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"

static void int_to_base36(int n, char *out, int bufsize) {
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char tmp[64];
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    int i = 0;
    if (n == 0) {
        tmp[i++] = '0';
    } else {
        while (n > 0) {
            tmp[i++] = digits[n % 36];
            n /= 36;
        }
    }
    int out_i = 0;
    if (neg && out_i + 1 < bufsize)
        out[out_i++] = '-';
    for (int j = i - 1; j >= 0 && out_i + 1 < bufsize; --j) {
        out[out_i++] = tmp[j];
    }
    out[out_i] = '\0';
}

static char *cl_chunkFileForXZ(ChunkLoader *self, int x, int z);

static Chunk *cl_loadChunk(IChunkLoader *iself, World *world, int x, int z) {
    ChunkLoader *self = (ChunkLoader *)iself;
    char *path = cl_chunkFileForXZ(self, x, z);
    if (path == NULL)
        return NULL;

    FILE *f = fopen(path, "rb");
    free(path);
    if (f == NULL)
        return NULL;

    NBTTagCompound *var6 = CompressedStreamTools_func_1138_a(f);
    fclose(f);
    if (var6 == NULL)
        return NULL;

    if (!NBTTagCompound_hasKey(var6, "Level")) {
        fprintf(stderr, "Chunk file at %d,%d is missing level data, skipping\n", x, z);
        return NULL;
    }
    NBTTagCompound *level = NBTTagCompound_getCompoundTag(var6, "Level");
    if (!NBTTagCompound_hasKey(level, "Blocks")) {
        fprintf(stderr, "Chunk file at %d,%d is missing block data, skipping\n", x, z);
        return NULL;
    }

    Chunk *var7 = ChunkLoader_loadChunkIntoWorldFromCompound(world, level);
    if (var7 == NULL)
        return NULL;

    if (!Chunk_isAtLocation(var7, x, z)) {
        fprintf(stderr,
                "Chunk file at %d,%d is in the wrong location; relocating. "
                "(Expected %d, %d, got %d, %d)\n",
                x, z, x, z, var7->xPosition, var7->zPosition);
        NBTTagCompound_setInteger(level, "xPos", x);
        NBTTagCompound_setInteger(level, "zPos", z);
        var7 = ChunkLoader_loadChunkIntoWorldFromCompound(world, level);
    }

    if (var7 != NULL) {
        Chunk_func_25124_i(var7);
    }
    return var7;
}

static void cl_saveChunk(IChunkLoader *iself, World *world, Chunk *chunk) {
    ChunkLoader *self = (ChunkLoader *)iself;

    World_checkSessionLock(world);

    char *chunkPath = cl_chunkFileForXZ(self, chunk->xPosition, chunk->zPosition);
    if (chunkPath == NULL)
        return;

    struct stat st;
    WorldInfo *winfo = World_getWorldInfo(world);
    if (stat(chunkPath, &st) == 0) {
        WorldInfo_setSizeOnDisk(winfo, WorldInfo_getSizeOnDisk(winfo) - (int64_t)st.st_size);
    }

    char tmpPath[4096];
    snprintf(tmpPath, sizeof(tmpPath), "%s/tmp_chunk.dat", self->saveDir);

    FILE *f = fopen(tmpPath, "wb");
    if (f == NULL) {
        free(chunkPath);
        return;
    }

    NBTTagCompound *var6 = NBTTagCompound_create();
    NBTTagCompound *var7 = NBTTagCompound_create();
    NBTTagCompound_setCompoundTag(var6, "Level", var7);
    ChunkLoader_storeChunkInCompound(chunk, world, var7);
    CompressedStreamTools_writeGzippedCompoundToOutputStream(var6, f);
    fclose(f);

    remove(chunkPath);
    rename(tmpPath, chunkPath);

    if (stat(chunkPath, &st) == 0) {
        WorldInfo_setSizeOnDisk(winfo, WorldInfo_getSizeOnDisk(winfo) + (int64_t)st.st_size);
    }

    free(chunkPath);
}

static void cl_saveExtraChunkData(IChunkLoader *iself, World *world, Chunk *chunk) {
    (void)iself;
    (void)world;
    (void)chunk;
}

static void cl_func_814_a(IChunkLoader *iself) { (void)iself; }

static void cl_saveExtraData(IChunkLoader *iself) { (void)iself; }

static const IChunkLoaderVtable ChunkLoader_vtable = {
    .loadChunk = cl_loadChunk,
    .saveChunk = cl_saveChunk,
    .saveExtraChunkData = cl_saveExtraChunkData,
    .func_814_a = cl_func_814_a,
    .saveExtraData = cl_saveExtraData,
};

ChunkLoader *ChunkLoader_create(const char *saveDir, int var2) {
    ChunkLoader *self = (ChunkLoader *)calloc(1, sizeof(ChunkLoader));
    self->base.vtable = &ChunkLoader_vtable;
    self->saveDir = strdup(saveDir);
    self->createIfNecessary = var2;
    return self;
}

void ChunkLoader_destroy(ChunkLoader *self) {
    free(self->saveDir);
    free(self);
}

static char *cl_chunkFileForXZ(ChunkLoader *self, int x, int z) {
    char xFolder[16], zFolder[16], xStr[16], zStr[16];
    int_to_base36(x & 63, xFolder, sizeof(xFolder));
    int_to_base36(z & 63, zFolder, sizeof(zFolder));
    int_to_base36(x, xStr, sizeof(xStr));
    int_to_base36(z, zStr, sizeof(zStr));

    char dir1[4096], dir2[4096], filePath[4096];
    snprintf(dir1, sizeof(dir1), "%s/%s", self->saveDir, xFolder);
    snprintf(dir2, sizeof(dir2), "%s/%s/%s", self->saveDir, xFolder, zFolder);
    snprintf(filePath, sizeof(filePath), "%s/%s/%s/c.%s.%s.dat", self->saveDir, xFolder, zFolder,
             xStr, zStr);

    struct stat st;
    if (stat(dir1, &st) != 0) {
        if (!self->createIfNecessary)
            return NULL;
        mc_mkdir(dir1);
    }

    if (stat(dir2, &st) != 0) {
        if (!self->createIfNecessary)
            return NULL;
        mc_mkdir(dir2);
    }

    if (stat(filePath, &st) != 0 && !self->createIfNecessary)
        return NULL;

    char *out = (char *)malloc(4096);
    snprintf(out, 4096, "%s", filePath);
    return out;
}

char *ChunkLoader_chunkFileForXZ(ChunkLoader *self, int x, int z) {
    return cl_chunkFileForXZ(self, x, z);
}

void ChunkLoader_storeChunkInCompound(Chunk *chunk, World *world, NBTTagCompound *tag) {
    World_checkSessionLock(world);

    NBTTagCompound_setInteger(tag, "xPos", chunk->xPosition);
    NBTTagCompound_setInteger(tag, "zPos", chunk->zPosition);
    NBTTagCompound_setLong(tag, "LastUpdate", World_getWorldTime(world));

    NBTTagCompound_setByteArray(tag, "Blocks", chunk->blocks,
                                CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

    NBTTagCompound_setByteArray(tag, "Data", chunk->data->data,
                                CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);

    NBTTagCompound_setByteArray(tag, "SkyLight", chunk->skylightMap->data,
                                CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);

    NBTTagCompound_setByteArray(tag, "BlockLight", chunk->blocklightMap->data,
                                CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);

    NBTTagCompound_setByteArray(tag, "HeightMap", chunk->heightMap, CHUNK_SIZE_X * CHUNK_SIZE_Z);

    NBTTagCompound_setBoolean(tag, "TerrainPopulated", chunk->isTerrainPopulated);

    chunk->hasEntities = 0;
    NBTTagList *entities = NBTTagList_create();
    for (int var4 = 0; var4 < CHUNK_ENTITY_SECTIONS; ++var4) {
        for (int var5i = 0; var5i < chunk->entitySectionCounts[var4]; ++var5i) {
            Entity *var6 = chunk->entitySections[var4][var5i];
            chunk->hasEntities = 1;
            NBTTagCompound *var7 = NBTTagCompound_create();
            if (Entity_addEntityID(var6, var7)) {
                NBTTagList_setTag(entities, (NBTBase *)var7);
            } else {
                NBT_free(var7);
            }
        }
    }
    NBTTagCompound_setTag(tag, "Entities", (NBTBase *)entities);

    NBTTagList *tileEntities = NBTTagList_create();
    for (int i = 0; i < chunk->numTileEntities; ++i) {
        TileEntity *te = (TileEntity *)chunk->tileEntities[i];
        NBTTagCompound *teTag = NBTTagCompound_create();
        TileEntity_writeToNBT(te, teTag);
        NBTTagList_setTag(tileEntities, (NBTBase *)teTag);
    }
    NBTTagCompound_setTag(tag, "TileEntities", (NBTBase *)tileEntities);
}

Chunk *ChunkLoader_loadChunkIntoWorldFromCompound(World *world, NBTTagCompound *tag) {
    int var2 = NBTTagCompound_getInteger(tag, "xPos");
    int var3 = NBTTagCompound_getInteger(tag, "zPos");
    Chunk *var4 = Chunk_create(world, var2, var3);

    uint8_t *blocks = NBTTagCompound_getByteArray(tag, "Blocks");
    if (blocks != NULL) {
        memcpy(var4->blocks, blocks, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
    }

    uint8_t *dataArr = NBTTagCompound_getByteArray(tag, "Data");
    if (dataArr != NULL) {
        NibbleArray_free(var4->data);
        var4->data = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
        memcpy(var4->data->data, dataArr, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);
    }

    uint8_t *skyArr = NBTTagCompound_getByteArray(tag, "SkyLight");
    if (skyArr != NULL) {
        NibbleArray_free(var4->skylightMap);
        var4->skylightMap = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
        memcpy(var4->skylightMap->data, skyArr, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);
    }

    uint8_t *blkArr = NBTTagCompound_getByteArray(tag, "BlockLight");
    if (blkArr != NULL) {
        NibbleArray_free(var4->blocklightMap);
        var4->blocklightMap = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
        memcpy(var4->blocklightMap->data, blkArr, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z / 2);
    }

    uint8_t *hmap = NBTTagCompound_getByteArray(tag, "HeightMap");
    if (hmap != NULL) {
        memcpy(var4->heightMap, hmap, CHUNK_SIZE_X * CHUNK_SIZE_Z);
    }

    var4->isTerrainPopulated = NBTTagCompound_getBoolean(tag, "TerrainPopulated");

    if (!NibbleArray_isValid(var4->data)) {
        NibbleArray_free(var4->data);
        var4->data = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
    }

    if (hmap == NULL || !NibbleArray_isValid(var4->skylightMap)) {
        memset(var4->heightMap, 0, sizeof(var4->heightMap));
        NibbleArray_free(var4->skylightMap);
        var4->skylightMap = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
        Chunk_func_1024_c(var4);
    }

    if (!NibbleArray_isValid(var4->blocklightMap)) {
        NibbleArray_free(var4->blocklightMap);
        var4->blocklightMap = NibbleArray_create(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
        Chunk_func_1014_a(var4);
    }

    NBTTagList *entList = NBTTagCompound_getTagList(tag, "Entities");
    if (entList != NULL) {
        for (int i = 0; i < NBTTagList_tagCount(entList); ++i) {
            NBTTagCompound *eTag = (NBTTagCompound *)NBTTagList_tagAt(entList, i);
            Entity *ent = EntityList_createEntityFromNBT(eTag, world);
            var4->hasEntities = 1;
            if (ent != NULL) {
                Chunk_addEntity(var4, ent);
            }
        }
    }

    NBTTagList *teList = NBTTagCompound_getTagList(tag, "TileEntities");
    if (teList != NULL) {
        for (int i = 0; i < NBTTagList_tagCount(teList); ++i) {
            NBTTagCompound *teTag = (NBTTagCompound *)NBTTagList_tagAt(teList, i);
            TileEntity *te = TileEntity_createAndLoadEntity(teTag);
            if (te != NULL) {
                Chunk_addTileEntity(var4, te);
            }
        }
    }

    return var4;
}
