#include "Teleporter.h"
#include "World.h"
#include "Entity.h"
#include "Block.h"
#include "MathHelper.h"
#include "Material.h"
#include <stdlib.h>

Teleporter *Teleporter_create(void) {
    Teleporter *self = (Teleporter *)calloc(1, sizeof(Teleporter));

    JavaRandom_init(&self->field_4232_a, 0);
    return self;
}

void Teleporter_destroy(Teleporter *self) { free(self); }

void Teleporter_func_4107_a(Teleporter *self, World *var1, Entity *var2) {
    if (!Teleporter_func_4106_b(self, var1, var2)) {
        Teleporter_func_4108_c(self, var1, var2);
        Teleporter_func_4106_b(self, var1, var2);
    }
}

int Teleporter_func_4106_b(Teleporter *self, World *var1, Entity *var2) {
    short var3 = 128;
    double var4 = -1.0;
    int var6 = 0;
    int var7 = 0;
    int var8 = 0;
    int var9 = MathHelper_floor_double(var2->posX);
    int var10 = MathHelper_floor_double(var2->posZ);
    double var18;
    int var11;
    (void)self;

    for (var11 = var9 - var3; var11 <= var9 + var3; ++var11) {
        double var12 = (double)var11 + 0.5 - var2->posX;
        int var14;

        for (var14 = var10 - var3; var14 <= var10 + var3; ++var14) {
            double var15 = (double)var14 + 0.5 - var2->posZ;
            int var17;

            for (var17 = 127; var17 >= 0; --var17) {
                if (World_getBlockId(var1, var11, var17, var14) == Block_portal->blockID) {
                    while (World_getBlockId(var1, var11, var17 - 1, var14) ==
                           Block_portal->blockID) {
                        --var17;
                    }

                    var18 = (double)var17 + 0.5 - var2->posY;
                    {
                        double var20 = var12 * var12 + var18 * var18 + var15 * var15;
                        if (var4 < 0.0 || var20 < var4) {
                            var4 = var20;
                            var6 = var11;
                            var7 = var17;
                            var8 = var14;
                        }
                    }
                }
            }
        }
    }

    if (var4 >= 0.0) {
        double var22 = (double)var6 + 0.5;
        double var16 = (double)var7 + 0.5;
        var18 = (double)var8 + 0.5;

        if (World_getBlockId(var1, var6 - 1, var7, var8) == Block_portal->blockID) {
            var22 -= 0.5;
        }
        if (World_getBlockId(var1, var6 + 1, var7, var8) == Block_portal->blockID) {
            var22 += 0.5;
        }
        if (World_getBlockId(var1, var6, var7, var8 - 1) == Block_portal->blockID) {
            var18 -= 0.5;
        }
        if (World_getBlockId(var1, var6, var7, var8 + 1) == Block_portal->blockID) {
            var18 += 0.5;
        }

        var2->vtable->setLocationAndAngles(var2, var22, var16, var18, var2->rotationYaw, 0.0F);
        var2->motionX = var2->motionY = var2->motionZ = 0.0;
        return 1;
    } else {
        return 0;
    }
}

int Teleporter_func_4108_c(Teleporter *self, World *var1, Entity *var2) {
    int var3 = 16;
    double var4 = -1.0;
    int var6 = MathHelper_floor_double(var2->posX);
    int var7 = MathHelper_floor_double(var2->posY);
    int var8 = MathHelper_floor_double(var2->posZ);
    int var9 = var6;
    int var10 = var7;
    int var11 = var8;
    int var12 = 0;
    int var13 = JavaRandom_nextInt(&self->field_4232_a, 4);

    int var14;
    double var15;
    int var17;
    double var18;
    int var20;
    int var21;
    int var22;
    int var23;
    int var24;
    int var25;
    int var26;
    int var27;
    int var28;
    double var32;
    double var33;

    for (var14 = var6 - var3; var14 <= var6 + var3; ++var14) {
        var15 = (double)var14 + 0.5 - var2->posX;

        for (var17 = var8 - var3; var17 <= var8 + var3; ++var17) {
            var18 = (double)var17 + 0.5 - var2->posZ;

            for (var20 = 127; var20 >= 0; --var20) {
                if (World_isAirBlock(var1, var14, var20, var17)) {
                    while (var20 > 0 && World_isAirBlock(var1, var14, var20 - 1, var17)) {
                        --var20;
                    }

                    for (var21 = var13; var21 < var13 + 4; ++var21) {
                        var22 = var21 % 2;
                        var23 = 1 - var22;
                        if (var21 % 4 >= 2) {
                            var22 = -var22;
                            var23 = -var23;
                        }

                        for (var24 = 0; var24 < 3; ++var24) {
                            for (var25 = 0; var25 < 4; ++var25) {
                                for (var26 = -1; var26 < 4; ++var26) {
                                    var27 = var14 + (var25 - 1) * var22 + var24 * var23;
                                    var28 = var20 + var26;
                                    {
                                        int var29 = var17 + (var25 - 1) * var23 - var24 * var22;
                                        if ((var26 < 0 &&
                                             !World_getBlockMaterial(var1, var27, var28, var29)
                                                  ->isSolid) ||
                                            (var26 >= 0 &&
                                             !World_isAirBlock(var1, var27, var28, var29))) {

                                            goto label293_continue;
                                        }
                                    }
                                }
                            }
                        }

                        var32 = (double)var20 + 0.5 - var2->posY;
                        var33 = var15 * var15 + var32 * var32 + var18 * var18;
                        if (var4 < 0.0 || var33 < var4) {
                            var4 = var33;
                            var9 = var14;
                            var10 = var20;
                            var11 = var17;
                            var12 = var21 % 4;
                        }
                    }
                }
            label293_continue:;
            }
        }
    }

    if (var4 < 0.0) {
        for (var14 = var6 - var3; var14 <= var6 + var3; ++var14) {
            var15 = (double)var14 + 0.5 - var2->posX;

            for (var17 = var8 - var3; var17 <= var8 + var3; ++var17) {
                var18 = (double)var17 + 0.5 - var2->posZ;

                for (var20 = 127; var20 >= 0; --var20) {
                    if (World_isAirBlock(var1, var14, var20, var17)) {
                        while (World_isAirBlock(var1, var14, var20 - 1, var17)) {
                            --var20;
                        }

                        for (var21 = var13; var21 < var13 + 2; ++var21) {
                            var22 = var21 % 2;
                            var23 = 1 - var22;

                            for (var24 = 0; var24 < 4; ++var24) {
                                for (var25 = -1; var25 < 4; ++var25) {
                                    var26 = var14 + (var24 - 1) * var22;
                                    var27 = var20 + var25;
                                    var28 = var17 + (var24 - 1) * var23;
                                    if ((var25 < 0 &&
                                         !World_getBlockMaterial(var1, var26, var27, var28)
                                              ->isSolid) ||
                                        (var25 >= 0 &&
                                         !World_isAirBlock(var1, var26, var27, var28))) {

                                        goto label231_continue;
                                    }
                                }
                            }

                            var32 = (double)var20 + 0.5 - var2->posY;
                            var33 = var15 * var15 + var32 * var32 + var18 * var18;
                            if (var4 < 0.0 || var33 < var4) {
                                var4 = var33;
                                var9 = var14;
                                var10 = var20;
                                var11 = var17;
                                var12 = var21 % 2;
                            }
                        }
                    }
                label231_continue:;
                }
            }
        }
    }

    {
        int var30 = var9;
        int var16 = var10;
        int var31;
        int var19;
        int var34_bool;

        var17 = var11;
        var31 = var12 % 2;
        var19 = 1 - var31;
        if (var12 % 4 >= 2) {
            var31 = -var31;
            var19 = -var19;
        }

        if (var4 < 0.0) {
            if (var10 < 70)
                var10 = 70;
            if (var10 > 118)
                var10 = 118;
            var16 = var10;

            for (var20 = -1; var20 <= 1; ++var20) {
                for (var21 = 1; var21 < 3; ++var21) {
                    for (var22 = -1; var22 < 3; ++var22) {
                        var23 = var30 + (var21 - 1) * var31 + var20 * var19;
                        var24 = var16 + var22;
                        var25 = var17 + (var21 - 1) * var19 - var20 * var31;
                        var34_bool = var22 < 0;
                        World_setBlockWithNotify(var1, var23, var24, var25,
                                                 var34_bool ? Block_obsidian->blockID : 0);
                    }
                }
            }
        }

        for (var20 = 0; var20 < 4; ++var20) {
            var1->editingBlocks = 1;

            for (var21 = 0; var21 < 4; ++var21) {
                for (var22 = -1; var22 < 4; ++var22) {
                    var23 = var30 + (var21 - 1) * var31;
                    var24 = var16 + var22;
                    var25 = var17 + (var21 - 1) * var19;
                    var34_bool = var21 == 0 || var21 == 3 || var22 == -1 || var22 == 3;
                    World_setBlockWithNotify(var1, var23, var24, var25,
                                             var34_bool ? Block_obsidian->blockID
                                                        : Block_portal->blockID);
                }
            }

            var1->editingBlocks = 0;

            for (var21 = 0; var21 < 4; ++var21) {
                for (var22 = -1; var22 < 4; ++var22) {
                    var23 = var30 + (var21 - 1) * var31;
                    var24 = var16 + var22;
                    var25 = var17 + (var21 - 1) * var19;
                    World_notifyBlocksOfNeighborChange(var1, var23, var24, var25,
                                                       World_getBlockId(var1, var23, var24, var25));
                }
            }
        }
    }

    return 1;
}
