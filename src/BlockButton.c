#include "BlockButton.h"
#include "World.h"
#include <stdlib.h>

BlockVtable BlockButton_defaultVtable;

void BlockButton_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->getCollisionBoundingBoxFromPool = BlockButton_getCollisionBoundingBoxFromPool;
    vt->tickRate = BlockButton_tickRate;
    vt->isOpaqueCube = BlockButton_isOpaqueCube;
    vt->renderAsNormalBlock = BlockButton_renderAsNormalBlock;
    vt->canPlaceBlockOnSide = BlockButton_canPlaceBlockOnSide;
    vt->canPlaceBlockAt = BlockButton_canPlaceBlockAt;
    vt->onBlockPlaced = BlockButton_onBlockPlaced;
    vt->onNeighborBlockChange = BlockButton_onNeighborBlockChange;
    vt->setBlockBoundsBasedOnState = BlockButton_setBlockBoundsBasedOnState;
    vt->onBlockClicked = BlockButton_onBlockClicked;
    vt->blockActivated = BlockButton_blockActivated;
    vt->onBlockRemoval = BlockButton_onBlockRemoval;
    vt->isPoweringTo = BlockButton_isPoweringTo;
    vt->isIndirectlyPoweringTo = BlockButton_isIndirectlyPoweringTo;
    vt->canProvidePower = BlockButton_canProvidePower;
    vt->updateTick = BlockButton_updateTick;
    vt->setBlockBoundsForItemRender = BlockButton_setBlockBoundsForItemRender;
}

void BlockButton_construct(BlockButton *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockButton_initVtable(&BlockButton_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockButton_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_circuits);
    Block_setTickOnLoad(&self->base, 1);
}

BlockButton *BlockButton_create(int id, int tex) {
    BlockButton *self = (BlockButton *)calloc(1, sizeof(BlockButton));
    BlockButton_construct(self, id, tex);
    return self;
}

AxisAlignedBB *BlockButton_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                           int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

int BlockButton_tickRate(Block *self) {
    (void)self;
    return 20;
}

int BlockButton_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockButton_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockButton_canPlaceBlockOnSide(Block *self, World *var1, int var2, int var3, int var4,
                                    int var5) {
    (void)self;
    return var5 == 2 && World_isBlockNormalCube(var1, var2, var3, var4 + 1)   ? 1
           : var5 == 3 && World_isBlockNormalCube(var1, var2, var3, var4 - 1) ? 1
           : var5 == 4 && World_isBlockNormalCube(var1, var2 + 1, var3, var4)
               ? 1
               : var5 == 5 && World_isBlockNormalCube(var1, var2 - 1, var3, var4);
}

int BlockButton_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2 - 1, var3, var4)   ? 1
           : World_isBlockNormalCube(var1, var2 + 1, var3, var4) ? 1
           : World_isBlockNormalCube(var1, var2, var3, var4 - 1)
               ? 1
               : World_isBlockNormalCube(var1, var2, var3, var4 + 1);
}

static int getOrientation(World *var1, int var2, int var3, int var4) {
    return World_isBlockNormalCube(var1, var2 - 1, var3, var4)   ? 1
           : World_isBlockNormalCube(var1, var2 + 1, var3, var4) ? 2
           : World_isBlockNormalCube(var1, var2, var3, var4 - 1) ? 3
           : World_isBlockNormalCube(var1, var2, var3, var4 + 1) ? 4
                                                                 : 1;
}

void BlockButton_onBlockPlaced(Block *self, World *var1, int var2, int var3, int var4, int var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = var6 & 8;
    var6 &= 7;
    if (var5 == 2 && World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
        var6 = 4;
    } else if (var5 == 3 && World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
        var6 = 3;
    } else if (var5 == 4 && World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
        var6 = 2;
    } else if (var5 == 5 && World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
        var6 = 1;
    } else {
        var6 = getOrientation(var1, var2, var3, var4);
    }
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 + var7);
}

static int func_305_h(Block *self, World *var1, int var2, int var3, int var4) {
    if (!self->vtable->canPlaceBlockAt(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
        return 0;
    }
    return 1;
}

void BlockButton_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    (void)var5;
    if (func_305_h(self, var1, var2, var3, var4)) {
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
        if (var7) {
            Block_dropBlockAsItem(self, var1, var2, var3, var4,
                                  World_getBlockMetadata(var1, var2, var3, var4));
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
    }
}

void BlockButton_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                            int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    int var6 = var5 & 7;
    int var7 = (var5 & 8) > 0;
    float var8 = 6.0F / 16.0F;
    float var9 = 10.0F / 16.0F;
    float var10 = 3.0F / 16.0F;
    float var11 = 2.0F / 16.0F;
    if (var7) {
        var11 = 1.0F / 16.0F;
    }
    if (var6 == 1) {
        Block_setBlockBounds(self, 0.0F, var8, 0.5F - var10, var11, var9, 0.5F + var10);
    } else if (var6 == 2) {
        Block_setBlockBounds(self, 1.0F - var11, var8, 0.5F - var10, 1.0F, var9, 0.5F + var10);
    } else if (var6 == 3) {
        Block_setBlockBounds(self, 0.5F - var10, var8, 0.0F, 0.5F + var10, var9, var11);
    } else if (var6 == 4) {
        Block_setBlockBounds(self, 0.5F - var10, var8, 1.0F - var11, 0.5F + var10, var9, 1.0F);
    }
}

void BlockButton_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                                EntityPlayer *var5) {
    self->vtable->blockActivated(self, var1, var2, var3, var4, var5);
}

int BlockButton_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                               EntityPlayer *var5) {
    (void)var5;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = var6 & 7;
    int var8 = 8 - (var6 & 8);
    if (var8 == 0) {
        return 1;
    } else {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 + var8);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                              "random.click", 0.3F, 0.6F);
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
        World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                  self->vtable->tickRate(self));
        return 1;
    }
}

void BlockButton_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
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

int BlockButton_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                             int var5) {
    (void)self;
    (void)var5;
    return (var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 8) > 0;
}

int BlockButton_isIndirectlyPoweringTo(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var6 & 8) == 0) {
        return 0;
    }
    int var7 = var6 & 7;
    return var7 == 5 && var5 == 1   ? 1
           : var7 == 4 && var5 == 2 ? 1
           : var7 == 3 && var5 == 3 ? 1
           : var7 == 2 && var5 == 4 ? 1
                                    : var7 == 1 && var5 == 5;
}

int BlockButton_canProvidePower(Block *self) {
    (void)self;
    return 1;
}

void BlockButton_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                            JavaRandom *var5) {
    (void)var5;
    if (!var1->isRemote) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if ((var6 & 8) != 0) {
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 & 7);
            World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
            int var7 = var6 & 7;
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
            World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                                  "random.click", 0.3F, 0.5F);
            World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        }
    }
}

void BlockButton_setBlockBoundsForItemRender(Block *self) {
    float var1 = 3.0F / 16.0F;
    float var2 = 2.0F / 16.0F;
    float var3 = 2.0F / 16.0F;
    Block_setBlockBounds(self, 0.5F - var1, 0.5F - var2, 0.5F - var3, 0.5F + var1, 0.5F + var2,
                         0.5F + var3);
}
