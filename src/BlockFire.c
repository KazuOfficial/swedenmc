#include "BlockFire.h"
#include "Material.h"
#include "World.h"
#include "Block.h"
#include "IBlockAccess.h"
#include "JavaRandom.h"
#include <stdlib.h>
#include <string.h>

BlockVtable BlockFire_defaultVtable;

void BlockFire_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->initializeBlock = BlockFire_initializeBlock;
    vt->getCollisionBoundingBoxFromPool = BlockFire_getCollisionBoundingBoxFromPool;
    vt->isOpaqueCube = BlockFire_isOpaqueCube;
    vt->renderAsNormalBlock = BlockFire_renderAsNormalBlock;
    vt->getRenderType = BlockFire_getRenderType;
    vt->quantityDropped = BlockFire_quantityDropped;
    vt->tickRate = BlockFire_tickRate;
    vt->updateTick = BlockFire_updateTick;
    vt->isCollidable = BlockFire_isCollidable;
    vt->canPlaceBlockAt = BlockFire_canPlaceBlockAt;
    vt->onNeighborBlockChange = BlockFire_onNeighborBlockChange;
    vt->onBlockAdded = BlockFire_onBlockAdded;
    vt->randomDisplayTick = BlockFire_randomDisplayTick;
}

void BlockFire_construct(BlockFire *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFire_initVtable(&BlockFire_defaultVtable);
        vtable_init = 1;
    }
    memset(self->chanceToEncourageFire, 0, sizeof(self->chanceToEncourageFire));
    memset(self->abilityToCatchFire, 0, sizeof(self->abilityToCatchFire));
    self->base.vtable = &BlockFire_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_fire);
    Block_setTickOnLoad(&self->base, 1);
}

BlockFire *BlockFire_create(int id, int tex) {
    BlockFire *self = (BlockFire *)calloc(1, sizeof(BlockFire));
    BlockFire_construct(self, id, tex);
    return self;
}

static void setBurnRate(BlockFire *self, int var1, int var2, int var3) {
    self->chanceToEncourageFire[var1] = var2;
    self->abilityToCatchFire[var1] = var3;
}

void BlockFire_initializeBlock(Block *self) {
    BlockFire *b = (BlockFire *)self;
    setBurnRate(b, Block_planks->blockID, 5, 20);
    setBurnRate(b, Block_fence->blockID, 5, 20);
    setBurnRate(b, Block_stairCompactPlanks->blockID, 5, 20);
    setBurnRate(b, Block_wood->blockID, 5, 5);
    setBurnRate(b, Block_leaves->blockID, 30, 60);
    setBurnRate(b, Block_bookShelf->blockID, 30, 20);
    setBurnRate(b, Block_tnt->blockID, 15, 100);
    setBurnRate(b, Block_tallGrass->blockID, 60, 100);
    setBurnRate(b, Block_cloth->blockID, 30, 60);
}

AxisAlignedBB *BlockFire_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                         int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

int BlockFire_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockFire_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockFire_getRenderType(Block *self) {
    (void)self;
    return 3;
}

int BlockFire_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 0;
}

int BlockFire_tickRate(Block *self) {
    (void)self;
    return 40;
}

int BlockFire_isCollidable(Block *self) {
    (void)self;
    return 0;
}

int BlockFire_canBlockCatchFire(Block *self, IBlockAccess *var1, int var2, int var3, int var4) {
    BlockFire *b = (BlockFire *)self;
    return b->chanceToEncourageFire[var1->vtable->getBlockId(var1, var2, var3, var4)] > 0;
}

int BlockFire_getChanceToEncourageFire(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    BlockFire *b = (BlockFire *)self;
    int var6 = b->chanceToEncourageFire[World_getBlockId(var1, var2, var3, var4)];
    return var6 > var5 ? var6 : var5;
}

static int func_263_h(Block *self, World *var1, int var2, int var3, int var4) {
    return BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2 + 1, var3, var4)
               ? 1
               : (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2 - 1, var3, var4)
                      ? 1
                      : (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3 - 1,
                                                     var4)
                             ? 1
                             : (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2,
                                                            var3 + 1, var4)
                                    ? 1
                                    : (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2,
                                                                   var3, var4 - 1)
                                           ? 1
                                           : BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1,
                                                                         var2, var3, var4 + 1)))));
}

static int getChanceOfNeighborsEncouragingFire(Block *self, World *var1, int var2, int var3,
                                               int var4) {
    int var5 = 0;
    if (!World_isAirBlock(var1, var2, var3, var4)) {
        return 0;
    } else {
        int var6 = BlockFire_getChanceToEncourageFire(self, var1, var2 + 1, var3, var4, var5);
        var6 = BlockFire_getChanceToEncourageFire(self, var1, var2 - 1, var3, var4, var6);
        var6 = BlockFire_getChanceToEncourageFire(self, var1, var2, var3 - 1, var4, var6);
        var6 = BlockFire_getChanceToEncourageFire(self, var1, var2, var3 + 1, var4, var6);
        var6 = BlockFire_getChanceToEncourageFire(self, var1, var2, var3, var4 - 1, var6);
        var6 = BlockFire_getChanceToEncourageFire(self, var1, var2, var3, var4 + 1, var6);
        return var6;
    }
}

static void tryToCatchBlockOnFire(Block *self, World *var1, int var2, int var3, int var4, int var5,
                                  JavaRandom *var6, int var7) {
    BlockFire *b = (BlockFire *)self;
    int var8 = b->abilityToCatchFire[World_getBlockId(var1, var2, var3, var4)];
    if (JavaRandom_nextInt(var6, var5) < var8) {
        int var9 = World_getBlockId(var1, var2, var3, var4) == Block_tnt->blockID;
        if (JavaRandom_nextInt(var6, var7 + 10) < 5 &&
            !World_canBlockBeRainedOn(var1, var2, var3, var4)) {
            int var10 = var7 + JavaRandom_nextInt(var6, 5) / 4;
            if (var10 > 15) {
                var10 = 15;
            }
            World_setBlockAndMetadataWithNotify(var1, var2, var3, var4, self->blockID, var10);
        } else {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
        if (var9) {
            Block_tnt->vtable->onBlockDestroyedByPlayer(Block_tnt, var1, var2, var3, var4, 1);
        }
    }
}

int BlockFire_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4) ||
           func_263_h(self, var1, var2, var3, var4);
}

void BlockFire_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                     int var5) {
    (void)var5;
    if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) &&
        !func_263_h(self, var1, var2, var3, var4)) {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

void BlockFire_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    if (World_getBlockId(var1, var2, var3 - 1, var4) != Block_obsidian->blockID ||
        !BlockPortal_tryToCreatePortal(Block_portal, var1, var2, var3, var4)) {
        if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) &&
            !func_263_h(self, var1, var2, var3, var4)) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        } else {
            World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                      self->vtable->tickRate(self));
        }
    }
}

void BlockFire_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                          JavaRandom *var5) {
    int var6 = World_getBlockId(var1, var2, var3 - 1, var4) == Block_netherrack->blockID;
    if (!self->vtable->canPlaceBlockAt(self, var1, var2, var3, var4)) {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }

    if (var6 || !World_func_27161_C(var1) ||
        (!World_canBlockBeRainedOn(var1, var2, var3, var4) &&
         !World_canBlockBeRainedOn(var1, var2 - 1, var3, var4) &&
         !World_canBlockBeRainedOn(var1, var2 + 1, var3, var4) &&
         !World_canBlockBeRainedOn(var1, var2, var3, var4 - 1) &&
         !World_canBlockBeRainedOn(var1, var2, var3, var4 + 1))) {
        int var7 = World_getBlockMetadata(var1, var2, var3, var4);
        if (var7 < 15) {
            World_setBlockMetadata(var1, var2, var3, var4, var7 + JavaRandom_nextInt(var5, 3) / 2);
        }

        World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                  self->vtable->tickRate(self));
        if (!var6 && !func_263_h(self, var1, var2, var3, var4)) {
            if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) || var7 > 3) {
                World_setBlockWithNotify(var1, var2, var3, var4, 0);
            }
        } else if (!var6 &&
                   !BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3 - 1, var4) &&
                   var7 == 15 && JavaRandom_nextInt(var5, 4) == 0) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        } else {
            tryToCatchBlockOnFire(self, var1, var2 + 1, var3, var4, 300, var5, var7);
            tryToCatchBlockOnFire(self, var1, var2 - 1, var3, var4, 300, var5, var7);
            tryToCatchBlockOnFire(self, var1, var2, var3 - 1, var4, 250, var5, var7);
            tryToCatchBlockOnFire(self, var1, var2, var3 + 1, var4, 250, var5, var7);
            tryToCatchBlockOnFire(self, var1, var2, var3, var4 - 1, 300, var5, var7);
            tryToCatchBlockOnFire(self, var1, var2, var3, var4 + 1, 300, var5, var7);

            int var8, var9, var10;
            for (var8 = var2 - 1; var8 <= var2 + 1; ++var8) {
                for (var9 = var4 - 1; var9 <= var4 + 1; ++var9) {
                    for (var10 = var3 - 1; var10 <= var3 + 4; ++var10) {
                        if (var8 != var2 || var10 != var3 || var9 != var4) {
                            int var11 = 100;
                            if (var10 > var3 + 1) {
                                var11 += (var10 - (var3 + 1)) * 100;
                            }

                            int var12 =
                                getChanceOfNeighborsEncouragingFire(self, var1, var8, var10, var9);
                            if (var12 > 0) {
                                int var13 = (var12 + 40) / (var7 + 30);
                                if (var13 > 0 && JavaRandom_nextInt(var5, var11) <= var13 &&
                                    (!World_func_27161_C(var1) ||
                                     !World_canBlockBeRainedOn(var1, var8, var10, var9)) &&
                                    !World_canBlockBeRainedOn(var1, var8 - 1, var10, var4) &&
                                    !World_canBlockBeRainedOn(var1, var8 + 1, var10, var9) &&
                                    !World_canBlockBeRainedOn(var1, var8, var10, var9 - 1) &&
                                    !World_canBlockBeRainedOn(var1, var8, var10, var9 + 1)) {
                                    int var14 = var7 + JavaRandom_nextInt(var5, 5) / 4;
                                    if (var14 > 15) {
                                        var14 = 15;
                                    }
                                    World_setBlockAndMetadataWithNotify(var1, var8, var10, var9,
                                                                        self->blockID, var14);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

void BlockFire_randomDisplayTick(Block *self, World *var1, int var2, int var3, int var4,
                                 JavaRandom *var5) {
    if (JavaRandom_nextInt(var5, 24) == 0) {
        World_playSoundEffect(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F),
                              (double)((float)var4 + 0.5F), "fire.fire",
                              1.0F + JavaRandom_nextFloat(var5),
                              JavaRandom_nextFloat(var5) * 0.7F + 0.3F);
    }

    int var6;
    float var7, var8, var9;
    if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4) &&
        !BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3 - 1, var4)) {
        if (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2 - 1, var3, var4)) {
            for (var6 = 0; var6 < 2; ++var6) {
                var7 = (float)var2 + JavaRandom_nextFloat(var5) * 0.1F;
                var8 = (float)var3 + JavaRandom_nextFloat(var5);
                var9 = (float)var4 + JavaRandom_nextFloat(var5);
                World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9,
                                    0.0, 0.0, 0.0);
            }
        }

        if (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2 + 1, var3, var4)) {
            for (var6 = 0; var6 < 2; ++var6) {
                var7 = (float)(var2 + 1) - JavaRandom_nextFloat(var5) * 0.1F;
                var8 = (float)var3 + JavaRandom_nextFloat(var5);
                var9 = (float)var4 + JavaRandom_nextFloat(var5);
                World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9,
                                    0.0, 0.0, 0.0);
            }
        }

        if (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3, var4 - 1)) {
            for (var6 = 0; var6 < 2; ++var6) {
                var7 = (float)var2 + JavaRandom_nextFloat(var5);
                var8 = (float)var3 + JavaRandom_nextFloat(var5);
                var9 = (float)var4 + JavaRandom_nextFloat(var5) * 0.1F;
                World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9,
                                    0.0, 0.0, 0.0);
            }
        }

        if (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3, var4 + 1)) {
            for (var6 = 0; var6 < 2; ++var6) {
                var7 = (float)var2 + JavaRandom_nextFloat(var5);
                var8 = (float)var3 + JavaRandom_nextFloat(var5);
                var9 = (float)(var4 + 1) - JavaRandom_nextFloat(var5) * 0.1F;
                World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9,
                                    0.0, 0.0, 0.0);
            }
        }

        if (BlockFire_canBlockCatchFire(self, (IBlockAccess *)var1, var2, var3 + 1, var4)) {
            for (var6 = 0; var6 < 2; ++var6) {
                var7 = (float)var2 + JavaRandom_nextFloat(var5);
                var8 = (float)(var3 + 1) - JavaRandom_nextFloat(var5) * 0.1F;
                var9 = (float)var4 + JavaRandom_nextFloat(var5);
                World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9,
                                    0.0, 0.0, 0.0);
            }
        }
    } else {
        for (var6 = 0; var6 < 3; ++var6) {
            var7 = (float)var2 + JavaRandom_nextFloat(var5);
            var8 = (float)var3 + JavaRandom_nextFloat(var5) * 0.5F + 0.5F;
            var9 = (float)var4 + JavaRandom_nextFloat(var5);
            World_spawnParticle(var1, "largesmoke", (double)var7, (double)var8, (double)var9, 0.0,
                                0.0, 0.0);
        }
    }
}
