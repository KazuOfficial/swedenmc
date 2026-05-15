#include "Block.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockLadder_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockLadder_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockLadder_getRenderType(Block *self) {
    (void)self;
    return 8;
}

static AxisAlignedBB *BlockLadder_getCollisionBoundingBoxFromPool(Block *self, World *var1,
                                                                  int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    float var6 = 2.0f / 16.0f;
    if (var5 == 2) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 1.0f - var6, 1.0f, 1.0f, 1.0f);
    }
    if (var5 == 3) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, var6);
    }
    if (var5 == 4) {
        Block_setBlockBounds(self, 1.0f - var6, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    }
    if (var5 == 5) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, var6, 1.0f, 1.0f);
    }
    return Block_defaultVtable.getCollisionBoundingBoxFromPool(self, var1, var2, var3, var4);
}

static AxisAlignedBB *BlockLadder_getSelectedBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                                 int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    float var6 = 2.0f / 16.0f;
    if (var5 == 2) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 1.0f - var6, 1.0f, 1.0f, 1.0f);
    }
    if (var5 == 3) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, var6);
    }
    if (var5 == 4) {
        Block_setBlockBounds(self, 1.0f - var6, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    }
    if (var5 == 5) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, var6, 1.0f, 1.0f);
    }
    return Block_defaultVtable.getSelectedBoundingBoxFromPool(self, var1, var2, var3, var4);
}

static int BlockLadder_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2 - 1, var3, var4)
               ? 1
               : (World_isBlockNormalCube(var1, var2 + 1, var3, var4)
                      ? 1
                      : (World_isBlockNormalCube(var1, var2, var3, var4 - 1)
                             ? 1
                             : World_isBlockNormalCube(var1, var2, var3, var4 + 1)));
}

static void BlockLadder_onBlockPlaced(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var6 == 0 || var5 == 2) && World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
        var6 = 2;
    }
    if ((var6 == 0 || var5 == 3) && World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
        var6 = 3;
    }
    if ((var6 == 0 || var5 == 4) && World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
        var6 = 4;
    }
    if ((var6 == 0 || var5 == 5) && World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
        var6 = 5;
    }
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
}

static void BlockLadder_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = 0;
    if (var6 == 2 && World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
        var7 = 1;
    }
    if (var6 == 3 && World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
        var7 = 1;
    }
    if (var6 == 4 && World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
        var7 = 1;
    }
    if (var6 == 5 && World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
        var7 = 1;
    }
    if (!var7) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4, var6);
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
    Block_defaultVtable.onNeighborBlockChange(self, var1, var2, var3, var4, var5);
}

static int BlockLadder_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 1;
}

Block *BlockLadder_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockLadder_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockLadder_renderAsNormalBlock;
        vtable.getRenderType = BlockLadder_getRenderType;
        vtable.getCollisionBoundingBoxFromPool = BlockLadder_getCollisionBoundingBoxFromPool;
        vtable.getSelectedBoundingBoxFromPool = BlockLadder_getSelectedBoundingBoxFromPool;
        vtable.canPlaceBlockAt = BlockLadder_canPlaceBlockAt;
        vtable.onBlockPlaced = BlockLadder_onBlockPlaced;
        vtable.onNeighborBlockChange = BlockLadder_onNeighborBlockChange;
        vtable.quantityDropped = BlockLadder_quantityDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_circuits);
    return self;
}
