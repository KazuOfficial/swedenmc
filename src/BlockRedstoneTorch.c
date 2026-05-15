#include "Block.h"
#include "BlockTorch.h"
#include "Material.h"
#include "JavaRandom.h"
#include "RedstoneUpdateInfo.h"
#include "World.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    Block base;
    int torchActive;
} BlockRedstoneTorch;

static RedstoneUpdateInfo **s_torchUpdates = NULL;
static int s_torchUpdates_cnt = 0;
static int s_torchUpdates_cap = 0;

static void torchUpdates_add(RedstoneUpdateInfo *info) {
    if (s_torchUpdates_cnt >= s_torchUpdates_cap) {
        s_torchUpdates_cap = s_torchUpdates_cap ? s_torchUpdates_cap * 2 : 8;
        s_torchUpdates = (RedstoneUpdateInfo **)realloc(
            s_torchUpdates, s_torchUpdates_cap * sizeof(RedstoneUpdateInfo *));
    }
    s_torchUpdates[s_torchUpdates_cnt++] = info;
}

static void torchUpdates_removeAt(int idx) {
    free(s_torchUpdates[idx]);
    for (int i = idx; i < s_torchUpdates_cnt - 1; ++i)
        s_torchUpdates[i] = s_torchUpdates[i + 1];
    --s_torchUpdates_cnt;
}

static int checkForBurnout(World *var1, int var2, int var3, int var4, int var5) {
    if (var5) {
        torchUpdates_add(RedstoneUpdateInfo_create(var2, var3, var4, World_getWorldTime(var1)));
    }

    int var6 = 0;
    for (int var7 = 0; var7 < s_torchUpdates_cnt; ++var7) {
        RedstoneUpdateInfo *var8 = s_torchUpdates[var7];
        if (var8->x == var2 && var8->y == var3 && var8->z == var4) {
            ++var6;
            if (var6 >= 8) {
                return 1;
            }
        }
    }

    return 0;
}

static int BlockRedstoneTorch_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    if (var1 == 1) {
        return Block_redstoneWire->vtable->getBlockTextureFromSideAndMetadata(Block_redstoneWire,
                                                                              var1, var2);
    }

    (void)var2;
    return self->blockIndexInTexture;
}

static int BlockRedstoneTorch_tickRate(Block *self) {
    (void)self;
    return 2;
}

static void BlockRedstoneTorch_onBlockAdded(Block *self, World *var1, int var2, int var3,
                                            int var4) {
    BlockRedstoneTorch *b = (BlockRedstoneTorch *)self;
    if (World_getBlockMetadata(var1, var2, var3, var4) == 0) {
        BlockTorch_onBlockAdded(self, var1, var2, var3, var4);
    }
    if (b->torchActive) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->blockID);
    }
}

static void BlockRedstoneTorch_onBlockRemoval(Block *self, World *var1, int var2, int var3,
                                              int var4) {
    BlockRedstoneTorch *b = (BlockRedstoneTorch *)self;
    if (b->torchActive) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->blockID);
    }
}

static int BlockRedstoneTorch_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3,
                                           int var4, int var5) {
    BlockRedstoneTorch *b = (BlockRedstoneTorch *)self;
    if (!b->torchActive)
        return 0;
    int var6 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    return var6 == 5 && var5 == 1
               ? 0
               : (var6 == 3 && var5 == 3
                      ? 0
                      : (var6 == 4 && var5 == 2
                             ? 0
                             : (var6 == 1 && var5 == 5 ? 0 : !(var6 == 2 && var5 == 4))));
}

static int func_30002_h(World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    return (var5 == 5 && World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 - 1, var4, 0))
               ? 1
               : ((var5 == 3 &&
                   World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 - 1, 2))
                      ? 1
                      : ((var5 == 4 &&
                          World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 + 1, 3))
                             ? 1
                             : ((var5 == 1 && World_isBlockIndirectlyProvidingPowerTo(
                                                  var1, var2 - 1, var3, var4, 4))
                                    ? 1
                                    : (var5 == 2 && World_isBlockIndirectlyProvidingPowerTo(
                                                        var1, var2 + 1, var3, var4, 5)))));
}

static void BlockRedstoneTorch_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                          JavaRandom *var5) {
    BlockRedstoneTorch *b = (BlockRedstoneTorch *)self;
    int var6 = func_30002_h(var1, var2, var3, var4);

    while (s_torchUpdates_cnt > 0 &&
           World_getWorldTime(var1) - s_torchUpdates[0]->updateTime > 100L) {
        torchUpdates_removeAt(0);
    }

    if (b->torchActive) {
        if (var6) {
            World_setBlockAndMetadataWithNotify(var1, var2, var3, var4,
                                                Block_torchRedstoneIdle->blockID,
                                                World_getBlockMetadata(var1, var2, var3, var4));
            if (checkForBurnout(var1, var2, var3, var4, 1)) {
                World_playSoundEffect(
                    var1, (double)((float)var2 + 0.5f), (double)((float)var3 + 0.5f),
                    (double)((float)var4 + 0.5f), "random.fizz", 0.5f,
                    2.6f + (JavaRandom_nextFloat(var5) - JavaRandom_nextFloat(var5)) * 0.8f);
                for (int var7 = 0; var7 < 5; ++var7) {
                    double var8 = (double)var2 + JavaRandom_nextDouble(var5) * 0.6 + 0.2;
                    double var10 = (double)var3 + JavaRandom_nextDouble(var5) * 0.6 + 0.2;
                    double var12 = (double)var4 + JavaRandom_nextDouble(var5) * 0.6 + 0.2;
                    World_spawnParticle(var1, "smoke", var8, var10, var12, 0.0, 0.0, 0.0);
                }
            }
        }
    } else if (!var6 && !checkForBurnout(var1, var2, var3, var4, 0)) {
        World_setBlockAndMetadataWithNotify(var1, var2, var3, var4,
                                            Block_torchRedstoneActive->blockID,
                                            World_getBlockMetadata(var1, var2, var3, var4));
    }
}

static void BlockRedstoneTorch_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                                     int var4, int var5) {
    BlockTorch_onNeighborBlockChange(self, var1, var2, var3, var4, var5);
    World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID, self->vtable->tickRate(self));
}

static int BlockRedstoneTorch_isIndirectlyPoweringTo(Block *self, World *var1, int var2, int var3,
                                                     int var4, int var5) {
    return var5 == 0
               ? self->vtable->isPoweringTo(self, (IBlockAccess *)var1, var2, var3, var4, var5)
               : 0;
}

static int BlockRedstoneTorch_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return Block_torchRedstoneActive->blockID;
}

static int BlockRedstoneTorch_canProvidePower(Block *self) {
    (void)self;
    return 1;
}

static void BlockRedstoneTorch_randomDisplayTick(Block *self, World *var1, int var2, int var3,
                                                 int var4, JavaRandom *var5) {
    BlockRedstoneTorch *b = (BlockRedstoneTorch *)self;
    if (!b->torchActive)
        return;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    double var7 = (double)((float)var2 + 0.5f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var9 = (double)((float)var3 + 0.7f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var11 = (double)((float)var4 + 0.5f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var13 = (double)0.22f;
    double var15 = (double)0.27f;
    if (var6 == 1) {
        World_spawnParticle(var1, "reddust", var7 - var15, var9 + var13, var11, 0.0, 0.0, 0.0);
    } else if (var6 == 2) {
        World_spawnParticle(var1, "reddust", var7 + var15, var9 + var13, var11, 0.0, 0.0, 0.0);
    } else if (var6 == 3) {
        World_spawnParticle(var1, "reddust", var7, var9 + var13, var11 - var15, 0.0, 0.0, 0.0);
    } else if (var6 == 4) {
        World_spawnParticle(var1, "reddust", var7, var9 + var13, var11 + var15, 0.0, 0.0, 0.0);
    } else {
        World_spawnParticle(var1, "reddust", var7, var9, var11, 0.0, 0.0, 0.0);
    }
}

Block *BlockRedstoneTorch_create(int id, int tex, int active) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockTorch_initVtable(&vtable);
        vtable.getBlockTextureFromSideAndMetadata =
            BlockRedstoneTorch_getBlockTextureFromSideAndMetadata;
        vtable.tickRate = BlockRedstoneTorch_tickRate;
        vtable.onBlockAdded = BlockRedstoneTorch_onBlockAdded;
        vtable.onBlockRemoval = BlockRedstoneTorch_onBlockRemoval;
        vtable.isPoweringTo = BlockRedstoneTorch_isPoweringTo;
        vtable.isIndirectlyPoweringTo = BlockRedstoneTorch_isIndirectlyPoweringTo;
        vtable.updateTick = BlockRedstoneTorch_updateTick;
        vtable.onNeighborBlockChange = BlockRedstoneTorch_onNeighborBlockChange;
        vtable.idDropped = BlockRedstoneTorch_idDropped;
        vtable.canProvidePower = BlockRedstoneTorch_canProvidePower;
        vtable.randomDisplayTick = BlockRedstoneTorch_randomDisplayTick;
        vtable_init = 1;
    }
    BlockRedstoneTorch *self = (BlockRedstoneTorch *)calloc(1, sizeof(BlockRedstoneTorch));
    self->torchActive = active;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, Material_circuits);
    Block_setTickOnLoad(&self->base, 1);
    return &self->base;
}
