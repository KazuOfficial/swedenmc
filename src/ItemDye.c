#include "ItemDye.h"
#include "ItemStack.h"
#include "Block.h"
#include "BlockCrops.h"
#include "BlockCloth.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include "EntitySheep.h"
#include "Entity.h"
#include <stdlib.h>

const char *ItemDye_dyeColors[16] = {"black",     "red",     "green",  "brown", "blue", "purple",
                                     "cyan",      "silver",  "gray",   "pink",  "lime", "yellow",
                                     "lightBlue", "magenta", "orange", "white"};

const int ItemDye_field_31002_bk[16] = {1973019, 11743532, 3887386,  5320730,  2437522, 8073150,
                                        2651799, 2651799,  4408131,  14188952, 4312372, 14602026,
                                        6719955, 12801229, 15435844, 15790320};

static int ItemDye_getIconFromDamage(Item *self, int var1) {
    return self->iconIndex + var1 % 8 * 16 + var1 / 8;
}

static const char *ItemDye_getItemNameIS(Item *self, ItemStack *var1) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "%s.%s", self->vtable->getItemName(self),
             ItemDye_dyeColors[var1->itemDamage]);
    return buf;
}

static int ItemDye_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3, int var4,
                             int var5, int var6, int var7) {
    (void)self;
    (void)var2;
    (void)var7;
    if (var1->itemDamage == 15) {
        int var8 = World_getBlockId(var3, var4, var5, var6);
        if (var8 == Block_sapling->blockID) {
            if (!var3->multiplayerWorld) {
                BlockSapling_growTree(Block_sapling, var3, var4, var5, var6, &var3->rand);
                --var1->stackSize;
            }
            return 1;
        }

        if (var8 == Block_crops->blockID) {
            if (!var3->multiplayerWorld) {
                BlockCrops_fertilize((BlockCrops *)Block_crops, var3, var4, var5, var6);
                --var1->stackSize;
            }
            return 1;
        }

        if (var8 == Block_grass->blockID) {
            if (!var3->multiplayerWorld) {
                --var1->stackSize;

                for (int var9 = 0; var9 < 128; ++var9) {
                    int var10 = var4;
                    int var11 = var5 + 1;
                    int var12 = var6;
                    int var13;
                    int skip = 0;

                    for (var13 = 0; var13 < var9 / 16; ++var13) {
                        var10 += JavaRandom_nextInt(&Item_itemRand, 3) - 1;
                        var11 += (JavaRandom_nextInt(&Item_itemRand, 3) - 1) *
                                 JavaRandom_nextInt(&Item_itemRand, 3) / 2;
                        var12 += JavaRandom_nextInt(&Item_itemRand, 3) - 1;
                        if (World_getBlockId(var3, var10, var11 - 1, var12) !=
                                Block_grass->blockID ||
                            World_isBlockNormalCube(var3, var10, var11, var12)) {
                            skip = 1;
                            break;
                        }
                    }

                    if (skip) {
                        continue;
                    }

                    if (World_getBlockId(var3, var10, var11, var12) == 0) {
                        if (JavaRandom_nextInt(&Item_itemRand, 10) != 0) {
                            World_setBlockAndMetadataWithNotify(var3, var10, var11, var12,
                                                                Block_tallGrass->blockID, 1);
                        } else if (JavaRandom_nextInt(&Item_itemRand, 3) != 0) {
                            World_setBlockWithNotify(var3, var10, var11, var12,
                                                     Block_plantYellow->blockID);
                        } else {
                            World_setBlockWithNotify(var3, var10, var11, var12,
                                                     Block_plantRed->blockID);
                        }
                    }
                }
            }
            return 1;
        }
    }

    return 0;
}

static void ItemDye_saddleEntity(Item *self, ItemStack *var1, EntityLiving *var2) {
    (void)self;
    if (((Entity *)var2)->isSheep) {
        EntitySheep *var3 = (EntitySheep *)var2;
        int var4 = BlockCloth_func_21034_c(var1->itemDamage);
        if (!EntitySheep_getSheared(var3) && EntitySheep_getFleeceColor(var3) != var4) {
            EntitySheep_setFleeceColor(var3, var4);
            --var1->stackSize;
        }
    }
}

Item *ItemDye_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.getIconFromDamage = ItemDye_getIconFromDamage;
        vtable.getItemNameIS = ItemDye_getItemNameIS;
        vtable.onItemUse = ItemDye_onItemUse;
        vtable.saddleEntity = ItemDye_saddleEntity;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    Item_setHasSubtypes(self, 1);
    Item_setMaxDamage(self, 0);
    return self;
}
