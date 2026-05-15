#include "MapGenCavesHell.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>

static void func_4128_a(MapGenCavesHell *self, int var1, int var2, uint8_t *var3, double var4,
                        double var6, double var8, float var10, float var11, float var12, int var13,
                        int var14, double var15) {
    double var17 = (double)(var1 * 16 + 8);
    double var19 = (double)(var2 * 16 + 8);
    float var21 = 0.0f;
    float var22 = 0.0f;
    JavaRandom var23;
    JavaRandom_init(&var23, JavaRandom_nextLong(&self->base.rand));
    if (var14 <= 0) {
        int var24 = self->base.range * 16 - 16;
        var14 = var24 - JavaRandom_nextInt(&var23, var24 / 4);
    }

    int var51 = 0;
    if (var13 == -1) {
        var13 = var14 / 2;
        var51 = 1;
    }

    int var25 = JavaRandom_nextInt(&var23, var14 / 2) + var14 / 4;

    int var26 = (JavaRandom_nextInt(&var23, 6) == 0);
    for (; var13 < var14; ++var13) {
        double var27 = 1.5 + (double)(MathHelper_sin((float)var13 * (float)M_PI / (float)var14) *
                                      var10 * 1.0f);
        double var29 = var27 * var15;
        float var31 = MathHelper_cos(var12);
        float var32 = MathHelper_sin(var12);
        var4 += (double)(MathHelper_cos(var11) * var31);
        var6 += (double)var32;
        var8 += (double)(MathHelper_sin(var11) * var31);
        if (var26) {
            var12 *= 0.92f;
        } else {
            var12 *= 0.7f;
        }

        var12 += var22 * 0.1f;
        var11 += var21 * 0.1f;
        var22 *= 0.9f;
        var21 *= 12.0f / 16.0f;
        var22 += (JavaRandom_nextFloat(&var23) - JavaRandom_nextFloat(&var23)) *
                 JavaRandom_nextFloat(&var23) * 2.0f;
        var21 += (JavaRandom_nextFloat(&var23) - JavaRandom_nextFloat(&var23)) *
                 JavaRandom_nextFloat(&var23) * 4.0f;
        if (!var51 && var13 == var25 && var10 > 1.0f) {
            func_4128_a(self, var1, var2, var3, var4, var6, var8,
                        JavaRandom_nextFloat(&var23) * 0.5f + 0.5f, var11 - (float)M_PI * 0.5f,
                        var12 / 3.0f, var13, var14, 1.0);
            func_4128_a(self, var1, var2, var3, var4, var6, var8,
                        JavaRandom_nextFloat(&var23) * 0.5f + 0.5f, var11 + (float)M_PI * 0.5f,
                        var12 / 3.0f, var13, var14, 1.0);
            return;
        }

        if (var51 || JavaRandom_nextInt(&var23, 4) != 0) {
            double var33 = var4 - var17;
            double var35 = var8 - var19;
            double var37 = (double)(var14 - var13);
            double var39 = (double)(var10 + 2.0f + 16.0f);
            if (var33 * var33 + var35 * var35 - var37 * var37 > var39 * var39) {
                return;
            }

            if (var4 >= var17 - 16.0 - var27 * 2.0 && var8 >= var19 - 16.0 - var27 * 2.0 &&
                var4 <= var17 + 16.0 + var27 * 2.0 && var8 <= var19 + 16.0 + var27 * 2.0) {
                int var52 = MathHelper_floor_double(var4 - var27) - var1 * 16 - 1;
                int var34 = MathHelper_floor_double(var4 + var27) - var1 * 16 + 1;
                int var53 = MathHelper_floor_double(var6 - var29) - 1;
                int var36 = MathHelper_floor_double(var6 + var29) + 1;
                int var54 = MathHelper_floor_double(var8 - var27) - var2 * 16 - 1;
                int var38 = MathHelper_floor_double(var8 + var27) - var2 * 16 + 1;
                if (var52 < 0)
                    var52 = 0;
                if (var34 > 16)
                    var34 = 16;
                if (var53 < 1)
                    var53 = 1;
                if (var36 > 120)
                    var36 = 120;
                if (var54 < 0)
                    var54 = 0;
                if (var38 > 16)
                    var38 = 16;

                int var55 = 0;

                int var40, var43;
                for (var40 = var52; !var55 && var40 < var34; ++var40) {
                    for (int var41 = var54; !var55 && var41 < var38; ++var41) {
                        for (int var42 = var36 + 1; !var55 && var42 >= var53 - 1; --var42) {
                            var43 = (var40 * 16 + var41) * 128 + var42;
                            if (var42 >= 0 && var42 < 128) {
                                if (var3[var43] == (uint8_t)Block_lavaMoving->blockID ||
                                    var3[var43] == (uint8_t)Block_lavaStill->blockID) {
                                    var55 = 1;
                                }
                                if (var42 != var53 - 1 && var40 != var52 && var40 != var34 - 1 &&
                                    var41 != var54 && var41 != var38 - 1) {
                                    var42 = var53;
                                }
                            }
                        }
                    }
                }

                if (!var55) {
                    for (var40 = var52; var40 < var34; ++var40) {
                        double var56 = ((double)(var40 + var1 * 16) + 0.5 - var4) / var27;
                        for (var43 = var54; var43 < var38; ++var43) {
                            double var44 = ((double)(var43 + var2 * 16) + 0.5 - var8) / var27;
                            int var46 = (var40 * 16 + var43) * 128 + var36;
                            for (int var47 = var36 - 1; var47 >= var53; --var47) {
                                double var48 = ((double)var47 + 0.5 - var6) / var29;
                                if (var48 > -0.7 &&
                                    var56 * var56 + var48 * var48 + var44 * var44 < 1.0) {
                                    uint8_t var50 = var3[var46];
                                    if (var50 == (uint8_t)Block_netherrack->blockID ||
                                        var50 == (uint8_t)Block_dirt->blockID ||
                                        var50 == (uint8_t)Block_grass->blockID) {
                                        var3[var46] = 0;
                                    }
                                }
                                --var46;
                            }
                        }
                    }

                    if (var51)
                        break;
                }
            }
        }
    }
}

static void func_4129_a(MapGenCavesHell *self, int var1, int var2, uint8_t *var3, double var4,
                        double var6, double var8) {
    func_4128_a(self, var1, var2, var3, var4, var6, var8,
                1.0f + JavaRandom_nextFloat(&self->base.rand) * 6.0f, 0.0f, 0.0f, -1, -1, 0.5);
}

static void hell_func_868_a(MapGenBase *base, World *world, int srcCX, int srcCZ, int tgtCX,
                            int tgtCZ, uint8_t *chunkBlocks) {
    (void)world;
    MapGenCavesHell *self = (MapGenCavesHell *)base;
    int var7 = JavaRandom_nextInt(
        &self->base.rand,
        JavaRandom_nextInt(&self->base.rand, JavaRandom_nextInt(&self->base.rand, 10) + 1) + 1);
    if (JavaRandom_nextInt(&self->base.rand, 5) != 0) {
        var7 = 0;
    }

    for (int var8 = 0; var8 < var7; ++var8) {
        double var9 = (double)(srcCX * 16 + JavaRandom_nextInt(&self->base.rand, 16));
        double var11 = (double)JavaRandom_nextInt(&self->base.rand, 128);
        double var13 = (double)(srcCZ * 16 + JavaRandom_nextInt(&self->base.rand, 16));
        int var15 = 1;
        if (JavaRandom_nextInt(&self->base.rand, 4) == 0) {
            func_4129_a(self, tgtCX, tgtCZ, chunkBlocks, var9, var11, var13);
            var15 += JavaRandom_nextInt(&self->base.rand, 4);
        }

        for (int var16 = 0; var16 < var15; ++var16) {
            float var17 = JavaRandom_nextFloat(&self->base.rand) * (float)M_PI * 2.0f;
            float var18 = (JavaRandom_nextFloat(&self->base.rand) - 0.5f) * 2.0f / 8.0f;
            float var19 = JavaRandom_nextFloat(&self->base.rand) * 2.0f +
                          JavaRandom_nextFloat(&self->base.rand);
            func_4128_a(self, tgtCX, tgtCZ, chunkBlocks, var9, var11, var13, var19 * 2.0f, var17,
                        var18, 0, 0, 0.5);
        }
    }
}

static MapGenBaseVtable MapGenCavesHell_vtable_def = {
    hell_func_868_a,
};

void MapGenCavesHell_init(MapGenCavesHell *self) {
    self->base.vtable = &MapGenCavesHell_vtable_def;
    self->base.range = 8;
}

MapGenCavesHell *MapGenCavesHell_create(void) {
    MapGenCavesHell *self = (MapGenCavesHell *)calloc(1, sizeof(MapGenCavesHell));
    MapGenCavesHell_init(self);
    return self;
}

void MapGenCavesHell_free(MapGenCavesHell *self) { free(self); }
