#include "BlockCake.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

BlockVtable BlockCake_defaultVtable;

void BlockCake_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->setBlockBoundsBasedOnState = BlockCake_setBlockBoundsBasedOnState;
    vt->setBlockBoundsForItemRender = BlockCake_setBlockBoundsForItemRender;
    vt->getCollisionBoundingBoxFromPool = BlockCake_getCollisionBoundingBoxFromPool;
    vt->getSelectedBoundingBoxFromPool = BlockCake_getSelectedBoundingBoxFromPool;
    vt->getBlockTextureFromSideAndMetadata = BlockCake_getBlockTextureFromSideAndMetadata;
    vt->getBlockTextureFromSide = BlockCake_getBlockTextureFromSide;
    vt->renderAsNormalBlock = BlockCake_renderAsNormalBlock;
    vt->isOpaqueCube = BlockCake_isOpaqueCube;
    vt->blockActivated = BlockCake_blockActivated;
    vt->onBlockClicked = BlockCake_onBlockClicked;
    vt->canPlaceBlockAt = BlockCake_canPlaceBlockAt;
    vt->onNeighborBlockChange = BlockCake_onNeighborBlockChange;
    vt->canBlockStay = BlockCake_canBlockStay;
    vt->quantityDropped = BlockCake_quantityDropped;
    vt->idDropped = BlockCake_idDropped;
}

void BlockCake_construct(BlockCake *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockCake_initVtable(&BlockCake_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockCake_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_cakeMaterial);
    Block_setTickOnLoad(&self->base, 1);
}

BlockCake *BlockCake_create(int id, int tex) {
    BlockCake *self = (BlockCake *)calloc(1, sizeof(BlockCake));
    BlockCake_construct(self, id, tex);
    return self;
}

void BlockCake_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    float var6 = 1.0F / 16.0F;
    float var7 = (float)(1 + var5 * 2) / 16.0F;
    float var8 = 0.5F;
    Block_setBlockBounds(self, var7, 0.0F, var6, 1.0F - var6, var8, 1.0F - var6);
}

void BlockCake_setBlockBoundsForItemRender(Block *self) {
    float var1 = 1.0F / 16.0F;
    float var2 = 0.5F;
    Block_setBlockBounds(self, var1, 0.0F, var1, 1.0F - var1, var2, 1.0F - var1);
}

AxisAlignedBB *BlockCake_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                         int var3, int var4) {
    (void)self;
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    float var6 = 1.0F / 16.0F;
    float var7 = (float)(1 + var5 * 2) / 16.0F;
    float var8 = 0.5F;
    return AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var7), (double)var3, (double)((float)var4 + var6),
        (double)((float)(var2 + 1) - var6), (double)((float)var3 + var8 - var6),
        (double)((float)(var4 + 1) - var6));
}

AxisAlignedBB *BlockCake_getSelectedBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                        int var3, int var4) {
    (void)self;
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    float var6 = 1.0F / 16.0F;
    float var7 = (float)(1 + var5 * 2) / 16.0F;
    float var8 = 0.5F;
    return AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var7), (double)var3, (double)((float)var4 + var6),
        (double)((float)(var2 + 1) - var6), (double)((float)var3 + var8),
        (double)((float)(var4 + 1) - var6));
}

int BlockCake_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    return var1 == 1   ? self->blockIndexInTexture
           : var1 == 0 ? self->blockIndexInTexture + 3
                       : (var2 > 0 && var1 == 4 ? self->blockIndexInTexture + 2
                                                : self->blockIndexInTexture + 1);
}

int BlockCake_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 1   ? self->blockIndexInTexture
           : var1 == 0 ? self->blockIndexInTexture + 3
                       : self->blockIndexInTexture + 1;
}

int BlockCake_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockCake_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static void eatCakeSlice(World *var1, int var2, int var3, int var4, EntityPlayer *var5) {
    if (var5->base.health < 20) {
        ((EntityLivingVtable *)var5->base.base.vtable)->heal(&var5->base, 3);
        int var6 = World_getBlockMetadata(var1, var2, var3, var4) + 1;
        if (var6 >= 6) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        } else {
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
            World_markBlockAsNeedsUpdate(var1, var2, var3, var4);
        }
    }
}

int BlockCake_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                             EntityPlayer *var5) {
    (void)self;
    eatCakeSlice(var1, var2, var3, var4, var5);
    return 1;
}

void BlockCake_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                              EntityPlayer *var5) {
    (void)self;
    eatCakeSlice(var1, var2, var3, var4, var5);
}

int BlockCake_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    if (!Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3, var4)) {
        return 0;
    }
    return self->vtable->canBlockStay(self, var1, var2, var3, var4);
}

void BlockCake_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                     int var5) {
    (void)var5;
    if (!self->vtable->canBlockStay(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

int BlockCake_canBlockStay(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return Material_isSolid(World_getBlockMaterial(var1, var2, var3 - 1, var4));
}

int BlockCake_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 0;
}

int BlockCake_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 0;
}
