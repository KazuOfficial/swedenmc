#include "ChunkCache.h"
#include "Material.h"
#include "Block.h"
#include <stdlib.h>
#include <string.h>

static int iba_getBlockId(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_getBlockId((ChunkCache *)self, x, y, z);
}
static TileEntity *iba_getBlockTileEntity(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_getBlockTileEntity((ChunkCache *)self, x, y, z);
}
static float iba_getBrightness(IBlockAccess *self, int x, int y, int z, int var4) {
    return ChunkCache_getBrightness((ChunkCache *)self, x, y, z, var4);
}
static float iba_getLightBrightness(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_getLightBrightness((ChunkCache *)self, x, y, z);
}
static int iba_getBlockMetadata(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_getBlockMetadata((ChunkCache *)self, x, y, z);
}
static Material *iba_getBlockMaterial(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_getBlockMaterial((ChunkCache *)self, x, y, z);
}
static int iba_isBlockOpaqueCube(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_isBlockOpaqueCube((ChunkCache *)self, x, y, z);
}
static int iba_isBlockNormalCube(IBlockAccess *self, int x, int y, int z) {
    return ChunkCache_isBlockNormalCube((ChunkCache *)self, x, y, z);
}
static struct WorldChunkManager *iba_getWorldChunkManager(IBlockAccess *self) {
    return ChunkCache_getWorldChunkManager((ChunkCache *)self);
}

IBlockAccessVtable ChunkCache_vtable = {
    .getBlockId = iba_getBlockId,
    .getBlockTileEntity = iba_getBlockTileEntity,
    .getBrightness = iba_getBrightness,
    .getLightBrightness = iba_getLightBrightness,
    .getBlockMetadata = iba_getBlockMetadata,
    .getBlockMaterial = iba_getBlockMaterial,
    .isBlockOpaqueCube = iba_isBlockOpaqueCube,
    .isBlockNormalCube = iba_isBlockNormalCube,
    .getWorldChunkManager = iba_getWorldChunkManager,
};

ChunkCache *ChunkCache_create(World *world, int minX, int minY, int minZ, int maxX, int maxY,
                              int maxZ) {
    (void)minY;
    (void)maxY;
    ChunkCache *self = (ChunkCache *)calloc(1, sizeof(ChunkCache));
    self->base.vtable = &ChunkCache_vtable;
    self->worldObj = world;
    self->chunkX = minX >> 4;
    self->chunkZ = minZ >> 4;
    int endCX = maxX >> 4;
    int endCZ = maxZ >> 4;
    self->sizeX = endCX - self->chunkX + 1;
    self->sizeZ = endCZ - self->chunkZ + 1;
    self->chunkArray = (Chunk **)calloc(self->sizeX * self->sizeZ, sizeof(Chunk *));
    for (int cx = self->chunkX; cx <= endCX; cx++) {
        for (int cz = self->chunkZ; cz <= endCZ; cz++) {
            int idx = (cx - self->chunkX) * self->sizeZ + (cz - self->chunkZ);
            self->chunkArray[idx] = World_chunkExists(world, cx, cz)
                                        ? world->vtable->getChunkFromChunkCoords(world, cx, cz)
                                        : NULL;
        }
    }
    return self;
}

void ChunkCache_destroy(ChunkCache *self) {
    free(self->chunkArray);
    free(self);
}

void ChunkCache_init(ChunkCache *self, World *world, int minX, int minY, int minZ, int maxX,
                     int maxY, int maxZ) {
    (void)minY;
    (void)maxY;
    self->base.vtable = &ChunkCache_vtable;
    self->worldObj = world;
    self->chunkX = minX >> 4;
    self->chunkZ = minZ >> 4;
    int endCX = maxX >> 4;
    int endCZ = maxZ >> 4;
    self->sizeX = endCX - self->chunkX + 1;
    self->sizeZ = endCZ - self->chunkZ + 1;
    self->chunkArray = (Chunk **)calloc(self->sizeX * self->sizeZ, sizeof(Chunk *));
    for (int cx = self->chunkX; cx <= endCX; cx++) {
        for (int cz = self->chunkZ; cz <= endCZ; cz++) {
            int idx = (cx - self->chunkX) * self->sizeZ + (cz - self->chunkZ);

            self->chunkArray[idx] = World_chunkExists(world, cx, cz)
                                        ? world->vtable->getChunkFromChunkCoords(world, cx, cz)
                                        : NULL;
        }
    }
}

void ChunkCache_freeContents(ChunkCache *self) {
    free(self->chunkArray);
    self->chunkArray = NULL;
}

int ChunkCache_getBlockId(ChunkCache *self, int x, int y, int z) {
    if (y < 0 || y >= 128)
        return 0;
    int cx = (x >> 4) - self->chunkX;
    int cz = (z >> 4) - self->chunkZ;
    if (cx < 0 || cx >= self->sizeX || cz < 0 || cz >= self->sizeZ)
        return 0;
    Chunk *chunk = self->chunkArray[cx * self->sizeZ + cz];
    if (!chunk)
        return 0;
    return Chunk_getBlockID(chunk, x & 15, y, z & 15);
}

int ChunkCache_getBlockMetadata(ChunkCache *self, int x, int y, int z) {
    if (y < 0 || y >= 128)
        return 0;
    int var4 = (x >> 4) - self->chunkX;
    int var5 = (z >> 4) - self->chunkZ;
    if (var4 < 0 || var4 >= self->sizeX || var5 < 0 || var5 >= self->sizeZ)
        return 0;
    Chunk *chunk = self->chunkArray[var4 * self->sizeZ + var5];
    if (!chunk)
        return 0;
    return Chunk_getBlockMetadata(chunk, x & 15, y, z & 15);
}

TileEntity *ChunkCache_getBlockTileEntity(ChunkCache *self, int x, int y, int z) {
    int var4 = (x >> 4) - self->chunkX;
    int var5 = (z >> 4) - self->chunkZ;
    if (var4 < 0 || var4 >= self->sizeX || var5 < 0 || var5 >= self->sizeZ)
        return NULL;
    Chunk *chunk = self->chunkArray[var4 * self->sizeZ + var5];
    if (!chunk)
        return NULL;
    return Chunk_getChunkBlockTileEntity(chunk, x & 15, y, z & 15);
}

static int ChunkCache_getLightValueExt(ChunkCache *self, int x, int y, int z, int var4);

static int ChunkCache_getLightValueExt(ChunkCache *self, int x, int y, int z, int var4) {
    if (x >= -32000000 && z >= -32000000 && x < 32000000 && z <= 32000000) {
        if (var4) {
            int var5 = ChunkCache_getBlockId(self, x, y, z);
            if (var5 == Block_stairSingle->blockID || var5 == Block_tilledField->blockID ||
                var5 == Block_stairCompactCobblestone->blockID ||
                var5 == Block_stairCompactPlanks->blockID) {
                int var6 = ChunkCache_getLightValueExt(self, x, y + 1, z, 0);
                int var7 = ChunkCache_getLightValueExt(self, x + 1, y, z, 0);
                int var8 = ChunkCache_getLightValueExt(self, x - 1, y, z, 0);
                int var9 = ChunkCache_getLightValueExt(self, x, y, z + 1, 0);
                int var10 = ChunkCache_getLightValueExt(self, x, y, z - 1, 0);
                if (var7 > var6)
                    var6 = var7;
                if (var8 > var6)
                    var6 = var8;
                if (var9 > var6)
                    var6 = var9;
                if (var10 > var6)
                    var6 = var10;
                return var6;
            }
        }
        if (y < 0) {
            return 0;
        } else if (y >= 128) {
            int var5 = 15 - self->worldObj->skylightSubtracted;
            if (var5 < 0)
                var5 = 0;
            return var5;
        } else {
            int var5 = (x >> 4) - self->chunkX;
            int var6 = (z >> 4) - self->chunkZ;
            if (var5 < 0 || var5 >= self->sizeX || var6 < 0 || var6 >= self->sizeZ)
                return 0;
            Chunk *chunk = self->chunkArray[var5 * self->sizeZ + var6];
            if (!chunk)
                return 0;
            return Chunk_getBlockLightValue(chunk, x & 15, y, z & 15,
                                            self->worldObj->skylightSubtracted);
        }
    } else {
        return 15;
    }
}

int ChunkCache_getLightValue(ChunkCache *self, int x, int y, int z) {
    return ChunkCache_getLightValueExt(self, x, y, z, 1);
}

float ChunkCache_getBrightness(ChunkCache *self, int x, int y, int z, int minLight) {
    int var5 = ChunkCache_getLightValue(self, x, y, z);
    if (var5 < minLight) {
        var5 = minLight;
    }
    return self->worldObj->worldProvider->lightBrightnessTable[var5];
}

float ChunkCache_getLightBrightness(ChunkCache *self, int x, int y, int z) {
    return self->worldObj->worldProvider
        ->lightBrightnessTable[ChunkCache_getLightValue(self, x, y, z)];
}

Material *ChunkCache_getBlockMaterial(ChunkCache *self, int x, int y, int z) {
    int var4 = ChunkCache_getBlockId(self, x, y, z);
    return var4 == 0 ? Material_air : Block_blocksList[var4]->blockMaterial;
}

int ChunkCache_isBlockOpaqueCube(ChunkCache *self, int x, int y, int z) {
    int id = ChunkCache_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return 0;
    return Block_blocksList[id]->vtable->isOpaqueCube(Block_blocksList[id]);
}

int ChunkCache_isBlockNormalCube(ChunkCache *self, int x, int y, int z) {
    int id = ChunkCache_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return 0;
    Block *b = Block_blocksList[id];
    return b->blockMaterial->isSolid && b->vtable->renderAsNormalBlock(b);
}

WorldChunkManager *ChunkCache_getWorldChunkManager(ChunkCache *self) {
    return World_getWorldChunkManager(self->worldObj);
}
