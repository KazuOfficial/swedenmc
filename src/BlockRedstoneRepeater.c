#include "Block.h"
#include "Material.h"
#include "World.h"
#include "Item.h"
#include "MathHelper.h"
#include "JavaRandom.h"
#include "EntityLiving.h"
#include <stdlib.h>

const double BlockRedstoneRepeater_field_22024_a[4] = {-0.0625, 1.0 / 16.0, 0.1875, 0.3125};

static const int BlockRedstoneRepeater_field_22023_b[4] = {1, 2, 3, 4};

typedef struct {
    Block base;
    int isRepeaterPowered;
} BlockRedstoneRepeater;

static int BlockRedstoneRepeater_func_22022_g(BlockRedstoneRepeater *self, World *var1, int var2,
                                              int var3, int var4, int var5);

static int BlockRedstoneRepeater_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockRedstoneRepeater_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static int BlockRedstoneRepeater_canProvidePower(Block *self) {
    (void)self;
    return 0;
}

static int BlockRedstoneRepeater_getRenderType(Block *self) {
    (void)self;
    return 15;
}

static int BlockRedstoneRepeater_canPlaceBlockAt(Block *self, World *var1, int var2, int var3,
                                                 int var4) {
    if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4))
        return 0;
    return Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3, var4);
}

static int BlockRedstoneRepeater_canBlockStay(Block *self, World *var1, int var2, int var3,
                                              int var4) {
    if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4))
        return 0;
    return Block_defaultVtable.canBlockStay(self, var1, var2, var3, var4);
}

static int BlockRedstoneRepeater_getBlockTextureFromSideAndMetadata(Block *self, int var1,
                                                                    int var2) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    (void)var2;
    if (var1 == 0)
        return b->isRepeaterPowered ? 99 : 115;
    if (var1 == 1)
        return b->isRepeaterPowered ? 147 : 131;
    return 5;
}

static int BlockRedstoneRepeater_shouldSideBeRendered(Block *self, IBlockAccess *var1, int var2,
                                                      int var3, int var4, int var5) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return var5 != 0 && var5 != 1;
}

static int BlockRedstoneRepeater_getBlockTextureFromSide(Block *self, int var1) {
    return self->vtable->getBlockTextureFromSideAndMetadata(self, var1, 0);
}

static int BlockRedstoneRepeater_isIndirectlyPoweringTo(Block *self, World *var1, int var2,
                                                        int var3, int var4, int var5) {
    return self->vtable->isPoweringTo(self, (IBlockAccess *)var1, var2, var3, var4, var5);
}

static int BlockRedstoneRepeater_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3,
                                              int var4, int var5) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    if (!b->isRepeaterPowered)
        return 0;
    int var6 = var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 3;
    return (var6 == 0 && var5 == 3)
               ? 1
               : ((var6 == 1 && var5 == 4)
                      ? 1
                      : ((var6 == 2 && var5 == 2) ? 1 : (var6 == 3 && var5 == 5)));
}

static void BlockRedstoneRepeater_onNeighborBlockChange(Block *self, World *var1, int var2,
                                                        int var3, int var4, int var5) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    (void)var5;
    if (!BlockRedstoneRepeater_canBlockStay(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    } else {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        int var7 = BlockRedstoneRepeater_func_22022_g(b, var1, var2, var3, var4, var6);
        int var8 = (var6 & 12) >> 2;
        if (b->isRepeaterPowered && !var7) {
            World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                      BlockRedstoneRepeater_field_22023_b[var8] * 2);
        } else if (!b->isRepeaterPowered && var7) {
            World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                      BlockRedstoneRepeater_field_22023_b[var8] * 2);
        }
    }
}

static void BlockRedstoneRepeater_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                             JavaRandom *var5) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    (void)var5;
    extern Block *Block_redstoneRepeaterIdle;
    extern Block *Block_redstoneRepeaterActive;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = BlockRedstoneRepeater_func_22022_g(b, var1, var2, var3, var4, var6);
    if (b->isRepeaterPowered && !var7) {
        World_setBlockAndMetadataWithNotify(var1, var2, var3, var4,
                                            Block_redstoneRepeaterIdle->blockID, var6);
    } else if (!b->isRepeaterPowered) {
        World_setBlockAndMetadataWithNotify(var1, var2, var3, var4,
                                            Block_redstoneRepeaterActive->blockID, var6);
        if (!var7) {
            int var8 = (var6 & 12) >> 2;
            World_scheduleBlockUpdate(var1, var2, var3, var4, Block_redstoneRepeaterActive->blockID,
                                      BlockRedstoneRepeater_field_22023_b[var8] * 2);
        }
    }
}

static int BlockRedstoneRepeater_blockActivated(Block *self, World *var1, int var2, int var3,
                                                int var4, EntityPlayer *var5) {
    (void)self;
    (void)var5;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = (var6 & 12) >> 2;
    var7 = (var7 + 1) << 2 & 12;
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 | (var6 & 3));
    return 1;
}

static void BlockRedstoneRepeater_onBlockPlacedBy(Block *self, World *var1, int var2, int var3,
                                                  int var4, EntityLiving *var5) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    int var6 =
        ((MathHelper_floor_double((double)(var5->base.rotationYaw * 4.0f / 360.0f) + 0.5) & 3) +
         2) %
        4;
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
    int var7 = BlockRedstoneRepeater_func_22022_g(b, var1, var2, var3, var4, var6);
    if (var7) {
        World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID, 1);
    }
}

static void BlockRedstoneRepeater_onBlockAdded(Block *self, World *var1, int var2, int var3,
                                               int var4) {
    World_notifyBlocksOfNeighborChange(var1, var2 + 1, var3, var4, self->blockID);
    World_notifyBlocksOfNeighborChange(var1, var2 - 1, var3, var4, self->blockID);
    World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 + 1, self->blockID);
    World_notifyBlocksOfNeighborChange(var1, var2, var3, var4 - 1, self->blockID);
    World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
    World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->blockID);
}

static int BlockRedstoneRepeater_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    extern Item *Item_redstoneRepeater;
    return Item_redstoneRepeater->shiftedIndex;
}

static void BlockRedstoneRepeater_randomDisplayTick(Block *self, World *var1, int var2, int var3,
                                                    int var4, JavaRandom *var5) {
    BlockRedstoneRepeater *b = (BlockRedstoneRepeater *)self;
    if (!b->isRepeaterPowered)
        return;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    double var7 = (double)((float)var2 + 0.5f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var9 = (double)((float)var3 + 0.4f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var11 = (double)((float)var4 + 0.5f) + (double)(JavaRandom_nextFloat(var5) - 0.5f) * 0.2;
    double var13 = 0.0;
    double var15 = 0.0;
    if (JavaRandom_nextInt(var5, 2) == 0) {
        switch (var6 & 3) {
        case 0:
            var15 = -0.3125;
            break;
        case 1:
            var13 = 0.3125;
            break;
        case 2:
            var15 = 0.3125;
            break;
        case 3:
            var13 = -0.3125;
            break;
        }
    } else {
        int var17 = (var6 & 12) >> 2;
        switch (var6 & 3) {
        case 0:
            var15 = BlockRedstoneRepeater_field_22024_a[var17];
            break;
        case 1:
            var13 = -BlockRedstoneRepeater_field_22024_a[var17];
            break;
        case 2:
            var15 = -BlockRedstoneRepeater_field_22024_a[var17];
            break;
        case 3:
            var13 = BlockRedstoneRepeater_field_22024_a[var17];
            break;
        }
    }
    World_spawnParticle(var1, "reddust", var7 + var13, var9, var11 + var15, 0.0, 0.0, 0.0);
}

static int BlockRedstoneRepeater_func_22022_g(BlockRedstoneRepeater *self, World *var1, int var2,
                                              int var3, int var4, int var5) {
    (void)self;
    extern Block *Block_redstoneWire;
    int var6 = var5 & 3;
    switch (var6) {
    case 0:
        return World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 + 1, 3) ||
               (World_getBlockId(var1, var2, var3, var4 + 1) == Block_redstoneWire->blockID &&
                World_getBlockMetadata(var1, var2, var3, var4 + 1) > 0);
    case 1:
        return World_isBlockIndirectlyProvidingPowerTo(var1, var2 - 1, var3, var4, 4) ||
               (World_getBlockId(var1, var2 - 1, var3, var4) == Block_redstoneWire->blockID &&
                World_getBlockMetadata(var1, var2 - 1, var3, var4) > 0);
    case 2:
        return World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 - 1, 2) ||
               (World_getBlockId(var1, var2, var3, var4 - 1) == Block_redstoneWire->blockID &&
                World_getBlockMetadata(var1, var2, var3, var4 - 1) > 0);
    case 3:
        return World_isBlockIndirectlyProvidingPowerTo(var1, var2 + 1, var3, var4, 5) ||
               (World_getBlockId(var1, var2 + 1, var3, var4) == Block_redstoneWire->blockID &&
                World_getBlockMetadata(var1, var2 + 1, var3, var4) > 0);
    default:
        return 0;
    }
}

Block *BlockRedstoneRepeater_create(int id, int powered) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.renderAsNormalBlock = BlockRedstoneRepeater_renderAsNormalBlock;
        vtable.isOpaqueCube = BlockRedstoneRepeater_isOpaqueCube;
        vtable.canProvidePower = BlockRedstoneRepeater_canProvidePower;
        vtable.getRenderType = BlockRedstoneRepeater_getRenderType;
        vtable.canPlaceBlockAt = BlockRedstoneRepeater_canPlaceBlockAt;
        vtable.canBlockStay = BlockRedstoneRepeater_canBlockStay;
        vtable.getBlockTextureFromSideAndMetadata =
            BlockRedstoneRepeater_getBlockTextureFromSideAndMetadata;
        vtable.shouldSideBeRendered = BlockRedstoneRepeater_shouldSideBeRendered;
        vtable.getBlockTextureFromSide = BlockRedstoneRepeater_getBlockTextureFromSide;
        vtable.isIndirectlyPoweringTo = BlockRedstoneRepeater_isIndirectlyPoweringTo;
        vtable.isPoweringTo = BlockRedstoneRepeater_isPoweringTo;
        vtable.onNeighborBlockChange = BlockRedstoneRepeater_onNeighborBlockChange;
        vtable.updateTick = BlockRedstoneRepeater_updateTick;
        vtable.blockActivated = BlockRedstoneRepeater_blockActivated;
        vtable.onBlockPlacedBy = BlockRedstoneRepeater_onBlockPlacedBy;
        vtable.onBlockAdded = BlockRedstoneRepeater_onBlockAdded;
        vtable.idDropped = BlockRedstoneRepeater_idDropped;
        vtable.randomDisplayTick = BlockRedstoneRepeater_randomDisplayTick;
        vtable_init = 1;
    }
    BlockRedstoneRepeater *self = (BlockRedstoneRepeater *)calloc(1, sizeof(BlockRedstoneRepeater));
    self->isRepeaterPowered = powered;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, 6, Material_circuits);
    Block_setBlockBounds(&self->base, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
    return &self->base;
}
