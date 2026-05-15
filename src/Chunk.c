#include "Chunk.h"
#include "Block.h"
#include "TileEntity.h"
#include "World.h"
#include "ChunkBlockMap.h"
#include "Entity.h"
#include "MathHelper.h"
#include "AxisAlignedBB.h"
#include "EnumSkyBlock.h"
#include <stdlib.h>
#include <string.h>

static void Chunk_func_1020_f(Chunk *chunk, int var1, int var2, int var3);
static void Chunk_func_996_c(Chunk *chunk, int var1, int var2);
static void Chunk_func_1003_g(Chunk *chunk, int var1, int var2, int var3);

#define CHUNK_NIBBLE_COUNT (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

Chunk *Chunk_create(World *world, int chunkX, int chunkZ) {
    Chunk *self = (Chunk *)calloc(1, sizeof(Chunk));
    self->worldObj = world;
    self->xPosition = chunkX;
    self->zPosition = chunkZ;
    self->data = NibbleArray_create(CHUNK_NIBBLE_COUNT);
    self->skylightMap = NibbleArray_create(CHUNK_NIBBLE_COUNT);
    self->blocklightMap = NibbleArray_create(CHUNK_NIBBLE_COUNT);
    return self;
}

Chunk *Chunk_create_with_blocks(World *world, uint8_t *blocks, int chunkX, int chunkZ) {
    Chunk *self = Chunk_create(world, chunkX, chunkZ);
    memcpy(self->blocks, blocks, CHUNK_NIBBLE_COUNT);
    return self;
}

void Chunk_destroy(Chunk *chunk) {
    NibbleArray_free(chunk->data);
    NibbleArray_free(chunk->skylightMap);
    NibbleArray_free(chunk->blocklightMap);
    free(chunk->tileEntities);
    for (int i = 0; i < CHUNK_ENTITY_SECTIONS; i++)
        free(chunk->entitySections[i]);
    free(chunk);
}

int Chunk_getBlockID(Chunk *chunk, int x, int y, int z) {
    return chunk->blocks[x << 11 | z << 7 | y] & 0xFF;
}

int Chunk_setBlockID(Chunk *chunk, int var1, int var2, int var3, int var4) {
    if (chunk->isEmptyChunk)
        return 1;
    uint8_t var5 = (uint8_t)var4;
    int var6 = chunk->heightMap[var3 << 4 | var1] & 0xFF;
    int var7 = chunk->blocks[var1 << 11 | var3 << 7 | var2] & 0xFF;
    if (var7 == var4) {
        return 0;
    }
    int var8 = chunk->xPosition * 16 + var1;
    int var9 = chunk->zPosition * 16 + var3;
    chunk->blocks[var1 << 11 | var3 << 7 | var2] = (uint8_t)(var5 & 0xFF);
    if (var7 != 0) {
        Block *b = Block_blocksList[var7];
        if (b)
            b->vtable->onBlockRemoval(b, chunk->worldObj, var8, var2, var9);
    }
    NibbleArray_setNibble(chunk->data, var1, var2, var3, 0);
    if (Block_lightOpacity[var5 & 0xFF] != 0) {
        if (var2 >= var6) {
            Chunk_func_1003_g(chunk, var1, var2 + 1, var3);
        }
    } else if (var2 == var6 - 1) {
        Chunk_func_1003_g(chunk, var1, var2, var3);
    }
    World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var8, var2, var9, var8, var2,
                                 var9);
    World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Block, var8, var2, var9, var8, var2,
                                 var9);
    Chunk_func_996_c(chunk, var1, var3);
    if (var4 != 0 && !chunk->worldObj->multiplayerWorld) {
        Block *b = Block_blocksList[var4];
        if (b)
            b->vtable->onBlockAdded(b, chunk->worldObj, var8, var2, var9);
    }
    chunk->isModified = 1;
    return 1;
}

int Chunk_getBlockMetadata(Chunk *chunk, int x, int y, int z) {
    return NibbleArray_getNibble(chunk->data, x, y, z);
}

int Chunk_setBlockMetadata(Chunk *chunk, int x, int y, int z, int meta) {
    if (chunk->isEmptyChunk)
        return 1;
    chunk->isModified = 1;
    NibbleArray_setNibble(chunk->data, x, y, z, meta);
    return 1;
}

int Chunk_getSkyLight(Chunk *chunk, int x, int y, int z) {
    if (y < 0 || y >= CHUNK_SIZE_Y)
        return 15;
    return NibbleArray_getNibble(chunk->skylightMap, x & 15, y, z & 15);
}

void Chunk_setSkyLight(Chunk *chunk, int x, int y, int z, int val) {
    if (y < 0 || y >= CHUNK_SIZE_Y)
        return;
    NibbleArray_setNibble(chunk->skylightMap, x & 15, y, z & 15, val & 0xF);
}

int Chunk_getBlockLight(Chunk *chunk, int x, int y, int z) {
    if (y < 0 || y >= CHUNK_SIZE_Y)
        return 0;
    return NibbleArray_getNibble(chunk->blocklightMap, x & 15, y, z & 15);
}

void Chunk_setBlockLight(Chunk *chunk, int x, int y, int z, int val) {
    if (y < 0 || y >= CHUNK_SIZE_Y)
        return;
    NibbleArray_setNibble(chunk->blocklightMap, x & 15, y, z & 15, val & 0xF);
}

int Chunk_getHeightValue(Chunk *chunk, int x, int z) { return chunk->heightMap[z << 4 | x] & 0xFF; }

void Chunk_generateHeightMap(Chunk *chunk) {
    if (chunk->isEmptyChunk)
        return;
    int lowestBlockHeight = 127;
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
            int y = 127;
            int base = (x << 11) | (z << 7);

            while (y > 0 && Block_lightOpacity[chunk->blocks[base + y - 1] & 0xFF] == 0) {
                --y;
            }

            chunk->heightMap[(z << 4) | x] = (uint8_t)y;
            if (y < lowestBlockHeight)
                lowestBlockHeight = y;
        }
    }
    chunk->lowestBlockHeight = lowestBlockHeight;
    chunk->isModified = 1;
}

void Chunk_onChunkLoad(Chunk *chunk) {
    if (chunk->isEmptyChunk)
        return;
    chunk->isChunkLoaded = 1;
    World_func_31054_a(chunk->worldObj, (TileEntity **)chunk->tileEntities, chunk->numTileEntities);
    for (int var1 = 0; var1 < CHUNK_ENTITY_SECTIONS; ++var1) {
        World_func_636_a(chunk->worldObj, chunk->entitySections[var1],
                         chunk->entitySectionCounts[var1]);
    }
}

void Chunk_onChunkUnload(Chunk *chunk) {
    if (chunk->isEmptyChunk)
        return;
    chunk->isChunkLoaded = 0;
    for (int i = 0; i < chunk->numTileEntities; i++) {
        TileEntity_invalidate((TileEntity *)chunk->tileEntities[i]);
    }
    for (int var3 = 0; var3 < CHUNK_ENTITY_SECTIONS; ++var3) {
        World_func_632_b(chunk->worldObj, chunk->entitySections[var3],
                         chunk->entitySectionCounts[var3]);
    }
}

int Chunk_needsSaving(Chunk *chunk, int forceSave) {
    if (chunk->neverSave)
        return 0;
    if (forceSave) {
        if (chunk->hasEntities && World_getWorldTime(chunk->worldObj) != chunk->lastSaveTime)
            return 1;
    } else if (chunk->hasEntities &&
               World_getWorldTime(chunk->worldObj) >= chunk->lastSaveTime + 600L)
        return 1;
    return chunk->isModified;
}

void Chunk_setChunkModified(Chunk *chunk) {
    if (chunk->isEmptyChunk)
        return;
    chunk->isModified = 1;
}

int Chunk_isAtLocation(Chunk *chunk, int chunkX, int chunkZ) {
    return chunkX == chunk->xPosition && chunkZ == chunk->zPosition;
}

void Chunk_func_4143_d(Chunk *chunk) { (void)chunk; }

void Chunk_func_25124_i(Chunk *chunk) {
    ChunkBlockMap_func_26002_a(chunk->blocks, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

__thread int Chunk_isLit = 0;

int Chunk_getBlockLightValue(Chunk *chunk, int x, int y, int z, int skylightSubtracted) {
    int var5 = NibbleArray_getNibble(chunk->skylightMap, x, y, z);
    if (var5 > 0)
        Chunk_isLit = 1;
    var5 -= skylightSubtracted;
    int var6 = NibbleArray_getNibble(chunk->blocklightMap, x, y, z);
    if (var6 > var5)
        var5 = var6;
    return var5;
}

TileEntity *Chunk_getChunkBlockTileEntity(Chunk *chunk, int x, int y, int z) {
    if (chunk->isEmptyChunk)
        return NULL;
    int wx = chunk->xPosition * 16 + (x & 15);
    int wz = chunk->zPosition * 16 + (z & 15);
    TileEntity *var5 = NULL;
    for (int i = 0; i < chunk->numTileEntities; i++) {
        TileEntity *te = (TileEntity *)chunk->tileEntities[i];
        if (te && te->xCoord == wx && te->yCoord == y && te->zCoord == wz) {
            var5 = te;
            break;
        }
    }
    if (var5 == NULL) {
        int var6 = Chunk_getBlockID(chunk, x & 15, y, z & 15);
        if (!Block_isBlockContainer[var6])
            return NULL;
        Block *var7 = Block_blocksList[var6];
        var7->vtable->onBlockAdded(var7, chunk->worldObj, wx, y, wz);

        for (int i = 0; i < chunk->numTileEntities; i++) {
            TileEntity *te = (TileEntity *)chunk->tileEntities[i];
            if (te && te->xCoord == wx && te->yCoord == y && te->zCoord == wz) {
                var5 = te;
                break;
            }
        }
    }
    if (var5 != NULL && TileEntity_isInvalid(var5)) {

        for (int i = 0; i < chunk->numTileEntities; i++) {
            if (chunk->tileEntities[i] == (void *)var5) {
                chunk->tileEntities[i] = chunk->tileEntities[--chunk->numTileEntities];
                break;
            }
        }
        return NULL;
    }
    return var5;
}

void Chunk_addTileEntity(Chunk *chunk, TileEntity *var1) {
    if (chunk->isEmptyChunk)
        return;
    int var2 = var1->xCoord - chunk->xPosition * 16;
    int var3 = var1->yCoord;
    int var4 = var1->zCoord - chunk->zPosition * 16;
    Chunk_setChunkBlockTileEntity(chunk, var2, var3, var4, var1);
    if (chunk->isChunkLoaded) {

        World *w = chunk->worldObj;
        if (w->numLoadedTileEntities >= w->loadedTileEntityCap) {
            int newcap = w->loadedTileEntityCap == 0 ? 16 : w->loadedTileEntityCap * 2;
            w->loadedTileEntityList = (TileEntity **)realloc(w->loadedTileEntityList,
                                                             (size_t)newcap * sizeof(TileEntity *));
            w->loadedTileEntityCap = newcap;
        }
        w->loadedTileEntityList[w->numLoadedTileEntities++] = var1;
    }
}

void Chunk_setChunkBlockTileEntity(Chunk *chunk, int var1, int var2, int var3, TileEntity *var4) {
    if (chunk->isEmptyChunk)
        return;
    var4->worldObj = chunk->worldObj;
    var4->xCoord = chunk->xPosition * 16 + var1;
    var4->yCoord = var2;
    var4->zCoord = chunk->zPosition * 16 + var3;
    int id = Chunk_getBlockID(chunk, var1 & 15, var2, var3 & 15);
    if (id != 0 && Block_isBlockContainer[id]) {
        TileEntity_validate(var4);

        int wx = var4->xCoord;
        int wz = var4->zCoord;
        for (int i = 0; i < chunk->numTileEntities; i++) {
            TileEntity *existing = (TileEntity *)chunk->tileEntities[i];
            if (existing && existing->xCoord == wx && existing->yCoord == var2 &&
                existing->zCoord == wz) {
                chunk->tileEntities[i] = (void *)var4;
                return;
            }
        }
        if (chunk->numTileEntities >= chunk->tileEntityCap) {
            int newcap = chunk->tileEntityCap == 0 ? 8 : chunk->tileEntityCap * 2;
            chunk->tileEntities =
                (void **)realloc(chunk->tileEntities, (size_t)newcap * sizeof(void *));
            chunk->tileEntityCap = newcap;
        }
        chunk->tileEntities[chunk->numTileEntities++] = (void *)var4;
    }
}

void Chunk_removeChunkBlockTileEntity(Chunk *chunk, int var1, int var2, int var3) {
    if (chunk->isEmptyChunk)
        return;
    if (chunk->isChunkLoaded) {
        int wx = chunk->xPosition * 16 + (var1 & 15);
        int wz = chunk->zPosition * 16 + (var3 & 15);
        for (int i = 0; i < chunk->numTileEntities; i++) {
            TileEntity *te = (TileEntity *)chunk->tileEntities[i];
            if (te && te->xCoord == wx && te->yCoord == var2 && te->zCoord == wz) {
                chunk->tileEntities[i] = chunk->tileEntities[--chunk->numTileEntities];
                TileEntity_invalidate(te);
                return;
            }
        }
    }
}

int Chunk_canBlockSeeTheSky(Chunk *chunk, int x, int y, int z) {
    if (chunk->isEmptyChunk)
        return 0;
    return y >= (chunk->heightMap[(z << 4) | x] & 0xFF);
}

void Chunk_func_1014_a(Chunk *chunk) { (void)chunk; }

static void Chunk_func_1020_f(Chunk *chunk, int var1, int var2, int var3) {
    int var4 = World_getHeightValue(chunk->worldObj, var1, var2);
    if (var4 > var3) {
        World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var1, var3, var2, var1,
                                     var4, var2);
        chunk->isModified = 1;
    } else if (var4 < var3) {
        World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var1, var4, var2, var1,
                                     var3, var2);
        chunk->isModified = 1;
    }
}

static void Chunk_func_996_c(Chunk *chunk, int var1, int var2) {
    int var3 = Chunk_getHeightValue(chunk, var1, var2);
    int var4 = chunk->xPosition * 16 + var1;
    int var5 = chunk->zPosition * 16 + var2;
    Chunk_func_1020_f(chunk, var4 - 1, var5, var3);
    Chunk_func_1020_f(chunk, var4 + 1, var5, var3);
    Chunk_func_1020_f(chunk, var4, var5 - 1, var3);
    Chunk_func_1020_f(chunk, var4, var5 + 1, var3);
}

static void Chunk_func_1003_g(Chunk *chunk, int var1, int var2, int var3) {
    int var4 = chunk->heightMap[var3 << 4 | var1] & 0xFF;
    int var5 = var4;
    if (var2 > var4) {
        var5 = var2;
    }
    int var6 = var1 << 11 | var3 << 7;
    while (var5 > 0 && Block_lightOpacity[chunk->blocks[var6 + var5 - 1] & 0xFF] == 0) {
        --var5;
    }
    if (var5 != var4) {
        World_markBlocksDirtyVertical(chunk->worldObj, var1, var3, var5, var4);
        chunk->heightMap[var3 << 4 | var1] = (uint8_t)var5;
        int var7;
        int var8;
        int var9;
        if (var5 < chunk->lowestBlockHeight) {
            chunk->lowestBlockHeight = var5;
        } else {
            var7 = 127;
            for (var8 = 0; var8 < 16; ++var8) {
                for (var9 = 0; var9 < 16; ++var9) {
                    if ((chunk->heightMap[var9 << 4 | var8] & 0xFF) < var7) {
                        var7 = chunk->heightMap[var9 << 4 | var8] & 0xFF;
                    }
                }
            }
            chunk->lowestBlockHeight = var7;
        }
        var7 = chunk->xPosition * 16 + var1;
        var8 = chunk->zPosition * 16 + var3;
        if (var5 < var4) {
            for (var9 = var5; var9 < var4; ++var9) {
                NibbleArray_setNibble(chunk->skylightMap, var1, var9, var3, 15);
            }
        } else {
            World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var7, var4, var8, var7,
                                         var5, var8);
            for (var9 = var4; var9 < var5; ++var9) {
                NibbleArray_setNibble(chunk->skylightMap, var1, var9, var3, 0);
            }
        }
        var9 = 15;
        int var10;
        for (var10 = var5; var5 > 0 && var9 > 0;
             NibbleArray_setNibble(chunk->skylightMap, var1, var5, var3, var9)) {
            --var5;
            int var11 = Block_lightOpacity[Chunk_getBlockID(chunk, var1, var5, var3)];
            if (var11 == 0) {
                var11 = 1;
            }
            var9 -= var11;
            if (var9 < 0) {
                var9 = 0;
            }
        }
        while (var5 > 0 && Block_lightOpacity[Chunk_getBlockID(chunk, var1, var5 - 1, var3)] == 0) {
            --var5;
        }
        if (var5 != var10) {
            World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var7 - 1, var5,
                                         var8 - 1, var7 + 1, var10, var8 + 1);
        }
        chunk->isModified = 1;
    }
}

void Chunk_func_1024_c(Chunk *chunk) {
    if (chunk->isEmptyChunk)
        return;
    int var1 = 127;
    int var2;
    int var3;
    for (var2 = 0; var2 < 16; ++var2) {
        for (var3 = 0; var3 < 16; ++var3) {
            int var4 = 127;
            int var5 = var2 << 11 | var3 << 7;
            while (var4 > 0 && Block_lightOpacity[chunk->blocks[var5 + var4 - 1] & 0xFF] == 0) {
                --var4;
            }
            chunk->heightMap[var3 << 4 | var2] = (uint8_t)var4;
            if (var4 < var1) {
                var1 = var4;
            }
            if (!chunk->worldObj->worldProvider->hasNoSky) {
                int var6 = 15;
                int var7 = 127;
                do {
                    var6 -= Block_lightOpacity[chunk->blocks[var5 + var7] & 0xFF];
                    if (var6 > 0) {
                        NibbleArray_setNibble(chunk->skylightMap, var2, var7, var3, var6);
                    }
                    --var7;
                } while (var7 > 0 && var6 > 0);
            }
        }
    }
    chunk->lowestBlockHeight = var1;
    for (var2 = 0; var2 < 16; ++var2) {
        for (var3 = 0; var3 < 16; ++var3) {
            Chunk_func_996_c(chunk, var2, var3);
        }
    }
    chunk->isModified = 1;
}

int Chunk_func_21167_h(Chunk *chunk) { return chunk->isEmptyChunk ? 1 : 0; }

void Chunk_func_997_a(Chunk *chunk, int64_t var1, JavaRandom *out) {

    int64_t seed = World_getRandomSeed(chunk->worldObj) +
                       (int64_t)(int)((unsigned int)chunk->xPosition *
                                      (unsigned int)chunk->xPosition * 4987142U) +
                       (int64_t)(int)((unsigned int)chunk->xPosition * 5947611U) +
                       (int64_t)(chunk->zPosition * chunk->zPosition) * 4392871L +
                       (int64_t)(int)((unsigned int)chunk->zPosition * 389711U) ^
                   var1;
    JavaRandom_init(out, seed);
}

int Chunk_setBlockIDWithMetadata(Chunk *chunk, int var1, int var2, int var3, int var4, int var5) {
    if (chunk->isEmptyChunk)
        return 1;
    uint8_t var6 = (uint8_t)var4;
    int var7 = chunk->heightMap[var3 << 4 | var1] & 0xFF;
    int var8 = chunk->blocks[var1 << 11 | var3 << 7 | var2] & 0xFF;
    if (var8 == var4 && NibbleArray_getNibble(chunk->data, var1, var2, var3) == var5) {
        return 0;
    }
    int var9 = chunk->xPosition * 16 + var1;
    int var10 = chunk->zPosition * 16 + var3;
    chunk->blocks[var1 << 11 | var3 << 7 | var2] = (uint8_t)(var6 & 0xFF);
    if (var8 != 0 && !chunk->worldObj->multiplayerWorld) {
        Block *b = Block_blocksList[var8];
        if (b)
            b->vtable->onBlockRemoval(b, chunk->worldObj, var9, var2, var10);
    }
    NibbleArray_setNibble(chunk->data, var1, var2, var3, var5);
    if (!chunk->worldObj->worldProvider->hasNoSky) {
        if (Block_lightOpacity[var6 & 0xFF] != 0) {
            if (var2 >= var7) {
                Chunk_func_1003_g(chunk, var1, var2 + 1, var3);
            }
        } else if (var2 == var7 - 1) {
            Chunk_func_1003_g(chunk, var1, var2, var3);
        }
        World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Sky, var9, var2, var10, var9,
                                     var2, var10);
    }
    World_scheduleLightingUpdate(chunk->worldObj, EnumSkyBlock_Block, var9, var2, var10, var9, var2,
                                 var10);
    Chunk_func_996_c(chunk, var1, var3);
    NibbleArray_setNibble(chunk->data, var1, var2, var3, var5);
    if (var4 != 0) {
        Block *b = Block_blocksList[var4];
        if (b)
            b->vtable->onBlockAdded(b, chunk->worldObj, var9, var2, var10);
    }
    chunk->isModified = 1;
    return 1;
}

int Chunk_setChunkData(Chunk *chunk, uint8_t *var1, int var2, int var3, int var4, int var5,
                       int var6, int var7, int var8) {
    if (chunk->isEmptyChunk) {
        int var9 = var5 - var2;
        int var10 = var6 - var3;
        int var11 = var7 - var4;
        int var12 = var9 * var10 * var11;
        return var12 + var12 / 2 * 3;
    }
    int var9;
    int var10;
    int var11;
    int var12;
    for (var9 = var2; var9 < var5; ++var9) {
        for (var10 = var4; var10 < var7; ++var10) {
            var11 = var9 << 11 | var10 << 7 | var3;
            var12 = var6 - var3;
            memcpy(chunk->blocks + var11, var1 + var8, (size_t)var12);
            var8 += var12;
        }
    }
    Chunk_generateHeightMap(chunk);
    for (var9 = var2; var9 < var5; ++var9) {
        for (var10 = var4; var10 < var7; ++var10) {
            var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
            var12 = (var6 - var3) / 2;
            memcpy(chunk->data->data + var11, var1 + var8, (size_t)var12);
            var8 += var12;
        }
    }
    for (var9 = var2; var9 < var5; ++var9) {
        for (var10 = var4; var10 < var7; ++var10) {
            var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
            var12 = (var6 - var3) / 2;
            memcpy(chunk->blocklightMap->data + var11, var1 + var8, (size_t)var12);
            var8 += var12;
        }
    }
    for (var9 = var2; var9 < var5; ++var9) {
        for (var10 = var4; var10 < var7; ++var10) {
            var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
            var12 = (var6 - var3) / 2;
            memcpy(chunk->skylightMap->data + var11, var1 + var8, (size_t)var12);
            var8 += var12;
        }
    }
    return var8;
}

int Chunk_getSavedLightValue(Chunk *chunk, EnumSkyBlock var1, int var2, int var3, int var4) {
    if (var1 == EnumSkyBlock_Sky)
        return NibbleArray_getNibble(chunk->skylightMap, var2, var3, var4);
    if (var1 == EnumSkyBlock_Block)
        return NibbleArray_getNibble(chunk->blocklightMap, var2, var3, var4);
    return 0;
}

void Chunk_setLightValue(Chunk *chunk, EnumSkyBlock var1, int var2, int var3, int var4, int var5) {
    if (chunk->isEmptyChunk)
        return;
    chunk->isModified = 1;
    if (var1 == EnumSkyBlock_Sky) {
        NibbleArray_setNibble(chunk->skylightMap, var2, var3, var4, var5);
    } else if (var1 == EnumSkyBlock_Block) {
        NibbleArray_setNibble(chunk->blocklightMap, var2, var3, var4, var5);
    }
}

void Chunk_addEntity(Chunk *chunk, struct Entity *var1) {
    if (chunk->isEmptyChunk)
        return;
    chunk->hasEntities = 1;
    int var4 = MathHelper_floor_double(var1->posY / 16.0);
    if (var4 < 0)
        var4 = 0;
    if (var4 >= CHUNK_ENTITY_SECTIONS)
        var4 = CHUNK_ENTITY_SECTIONS - 1;
    var1->addedToChunk = 1;
    var1->chunkCoordX = chunk->xPosition;
    var1->chunkCoordY = var4;
    var1->chunkCoordZ = chunk->zPosition;

    if (chunk->entitySectionCounts[var4] >= chunk->entitySectionCaps[var4]) {
        int newcap = chunk->entitySectionCaps[var4] == 0 ? 8 : chunk->entitySectionCaps[var4] * 2;
        chunk->entitySections[var4] = (struct Entity **)realloc(
            chunk->entitySections[var4], (size_t)newcap * sizeof(struct Entity *));
        chunk->entitySectionCaps[var4] = newcap;
    }
    chunk->entitySections[var4][chunk->entitySectionCounts[var4]++] = var1;
}

void Chunk_removeEntity(Chunk *chunk, struct Entity *var1) {
    if (chunk->isEmptyChunk)
        return;
    Chunk_removeEntityAtIndex(chunk, var1, var1->chunkCoordY);
}

void Chunk_removeEntityAtIndex(Chunk *chunk, struct Entity *var1, int var2) {
    if (chunk->isEmptyChunk)
        return;
    if (var2 < 0)
        var2 = 0;
    if (var2 >= CHUNK_ENTITY_SECTIONS)
        var2 = CHUNK_ENTITY_SECTIONS - 1;
    struct Entity **sec = chunk->entitySections[var2];
    int count = chunk->entitySectionCounts[var2];
    for (int i = 0; i < count; i++) {
        if (sec[i] == var1) {
            sec[i] = sec[count - 1];
            chunk->entitySectionCounts[var2]--;
            return;
        }
    }
}

int Chunk_getEntitiesWithinAABBForEntity(Chunk *chunk, struct Entity *var1, AxisAlignedBB *var2,
                                         struct Entity **outList, int outOffset, int outCap) {
    if (chunk->isEmptyChunk)
        return 0;
    int var4 = MathHelper_floor_double((var2->minY - 2.0) / 16.0);
    int var5 = MathHelper_floor_double((var2->maxY + 2.0) / 16.0);
    if (var4 < 0)
        var4 = 0;
    if (var5 >= CHUNK_ENTITY_SECTIONS)
        var5 = CHUNK_ENTITY_SECTIONS - 1;
    int written = 0;
    for (int var6 = var4; var6 <= var5; var6++) {
        struct Entity **var7 = chunk->entitySections[var6];
        int var7size = chunk->entitySectionCounts[var6];
        for (int var8 = 0; var8 < var7size; var8++) {
            struct Entity *var9 = var7[var8];
            if (var9 != var1 && AxisAlignedBB_intersectsWith(&var9->boundingBox, var2)) {
                if (outOffset + written < outCap) {
                    outList[outOffset + written] = var9;
                    written++;
                }
            }
        }
    }
    return written;
}

int Chunk_getEntitiesOfTypeWithinAAAB(Chunk *chunk, int (*match)(struct Entity *),
                                      AxisAlignedBB *var2, struct Entity **outList, int outOffset,
                                      int outCap) {
    if (chunk->isEmptyChunk)
        return 0;
    int var4 = MathHelper_floor_double((var2->minY - 2.0) / 16.0);
    int var5 = MathHelper_floor_double((var2->maxY + 2.0) / 16.0);
    if (var4 < 0)
        var4 = 0;
    if (var5 >= CHUNK_ENTITY_SECTIONS)
        var5 = CHUNK_ENTITY_SECTIONS - 1;
    int written = 0;
    for (int var6 = var4; var6 <= var5; var6++) {
        struct Entity **var7 = chunk->entitySections[var6];
        int var7size = chunk->entitySectionCounts[var6];
        for (int var8 = 0; var8 < var7size; var8++) {
            struct Entity *var9 = var7[var8];
            if (match(var9) && AxisAlignedBB_intersectsWith(&var9->boundingBox, var2)) {
                if (outOffset + written < outCap) {
                    outList[outOffset + written] = var9;
                    written++;
                }
            }
        }
    }
    return written;
}
