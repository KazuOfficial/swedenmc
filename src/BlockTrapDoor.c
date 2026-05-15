#include "Block.h"
#include "Material.h"
#include "World.h"
#include "IBlockAccess.h"
#include "EntityPlayer.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockTrapDoor_isTrapdoorOpen(int var0) { return (var0 & 4) != 0; }

static int BlockTrapDoor_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockTrapDoor_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockTrapDoor_getRenderType(Block *self) {
    (void)self;
    return 0;
}

static AxisAlignedBB *BlockTrapDoor_getSelectedBoundingBoxFromPool(Block *self, World *var1,
                                                                   int var2, int var3, int var4) {
    self->vtable->setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.getSelectedBoundingBoxFromPool(self, var1, var2, var3, var4);
}

static AxisAlignedBB *BlockTrapDoor_getCollisionBoundingBoxFromPool(Block *self, World *var1,
                                                                    int var2, int var3, int var4) {
    self->vtable->setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.getCollisionBoundingBoxFromPool(self, var1, var2, var3, var4);
}

static void BlockTrapDoor_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                     int var3, int var4) {
    int meta = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    float var2f = 3.0f / 16.0f;
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, var2f, 1.0f);
    if (BlockTrapDoor_isTrapdoorOpen(meta)) {
        if ((meta & 3) == 0) {
            Block_setBlockBounds(self, 0.0f, 0.0f, 1.0f - var2f, 1.0f, 1.0f, 1.0f);
        }
        if ((meta & 3) == 1) {
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, var2f);
        }
        if ((meta & 3) == 2) {
            Block_setBlockBounds(self, 1.0f - var2f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        }
        if ((meta & 3) == 3) {
            Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, var2f, 1.0f, 1.0f);
        }
    }
}

static void BlockTrapDoor_setBlockBoundsForItemRender(Block *self) {
    float var1 = 3.0f / 16.0f;
    Block_setBlockBounds(self, 0.0f, 0.5f - var1 / 2.0f, 0.0f, 1.0f, 0.5f + var1 / 2.0f, 1.0f);
}

static void BlockTrapDoor_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                                         EntityPlayer *var5) {
    self->vtable->blockActivated(self, var1, var2, var3, var4, var5);
}

static void BlockTrapDoor_onPoweredBlockChange(Block *self, World *var1, int var2, int var3,
                                               int var4, int var5) {
    (void)self;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = (var6 & 4) > 0;
    if (var7 != var5) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 ^ 4);
        World_func_28107_a(var1, NULL, 1003, var2, var3, var4, 0);
    }
}

static int BlockTrapDoor_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                                        EntityPlayer *var5) {
    if (self->blockMaterial == Material_iron) {
        return 1;
    } else {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 ^ 4);
        World_func_28107_a(var1, var5, 1003, var2, var3, var4, 0);
        return 1;
    }
}

static void BlockTrapDoor_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                                int var4, int var5) {
    if (!var1->multiplayerWorld) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        int var7 = var2;
        int var8 = var4;
        if ((var6 & 3) == 0) {
            var8 = var4 + 1;
        }
        if ((var6 & 3) == 1) {
            --var8;
        }
        if ((var6 & 3) == 2) {
            var7 = var2 + 1;
        }
        if ((var6 & 3) == 3) {
            --var7;
        }
        if (!World_isBlockNormalCube(var1, var7, var3, var8)) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
            Block_dropBlockAsItem(self, var1, var2, var3, var4, var6);
        }
        if (var5 > 0 && Block_blocksList[var5] != NULL &&
            Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5])) {
            int var9 = World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4);
            BlockTrapDoor_onPoweredBlockChange(self, var1, var2, var3, var4, var9);
        }
    }
}

static MovingObjectPosition *BlockTrapDoor_collisionRayTrace(Block *self, World *var1, int var2,
                                                             int var3, int var4, Vec3D *var5,
                                                             Vec3D *var6) {
    self->vtable->setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.collisionRayTrace(self, var1, var2, var3, var4, var5, var6);
}

static void BlockTrapDoor_onBlockPlaced(Block *self, World *var1, int var2, int var3, int var4,
                                        int var5) {
    (void)self;
    int var6 = 0;
    if (var5 == 2) {
        var6 = 0;
    }
    if (var5 == 3) {
        var6 = 1;
    }
    if (var5 == 4) {
        var6 = 2;
    }
    if (var5 == 5) {
        var6 = 3;
    }
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
}

static int BlockTrapDoor_canPlaceBlockOnSide(Block *self, World *var1, int var2, int var3, int var4,
                                             int var5) {
    (void)self;
    if (var5 == 0) {
        return 0;
    } else if (var5 == 1) {
        return 0;
    } else {
        if (var5 == 2) {
            ++var4;
        }
        if (var5 == 3) {
            --var4;
        }
        if (var5 == 4) {
            ++var2;
        }
        if (var5 == 5) {
            --var2;
        }
        return World_isBlockNormalCube(var1, var2, var3, var4);
    }
}

Block *BlockTrapDoor_create(int id, Material *mat) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockTrapDoor_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockTrapDoor_renderAsNormalBlock;
        vtable.getRenderType = BlockTrapDoor_getRenderType;
        vtable.getSelectedBoundingBoxFromPool = BlockTrapDoor_getSelectedBoundingBoxFromPool;
        vtable.getCollisionBoundingBoxFromPool = BlockTrapDoor_getCollisionBoundingBoxFromPool;
        vtable.setBlockBoundsBasedOnState = BlockTrapDoor_setBlockBoundsBasedOnState;
        vtable.setBlockBoundsForItemRender = BlockTrapDoor_setBlockBoundsForItemRender;
        vtable.onBlockClicked = BlockTrapDoor_onBlockClicked;
        vtable.blockActivated = BlockTrapDoor_blockActivated;
        vtable.onNeighborBlockChange = BlockTrapDoor_onNeighborBlockChange;
        vtable.collisionRayTrace = BlockTrapDoor_collisionRayTrace;
        vtable.onBlockPlaced = BlockTrapDoor_onBlockPlaced;
        vtable.canPlaceBlockOnSide = BlockTrapDoor_canPlaceBlockOnSide;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;

    Block_constructWithTexture(self, id, 84 + (mat == Material_iron ? 1 : 0), mat);

    float var3 = 0.5f;
    float var4 = 1.0f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, var4, 0.5f + var3);
    return self;
}
