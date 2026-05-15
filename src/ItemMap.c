#include "Item.h"
#include "ItemStack.h"
#include "ItemMap.h"
#include "ItemMapBase.h"
#include "MapData.h"
#include "MapDataBase.h"
#include "World.h"
#include "WorldInfo.h"
#include "WorldProvider.h"
#include "Chunk.h"
#include "Block.h"
#include "Material.h"
#include "MapColor.h"
#include "MathHelper.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static MapDataBase *MapData_factory(const char *name) {
    MapData *md = (MapData *)calloc(1, sizeof(MapData));
    MapData_init(md, name);
    return (MapDataBase *)md;
}

MapData *ItemMap_func_28013_a(short var0, World *var1) {
    char buf[64];
    snprintf(buf, sizeof(buf), "map_%d", (int)var0);
    MapData *var3 = (MapData *)World_loadItemData(var1, MapData_factory, buf);
    if (var3 == NULL) {
        int var4 = World_getUniqueDataId(var1, "map");
        char var2[64];
        snprintf(var2, sizeof(var2), "map_%d", var4);
        var3 = (MapData *)calloc(1, sizeof(MapData));
        MapData_init(var3, var2);
        World_setItemData(var1, var2, (MapDataBase *)var3);
    }
    return var3;
}

MapData *ItemMap_func_28012_a(Item *self, ItemStack *var1, World *var2) {
    (void)self;
    char buf[64];
    snprintf(buf, sizeof(buf), "map_%d", ItemStack_getItemDamage(var1));
    MapData *var4 = (MapData *)World_loadItemData(var2, MapData_factory, buf);
    if (var4 == NULL) {
        ItemStack_setItemDamage(var1, World_getUniqueDataId(var2, "map"));
        char var3[64];
        snprintf(var3, sizeof(var3), "map_%d", ItemStack_getItemDamage(var1));
        var4 = (MapData *)calloc(1, sizeof(MapData));
        MapData_init(var4, var3);
        var4->field_28180_b = WorldInfo_getSpawnX(World_getWorldInfo(var2));
        var4->field_28179_c = WorldInfo_getSpawnZ(World_getWorldInfo(var2));
        var4->field_28177_e = 3;
        var4->field_28178_d = (int8_t)var2->worldProvider->worldType;
        MapDataBase_markDirty((MapDataBase *)var4);
        World_setItemData(var2, var3, (MapDataBase *)var4);
    }
    return var4;
}

static void ItemMap_func_28011_a(Item *self, World *var1, Entity *var2, MapData *var3) {
    (void)self;
    if (var1->worldProvider->worldType == (int)var3->field_28178_d) {
        short var4 = 128;
        short var5 = 128;
        int var6 = 1 << var3->field_28177_e;
        int var7 = var3->field_28180_b;
        int var8 = var3->field_28179_c;
        int var9 = MathHelper_floor_double(var2->posX - (double)var7) / var6 + var4 / 2;
        int var10 = MathHelper_floor_double(var2->posZ - (double)var8) / var6 + var5 / 2;
        int var11 = 128 / var6;
        if (var1->worldProvider->hasNoSky) {
            var11 /= 2;
        }

        ++var3->field_28175_g;

        for (int var12 = var9 - var11 + 1; var12 < var9 + var11; ++var12) {
            if ((var12 & 15) == (var3->field_28175_g & 15)) {
                int var13 = 255;
                int var14 = 0;
                double var15 = 0.0;

                for (int var17 = var10 - var11 - 1; var17 < var10 + var11; ++var17) {
                    if (var12 >= 0 && var17 >= -1 && var12 < var4 && var17 < var5) {
                        int var18 = var12 - var9;
                        int var19 = var17 - var10;
                        int var20 =
                            (var18 * var18 + var19 * var19 > (var11 - 2) * (var11 - 2)) ? 1 : 0;
                        int var21 = (var7 / var6 + var12 - var4 / 2) * var6;
                        int var22 = (var8 / var6 + var17 - var5 / 2) * var6;
                        int8_t var23 = 0;
                        int8_t var24 = 0;
                        int8_t var25 = 0;
                        int var26[256];
                        memset(var26, 0, sizeof(var26));
                        Chunk *var27 = var1->vtable->getChunkFromBlockCoords(var1, var21, var22);
                        int var28 = var21 & 15;
                        int var29 = var22 & 15;
                        int var30 = 0;
                        double var31 = 0.0;
                        int var33;
                        int var34;
                        int var35;
                        int var38;
                        if (var1->worldProvider->hasNoSky) {
                            var33 = var21 + var22 * 231871;
                            var33 = var33 * var33 * 31287121 + var33 * 11;
                            if ((var33 >> 20 & 1) == 0) {
                                var26[Block_dirt->blockID] += 10;
                            } else {
                                var26[Block_stone->blockID] += 10;
                            }

                            var31 = 100.0;
                        } else {
                            for (var33 = 0; var33 < var6; ++var33) {
                                for (var34 = 0; var34 < var6; ++var34) {
                                    var35 =
                                        Chunk_getHeightValue(var27, var33 + var28, var34 + var29) +
                                        1;
                                    int var36 = 0;
                                    if (var35 > 1) {
                                        int var37 = 0;

                                        int label164_break = 0;
                                        while (!label164_break) {
                                            var37 = 1;
                                            var36 = Chunk_getBlockID(var27, var33 + var28,
                                                                     var35 - 1, var34 + var29);
                                            if (var36 == 0) {
                                                var37 = 0;
                                            } else if (var35 > 0 && var36 > 0 &&
                                                       Block_blocksList[var36] &&
                                                       Block_blocksList[var36]
                                                               ->blockMaterial->materialMapColor ==
                                                           MapColor_airColor) {
                                                var37 = 0;
                                            }

                                            if (!var37) {
                                                --var35;
                                                var36 = Chunk_getBlockID(var27, var33 + var28,
                                                                         var35 - 1, var34 + var29);
                                            }

                                            if (var37) {
                                                if (var36 == 0 || !Block_blocksList[var36] ||
                                                    !Material_getIsLiquid(
                                                        Block_blocksList[var36]->blockMaterial)) {
                                                    break;
                                                }

                                                var38 = var35 - 1;

                                                while (1) {
                                                    int var43 =
                                                        Chunk_getBlockID(var27, var33 + var28,
                                                                         var38--, var34 + var29);
                                                    ++var30;
                                                    if (var38 <= 0 || var43 == 0 ||
                                                        !Block_blocksList[var43] ||
                                                        !Material_getIsLiquid(
                                                            Block_blocksList[var43]
                                                                ->blockMaterial)) {
                                                        label164_break = 1;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    var31 += (double)var35 / (double)(var6 * var6);
                                    ++var26[var36];
                                }
                            }
                        }

                        var30 /= var6 * var6;
                        int var10000 = (int)var23 / (var6 * var6);
                        var10000 = (int)var24 / (var6 * var6);
                        var10000 = (int)var25 / (var6 * var6);
                        (void)var10000;
                        var33 = 0;
                        var34 = 0;

                        for (var35 = 0; var35 < 256; ++var35) {
                            if (var26[var35] > var33) {
                                var34 = var35;
                                var33 = var26[var35];
                            }
                        }

                        double var41 = (var31 - var15) * 4.0 / (double)(var6 + 4) +
                                       ((double)((var12 + var17) & 1) - 0.5) * 0.4;
                        int8_t var42 = 1;
                        if (var41 > 0.6) {
                            var42 = 2;
                        }

                        if (var41 < -0.6) {
                            var42 = 0;
                        }

                        var38 = 0;
                        if (var34 > 0 && Block_blocksList[var34]) {
                            MapColor *var44 =
                                Block_blocksList[var34]->blockMaterial->materialMapColor;
                            if (var44 == MapColor_waterColor) {
                                var41 = (double)var30 * 0.1 + (double)((var12 + var17) & 1) * 0.2;
                                var42 = 1;
                                if (var41 < 0.5) {
                                    var42 = 2;
                                }

                                if (var41 > 0.9) {
                                    var42 = 0;
                                }
                            }

                            var38 = var44->colorIndex;
                        }

                        var15 = var31;
                        if (var17 >= 0 && var18 * var18 + var19 * var19 < var11 * var11 &&
                            (!var20 || ((var12 + var17) & 1) != 0)) {
                            int8_t var45 = var3->field_28176_f[var12 + var17 * var4];
                            int8_t var40 = (int8_t)(var38 * 4 + var42);
                            if (var45 != var40) {
                                if (var13 > var17) {
                                    var13 = var17;
                                }

                                if (var14 < var17) {
                                    var14 = var17;
                                }

                                var3->field_28176_f[var12 + var17 * var4] = var40;
                            }
                        }
                    }
                }

                if (var13 <= var14) {
                    MapData_func_28170_a(var3, var12, var13, var14);
                }
            }
        }
    }
}

static void ItemMap_onUpdate(Item *self, ItemStack *var1, World *var2, Entity *var3, int var4,
                             int var5) {
    (void)var4;
    if (!var2->multiplayerWorld) {
        MapData *var6 = ItemMap_func_28012_a(self, var1, var2);
        if (var3->isPlayer) {
            EntityPlayer *var7 = (EntityPlayer *)var3;
            MapData_func_28169_a(var6, var7, var1);
        }

        if (var5) {
            ItemMap_func_28011_a(self, var2, var3, var6);
        }
    }
}

static void ItemMap_onCreated(Item *self, ItemStack *var1, World *var2, EntityPlayer *var3) {
    (void)self;
    ItemStack_setItemDamage(var1, World_getUniqueDataId(var2, "map"));
    char var4[64];
    snprintf(var4, sizeof(var4), "map_%d", ItemStack_getItemDamage(var1));
    MapData *var5 = (MapData *)calloc(1, sizeof(MapData));
    MapData_init(var5, var4);
    World_setItemData(var2, var4, (MapDataBase *)var5);
    var5->field_28180_b = MathHelper_floor_double(((Entity *)var3)->posX);
    var5->field_28179_c = MathHelper_floor_double(((Entity *)var3)->posZ);
    var5->field_28177_e = 3;
    var5->field_28178_d = (int8_t)var2->worldProvider->worldType;
    MapDataBase_markDirty((MapDataBase *)var5);
}

Item *ItemMap_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onUpdate = ItemMap_onUpdate;
        vtable.onCreated = ItemMap_onCreated;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    return self;
}
