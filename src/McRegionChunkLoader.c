#include "McRegionChunkLoader.h"
#include "RegionFileCache.h"
#include "ChunkLoader.h"
#include "CompressedStreamTools.h"
#include "NBTTagCompound.h"
#include "NBTBase.h"
#include "DataStream.h"
#include "Chunk.h"
#include "World.h"
#include "WorldInfo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Chunk *mrcl_loadChunk(IChunkLoader *iself, World *world, int x, int z) {
    McRegionChunkLoader *self = (McRegionChunkLoader *)iself;
    DataStream ds = RegionFileCache_getChunkInputStream(self->worldDir, x, z);
    if (ds.error || ds.buf == NULL) {
        return NULL;
    }
    NBTTagCompound *var5 = CompressedStreamTools_func_1141_a(&ds);
    free(ds.buf);
    if (var5 == NULL)
        return NULL;
    if (!NBTTagCompound_hasKey(var5, "Level")) {
        fprintf(stderr, "Chunk file at %d,%d is missing level data, skipping\n", x, z);
        NBT_free((NBTBase *)var5);
        return NULL;
    }
    if (!NBTTagCompound_hasKey(NBTTagCompound_getCompoundTag(var5, "Level"), "Blocks")) {
        fprintf(stderr, "Chunk file at %d,%d is missing block data, skipping\n", x, z);
        NBT_free((NBTBase *)var5);
        return NULL;
    }
    Chunk *var6 = ChunkLoader_loadChunkIntoWorldFromCompound(
        world, NBTTagCompound_getCompoundTag(var5, "Level"));
    if (!Chunk_isAtLocation(var6, x, z)) {
        fprintf(stderr,
                "Chunk file at %d,%d is in the wrong location; relocating. (Expected %d, %d, got "
                "%d, %d)\n",
                x, z, x, z, var6->xPosition, var6->zPosition);
        NBTTagCompound_setInteger(var5, "xPos", x);
        NBTTagCompound_setInteger(var5, "zPos", z);
        var6 = ChunkLoader_loadChunkIntoWorldFromCompound(
            world, NBTTagCompound_getCompoundTag(var5, "Level"));
    }
    Chunk_func_25124_i(var6);
    NBT_free((NBTBase *)var5);
    return var6;
}

static void mrcl_saveChunk(IChunkLoader *iself, World *world, Chunk *chunk) {
    McRegionChunkLoader *self = (McRegionChunkLoader *)iself;
    World_checkSessionLock(world);
    RegionFileChunkBuffer *var3 =
        RegionFileCache_getChunkOutputStream(self->worldDir, chunk->xPosition, chunk->zPosition);
    if (var3 == NULL)
        return;
    NBTTagCompound *var4 = NBTTagCompound_create();
    NBTTagCompound *var5 = NBTTagCompound_create();
    NBTTagCompound_setTag(var4, "Level", (NBTBase *)var5);
    ChunkLoader_storeChunkInCompound(chunk, world, var5);
    DataStream ds = DataStream_growable(65536);
    CompressedStreamTools_func_1139_a(var4, &ds);
    NBT_free((NBTBase *)var4);
    RegionFileChunkBuffer_writeBytes(var3, ds.buf, 0, (int)ds.pos);
    free(ds.buf);
    RegionFileChunkBuffer_close(var3);
    RegionFileChunkBuffer_destroy(var3);
    WorldInfo *var6 = World_getWorldInfo(world);
    WorldInfo_setSizeOnDisk(var6, WorldInfo_getSizeOnDisk(var6) +
                                      (long)RegionFileCache_getSizeDelta(
                                          self->worldDir, chunk->xPosition, chunk->zPosition));
}

static void mrcl_saveExtraChunkData(IChunkLoader *iself, World *world, Chunk *chunk) {
    (void)iself;
    (void)world;
    (void)chunk;
}

static void mrcl_func_814_a(IChunkLoader *iself) { (void)iself; }

static void mrcl_saveExtraData(IChunkLoader *iself) { (void)iself; }

static const IChunkLoaderVtable McRegionChunkLoader_vtable = {
    .loadChunk = mrcl_loadChunk,
    .saveChunk = mrcl_saveChunk,
    .saveExtraChunkData = mrcl_saveExtraChunkData,
    .func_814_a = mrcl_func_814_a,
    .saveExtraData = mrcl_saveExtraData,
};

McRegionChunkLoader *McRegionChunkLoader_create(const char *worldDir) {
    McRegionChunkLoader *self = (McRegionChunkLoader *)calloc(1, sizeof(McRegionChunkLoader));
    self->base.vtable = &McRegionChunkLoader_vtable;
    self->worldDir = strdup(worldDir);
    return self;
}

void McRegionChunkLoader_destroy(McRegionChunkLoader *self) {
    free(self->worldDir);
    free(self);
}
