#include "BlockRail.h"
#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "IBlockAccess.h"
#include "RailLogic.h"
#include "Vec3D.h"
#include <stdlib.h>

static int rail_func_27044_a(Block *self, World *var1, int var2, int var3, int var4, int var5,
                             int var6, int var7);
static int rail_func_27043_a(Block *self, World *var1, int var2, int var3, int var4, int var5,
                             int var6, int var7);

static AxisAlignedBB *rail_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                           int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

static int rail_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int rail_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int rail_getRenderType(Block *self) {
    (void)self;
    return 9;
}

static MovingObjectPosition *rail_collisionRayTrace(Block *self, World *var1, int var2, int var3,
                                                    int var4, Vec3D *var5, Vec3D *var6) {
    self->vtable->setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.collisionRayTrace(self, var1, var2, var3, var4, var5, var6);
}

static void rail_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                            int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    if (var5 >= 2 && var5 <= 5) {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 10.0f / 16.0f, 1.0f);
    } else {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
    }
}

static int rail_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    BlockRail *b = (BlockRail *)self;
    (void)var1;
    if (b->isPowered) {
        if (self->blockID == Block_railPowered->blockID && (var2 & 8) == 0) {
            return self->blockIndexInTexture - 16;
        }
    } else if (var2 >= 6) {
        return self->blockIndexInTexture - 16;
    }
    return self->blockIndexInTexture;
}

static int rail_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 1;
}

static int rail_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4);
}

static void rail_func_4031_h(Block *self, World *var1, int var2, int var3, int var4, int var5) {
    if (!var1->multiplayerWorld) {
        RailLogic *rl = RailLogic_create((BlockRail *)self, var1, var2, var3, var4);
        RailLogic_func_792_a(rl, World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4),
                             var5);
        RailLogic_destroy(rl);
    }
}

static void rail_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    if (!var1->multiplayerWorld) {
        rail_func_4031_h(self, var1, var2, var3, var4, 1);
    }
}

static void rail_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    BlockRail *b = (BlockRail *)self;
    if (!var1->multiplayerWorld) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        int var7 = var6;
        if (b->isPowered) {
            var7 = var6 & 7;
        }

        int var8 = 0;
        if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4)) {
            var8 = 1;
        }
        if (var7 == 2 && !World_isBlockNormalCube(var1, var2 + 1, var3, var4)) {
            var8 = 1;
        }
        if (var7 == 3 && !World_isBlockNormalCube(var1, var2 - 1, var3, var4)) {
            var8 = 1;
        }
        if (var7 == 4 && !World_isBlockNormalCube(var1, var2, var3, var4 - 1)) {
            var8 = 1;
        }
        if (var7 == 5 && !World_isBlockNormalCube(var1, var2, var3, var4 + 1)) {
            var8 = 1;
        }

        if (var8) {
            Block_dropBlockAsItem(self, var1, var2, var3, var4,
                                  World_getBlockMetadata(var1, var2, var3, var4));
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        } else if (self->blockID == Block_railPowered->blockID) {
            int var9 = World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4) ||
                       World_isBlockIndirectlyGettingPowered(var1, var2, var3 + 1, var4);
            var9 = var9 || rail_func_27044_a(self, var1, var2, var3, var4, var6, 1, 0) ||
                   rail_func_27044_a(self, var1, var2, var3, var4, var6, 0, 0);
            int var10 = 0;
            if (var9 && (var6 & 8) == 0) {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 | 8);
                var10 = 1;
            } else if (!var9 && (var6 & 8) != 0) {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7);
                var10 = 1;
            }
            if (var10) {
                World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
                if (var7 == 2 || var7 == 3 || var7 == 4 || var7 == 5) {
                    World_notifyBlocksOfNeighborChange(var1, var2, var3 + 1, var4, self->blockID);
                }
            }
        } else if (var5 > 0 && Block_blocksList[var5] != NULL &&
                   Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5]) &&
                   !b->isPowered) {
            RailLogic *rl = RailLogic_create(b, var1, var2, var3, var4);
            int nadj = RailLogic_getNAdjacentTracks(rl);
            RailLogic_destroy(rl);
            if (nadj == 3) {
                rail_func_4031_h(self, var1, var2, var3, var4, 0);
            }
        }
    }
}

static int rail_func_27044_a(Block *self, World *var1, int var2, int var3, int var4, int var5,
                             int var6, int var7) {
    if (var7 >= 8) {
        return 0;
    } else {
        int var8 = var5 & 7;
        int var9 = 1;
        switch (var8) {
        case 0:
            if (var6) {
                ++var4;
            } else {
                --var4;
            }
            break;
        case 1:
            if (var6) {
                --var2;
            } else {
                ++var2;
            }
            break;
        case 2:
            if (var6) {
                --var2;
            } else {
                ++var2;
                ++var3;
                var9 = 0;
            }
            var8 = 1;
            break;
        case 3:
            if (var6) {
                --var2;
                ++var3;
                var9 = 0;
            } else {
                ++var2;
            }
            var8 = 1;
            break;
        case 4:
            if (var6) {
                ++var4;
            } else {
                --var4;
                ++var3;
                var9 = 0;
            }
            var8 = 0;
            break;
        case 5:
            if (var6) {
                ++var4;
                ++var3;
                var9 = 0;
            } else {
                --var4;
            }
            var8 = 0;
            break;
        }
        return rail_func_27043_a(self, var1, var2, var3, var4, var6, var7, var8)
                   ? 1
                   : (var9 &&
                      rail_func_27043_a(self, var1, var2, var3 - 1, var4, var6, var7, var8));
    }
}

static int rail_func_27043_a(Block *self, World *var1, int var2, int var3, int var4, int var5,
                             int var6, int var7) {
    int var8 = World_getBlockId(var1, var2, var3, var4);
    if (var8 == Block_railPowered->blockID) {
        int var9 = World_getBlockMetadata(var1, var2, var3, var4);
        int var10 = var9 & 7;
        if (var7 == 1 && (var10 == 0 || var10 == 4 || var10 == 5)) {
            return 0;
        }
        if (var7 == 0 && (var10 == 1 || var10 == 2 || var10 == 3)) {
            return 0;
        }
        if ((var9 & 8) != 0) {
            if (!World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4) &&
                !World_isBlockIndirectlyGettingPowered(var1, var2, var3 + 1, var4)) {
                return rail_func_27044_a(self, var1, var2, var3, var4, var9, var5, var6 + 1);
            }
            return 1;
        }
    }
    return 0;
}

static int rail_getMobilityFlag(Block *self) {
    (void)self;
    return 0;
}

BlockVtable BlockRail_defaultVtable;

void BlockRail_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->getCollisionBoundingBoxFromPool = rail_getCollisionBoundingBoxFromPool;
    vt->isOpaqueCube = rail_isOpaqueCube;
    vt->renderAsNormalBlock = rail_renderAsNormalBlock;
    vt->getRenderType = rail_getRenderType;
    vt->collisionRayTrace = rail_collisionRayTrace;
    vt->setBlockBoundsBasedOnState = rail_setBlockBoundsBasedOnState;
    vt->getBlockTextureFromSideAndMetadata = rail_getBlockTextureFromSideAndMetadata;
    vt->quantityDropped = rail_quantityDropped;
    vt->canPlaceBlockAt = rail_canPlaceBlockAt;
    vt->onBlockAdded = rail_onBlockAdded;
    vt->onNeighborBlockChange = rail_onNeighborBlockChange;
    vt->getMobilityFlag = rail_getMobilityFlag;
}

void BlockRail_construct(BlockRail *self, int id, int tex, int booster) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockRail_initVtable(&BlockRail_defaultVtable);
        vtable_init = 1;
    }
    self->isPowered = booster;
    self->base.vtable = &BlockRail_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_circuits);

    Block_setBlockBounds(&self->base, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
}

Block *BlockRail_create(int id, int tex, int booster) {
    BlockRail *self = (BlockRail *)calloc(1, sizeof(BlockRail));
    BlockRail_construct(self, id, tex, booster);
    return &self->base;
}

int BlockRail_isRailBlockAt(World *world, int x, int y, int z) {
    int var4 = World_getBlockId(world, x, y, z);
    return var4 == Block_rail->blockID || var4 == Block_railPowered->blockID ||
           var4 == Block_railDetector->blockID;
}

int BlockRail_isRailBlock(int blockId) {
    return blockId == Block_rail->blockID || blockId == Block_railPowered->blockID ||
           blockId == Block_railDetector->blockID;
}

int BlockRail_getIsPowered(Block *self) { return ((BlockRail *)self)->isPowered; }
