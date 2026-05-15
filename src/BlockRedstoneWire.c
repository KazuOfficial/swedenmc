#include "Block.h"
#include "Material.h"
#include "World.h"
#include "Item.h"
#include "ChunkPosition.h"
#include "ModelBed.h"
#include "JavaRandom.h"
#include <stdlib.h>
#include <string.h>

typedef struct BlockRedstoneWire {
    Block base;
    int wiresProvidePower;

    ChunkPosition *field_21031_b;
    int field_21031_b_count;
    int field_21031_b_cap;
} BlockRedstoneWire;

static void BlockRedstoneWire_updateAndPropagateCurrentStrength(BlockRedstoneWire *self,
                                                                World *var1, int var2, int var3,
                                                                int var4);
static void BlockRedstoneWire_func_21030_a(BlockRedstoneWire *self, World *var1, int var2, int var3,
                                           int var4, int var5, int var6, int var7);
static void BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(BlockRedstoneWire *self,
                                                                  World *var1, int var2, int var3,
                                                                  int var4);
static int BlockRedstoneWire_getMaxCurrentStrength(BlockRedstoneWire *self, World *var1, int var2,
                                                   int var3, int var4, int var5);

static void field_21031_b_add(BlockRedstoneWire *self, int x, int y, int z) {
    for (int i = 0; i < self->field_21031_b_count; i++) {
        if (self->field_21031_b[i].x == x && self->field_21031_b[i].y == y &&
            self->field_21031_b[i].z == z)
            return;
    }
    if (self->field_21031_b_count >= self->field_21031_b_cap) {
        int newcap = self->field_21031_b_cap == 0 ? 8 : self->field_21031_b_cap * 2;
        self->field_21031_b =
            (ChunkPosition *)realloc(self->field_21031_b, (size_t)newcap * sizeof(ChunkPosition));
        self->field_21031_b_cap = newcap;
    }
    self->field_21031_b[self->field_21031_b_count].x = x;
    self->field_21031_b[self->field_21031_b_count].y = y;
    self->field_21031_b[self->field_21031_b_count].z = z;
    self->field_21031_b_count++;
}

static int BlockRedstoneWire_canPlaceBlockAt(Block *self, World *var1, int var2, int var3,
                                             int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4);
}

static void BlockRedstoneWire_updateAndPropagateCurrentStrength(BlockRedstoneWire *self,
                                                                World *var1, int var2, int var3,
                                                                int var4) {
    BlockRedstoneWire_func_21030_a(self, var1, var2, var3, var4, var2, var3, var4);

    int var5_count = self->field_21031_b_count;
    ChunkPosition *var5 = (ChunkPosition *)malloc((size_t)var5_count * sizeof(ChunkPosition));
    memcpy(var5, self->field_21031_b, (size_t)var5_count * sizeof(ChunkPosition));
    self->field_21031_b_count = 0;
    for (int var6 = 0; var6 < var5_count; ++var6) {
        World_notifyBlocksOfNeighborChange(var1, var5[var6].x, var5[var6].y, var5[var6].z,
                                           self->base.blockID);
    }
    free(var5);
}

static void BlockRedstoneWire_func_21030_a(BlockRedstoneWire *self, World *var1, int var2, int var3,
                                           int var4, int var5, int var6, int var7) {
    int var8 = World_getBlockMetadata(var1, var2, var3, var4);
    int var9 = 0;
    self->wiresProvidePower = 0;
    int var10 = World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4);
    self->wiresProvidePower = 1;
    int var11;
    int var12;
    int var13;
    if (var10) {
        var9 = 15;
    } else {
        for (var11 = 0; var11 < 4; ++var11) {
            var12 = var2;
            var13 = var4;
            if (var11 == 0) {
                var12 = var2 - 1;
            }
            if (var11 == 1) {
                ++var12;
            }
            if (var11 == 2) {
                var13 = var4 - 1;
            }
            if (var11 == 3) {
                ++var13;
            }
            if (var12 != var5 || var3 != var6 || var13 != var7) {
                var9 =
                    BlockRedstoneWire_getMaxCurrentStrength(self, var1, var12, var3, var13, var9);
            }
            if (World_isBlockNormalCube(var1, var12, var3, var13) &&
                !World_isBlockNormalCube(var1, var2, var3 + 1, var4)) {
                if (var12 != var5 || var3 + 1 != var6 || var13 != var7) {
                    var9 = BlockRedstoneWire_getMaxCurrentStrength(self, var1, var12, var3 + 1,
                                                                   var13, var9);
                }
            } else if (!World_isBlockNormalCube(var1, var12, var3, var13) &&
                       (var12 != var5 || var3 - 1 != var6 || var13 != var7)) {
                var9 = BlockRedstoneWire_getMaxCurrentStrength(self, var1, var12, var3 - 1, var13,
                                                               var9);
            }
        }
        if (var9 > 0) {
            --var9;
        } else {
            var9 = 0;
        }
    }
    if (var8 != var9) {
        var1->editingBlocks = 1;
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var9);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        var1->editingBlocks = 0;
        for (var11 = 0; var11 < 4; ++var11) {
            var12 = var2;
            var13 = var4;
            int var14 = var3 - 1;
            if (var11 == 0) {
                var12 = var2 - 1;
            }
            if (var11 == 1) {
                ++var12;
            }
            if (var11 == 2) {
                var13 = var4 - 1;
            }
            if (var11 == 3) {
                ++var13;
            }
            if (World_isBlockNormalCube(var1, var12, var3, var13)) {
                var14 += 2;
            }
            int var15 = 0;
            (void)var15;
            int var16 = BlockRedstoneWire_getMaxCurrentStrength(self, var1, var12, var3, var13, -1);
            var9 = World_getBlockMetadata(var1, var2, var3, var4);
            if (var9 > 0) {
                --var9;
            }
            if (var16 >= 0 && var16 != var9) {
                BlockRedstoneWire_func_21030_a(self, var1, var12, var3, var13, var2, var3, var4);
            }
            var16 = BlockRedstoneWire_getMaxCurrentStrength(self, var1, var12, var14, var13, -1);
            var9 = World_getBlockMetadata(var1, var2, var3, var4);
            if (var9 > 0) {
                --var9;
            }
            if (var16 >= 0 && var16 != var9) {
                BlockRedstoneWire_func_21030_a(self, var1, var12, var14, var13, var2, var3, var4);
            }
        }
        if (var8 == 0 || var9 == 0) {
            field_21031_b_add(self, var2, var3, var4);
            field_21031_b_add(self, var2 - 1, var3, var4);
            field_21031_b_add(self, var2 + 1, var3, var4);
            field_21031_b_add(self, var2, var3 - 1, var4);
            field_21031_b_add(self, var2, var3 + 1, var4);
            field_21031_b_add(self, var2, var3, var4 - 1);
            field_21031_b_add(self, var2, var3, var4 + 1);
        }
    }
}

static void BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(BlockRedstoneWire *self,
                                                                  World *var1, int var2, int var3,
                                                                  int var4) {
    if (World_getBlockId(var1, var2, var3, var4) == self->base.blockID) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->base.blockID);
    }
}

static int BlockRedstoneWire_getMaxCurrentStrength(BlockRedstoneWire *self, World *var1, int var2,
                                                   int var3, int var4, int var5) {
    if (World_getBlockId(var1, var2, var3, var4) != self->base.blockID) {
        return var5;
    }
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    return var6 > var5 ? var6 : var5;
}

static void BlockRedstoneWire_onBlockAdded(Block *selfB, World *var1, int var2, int var3,
                                           int var4) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    Block_defaultVtable.onBlockAdded(selfB, var1, var2, var3, var4);
    if (!var1->multiplayerWorld) {
        BlockRedstoneWire_updateAndPropagateCurrentStrength(self, var1, var2, var3, var4);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->base.blockID);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3, var4);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3, var4);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3, var4 - 1);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3, var4 + 1);
        if (World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3 + 1,
                                                                  var4);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3 - 1,
                                                                  var4);
        }
        if (World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3 + 1,
                                                                  var4);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3 - 1,
                                                                  var4);
        }
        if (World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 + 1,
                                                                  var4 - 1);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 - 1,
                                                                  var4 - 1);
        }
        if (World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 + 1,
                                                                  var4 + 1);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 - 1,
                                                                  var4 + 1);
        }
    }
}

static void BlockRedstoneWire_onBlockRemoval(Block *selfB, World *var1, int var2, int var3,
                                             int var4) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    Block_defaultVtable.onBlockRemoval(selfB, var1, var2, var3, var4);
    if (!var1->multiplayerWorld) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->base.blockID);
        BlockRedstoneWire_updateAndPropagateCurrentStrength(self, var1, var2, var3, var4);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3, var4);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3, var4);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3, var4 - 1);
        BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3, var4 + 1);
        if (World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3 + 1,
                                                                  var4);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 - 1, var3 - 1,
                                                                  var4);
        }
        if (World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3 + 1,
                                                                  var4);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2 + 1, var3 - 1,
                                                                  var4);
        }
        if (World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 + 1,
                                                                  var4 - 1);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 - 1,
                                                                  var4 - 1);
        }
        if (World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 + 1,
                                                                  var4 + 1);
        } else {
            BlockRedstoneWire_notifyWireNeighborsOfNeighborChange(self, var1, var2, var3 - 1,
                                                                  var4 + 1);
        }
    }
}

static void BlockRedstoneWire_onNeighborBlockChange(Block *selfB, World *var1, int var2, int var3,
                                                    int var4, int var5) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    if (!var1->multiplayerWorld) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        int var7 = BlockRedstoneWire_canPlaceBlockAt(selfB, var1, var2, var3, var4);
        if (!var7) {
            Block_dropBlockAsItem(selfB, var1, var2, var3, var4, var6);
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        } else {
            BlockRedstoneWire_updateAndPropagateCurrentStrength(self, var1, var2, var3, var4);
        }
        Block_defaultVtable.onNeighborBlockChange(selfB, var1, var2, var3, var4, var5);
    }
}

static int BlockRedstoneWire_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Item_redstone->shiftedIndex;
}

static int BlockRedstoneWire_isPoweringTo(Block *selfB, IBlockAccess *var1, int var2, int var3,
                                          int var4, int var5) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    extern int BlockRedstoneWire_isPowerProviderOrWire(IBlockAccess * var0, int var1, int var2,
                                                       int var3, int var4);
    if (!self->wiresProvidePower)
        return 0;
    if (var1->vtable->getBlockMetadata(var1, var2, var3, var4) == 0)
        return 0;
    if (var5 == 1)
        return 1;
    int var6 = BlockRedstoneWire_isPowerProviderOrWire(var1, var2 - 1, var3, var4, 1) ||
               (!var1->vtable->isBlockNormalCube(var1, var2 - 1, var3, var4) &&
                BlockRedstoneWire_isPowerProviderOrWire(var1, var2 - 1, var3 - 1, var4, -1));
    int var7 = BlockRedstoneWire_isPowerProviderOrWire(var1, var2 + 1, var3, var4, 3) ||
               (!var1->vtable->isBlockNormalCube(var1, var2 + 1, var3, var4) &&
                BlockRedstoneWire_isPowerProviderOrWire(var1, var2 + 1, var3 - 1, var4, -1));
    int var8 = BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3, var4 - 1, 2) ||
               (!var1->vtable->isBlockNormalCube(var1, var2, var3, var4 - 1) &&
                BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3 - 1, var4 - 1, -1));
    int var9 = BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3, var4 + 1, 0) ||
               (!var1->vtable->isBlockNormalCube(var1, var2, var3, var4 + 1) &&
                BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3 - 1, var4 + 1, -1));
    if (!var1->vtable->isBlockNormalCube(var1, var2, var3 + 1, var4)) {
        if (var1->vtable->isBlockNormalCube(var1, var2 - 1, var3, var4) &&
            BlockRedstoneWire_isPowerProviderOrWire(var1, var2 - 1, var3 + 1, var4, -1)) {
            var6 = 1;
        }
        if (var1->vtable->isBlockNormalCube(var1, var2 + 1, var3, var4) &&
            BlockRedstoneWire_isPowerProviderOrWire(var1, var2 + 1, var3 + 1, var4, -1)) {
            var7 = 1;
        }
        if (var1->vtable->isBlockNormalCube(var1, var2, var3, var4 - 1) &&
            BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3 + 1, var4 - 1, -1)) {
            var8 = 1;
        }
        if (var1->vtable->isBlockNormalCube(var1, var2, var3, var4 + 1) &&
            BlockRedstoneWire_isPowerProviderOrWire(var1, var2, var3 + 1, var4 + 1, -1)) {
            var9 = 1;
        }
    }
    return (!var8 && !var7 && !var6 && !var9 && var5 >= 2 && var5 <= 5) ? 1
           : (var5 == 2 && var8 && !var6 && !var7)                      ? 1
           : (var5 == 3 && var9 && !var6 && !var7)                      ? 1
           : (var5 == 4 && var6 && !var8 && !var9) ? 1
                                                   : (var5 == 5 && var7 && !var8 && !var9);
}

static int BlockRedstoneWire_isIndirectlyPoweringTo(Block *selfB, World *var1, int var2, int var3,
                                                    int var4, int var5) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    if (!self->wiresProvidePower)
        return 0;
    return BlockRedstoneWire_isPoweringTo(selfB, (IBlockAccess *)var1, var2, var3, var4, var5);
}

static int BlockRedstoneWire_canProvidePower(Block *selfB) {
    BlockRedstoneWire *self = (BlockRedstoneWire *)selfB;
    return self->wiresProvidePower;
}

static void BlockRedstoneWire_randomDisplayTick(Block *self, World *var1, int var2, int var3,
                                                int var4, JavaRandom *var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if (var6 > 0) {
        double var7 = (double)var2 + 0.5 + ((double)JavaRandom_nextFloat(var5) - 0.5) * 0.2;
        double var9 = (double)((float)var3 + 1.0f / 16.0f);
        double var11 = (double)var4 + 0.5 + ((double)JavaRandom_nextFloat(var5) - 0.5) * 0.2;
        float var13 = (float)var6 / 15.0f;
        float var14 = var13 * 0.6f + 0.4f;
        if (var6 == 0) {
            var14 = 0.0f;
        }
        float var15 = var13 * var13 * 0.7f - 0.5f;
        float var16 = var13 * var13 * 0.6f - 0.7f;
        if (var15 < 0.0f) {
            var15 = 0.0f;
        }
        if (var16 < 0.0f) {
            var16 = 0.0f;
        }
        World_spawnParticle(var1, "reddust", var7, var9, var11, (double)var14, (double)var15,
                            (double)var16);
    }
}

static int BlockRedstoneWire_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    (void)var1;
    (void)var2;
    return self->blockIndexInTexture;
}

static int BlockRedstoneWire_colorMultiplier(Block *self, IBlockAccess *world, int x, int y,
                                             int z) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
    return 8388608;
}

int BlockRedstoneWire_isPowerProviderOrWire(IBlockAccess *var0, int var1, int var2, int var3,
                                            int var4) {
    extern Block *Block_redstoneWire;
    extern Block *Block_redstoneRepeaterIdle;
    extern Block *Block_redstoneRepeaterActive;
    int var5 = var0->vtable->getBlockId(var0, var1, var2, var3);
    if (var5 == Block_redstoneWire->blockID)
        return 1;
    if (var5 == 0)
        return 0;
    if (Block_blocksList[var5] &&
        Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5]))
        return 1;
    if (var5 != Block_redstoneRepeaterIdle->blockID &&
        var5 != Block_redstoneRepeaterActive->blockID)
        return 0;
    int var6 = var0->vtable->getBlockMetadata(var0, var1, var2, var3);
    return var4 == ModelBed_field_22279_b[var6 & 3];
}

static int BlockRedstoneWire_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockRedstoneWire_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockRedstoneWire_getRenderType(Block *self) {
    (void)self;
    return 5;
}

static AxisAlignedBB *BlockRedstoneWire_getCollisionBoundingBoxFromPool(Block *self, World *w,
                                                                        int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}

Block *BlockRedstoneWire_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSideAndMetadata =
            BlockRedstoneWire_getBlockTextureFromSideAndMetadata;
        vtable.isOpaqueCube = BlockRedstoneWire_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockRedstoneWire_renderAsNormalBlock;
        vtable.getRenderType = BlockRedstoneWire_getRenderType;
        vtable.canProvidePower = BlockRedstoneWire_canProvidePower;
        vtable.getCollisionBoundingBoxFromPool = BlockRedstoneWire_getCollisionBoundingBoxFromPool;
        vtable.idDropped = BlockRedstoneWire_idDropped;
        vtable.canPlaceBlockAt = BlockRedstoneWire_canPlaceBlockAt;
        vtable.onBlockAdded = BlockRedstoneWire_onBlockAdded;
        vtable.onBlockRemoval = BlockRedstoneWire_onBlockRemoval;
        vtable.onNeighborBlockChange = BlockRedstoneWire_onNeighborBlockChange;
        vtable.isPoweringTo = BlockRedstoneWire_isPoweringTo;
        vtable.isIndirectlyPoweringTo = BlockRedstoneWire_isIndirectlyPoweringTo;
        vtable.colorMultiplier = BlockRedstoneWire_colorMultiplier;
        vtable.randomDisplayTick = BlockRedstoneWire_randomDisplayTick;
        vtable_init = 1;
    }
    BlockRedstoneWire *self = (BlockRedstoneWire *)calloc(1, sizeof(BlockRedstoneWire));
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, Material_circuits);
    Block_setBlockBounds(&self->base, 0.0f, 0.0f, 0.0f, 1.0f, 0.0625f, 1.0f);
    self->wiresProvidePower = 1;
    return &self->base;
}
