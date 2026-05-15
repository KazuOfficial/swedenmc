#include "Block.h"
#include "Material.h"
#include "MathHelper.h"
#include "PistonBlockTextures.h"
#include "TileEntityPiston.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include <stdlib.h>
#include <math.h>

typedef struct {
    Block base;
    int isSticky;
    int field_31048_b;
} BlockPistonBase;

int BlockPistonBase_isPowered(int meta) { return (meta & 8) != 0; }

int BlockPistonBase_func_31044_d(int meta) { return meta & 7; }

int BlockPistonBase_func_31040_i(Block *self) {
    return ((BlockPistonBase *)self)->isSticky ? 106 : 107;
}

static int BlockPistonBase_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonBase_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonBase_getRenderType(Block *self) {
    (void)self;
    return 16;
}

static int BlockPistonBase_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    int var3 = BlockPistonBase_func_31044_d(var2);
    if (var3 > 5)
        return self->blockIndexInTexture;
    if (var1 == var3) {
        return (!BlockPistonBase_isPowered(var2) && self->minX <= 0.0 && self->minY <= 0.0 &&
                self->minZ <= 0.0 && self->maxX >= 1.0 && self->maxY >= 1.0 && self->maxZ >= 1.0)
                   ? self->blockIndexInTexture
                   : 110;
    }
    return (var1 == PistonBlockTextures_field_31057_a[var3]) ? 109 : 108;
}

static int BlockPistonBase_blockActivated(Block *self, World *world, int x, int y, int z,
                                          EntityPlayer *player) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
    (void)player;
    return 0;
}

static int func_31039_c(World *var0, int var1, int var2, int var3, EntityPlayer *var4) {
    (void)var0;
    Entity *e = &var4->base.base;
    if (MathHelper_abs((float)e->posX - (float)var1) < 2.0f &&
        MathHelper_abs((float)e->posZ - (float)var3) < 2.0f) {
        double var5 = e->posY + 1.82 - (double)e->yOffset;
        if (var5 - (double)var2 > 2.0)
            return 1;
        if ((double)var2 - var5 > 0.0)
            return 0;
    }
    int var7 = MathHelper_floor_double((double)(e->rotationYaw * 4.0f / 360.0f) + 0.5) & 3;
    return var7 == 0 ? 2 : (var7 == 1 ? 5 : (var7 == 2 ? 3 : (var7 == 3 ? 4 : 0)));
}

static int func_31041_f(World *var1, int var2, int var3, int var4, int var5) {
    return (var5 != 0 && World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 - 1, var4, 0)) ? 1
           : (var5 != 1 && World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 1, var4, 1))
               ? 1
           : (var5 != 2 && World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 - 1, 2))
               ? 1
           : (var5 != 3 && World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4 + 1, 3))
               ? 1
           : (var5 != 5 && World_isBlockIndirectlyProvidingPowerTo(var1, var2 + 1, var3, var4, 5))
               ? 1
           : (var5 != 4 && World_isBlockIndirectlyProvidingPowerTo(var1, var2 - 1, var3, var4, 4))
               ? 1
           : World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3, var4, 0)         ? 1
           : World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 2, var4, 1)     ? 1
           : World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 1, var4 - 1, 2) ? 1
           : World_isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 1, var4 + 1, 3) ? 1
           : World_isBlockIndirectlyProvidingPowerTo(var1, var2 - 1, var3 + 1, var4, 4)
               ? 1
               : World_isBlockIndirectlyProvidingPowerTo(var1, var2 + 1, var3 + 1, var4, 5);
}

static int canPushBlock(int var0, World *var1, int var2, int var3, int var4, int var5) {
    if (var0 == Block_obsidian->blockID)
        return 0;
    if (!Block_blocksList[var0])
        return 0;
    if (var0 != Block_pistonBase->blockID && var0 != Block_pistonStickyBase->blockID) {
        if (Block_blocksList[var0]->blockHardness == -1.0f)
            return 0;
        if (Block_blocksList[var0]->vtable->getMobilityFlag(Block_blocksList[var0]) == 2)
            return 0;
        if (!var5 && Block_blocksList[var0]->vtable->getMobilityFlag(Block_blocksList[var0]) == 1)
            return 0;
    } else {
        if (BlockPistonBase_isPowered(World_getBlockMetadata(var1, var2, var3, var4)))
            return 0;
    }
    TileEntity *var6 = World_getBlockTileEntity(var1, var2, var3, var4);
    return var6 == NULL;
}

static int func_31045_h(World *var0, int var1, int var2, int var3, int var4) {
    int var5 = var1 + PistonBlockTextures_field_31056_b[var4];
    int var6 = var2 + PistonBlockTextures_field_31059_c[var4];
    int var7 = var3 + PistonBlockTextures_field_31058_d[var4];
    int var8 = 0;
    while (1) {
        if (var8 < 13) {
            if (var6 <= 0 || var6 >= 127)
                return 0;
            int var9 = World_getBlockId(var0, var5, var6, var7);
            if (var9 != 0) {
                if (!canPushBlock(var9, var0, var5, var6, var7, 1))
                    return 0;
                if (Block_blocksList[var9]->vtable->getMobilityFlag(Block_blocksList[var9]) != 1) {
                    if (var8 == 12)
                        return 0;
                    var5 += PistonBlockTextures_field_31056_b[var4];
                    var6 += PistonBlockTextures_field_31059_c[var4];
                    var7 += PistonBlockTextures_field_31058_d[var4];
                    ++var8;
                    continue;
                }
            }
        }
        return 1;
    }
}

static int func_31047_i(BlockPistonBase *bp, World *var1, int var2, int var3, int var4, int var5) {
    int var6 = var2 + PistonBlockTextures_field_31056_b[var5];
    int var7 = var3 + PistonBlockTextures_field_31059_c[var5];
    int var8 = var4 + PistonBlockTextures_field_31058_d[var5];
    int var9 = 0;
    while (1) {
        int var10;
        if (var9 < 13) {
            if (var7 <= 0 || var7 >= 127)
                return 0;
            var10 = World_getBlockId(var1, var6, var7, var8);
            if (var10 != 0) {
                if (!canPushBlock(var10, var1, var6, var7, var8, 1))
                    return 0;
                if (Block_blocksList[var10]->vtable->getMobilityFlag(Block_blocksList[var10]) !=
                    1) {
                    if (var9 == 12)
                        return 0;
                    var6 += PistonBlockTextures_field_31056_b[var5];
                    var7 += PistonBlockTextures_field_31059_c[var5];
                    var8 += PistonBlockTextures_field_31058_d[var5];
                    ++var9;
                    continue;
                }
                Block_dropBlockAsItem(Block_blocksList[var10], var1, var6, var7, var8,
                                      World_getBlockMetadata(var1, var6, var7, var8));
                World_setBlockWithNotify(var1, var6, var7, var8, 0);
            }
        }

        while (var6 != var2 || var7 != var3 || var8 != var4) {
            var9 = var6 - PistonBlockTextures_field_31056_b[var5];
            var10 = var7 - PistonBlockTextures_field_31059_c[var5];
            int var11 = var8 - PistonBlockTextures_field_31058_d[var5];
            int var12 = World_getBlockId(var1, var9, var10, var11);
            int var13 = World_getBlockMetadata(var1, var9, var10, var11);
            if (var12 == bp->base.blockID && var9 == var2 && var10 == var3 && var11 == var4) {
                World_setBlockAndMetadata(var1, var6, var7, var8, Block_pistonMoving->blockID,
                                          var5 | (bp->isSticky ? 8 : 0));
                World_setBlockTileEntity(
                    var1, var6, var7, var8,
                    BlockPistonMoving_func_31036_a(Block_pistonExtension->blockID,
                                                   var5 | (bp->isSticky ? 8 : 0), var5, 1, 0));
            } else {
                World_setBlockAndMetadata(var1, var6, var7, var8, Block_pistonMoving->blockID,
                                          var13);
                World_setBlockTileEntity(var1, var6, var7, var8,
                                         BlockPistonMoving_func_31036_a(var12, var13, var5, 1, 0));
            }
            var6 = var9;
            var7 = var10;
            var8 = var11;
        }
        return 1;
    }
}

static void func_31043_h(BlockPistonBase *bp, World *var1, int var2, int var3, int var4) {
    (void)bp;
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    int var6 = BlockPistonBase_func_31044_d(var5);
    int var7 = func_31041_f(var1, var2, var3, var4, var6);
    if (var5 != 7) {
        if (var7 && !BlockPistonBase_isPowered(var5)) {
            if (func_31045_h(var1, var2, var3, var4, var6)) {
                World_setBlockMetadata(var1, var2, var3, var4, var6 | 8);
                World_playNoteAt(var1, var2, var3, var4, 0, var6);
            }
        } else if (!var7 && BlockPistonBase_isPowered(var5)) {
            World_setBlockMetadata(var1, var2, var3, var4, var6);
            World_playNoteAt(var1, var2, var3, var4, 1, var6);
        }
    }
}

static void BlockPistonBase_onBlockPlacedBy(Block *self, World *var1, int var2, int var3, int var4,
                                            EntityLiving *var5) {
    BlockPistonBase *bp = (BlockPistonBase *)self;
    int var6 = func_31039_c(var1, var2, var3, var4, (EntityPlayer *)var5);
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
    if (!var1->multiplayerWorld) {
        func_31043_h(bp, var1, var2, var3, var4);
    }
}

static void BlockPistonBase_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                                  int var4, int var5) {
    (void)var5;
    BlockPistonBase *bp = (BlockPistonBase *)self;
    if (!var1->multiplayerWorld && !bp->field_31048_b) {
        func_31043_h(bp, var1, var2, var3, var4);
    }
}

static void BlockPistonBase_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    BlockPistonBase *bp = (BlockPistonBase *)self;
    if (!var1->multiplayerWorld && World_getBlockTileEntity(var1, var2, var3, var4) == NULL) {
        func_31043_h(bp, var1, var2, var3, var4);
    }
}

static void BlockPistonBase_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                       int var3, int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    if (BlockPistonBase_isPowered(var5)) {
        switch (BlockPistonBase_func_31044_d(var5)) {
        case 0:
            Block_setBlockBounds(self, 0.0f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 1:
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 12.0f / 16.0f, 1.0f);
            break;
        case 2:
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f);
            break;
        case 3:
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 12.0f / 16.0f);
            break;
        case 4:
            Block_setBlockBounds(self, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 5:
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f);
            break;
        default:
            break;
        }
    } else {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    }
}

static void BlockPistonBase_setBlockBoundsForItemRender(Block *self) {
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

static void BlockPistonBase_getCollidingBoundingBoxes(Block *self, World *world, int x, int y,
                                                      int z, AxisAlignedBB *bb, void *list) {
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Block_defaultVtable.getCollidingBoundingBoxes(self, world, x, y, z, bb, list);
}

static void BlockPistonBase_playBlock(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5, int var6) {
    BlockPistonBase *bp = (BlockPistonBase *)self;
    bp->field_31048_b = 1;
    if (var5 == 0) {
        if (func_31047_i(bp, var1, var2, var3, var4, var6)) {
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 | 8);
            World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                                  "tile.piston.out", 0.5f,
                                  JavaRandom_nextFloat(&var1->rand) * 0.25f + 0.6f);
        }
    } else if (var5 == 1) {
        TileEntity *var8 =
            World_getBlockTileEntity(var1, var2 + PistonBlockTextures_field_31056_b[var6],
                                     var3 + PistonBlockTextures_field_31059_c[var6],
                                     var4 + PistonBlockTextures_field_31058_d[var6]);
        if (var8 != NULL && var8->tileEntityTypeId == TE_TYPE_PISTON) {
            TileEntityPiston_func_31011_l((TileEntityPiston *)var8);
        }

        World_setBlockAndMetadata(var1, var2, var3, var4, Block_pistonMoving->blockID, var6);
        World_setBlockTileEntity(var1, var2, var3, var4,
                                 BlockPistonMoving_func_31036_a(self->blockID, var6, var6, 0, 1));

        if (bp->isSticky) {
            int var9 = var2 + PistonBlockTextures_field_31056_b[var6] * 2;
            int var10 = var3 + PistonBlockTextures_field_31059_c[var6] * 2;
            int var11 = var4 + PistonBlockTextures_field_31058_d[var6] * 2;
            int var12 = World_getBlockId(var1, var9, var10, var11);
            int var13 = World_getBlockMetadata(var1, var9, var10, var11);
            int var14 = 0;
            if (var12 == Block_pistonMoving->blockID) {
                TileEntity *var15 = World_getBlockTileEntity(var1, var9, var10, var11);
                if (var15 != NULL && var15->tileEntityTypeId == TE_TYPE_PISTON) {
                    TileEntityPiston *var16 = (TileEntityPiston *)var15;
                    if (TileEntityPiston_func_31009_d(var16) == var6 &&
                        TileEntityPiston_func_31015_b(var16)) {
                        TileEntityPiston_func_31011_l(var16);
                        var12 = TileEntityPiston_getStoredBlockID(var16);
                        var13 = TileEntityPiston_getBlockMetadata(var16);
                        var14 = 1;
                    }
                }
            }
            if (var14 || var12 <= 0 || !canPushBlock(var12, var1, var9, var10, var11, 0) ||
                (Block_blocksList[var12]->vtable->getMobilityFlag(Block_blocksList[var12]) != 0 &&
                 var12 != Block_pistonBase->blockID && var12 != Block_pistonStickyBase->blockID)) {
                if (!var14) {
                    bp->field_31048_b = 0;
                    World_setBlockWithNotify(var1, var2 + PistonBlockTextures_field_31056_b[var6],
                                             var3 + PistonBlockTextures_field_31059_c[var6],
                                             var4 + PistonBlockTextures_field_31058_d[var6], 0);
                    bp->field_31048_b = 1;
                }
            } else {
                bp->field_31048_b = 0;
                World_setBlockWithNotify(var1, var9, var10, var11, 0);
                bp->field_31048_b = 1;
                var2 += PistonBlockTextures_field_31056_b[var6];
                var3 += PistonBlockTextures_field_31059_c[var6];
                var4 += PistonBlockTextures_field_31058_d[var6];
                World_setBlockAndMetadata(var1, var2, var3, var4, Block_pistonMoving->blockID,
                                          var13);
                World_setBlockTileEntity(var1, var2, var3, var4,
                                         BlockPistonMoving_func_31036_a(var12, var13, var6, 0, 0));
            }
        } else {
            bp->field_31048_b = 0;
            World_setBlockWithNotify(var1, var2 + PistonBlockTextures_field_31056_b[var6],
                                     var3 + PistonBlockTextures_field_31059_c[var6],
                                     var4 + PistonBlockTextures_field_31058_d[var6], 0);
            bp->field_31048_b = 1;
        }

        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                              "tile.piston.in", 0.5f,
                              JavaRandom_nextFloat(&var1->rand) * 0.15f + 0.6f);
    }
    bp->field_31048_b = 0;
}

Block *BlockPistonBase_create(int id, int tex, int sticky) {
    static BlockVtable vtable_normal;
    static BlockVtable vtable_sticky;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable_normal = Block_defaultVtable;
        vtable_normal.isOpaqueCube = BlockPistonBase_isOpaqueCube;
        vtable_normal.renderAsNormalBlock = BlockPistonBase_renderAsNormalBlock;
        vtable_normal.getRenderType = BlockPistonBase_getRenderType;
        vtable_normal.getBlockTextureFromSideAndMetadata =
            BlockPistonBase_getBlockTextureFromSideAndMetadata;
        vtable_normal.blockActivated = BlockPistonBase_blockActivated;
        vtable_normal.onBlockPlacedBy = BlockPistonBase_onBlockPlacedBy;
        vtable_normal.onNeighborBlockChange = BlockPistonBase_onNeighborBlockChange;
        vtable_normal.onBlockAdded = BlockPistonBase_onBlockAdded;
        vtable_normal.setBlockBoundsBasedOnState = BlockPistonBase_setBlockBoundsBasedOnState;
        vtable_normal.setBlockBoundsForItemRender = BlockPistonBase_setBlockBoundsForItemRender;
        vtable_normal.getCollidingBoundingBoxes = BlockPistonBase_getCollidingBoundingBoxes;
        vtable_normal.playBlock = BlockPistonBase_playBlock;
        vtable_sticky = vtable_normal;
        vtable_init = 1;
    }

    BlockPistonBase *self = (BlockPistonBase *)calloc(1, sizeof(BlockPistonBase));
    self->isSticky = sticky;
    self->field_31048_b = 0;
    self->base.vtable = sticky ? &vtable_sticky : &vtable_normal;
    Block_constructWithTexture(&self->base, id, tex, Material_field_31067_B);
    self->base.stepSound = Block_soundStoneFootstep;
    self->base.blockHardness = 0.5f;
    self->base.blockResistance = 0.5f / 5.0f;
    return &self->base;
}
