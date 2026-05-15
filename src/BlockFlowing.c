#include "BlockFluid.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int numAdjacentSources;
    int isOptimalFlowDirection[4];
    int flowCost[4];
} BlockFlowing;

static void BlockFlowing_func_30003_j(BlockFlowing *self, World *var1, int var2, int var3,
                                      int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    World_setBlockAndMetadata(var1, var2, var3, var4, self->base.blockID + 1, var5);
    World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
    World_markBlockNeedsUpdate(var1, var2, var3, var4);
}

static int BlockFlowing_blockBlocksFlow(BlockFlowing *self, World *var1, int var2, int var3,
                                        int var4) {
    (void)self;
    int var5 = World_getBlockId(var1, var2, var3, var4);
    if (var5 != Block_doorWood->blockID && var5 != Block_doorSteel->blockID &&
        var5 != Block_signPost->blockID && var5 != Block_ladder->blockID &&
        var5 != Block_reed->blockID) {
        if (var5 == 0) {
            return 0;
        } else {
            Material *var6 = Block_blocksList[var5]->blockMaterial;
            return Material_getIsSolid(var6);
        }
    } else {
        return 1;
    }
}

static int BlockFlowing_liquidCanDisplaceBlock(BlockFlowing *self, World *var1, int var2, int var3,
                                               int var4) {
    Material *var5 = World_getBlockMaterial(var1, var2, var3, var4);
    if (var5 == self->base.blockMaterial)
        return 0;
    if (var5 == Material_lava)
        return 0;
    return !BlockFlowing_blockBlocksFlow(self, var1, var2, var3, var4);
}

static int BlockFlowing_getSmallestFlowDecay(BlockFlowing *self, World *var1, int var2, int var3,
                                             int var4, int var5) {
    int var6 = BlockFluid_getFlowDecay(&self->base, var1, var2, var3, var4);
    if (var6 < 0) {
        return var5;
    } else {
        if (var6 == 0) {
            ++self->numAdjacentSources;
        }
        if (var6 >= 8) {
            var6 = 0;
        }
        return (var5 >= 0 && var6 >= var5) ? var5 : var6;
    }
}

static int BlockFlowing_calculateFlowCost(BlockFlowing *self, World *var1, int var2, int var3,
                                          int var4, int var5, int var6) {
    int var7 = 1000;
    for (int var8 = 0; var8 < 4; ++var8) {
        if ((var8 != 0 || var6 != 1) && (var8 != 1 || var6 != 0) && (var8 != 2 || var6 != 3) &&
            (var8 != 3 || var6 != 2)) {
            int var9 = var2;
            int var11 = var4;
            if (var8 == 0) {
                var9 = var2 - 1;
            }
            if (var8 == 1) {
                ++var9;
            }
            if (var8 == 2) {
                var11 = var4 - 1;
            }
            if (var8 == 3) {
                ++var11;
            }
            if (!BlockFlowing_blockBlocksFlow(self, var1, var9, var3, var11) &&
                (World_getBlockMaterial(var1, var9, var3, var11) != self->base.blockMaterial ||
                 World_getBlockMetadata(var1, var9, var3, var11) != 0)) {
                if (!BlockFlowing_blockBlocksFlow(self, var1, var9, var3 - 1, var11)) {
                    return var5;
                }
                if (var5 < 4) {
                    int var12 = BlockFlowing_calculateFlowCost(self, var1, var9, var3, var11,
                                                               var5 + 1, var8);
                    if (var12 < var7) {
                        var7 = var12;
                    }
                }
            }
        }
    }
    return var7;
}

static int *BlockFlowing_getOptimalFlowDirections(BlockFlowing *self, World *var1, int var2,
                                                  int var3, int var4) {
    int var5, var6;
    for (var5 = 0; var5 < 4; ++var5) {
        self->flowCost[var5] = 1000;
        var6 = var2;
        int var8 = var4;
        if (var5 == 0) {
            var6 = var2 - 1;
        }
        if (var5 == 1) {
            ++var6;
        }
        if (var5 == 2) {
            var8 = var4 - 1;
        }
        if (var5 == 3) {
            ++var8;
        }
        if (!BlockFlowing_blockBlocksFlow(self, var1, var6, var3, var8) &&
            (World_getBlockMaterial(var1, var6, var3, var8) != self->base.blockMaterial ||
             World_getBlockMetadata(var1, var6, var3, var8) != 0)) {
            if (!BlockFlowing_blockBlocksFlow(self, var1, var6, var3 - 1, var8)) {
                self->flowCost[var5] = 0;
            } else {
                self->flowCost[var5] =
                    BlockFlowing_calculateFlowCost(self, var1, var6, var3, var8, 1, var5);
            }
        }
    }
    var5 = self->flowCost[0];
    for (var6 = 1; var6 < 4; ++var6) {
        if (self->flowCost[var6] < var5) {
            var5 = self->flowCost[var6];
        }
    }
    for (var6 = 0; var6 < 4; ++var6) {
        self->isOptimalFlowDirection[var6] = (self->flowCost[var6] == var5);
    }
    return self->isOptimalFlowDirection;
}

static void BlockFlowing_flowIntoBlock(BlockFlowing *self, World *var1, int var2, int var3,
                                       int var4, int var5) {
    if (BlockFlowing_liquidCanDisplaceBlock(self, var1, var2, var3, var4)) {
        int var6 = World_getBlockId(var1, var2, var3, var4);
        if (var6 > 0) {
            if (self->base.blockMaterial == Material_lava) {
                BlockFluid_triggerLavaMixEffects(&self->base, var1, var2, var3, var4);
            } else {
                Block_dropBlockAsItem(Block_blocksList[var6], var1, var2, var3, var4,
                                      World_getBlockMetadata(var1, var2, var3, var4));
            }
        }
        World_setBlockAndMetadataWithNotify(var1, var2, var3, var4, self->base.blockID, var5);
    }
}

static void BlockFlowing_updateTick(Block *selfB, World *var1, int var2, int var3, int var4,
                                    JavaRandom *var5) {
    BlockFlowing *self = (BlockFlowing *)selfB;
    int var6 = BlockFluid_getFlowDecay(selfB, var1, var2, var3, var4);
    int var7 = 1;
    if (selfB->blockMaterial == Material_lava && !var1->worldProvider->isHellWorld) {
        var7 = 2;
    }

    int var8 = 1;
    int var10;
    if (var6 > 0) {
        int var9 = -100;
        self->numAdjacentSources = 0;
        int var12 = BlockFlowing_getSmallestFlowDecay(self, var1, var2 - 1, var3, var4, var9);
        var12 = BlockFlowing_getSmallestFlowDecay(self, var1, var2 + 1, var3, var4, var12);
        var12 = BlockFlowing_getSmallestFlowDecay(self, var1, var2, var3, var4 - 1, var12);
        var12 = BlockFlowing_getSmallestFlowDecay(self, var1, var2, var3, var4 + 1, var12);
        var10 = var12 + var7;
        if (var10 >= 8 || var12 < 0) {
            var10 = -1;
        }

        if (BlockFluid_getFlowDecay(selfB, var1, var2, var3 + 1, var4) >= 0) {
            int var11 = BlockFluid_getFlowDecay(selfB, var1, var2, var3 + 1, var4);
            if (var11 >= 8) {
                var10 = var11;
            } else {
                var10 = var11 + 8;
            }
        }

        if (self->numAdjacentSources >= 2 && selfB->blockMaterial == Material_water) {
            if (Material_getIsSolid(World_getBlockMaterial(var1, var2, var3 - 1, var4))) {
                var10 = 0;
            } else if (World_getBlockMaterial(var1, var2, var3 - 1, var4) == selfB->blockMaterial &&
                       World_getBlockMetadata(var1, var2, var3, var4) == 0) {
                var10 = 0;
            }
        }

        if (selfB->blockMaterial == Material_lava && var6 < 8 && var10 < 8 && var10 > var6 &&
            JavaRandom_nextInt(var5, 4) != 0) {
            var10 = var6;
            var8 = 0;
        }

        if (var10 != var6) {
            var6 = var10;
            if (var10 < 0) {
                World_setBlockWithNotify(var1, var2, var3, var4, 0);
            } else {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var10);
                World_scheduleBlockUpdate(var1, var2, var3, var4, selfB->blockID,
                                          selfB->vtable->tickRate(selfB));
                World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, selfB->blockID);
            }
        } else if (var8) {
            BlockFlowing_func_30003_j(self, var1, var2, var3, var4);
        }
    } else {
        BlockFlowing_func_30003_j(self, var1, var2, var3, var4);
    }

    if (BlockFlowing_liquidCanDisplaceBlock(self, var1, var2, var3 - 1, var4)) {
        if (var6 >= 8) {
            World_setBlockAndMetadataWithNotify(var1, var2, var3 - 1, var4, selfB->blockID, var6);
        } else {
            World_setBlockAndMetadataWithNotify(var1, var2, var3 - 1, var4, selfB->blockID,
                                                var6 + 8);
        }
    } else if (var6 >= 0 &&
               (var6 == 0 || BlockFlowing_blockBlocksFlow(self, var1, var2, var3 - 1, var4))) {
        int *var13 = BlockFlowing_getOptimalFlowDirections(self, var1, var2, var3, var4);
        var10 = var6 + var7;
        if (var6 >= 8) {
            var10 = 1;
        }
        if (var10 >= 8) {
            return;
        }
        if (var13[0]) {
            BlockFlowing_flowIntoBlock(self, var1, var2 - 1, var3, var4, var10);
        }
        if (var13[1]) {
            BlockFlowing_flowIntoBlock(self, var1, var2 + 1, var3, var4, var10);
        }
        if (var13[2]) {
            BlockFlowing_flowIntoBlock(self, var1, var2, var3, var4 - 1, var10);
        }
        if (var13[3]) {
            BlockFlowing_flowIntoBlock(self, var1, var2, var3, var4 + 1, var10);
        }
    }
}

static void BlockFlowing_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    BlockFluid_onBlockAdded(self, var1, var2, var3, var4);
    if (World_getBlockId(var1, var2, var3, var4) == self->blockID) {
        World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                  self->vtable->tickRate(self));
    }
}

Block *BlockFlowing_create(int id, Material *mat) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFluid_initVtable(&vtable);
        vtable.onBlockAdded = BlockFlowing_onBlockAdded;
        vtable.updateTick = BlockFlowing_updateTick;
        vtable_init = 1;
    }
    BlockFlowing *self = (BlockFlowing *)calloc(1, sizeof(BlockFlowing));
    self->base.vtable = &vtable;
    int tex = (mat == Material_lava ? 14 : 12) * 16 + 13;
    Block_constructWithTexture(&self->base, id, tex, mat);
    Block_setTickOnLoad(&self->base, 1);
    return &self->base;
}
