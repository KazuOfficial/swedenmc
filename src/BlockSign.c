#include "Block.h"
#include "BlockContainer.h"
#include "Material.h"
#include "World.h"
#include "IBlockAccess.h"
#include "TileEntitySign.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int isFreestanding;
} BlockSign;

static AxisAlignedBB *BlockSign_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                                int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

static void BlockSign_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                 int var3, int var4) {
    BlockSign *bs = (BlockSign *)self;
    if (!bs->isFreestanding) {
        int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
        float var6 = 9.0f / 32.0f;
        float var7 = 25.0f / 32.0f;
        float var8 = 0.0f;
        float var9 = 1.0f;
        float var10 = 2.0f / 16.0f;
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        if (var5 == 2) {
            Block_setBlockBounds(self, var8, var6, 1.0f - var10, var9, var7, 1.0f);
        }
        if (var5 == 3) {
            Block_setBlockBounds(self, var8, var6, 0.0f, var9, var7, var10);
        }
        if (var5 == 4) {
            Block_setBlockBounds(self, 1.0f - var10, var6, var8, 1.0f, var7, var9);
        }
        if (var5 == 5) {
            Block_setBlockBounds(self, 0.0f, var6, var8, var10, var7, var9);
        }
    }
}

static AxisAlignedBB *BlockSign_getSelectedBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                               int var3, int var4) {
    BlockSign_setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.getSelectedBoundingBoxFromPool(self, var1, var2, var3, var4);
}

static int BlockSign_getRenderType(Block *self) {
    (void)self;
    return -1;
}

static int BlockSign_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

static int BlockSign_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static void BlockSign_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    World_setBlockTileEntity(var1, var2, var3, var4, (TileEntity *)TileEntitySign_create());
}

static int BlockSign_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 323;
}

static void BlockSign_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                            int var5) {
    BlockSign *bs = (BlockSign *)self;
    int var6 = 0;
    if (bs->isFreestanding) {
        if (!Material_isSolid(World_getBlockMaterial(var1, var2, var3 - 1, var4))) {
            var6 = 1;
        }
    } else {
        int var7 = World_getBlockMetadata(var1, var2, var3, var4);
        var6 = 1;
        if (var7 == 2 && Material_isSolid(World_getBlockMaterial(var1, var2, var3, var4 + 1))) {
            var6 = 0;
        }
        if (var7 == 3 && Material_isSolid(World_getBlockMaterial(var1, var2, var3, var4 - 1))) {
            var6 = 0;
        }
        if (var7 == 4 && Material_isSolid(World_getBlockMaterial(var1, var2 + 1, var3, var4))) {
            var6 = 0;
        }
        if (var7 == 5 && Material_isSolid(World_getBlockMaterial(var1, var2 - 1, var3, var4))) {
            var6 = 0;
        }
    }
    if (var6) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
    Block_defaultVtable.onNeighborBlockChange(self, var1, var2, var3, var4, var5);
}

Block *BlockSign_create(int id, int isFreestanding) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable);
        vtable.getCollisionBoundingBoxFromPool = BlockSign_getCollisionBoundingBoxFromPool;
        vtable.getSelectedBoundingBoxFromPool = BlockSign_getSelectedBoundingBoxFromPool;
        vtable.setBlockBoundsBasedOnState = BlockSign_setBlockBoundsBasedOnState;
        vtable.getRenderType = BlockSign_getRenderType;
        vtable.renderAsNormalBlock = BlockSign_renderAsNormalBlock;
        vtable.isOpaqueCube = BlockSign_isOpaqueCube;
        vtable.onBlockAdded = BlockSign_onBlockAdded;
        vtable.idDropped = BlockSign_idDropped;
        vtable.onNeighborBlockChange = BlockSign_onNeighborBlockChange;
        vtable_init = 1;
    }
    BlockSign *self = (BlockSign *)calloc(1, sizeof(BlockSign));
    self->isFreestanding = isFreestanding;
    self->base.vtable = &vtable;

    BlockContainer_constructWithTexture(&self->base, id, 4, Material_wood);

    float var4 = 0.25f, var5 = 1.0f;
    Block_setBlockBounds(&self->base, 0.5f - var4, 0.0f, 0.5f - var4, 0.5f + var4, var5,
                         0.5f + var4);
    return &self->base;
}
