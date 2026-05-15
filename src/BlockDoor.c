#include "BlockDoor.h"
#include "Material.h"
#include "World.h"
#include "EntityPlayer.h"
#include "Item.h"
#include "Vec3D.h"
#include "JavaRandom.h"
#include <stdlib.h>

BlockVtable BlockDoor_defaultVtable;

void BlockDoor_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->getBlockTextureFromSideAndMetadata = BlockDoor_getBlockTextureFromSideAndMetadata;
    vt->isOpaqueCube = BlockDoor_isOpaqueCube;
    vt->renderAsNormalBlock = BlockDoor_renderAsNormalBlock;
    vt->getRenderType = BlockDoor_getRenderType;
    vt->getSelectedBoundingBoxFromPool = BlockDoor_getSelectedBoundingBoxFromPool;
    vt->getCollisionBoundingBoxFromPool = BlockDoor_getCollisionBoundingBoxFromPool;
    vt->setBlockBoundsBasedOnState = BlockDoor_setBlockBoundsBasedOnState;
    vt->onBlockClicked = BlockDoor_onBlockClicked;
    vt->blockActivated = BlockDoor_blockActivated;
    vt->onNeighborBlockChange = BlockDoor_onNeighborBlockChange;
    vt->idDropped = BlockDoor_idDropped;
    vt->collisionRayTrace = BlockDoor_collisionRayTrace;
    vt->canPlaceBlockAt = BlockDoor_canPlaceBlockAt;
    vt->getMobilityFlag = BlockDoor_getMobilityFlag;
}

void BlockDoor_construct(BlockDoor *self, int id, Material *var2) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockDoor_initVtable(&BlockDoor_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockDoor_defaultVtable;
    Block_construct(&self->base, id, var2);
    self->base.blockIndexInTexture = 97;
    if (var2 == Material_iron) {
        ++self->base.blockIndexInTexture;
    }
    float var3 = 0.5F;
    float var4 = 1.0F;
    Block_setBlockBounds(&self->base, 0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var4,
                         0.5F + var3);
}

BlockDoor *BlockDoor_create(int id, Material *mat) {
    BlockDoor *self = (BlockDoor *)calloc(1, sizeof(BlockDoor));
    BlockDoor_construct(self, id, mat);
    return self;
}

int BlockDoor_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    if (var1 != 0 && var1 != 1) {
        int var3 = BlockDoor_getState(var2);
        if ((var3 == 0 || var3 == 2) ^ (var1 <= 3)) {
            return self->blockIndexInTexture;
        } else {
            int var4 = var3 / 2 + ((var1 & 1) ^ var3);
            var4 += (var2 & 4) / 4;
            int var5 = self->blockIndexInTexture - (var2 & 8) * 2;
            if ((var4 & 1) != 0) {
                var5 = -var5;
            }
            return var5;
        }
    } else {
        return self->blockIndexInTexture;
    }
}

int BlockDoor_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockDoor_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockDoor_getRenderType(Block *self) {
    (void)self;
    return 7;
}

AxisAlignedBB *BlockDoor_getSelectedBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                        int var3, int var4) {
    BlockDoor_setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.getSelectedBoundingBoxFromPool(self, var1, var2, var3, var4);
}

AxisAlignedBB *BlockDoor_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                         int var3, int var4) {
    BlockDoor_setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.getCollisionBoundingBoxFromPool(self, var1, var2, var3, var4);
}

void BlockDoor_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4) {
    BlockDoor_setDoorRotation(
        self, BlockDoor_getState(var1->vtable->getBlockMetadata(var1, var2, var3, var4)));
}

void BlockDoor_setDoorRotation(Block *self, int var1) {
    float var2 = 3.0F / 16.0F;
    Block_setBlockBounds(self, 0.0F, 0.0F, 0.0F, 1.0F, 2.0F, 1.0F);
    if (var1 == 0) {
        Block_setBlockBounds(self, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, var2);
    }
    if (var1 == 1) {
        Block_setBlockBounds(self, 1.0F - var2, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
    }
    if (var1 == 2) {
        Block_setBlockBounds(self, 0.0F, 0.0F, 1.0F - var2, 1.0F, 1.0F, 1.0F);
    }
    if (var1 == 3) {
        Block_setBlockBounds(self, 0.0F, 0.0F, 0.0F, var2, 1.0F, 1.0F);
    }
}

void BlockDoor_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                              EntityPlayer *var5) {
    BlockDoor_blockActivated(self, var1, var2, var3, var4, var5);
}

int BlockDoor_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                             EntityPlayer *var5) {
    if (self->blockMaterial == Material_iron) {
        return 1;
    } else {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if ((var6 & 8) != 0) {
            if (World_getBlockId(var1, var2, var3 - 1, var4) == self->blockID) {
                BlockDoor_blockActivated(self, var1, var2, var3 - 1, var4, var5);
            }
            return 1;
        } else {
            if (World_getBlockId(var1, var2, var3 + 1, var4) == self->blockID) {
                World_setBlockMetadataWithNotify(var1, var2, var3 + 1, var4, (var6 ^ 4) + 8);
            }
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 ^ 4);
            World_markBlocksDirty(var1, var2, var3 - 1, var4, var2, var3, var4);
            World_func_28107_a(var1, var5, 1003, var2, var3, var4, 0);
            return 1;
        }
    }
}

void BlockDoor_onPoweredBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                    int var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var6 & 8) != 0) {
        if (World_getBlockId(var1, var2, var3 - 1, var4) == self->blockID) {
            BlockDoor_onPoweredBlockChange(self, var1, var2, var3 - 1, var4, var5);
        }
    } else {
        int var7 = (World_getBlockMetadata(var1, var2, var3, var4) & 4) > 0;
        if (var7 != var5) {
            if (World_getBlockId(var1, var2, var3 + 1, var4) == self->blockID) {
                World_setBlockMetadataWithNotify(var1, var2, var3 + 1, var4, (var6 ^ 4) + 8);
            }
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 ^ 4);
            World_markBlocksDirty(var1, var2, var3 - 1, var4, var2, var3, var4);
            World_func_28107_a(var1, NULL, 1003, var2, var3, var4, 0);
        }
    }
}

void BlockDoor_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                     int var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    if ((var6 & 8) != 0) {
        if (World_getBlockId(var1, var2, var3 - 1, var4) != self->blockID) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
        if (var5 > 0 && Block_blocksList[var5] != NULL &&
            Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5])) {
            BlockDoor_onNeighborBlockChange(self, var1, var2, var3 - 1, var4, var5);
        }
    } else {
        int var7 = 0;
        if (World_getBlockId(var1, var2, var3 + 1, var4) != self->blockID) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
            var7 = 1;
        }
        if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4)) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
            var7 = 1;
            if (World_getBlockId(var1, var2, var3 + 1, var4) == self->blockID) {
                World_setBlockWithNotify(var1, var2, var3 + 1, var4, 0);
            }
        }
        if (var7) {
            if (!var1->multiplayerWorld) {
                Block_dropBlockAsItem(self, var1, var2, var3, var4, var6);
            }
        } else if (var5 > 0 && Block_blocksList[var5] != NULL &&
                   Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5])) {
            int var8 = World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4) ||
                       World_isBlockIndirectlyGettingPowered(var1, var2, var3 + 1, var4);
            BlockDoor_onPoweredBlockChange(self, var1, var2, var3, var4, var8);
        }
    }
}

int BlockDoor_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)var2;
    return (var1 & 8) != 0 ? 0
                           : (self->blockMaterial == Material_iron ? Item_doorSteel->shiftedIndex
                                                                   : Item_doorWood->shiftedIndex);
}

MovingObjectPosition *BlockDoor_collisionRayTrace(Block *self, World *var1, int var2, int var3,
                                                  int var4, Vec3D *var5, Vec3D *var6) {
    BlockDoor_setBlockBoundsBasedOnState(self, (IBlockAccess *)var1, var2, var3, var4);
    return Block_defaultVtable.collisionRayTrace(self, var1, var2, var3, var4, var5, var6);
}

int BlockDoor_getState(int var1) { return (var1 & 4) == 0 ? (var1 - 1) & 3 : var1 & 3; }

int BlockDoor_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    if (var3 >= 127)
        return 0;
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4) &&
           Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3, var4) &&
           Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3 + 1, var4);
}

int BlockDoor_isOpen(int var0) { return (var0 & 4) != 0; }

int BlockDoor_getMobilityFlag(Block *self) {
    (void)self;
    return 1;
}
