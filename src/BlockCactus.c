#include "BlockCactus.h"
#include "World.h"
#include "Entity.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

BlockVtable BlockCactus_defaultVtable;

void BlockCactus_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->updateTick = BlockCactus_updateTick;
    vt->getCollisionBoundingBoxFromPool = BlockCactus_getCollisionBoundingBoxFromPool;
    vt->getSelectedBoundingBoxFromPool = BlockCactus_getSelectedBoundingBoxFromPool;
    vt->getBlockTextureFromSide = BlockCactus_getBlockTextureFromSide;
    vt->renderAsNormalBlock = BlockCactus_renderAsNormalBlock;
    vt->isOpaqueCube = BlockCactus_isOpaqueCube;
    vt->getRenderType = BlockCactus_getRenderType;
    vt->canPlaceBlockAt = BlockCactus_canPlaceBlockAt;
    vt->onNeighborBlockChange = BlockCactus_onNeighborBlockChange;
    vt->canBlockStay = BlockCactus_canBlockStay;
    vt->onEntityCollidedWithBlock = BlockCactus_onEntityCollidedWithBlock;
}

void BlockCactus_construct(BlockCactus *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockCactus_initVtable(&BlockCactus_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockCactus_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_cactus);
    Block_setTickOnLoad(&self->base, 1);
}

BlockCactus *BlockCactus_create(int id, int tex) {
    BlockCactus *self = (BlockCactus *)calloc(1, sizeof(BlockCactus));
    BlockCactus_construct(self, id, tex);
    return self;
}

void BlockCactus_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                            JavaRandom *var5) {
    (void)var5;
    if (World_isAirBlock(var1, var2, var3 + 1, var4)) {
        int var6;
        for (var6 = 1; World_getBlockId(var1, var2, var3 - var6, var4) == self->blockID; ++var6) {
        }
        if (var6 < 3) {
            int var7 = World_getBlockMetadata(var1, var2, var3, var4);
            if (var7 == 15) {
                World_setBlockWithNotify(var1, var2, var3 + 1, var4, self->blockID);
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, 0);
            } else {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 + 1);
            }
        }
    }
}

AxisAlignedBB *BlockCactus_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                           int var3, int var4) {
    (void)self;
    (void)var1;
    float var5 = 1.0F / 16.0F;
    return AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var5), (double)var3, (double)((float)var4 + var5),
        (double)((float)(var2 + 1) - var5), (double)((float)(var3 + 1) - var5),
        (double)((float)(var4 + 1) - var5));
}

AxisAlignedBB *BlockCactus_getSelectedBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                          int var3, int var4) {
    (void)self;
    (void)var1;
    float var5 = 1.0F / 16.0F;
    return AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var5), (double)var3, (double)((float)var4 + var5),
        (double)((float)(var2 + 1) - var5), (double)(var3 + 1), (double)((float)(var4 + 1) - var5));
}

int BlockCactus_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 1   ? self->blockIndexInTexture - 1
           : var1 == 0 ? self->blockIndexInTexture + 1
                       : self->blockIndexInTexture;
}

int BlockCactus_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockCactus_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockCactus_getRenderType(Block *self) {
    (void)self;
    return 13;
}

int BlockCactus_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    if (!Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3, var4)) {
        return 0;
    }
    return self->vtable->canBlockStay(self, var1, var2, var3, var4);
}

void BlockCactus_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    (void)var5;
    if (!self->vtable->canBlockStay(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

int BlockCactus_canBlockStay(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    if (Material_isSolid(World_getBlockMaterial(var1, var2 - 1, var3, var4))) {
        return 0;
    }
    if (Material_isSolid(World_getBlockMaterial(var1, var2 + 1, var3, var4))) {
        return 0;
    }
    if (Material_isSolid(World_getBlockMaterial(var1, var2, var3, var4 - 1))) {
        return 0;
    }
    if (Material_isSolid(World_getBlockMaterial(var1, var2, var3, var4 + 1))) {
        return 0;
    }
    int var5 = World_getBlockId(var1, var2, var3 - 1, var4);
    return var5 == Block_cactus->blockID || var5 == Block_sand->blockID;
}

void BlockCactus_onEntityCollidedWithBlock(Block *self, World *var1, int var2, int var3, int var4,
                                           Entity *var5) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    var5->vtable->attackEntityFrom(var5, NULL, 1);
}
