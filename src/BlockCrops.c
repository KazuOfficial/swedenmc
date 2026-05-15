#include "BlockCrops.h"
#include "World.h"
#include "Item.h"
#include "EntityItem.h"
#include "JavaRandom.h"
#include <stdlib.h>

BlockVtable BlockCrops_defaultVtable;

void BlockCrops_initVtable(BlockVtable *vt) {
    BlockFlower_initVtable(vt);
    vt->canThisPlantGrowOnThisBlockID = BlockCrops_canThisPlantGrowOnThisBlockID;
    vt->updateTick = BlockCrops_updateTick;
    vt->getBlockTextureFromSideAndMetadata = BlockCrops_getBlockTextureFromSideAndMetadata;
    vt->getRenderType = BlockCrops_getRenderType;
    vt->dropBlockAsItemWithChance = BlockCrops_dropBlockAsItemWithChance;
    vt->idDropped = BlockCrops_idDropped;
    vt->quantityDropped = BlockCrops_quantityDropped;
}

void BlockCrops_construct(BlockCrops *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockCrops_initVtable(&BlockCrops_defaultVtable);
        vtable_init = 1;
    }

    self->base.vtable = &BlockCrops_defaultVtable;
    Block_construct(&self->base, id, Material_plants);
    self->base.blockIndexInTexture = tex;
    Block_setTickOnLoad(&self->base, 1);
    float var3 = 0.5F;
    Block_setBlockBounds(&self->base, 0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, 0.25F,
                         0.5F + var3);
}

BlockCrops *BlockCrops_create(int id, int tex) {
    BlockCrops *self = (BlockCrops *)calloc(1, sizeof(BlockCrops));
    BlockCrops_construct(self, id, tex);
    return self;
}

int BlockCrops_canThisPlantGrowOnThisBlockID(Block *self, int var1) {
    (void)self;
    return var1 == Block_tilledField->blockID;
}

static float getGrowthRate(Block *self, World *var1, int var2, int var3, int var4) {
    float var5 = 1.0F;
    int var6 = World_getBlockId(var1, var2, var3, var4 - 1);
    int var7 = World_getBlockId(var1, var2, var3, var4 + 1);
    int var8 = World_getBlockId(var1, var2 - 1, var3, var4);
    int var9 = World_getBlockId(var1, var2 + 1, var3, var4);
    int var10 = World_getBlockId(var1, var2 - 1, var3, var4 - 1);
    int var11 = World_getBlockId(var1, var2 + 1, var3, var4 - 1);
    int var12 = World_getBlockId(var1, var2 + 1, var3, var4 + 1);
    int var13 = World_getBlockId(var1, var2 - 1, var3, var4 + 1);
    int var14 = var8 == self->blockID || var9 == self->blockID;
    int var15 = var6 == self->blockID || var7 == self->blockID;
    int var16 = var10 == self->blockID || var11 == self->blockID || var12 == self->blockID ||
                var13 == self->blockID;

    for (int var17 = var2 - 1; var17 <= var2 + 1; ++var17) {
        for (int var18 = var4 - 1; var18 <= var4 + 1; ++var18) {
            int var19 = World_getBlockId(var1, var17, var3 - 1, var18);
            float var20 = 0.0F;
            if (var19 == Block_tilledField->blockID) {
                var20 = 1.0F;
                if (World_getBlockMetadata(var1, var17, var3 - 1, var18) > 0) {
                    var20 = 3.0F;
                }
            }
            if (var17 != var2 || var18 != var4) {
                var20 /= 4.0F;
            }
            var5 += var20;
        }
    }

    if (var16 || (var14 && var15)) {
        var5 /= 2.0F;
    }

    return var5;
}

void BlockCrops_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                           JavaRandom *var5) {
    BlockFlower_updateTick(self, var1, var2, var3, var4, var5);
    if (World_getBlockLightValue(var1, var2, var3 + 1, var4) >= 9) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if (var6 < 7) {
            float var7 = getGrowthRate(self, var1, var2, var3, var4);
            if (JavaRandom_nextInt(var5, (int)(100.0F / var7)) == 0) {
                ++var6;
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
            }
        }
    }
}

void BlockCrops_fertilize(BlockCrops *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, 7);
}

int BlockCrops_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    (void)var1;
    if (var2 < 0) {
        var2 = 7;
    }
    return self->blockIndexInTexture + var2;
}

int BlockCrops_getRenderType(Block *self) {
    (void)self;
    return 6;
}

void BlockCrops_dropBlockAsItemWithChance(Block *self, World *var1, int var2, int var3, int var4,
                                          int var5, float var6) {
    Block_defaultVtable.dropBlockAsItemWithChance(self, var1, var2, var3, var4, var5, var6);
    if (!var1->multiplayerWorld) {
        for (int var7 = 0; var7 < 3; ++var7) {
            if (JavaRandom_nextInt(&var1->rand, 15) <= var5) {
                float var8 = 0.7F;
                float var9 = JavaRandom_nextFloat(&var1->rand) * var8 + (1.0F - var8) * 0.5F;
                float var10 = JavaRandom_nextFloat(&var1->rand) * var8 + (1.0F - var8) * 0.5F;
                float var11 = JavaRandom_nextFloat(&var1->rand) * var8 + (1.0F - var8) * 0.5F;
                ItemStack stack = ItemStack_fromItemID(Item_seeds->shiftedIndex, 1, 0);
                EntityItem *var12 = EntityItem_create(var1, (double)((float)var2 + var9),
                                                      (double)((float)var3 + var10),
                                                      (double)((float)var4 + var11), stack);
                var12->delayBeforeCanPickup = 10;
                World_entityJoinedWorld(var1, (Entity *)var12);
            }
        }
    }
}

int BlockCrops_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var2;
    return var1 == 7 ? Item_wheat->shiftedIndex : -1;
}

int BlockCrops_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 1;
}
