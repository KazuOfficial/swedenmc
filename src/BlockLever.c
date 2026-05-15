#include "Block.h"
#include "Material.h"
#include "IBlockAccess.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static AxisAlignedBB *lev_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                          int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

static int lev_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int lev_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int lev_getRenderType(Block *self) {
    (void)self;
    return 12;
}
static int lev_canProvidePower(Block *self) {
    (void)self;
    return 1;
}

static int lev_canPlaceBlockOnSide(Block *self, World *var1, int var2, int var3, int var4,
                                   int var5) {
    (void)self;
    return var5 == 1 && World_isBlockNormalCube(var1, var2, var3 - 1, var4)
               ? 1
               : (var5 == 2 && World_isBlockNormalCube(var1, var2, var3, var4 + 1)
                      ? 1
                      : (var5 == 3 && World_isBlockNormalCube(var1, var2, var3, var4 - 1)
                             ? 1
                             : (var5 == 4 && World_isBlockNormalCube(var1, var2 + 1, var3, var4)
                                    ? 1
                                    : (var5 == 5 &&
                                       World_isBlockNormalCube(var1, var2 - 1, var3, var4)))));
}

static int lev_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2 - 1, var3, var4)
               ? 1
               : (World_isBlockNormalCube(var1, var2 + 1, var3, var4)
                      ? 1
                      : (World_isBlockNormalCube(var1, var2, var3, var4 - 1)
                             ? 1
                             : (World_isBlockNormalCube(var1, var2, var3, var4 + 1)
                                    ? 1
                                    : World_isBlockNormalCube(var1, var2, var3 - 1, var4))));
}

static void lev_onBlockPlaced(Block *self, World *var1, int var2, int var3, int var4, int var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = var6 & 8;
    var6 &= 7;
    var6 = -1;
    if (var5 == 1 && World_isBlockNormalCube(var1, var2, var3 - 1, var4)) {
        var6 = 5 + JavaRandom_nextInt(&var1->rand, 2);
    }
    if (var5 == 2 && World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
        var6 = 4;
    }
    if (var5 == 3 && World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
        var6 = 3;
    }
    if (var5 == 4 && World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
        var6 = 2;
    }
    if (var5 == 5 && World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
        var6 = 1;
    }
    if (var6 == -1) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    } else {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 + var7);
    }
}

static int lev_checkIfAttachedToBlock(Block *self, World *var1, int var2, int var3, int var4) {
    if (!lev_canPlaceBlockAt(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
        return 0;
    } else {
        return 1;
    }
}

static void lev_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5) {
    (void)var5;
    if (lev_checkIfAttachedToBlock(self, var1, var2, var3, var4)) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4) & 7;
        int var7 = 0;
        if (!World_isBlockNormalCube(var1, var2 - 1, var3, var4) && var6 == 1) {
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2 + 1, var3, var4) && var6 == 2) {
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2, var3, var4 - 1) && var6 == 3) {
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2, var3, var4 + 1) && var6 == 4) {
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) && var6 == 5) {
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) && var6 == 6) {
            var7 = 1;
        }
        if (var7) {
            Block_dropBlockAsItem(self, var1, var2, var3, var4,
                                  World_getBlockMetadata(var1, var2, var3, var4));
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
    }
}

static void lev_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                           int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 7;
    float var6 = 3.0f / 16.0f;
    if (var5 == 1) {
        Block_setBlockBounds(self, 0.0f, 0.2f, 0.5f - var6, var6 * 2.0f, 0.8f, 0.5f + var6);
    } else if (var5 == 2) {
        Block_setBlockBounds(self, 1.0f - var6 * 2.0f, 0.2f, 0.5f - var6, 1.0f, 0.8f, 0.5f + var6);
    } else if (var5 == 3) {
        Block_setBlockBounds(self, 0.5f - var6, 0.2f, 0.0f, 0.5f + var6, 0.8f, var6 * 2.0f);
    } else if (var5 == 4) {
        Block_setBlockBounds(self, 0.5f - var6, 0.2f, 1.0f - var6 * 2.0f, 0.5f + var6, 0.8f, 1.0f);
    } else {
        var6 = 0.25f;
        Block_setBlockBounds(self, 0.5f - var6, 0.0f, 0.5f - var6, 0.5f + var6, 0.6f, 0.5f + var6);
    }
}

static void lev_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                               EntityPlayer *var5) {
    self->vtable->blockActivated(self, var1, var2, var3, var4, var5);
}

static int lev_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                              EntityPlayer *var5) {
    (void)var5;
    if (var1->multiplayerWorld) {
        return 1;
    } else {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        int var7 = var6 & 7;
        int var8 = 8 - (var6 & 8);
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 + var8);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                              "random.click", 0.3f, var8 > 0 ? 0.6f : 0.5f);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
        if (var7 == 1) {
            World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->blockID);
        } else if (var7 == 2) {
            World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->blockID);
        } else if (var7 == 3) {
            World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->blockID);
        } else if (var7 == 4) {
            World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->blockID);
        } else {
            World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        }
        return 1;
    }
}

static void lev_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var5 & 8) > 0) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
        int var6 = var5 & 7;
        if (var6 == 1) {
            World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->blockID);
        } else if (var6 == 2) {
            World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->blockID);
        } else if (var6 == 3) {
            World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->blockID);
        } else if (var6 == 4) {
            World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->blockID);
        } else {
            World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        }
    }
    Block_defaultVtable.onBlockRemoval(self, var1, var2, var3, var4);
}

static int lev_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                            int var5) {
    (void)self;
    (void)var5;
    return (var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 8) > 0;
}

static int lev_isIndirectlyPoweringTo(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var6 & 8) == 0) {
        return 0;
    } else {
        int var7 = var6 & 7;
        return var7 == 6 && var5 == 1
                   ? 1
                   : (var7 == 5 && var5 == 1
                          ? 1
                          : (var7 == 4 && var5 == 2
                                 ? 1
                                 : (var7 == 3 && var5 == 3
                                        ? 1
                                        : (var7 == 2 && var5 == 4 ? 1
                                                                  : (var7 == 1 && var5 == 5)))));
    }
}

Block *BlockLever_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getCollisionBoundingBoxFromPool = lev_getCollisionBoundingBoxFromPool;
        vtable.isOpaqueCube = lev_isOpaqueCube;
        vtable.renderAsNormalBlock = lev_renderAsNormalBlock;
        vtable.getRenderType = lev_getRenderType;
        vtable.canProvidePower = lev_canProvidePower;
        vtable.canPlaceBlockOnSide = lev_canPlaceBlockOnSide;
        vtable.canPlaceBlockAt = lev_canPlaceBlockAt;
        vtable.onBlockPlaced = lev_onBlockPlaced;
        vtable.onNeighborBlockChange = lev_onNeighborBlockChange;
        vtable.setBlockBoundsBasedOnState = lev_setBlockBoundsBasedOnState;
        vtable.onBlockClicked = lev_onBlockClicked;
        vtable.blockActivated = lev_blockActivated;
        vtable.onBlockRemoval = lev_onBlockRemoval;
        vtable.isPoweringTo = lev_isPoweringTo;
        vtable.isIndirectlyPoweringTo = lev_isIndirectlyPoweringTo;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_circuits);
    return self;
}
