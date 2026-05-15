#ifndef CHUNK_CACHE_H
#define CHUNK_CACHE_H

#include "IBlockAccess.h"
#include "World.h"
#include "Chunk.h"
#include "Block.h"
#include "Material.h"
#include "WorldChunkManager.h"
#include <stdlib.h>

typedef struct ChunkCache {
    IBlockAccess base;
    int chunkX;
    int chunkZ;
    int sizeX;
    int sizeZ;
    Chunk **chunkArray;
    World *worldObj;
} ChunkCache;

ChunkCache *ChunkCache_create(World *world, int minX, int minY, int minZ, int maxX, int maxY,
                              int maxZ);
void ChunkCache_destroy(ChunkCache *self);

void ChunkCache_init(ChunkCache *self, World *world, int minX, int minY, int minZ, int maxX,
                     int maxY, int maxZ);
void ChunkCache_freeContents(ChunkCache *self);

int ChunkCache_getBlockId(ChunkCache *self, int x, int y, int z);
int ChunkCache_getBlockMetadata(ChunkCache *self, int x, int y, int z);
TileEntity *ChunkCache_getBlockTileEntity(ChunkCache *self, int x, int y, int z);
int ChunkCache_getLightValue(ChunkCache *self, int x, int y, int z);
float ChunkCache_getBrightness(ChunkCache *self, int x, int y, int z, int minLight);
float ChunkCache_getLightBrightness(ChunkCache *self, int x, int y, int z);

Material *ChunkCache_getBlockMaterial(ChunkCache *self, int x, int y, int z);
int ChunkCache_isBlockOpaqueCube(ChunkCache *self, int x, int y, int z);
int ChunkCache_isBlockNormalCube(ChunkCache *self, int x, int y, int z);

WorldChunkManager *ChunkCache_getWorldChunkManager(ChunkCache *self);

extern IBlockAccessVtable ChunkCache_vtable;

#endif
