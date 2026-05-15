#include "BlockDispenser.h"
#include "BlockContainer.h"
#include "Material.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include "EntityItem.h"
#include "EntityArrow.h"
#include "EntityEgg.h"
#include "EntitySnowball.h"
#include "TileEntityDispenser.h"
#include "Item.h"
#include "ItemStack.h"
#include "JavaRandom.h"
#include <stdlib.h>

BlockVtable BlockDispenser_defaultVtable;

void BlockDispenser_initVtable(BlockVtable *vt) {
    BlockContainer_initVtable(vt);
    vt->tickRate = BlockDispenser_tickRate;
    vt->idDropped = BlockDispenser_idDropped;
    vt->onBlockAdded = BlockDispenser_onBlockAdded;
    vt->getBlockTexture = BlockDispenser_getBlockTexture;
    vt->getBlockTextureFromSide = BlockDispenser_getBlockTextureFromSide;
    vt->blockActivated = BlockDispenser_blockActivated;
    vt->onNeighborBlockChange = BlockDispenser_onNeighborBlockChange;
    vt->updateTick = BlockDispenser_updateTick;
    vt->onBlockPlacedBy = BlockDispenser_onBlockPlacedBy;
    vt->onBlockRemoval = BlockDispenser_onBlockRemoval;
}

void BlockDispenser_construct(BlockDispenser *self, int id) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockDispenser_initVtable(&BlockDispenser_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockDispenser_defaultVtable;
    BlockContainer_construct(&self->base, id, Material_rock);
    self->base.blockIndexInTexture = 45;
    JavaRandom_init(&self->random, 0);
}

BlockDispenser *BlockDispenser_create(int id) {
    BlockDispenser *self = (BlockDispenser *)calloc(1, sizeof(BlockDispenser));
    BlockDispenser_construct(self, id);
    return self;
}

int BlockDispenser_tickRate(Block *self) {
    (void)self;
    return 4;
}

int BlockDispenser_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return Block_dispenser->blockID;
}

static void setDispenserDefaultDirection(World *var1, int var2, int var3, int var4) {
    if (!var1->isRemote) {
        int var5 = World_getBlockId(var1, var2, var3, var4 - 1);
        int var6 = World_getBlockId(var1, var2, var3, var4 + 1);
        int var7 = World_getBlockId(var1, var2 - 1, var3, var4);
        int var8 = World_getBlockId(var1, var2 + 1, var3, var4);
        int var9 = 3;
        if (Block_opaqueCubeLookup[var5] && !Block_opaqueCubeLookup[var6]) {
            var9 = 3;
        }
        if (Block_opaqueCubeLookup[var6] && !Block_opaqueCubeLookup[var5]) {
            var9 = 2;
        }
        if (Block_opaqueCubeLookup[var7] && !Block_opaqueCubeLookup[var8]) {
            var9 = 5;
        }
        if (Block_opaqueCubeLookup[var8] && !Block_opaqueCubeLookup[var7]) {
            var9 = 4;
        }
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var9);
    }
}

void BlockDispenser_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    Block_defaultVtable.onBlockAdded(self, var1, var2, var3, var4);
    World_setBlockTileEntity(var1, var2, var3, var4, (TileEntity *)TileEntityDispenser_create());
    setDispenserDefaultDirection(var1, var2, var3, var4);
}

int BlockDispenser_getBlockTexture(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                                   int var5) {
    if (var5 == 1) {
        return self->blockIndexInTexture + 17;
    } else if (var5 == 0) {
        return self->blockIndexInTexture + 17;
    } else {
        int var6 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
        return var5 != var6 ? self->blockIndexInTexture : self->blockIndexInTexture + 1;
    }
}

int BlockDispenser_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 1
               ? self->blockIndexInTexture + 17
               : (var1 == 0
                      ? self->blockIndexInTexture + 17
                      : (var1 == 3 ? self->blockIndexInTexture + 1 : self->blockIndexInTexture));
}

int BlockDispenser_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                                  EntityPlayer *var5) {
    (void)self;
    if (var1->isRemote) {
        return 1;
    } else {
        TileEntityDispenser *var6 =
            (TileEntityDispenser *)World_getBlockTileEntity(var1, var2, var3, var4);
        EntityPlayer_displayGUIDispenser(var5, var6);
        return 1;
    }
}

static void dispenseItem(Block *self, World *var1, int var2, int var3, int var4, JavaRandom *var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var9 = 0;
    int var10 = 0;
    if (var6 == 3) {
        var10 = 1;
    } else if (var6 == 2) {
        var10 = -1;
    } else if (var6 == 5) {
        var9 = 1;
    } else {
        var9 = -1;
    }

    TileEntityDispenser *var11 =
        (TileEntityDispenser *)World_getBlockTileEntity(var1, var2, var3, var4);
    ItemStack *var12 = TileEntityDispenser_getRandomStackFromInventory(var11);
    double var13 = (double)var2 + (double)var9 * 0.6 + 0.5;
    double var15 = (double)var3 + 0.5;
    double var17 = (double)var4 + (double)var10 * 0.6 + 0.5;
    if (var12 == NULL) {
        World_func_28106_e(var1, 1001, var2, var3, var4, 0);
    } else {
        if (var12->itemID == Item_arrow->shiftedIndex) {
            EntityArrow *var19 = (EntityArrow *)calloc(1, sizeof(EntityArrow));
            EntityArrow_construct2(var19, var1, var13, var15, var17);
            EntityArrow_setArrowHeading(var19, (double)var9, (double)0.1F, (double)var10, 1.1F,
                                        6.0F);
            var19->doesArrowBelongToPlayer = 1;
            World_entityJoinedWorld(var1, (Entity *)var19);
            World_func_28106_e(var1, 1002, var2, var3, var4, 0);
        } else if (var12->itemID == Item_egg->shiftedIndex) {
            EntityEgg *var22 = (EntityEgg *)calloc(1, sizeof(EntityEgg));
            EntityEgg_construct3(var22, var1, var13, var15, var17);
            EntityEgg_setEggHeading(var22, (double)var9, (double)0.1F, (double)var10, 1.1F, 6.0F);
            World_entityJoinedWorld(var1, (Entity *)var22);
            World_func_28106_e(var1, 1002, var2, var3, var4, 0);
        } else if (var12->itemID == Item_snowball->shiftedIndex) {
            EntitySnowball *var23 = (EntitySnowball *)calloc(1, sizeof(EntitySnowball));
            EntitySnowball_construct3(var23, var1, var13, var15, var17);
            EntitySnowball_setSnowballHeading(var23, (double)var9, (double)0.1F, (double)var10,
                                              1.1F, 6.0F);
            World_entityJoinedWorld(var1, (Entity *)var23);
            World_func_28106_e(var1, 1002, var2, var3, var4, 0);
        } else {
            ItemStack var24item =
                ItemStack_fromItemID(var12->itemID, 1, ItemStack_getItemDamage(var12));
            EntityItem *var24 = EntityItem_create(var1, var13, var15 - 0.3, var17, var24item);
            double var20 = JavaRandom_nextDouble(var5) * 0.1 + 0.2;
            var24->base.motionX = (double)var9 * var20;
            var24->base.motionY = (double)0.2F;
            var24->base.motionZ = (double)var10 * var20;
            var24->base.motionX += JavaRandom_nextGaussian(var5) * (double)0.0075F * 6.0;
            var24->base.motionY += JavaRandom_nextGaussian(var5) * (double)0.0075F * 6.0;
            var24->base.motionZ += JavaRandom_nextGaussian(var5) * (double)0.0075F * 6.0;
            World_entityJoinedWorld(var1, (Entity *)var24);
            World_func_28106_e(var1, 1000, var2, var3, var4, 0);
        }
        World_func_28106_e(var1, 2000, var2, var3, var4, var9 + 1 + (var10 + 1) * 3);
    }
    (void)self;
}

void BlockDispenser_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                          int var5) {
    if (var5 > 0 && Block_blocksList[var5] != NULL &&
        Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5])) {
        int var6 = World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4) ||
                   World_isBlockIndirectlyGettingPowered(var1, var2, var3 + 1, var4);
        if (var6) {
            World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                      self->vtable->tickRate(self));
        }
    }
}

void BlockDispenser_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                               JavaRandom *var5) {
    if (World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4) ||
        World_isBlockIndirectlyGettingPowered(var1, var2, var3 + 1, var4)) {
        dispenseItem(self, var1, var2, var3, var4, var5);
    }
}

void BlockDispenser_onBlockPlacedBy(Block *self, World *var1, int var2, int var3, int var4,
                                    EntityLiving *var5) {
    (void)self;
    int var6 = MathHelper_floor_double((double)(var5->base.rotationYaw * 4.0F / 360.0F) + 0.5) & 3;
    if (var6 == 0) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 2);
    }
    if (var6 == 1) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 5);
    }
    if (var6 == 2) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 3);
    }
    if (var6 == 3) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 4);
    }
}

void BlockDispenser_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    BlockDispenser *bself = (BlockDispenser *)self;
    TileEntityDispenser *var5 =
        (TileEntityDispenser *)World_getBlockTileEntity(var1, var2, var3, var4);
    int var6;
    for (var6 = 0; var6 < DISPENSER_SIZE; ++var6) {
        ItemStack *var7 = TileEntityDispenser_getStackInSlot(var5, var6);
        if (var7 != NULL) {
            float var8 = JavaRandom_nextFloat(&bself->random) * 0.8F + 0.1F;
            float var9 = JavaRandom_nextFloat(&bself->random) * 0.8F + 0.1F;
            float var10 = JavaRandom_nextFloat(&bself->random) * 0.8F + 0.1F;

            while (var7->stackSize > 0) {
                int var11 = JavaRandom_nextInt(&bself->random, 21) + 10;
                if (var11 > var7->stackSize) {
                    var11 = var7->stackSize;
                }
                var7->stackSize -= var11;
                ItemStack var12item =
                    ItemStack_fromItemID(var7->itemID, var11, ItemStack_getItemDamage(var7));
                EntityItem *var12 = EntityItem_create(var1, (double)((float)var2 + var8),
                                                      (double)((float)var3 + var9),
                                                      (double)((float)var4 + var10), var12item);
                float var13 = 0.05F;
                var12->base.motionX =
                    (double)((float)JavaRandom_nextGaussian(&bself->random) * var13);
                var12->base.motionY =
                    (double)((float)JavaRandom_nextGaussian(&bself->random) * var13 + 0.2F);
                var12->base.motionZ =
                    (double)((float)JavaRandom_nextGaussian(&bself->random) * var13);
                World_entityJoinedWorld(var1, (Entity *)var12);
            }
        }
    }

    World_removeBlockTileEntity(var1, var2, var3, var4);
}
