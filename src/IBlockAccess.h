#ifndef IBLOCK_ACCESS_H
#define IBLOCK_ACCESS_H

#include "GameForward.h"
#include "Material.h"

struct WorldChunkManager;

typedef struct IBlockAccess IBlockAccess;

typedef struct IBlockAccessVtable {

    int (*getBlockId)(IBlockAccess *self, int x, int y, int z);

    TileEntity *(*getBlockTileEntity)(IBlockAccess *self, int x, int y, int z);

    float (*getBrightness)(IBlockAccess *self, int x, int y, int z, int var4);

    float (*getLightBrightness)(IBlockAccess *self, int x, int y, int z);

    int (*getBlockMetadata)(IBlockAccess *self, int x, int y, int z);

    Material *(*getBlockMaterial)(IBlockAccess *self, int x, int y, int z);

    int (*isBlockOpaqueCube)(IBlockAccess *self, int x, int y, int z);

    int (*isBlockNormalCube)(IBlockAccess *self, int x, int y, int z);

    struct WorldChunkManager *(*getWorldChunkManager)(IBlockAccess *self);
} IBlockAccessVtable;

struct IBlockAccess {
    const IBlockAccessVtable *vtable;
};

#endif
