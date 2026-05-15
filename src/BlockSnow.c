#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityItem.h"
#include "Item.h"
#include "ItemStack.h"
#include "StatList.h"
#include "AxisAlignedBB.h"
#include "IBlockAccess.h"
#include "EnumSkyBlock.h"
#include <stdlib.h>

static int BlockSnow_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static int BlockSnow_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

static AxisAlignedBB *BlockSnow_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                                int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4) & 7;
    if (var5 >= 3) {
        return AxisAlignedBB_getBoundingBoxFromPool(
            (double)var2 + self->minX, (double)var3 + self->minY, (double)var4 + self->minZ,
            (double)var2 + self->maxX, (double)var3 + 0.5, (double)var4 + self->maxZ);
    }
    return NULL;
}

static void BlockSnow_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                 int var3, int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 7;
    float var6 = (float)(2 * (1 + var5)) / 16.0f;
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, var6, 1.0f);
}

static int BlockSnow_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    int var5 = World_getBlockId(var1, var2, var3 - 1, var4);
    if (var5 == 0 || !Block_blocksList[var5])
        return 0;
    if (!Block_blocksList[var5]->vtable->isOpaqueCube(Block_blocksList[var5]))
        return 0;
    return Material_getIsSolid(World_getBlockMaterial(var1, var2, var3 - 1, var4));
}

static int BlockSnow_func_314_h(Block *self, World *var1, int var2, int var3, int var4) {
    if (!BlockSnow_canPlaceBlockAt(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
        return 0;
    }
    return 1;
}

static void BlockSnow_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                            int var5) {
    (void)var5;
    BlockSnow_func_314_h(self, var1, var2, var3, var4);
}

static void BlockSnow_harvestBlock(Block *self, World *var1, EntityPlayer *var2, int var3, int var4,
                                   int var5, int var6) {
    (void)var6;
    int var7 = Item_snowball->shiftedIndex;
    float var8 = 0.7f;
    double var9 = (double)(JavaRandom_nextFloat(&var1->rand) * var8) + (double)(1.0f - var8) * 0.5;
    double var11 = (double)(JavaRandom_nextFloat(&var1->rand) * var8) + (double)(1.0f - var8) * 0.5;
    double var13 = (double)(JavaRandom_nextFloat(&var1->rand) * var8) + (double)(1.0f - var8) * 0.5;
    ItemStack is = ItemStack_fromItemID(var7, 1, 0);
    EntityItem *var15 = EntityItem_create(var1, (double)var3 + var9, (double)var4 + var11,
                                          (double)var5 + var13, is);
    var15->delayBeforeCanPickup = 10;
    World_entityJoinedWorld(var1, (Entity *)var15);
    World_setBlockWithNotify(var1, var3, var4, var5, 0);
    EntityPlayer_addStat(var2, StatList_mineBlockStatArray[self->blockID], 1);
}

static int BlockSnow_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Item_snowball->shiftedIndex;
}

static int BlockSnow_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}

static void BlockSnow_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                 JavaRandom *rand) {
    (void)rand;
    if (World_getSavedLightValue(var1, EnumSkyBlock_Block, var2, var3, var4) > 11) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

static int BlockSnow_shouldSideBeRendered(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4, int var5) {
    if (var5 == 1)
        return 1;
    return Block_defaultVtable.shouldSideBeRendered(self, var1, var2, var3, var4, var5);
}

Block *BlockSnow_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockSnow_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockSnow_renderAsNormalBlock;
        vtable.getCollisionBoundingBoxFromPool = BlockSnow_getCollisionBoundingBoxFromPool;
        vtable.setBlockBoundsBasedOnState = BlockSnow_setBlockBoundsBasedOnState;
        vtable.canPlaceBlockAt = BlockSnow_canPlaceBlockAt;
        vtable.onNeighborBlockChange = BlockSnow_onNeighborBlockChange;
        vtable.harvestBlock = BlockSnow_harvestBlock;
        vtable.idDropped = BlockSnow_idDropped;
        vtable.quantityDropped = BlockSnow_quantityDropped;
        vtable.updateTick = BlockSnow_updateTick;
        vtable.shouldSideBeRendered = BlockSnow_shouldSideBeRendered;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_snow);
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
    Block_setTickOnLoad(self, 1);
    return self;
}
