#include "WorldGenBigTree.h"
#include "Block.h"
#include "World.h"
#include "MathHelper.h"
#include "JavaRandom.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static float func_528_a(WorldGenBigTree *self, int var1);
static int func_524_a(WorldGenBigTree *self, int *var1, int *var2);

const int WorldGenBigTree_field_882_a[6] = {2, 0, 0, 1, 2, 1};

static void func_521_a(WorldGenBigTree *self) {
    self->height = (int)((double)self->field_878_e * self->field_876_g);
    if (self->height >= self->field_878_e)
        self->height = self->field_878_e - 1;

    int var1 = (int)(1.382 + pow(self->field_872_k * (double)self->field_878_e / 13.0, 2.0));
    if (var1 < 1)
        var1 = 1;

    int rows = var1 * self->field_878_e;
    int (*var2)[4] = (int (*)[4])calloc(rows, sizeof(int[4]));

    int var3 = self->basePos[1] + self->field_878_e - self->field_869_n;
    int var4 = 1;
    int var5 = self->basePos[1] + self->height;
    int var6 = var3 - self->basePos[1];

    var2[0][0] = self->basePos[0];
    var2[0][1] = var3;
    var2[0][2] = self->basePos[2];
    var2[0][3] = var5;
    --var3;

    while (1) {
        while (var6 >= 0) {
            int var7 = 0;
            float var8 = func_528_a(self, var6);

            if (var8 < 0.0f) {
                --var3;
                --var6;
            } else {
                for (double var9 = 0.5; var7 < var1; ++var7) {
                    double var11 = self->field_873_j * (double)var8 *
                                   ((double)JavaRandom_nextFloat(&self->field_881_b) + 0.328);
                    double var13 = (double)JavaRandom_nextFloat(&self->field_881_b) * 2.0 * 3.14159;
                    int var15 = MathHelper_floor_double(var11 * sin(var13) +
                                                        (double)self->basePos[0] + var9);
                    int var16 = MathHelper_floor_double(var11 * cos(var13) +
                                                        (double)self->basePos[2] + var9);

                    int var17[3] = {var15, var3, var16};
                    int var18[3] = {var15, var3 + self->field_869_n, var16};

                    if (func_524_a(self, var17, var18) == -1) {
                        int var19[3] = {self->basePos[0], self->basePos[1], self->basePos[2]};
                        double var20 = sqrt(pow((double)abs(self->basePos[0] - var17[0]), 2.0) +
                                            pow((double)abs(self->basePos[2] - var17[2]), 2.0));
                        double var22 = var20 * self->field_874_i;
                        if ((double)var17[1] - var22 > (double)var5)
                            var19[1] = var5;
                        else
                            var19[1] = (int)((double)var17[1] - var22);

                        if (func_524_a(self, var19, var17) == -1 && var4 < rows) {
                            var2[var4][0] = var15;
                            var2[var4][1] = var3;
                            var2[var4][2] = var16;
                            var2[var4][3] = var19[1];
                            ++var4;
                        }
                    }
                }
                --var3;
                --var6;
            }
        }

        if (self->field_868_o)
            free(self->field_868_o);
        self->field_868_o = (int (*)[4])calloc(var4, sizeof(int[4]));
        memcpy(self->field_868_o, var2, var4 * sizeof(int[4]));
        self->field_868_o_count = var4;
        free(var2);
        return;
    }
}

static void func_523_a(WorldGenBigTree *self, int var1, int var2, int var3, float var4, int var5,
                       int var6) {
    int var7 = (int)((double)var4 + 0.618);
    int var8 = WorldGenBigTree_field_882_a[var5];
    int var9 = WorldGenBigTree_field_882_a[var5 + 3];
    int var10[3] = {var1, var2, var3};
    int var11[3] = {0, 0, 0};
    int var12 = -var7;
    int var13 = -var7;

    for (var11[var5] = var10[var5]; var12 <= var7; ++var12) {
        var11[var8] = var10[var8] + var12;
        var13 = -var7;
        for (; var13 <= var7; ++var13) {
            double var15 =
                sqrt(pow((double)abs(var12) + 0.5, 2.0) + pow((double)abs(var13) + 0.5, 2.0));
            if (var15 > (double)var4)
                continue;
            var11[var9] = var10[var9] + var13;
            int var14 = World_getBlockId(self->worldObj, var11[0], var11[1], var11[2]);
            if (var14 != 0 && var14 != 18)
                continue;
            World_setBlock(self->worldObj, var11[0], var11[1], var11[2], var6);
        }
    }
}

static float func_528_a(WorldGenBigTree *self, int var1) {
    if ((double)var1 < (double)((float)self->field_878_e) * 0.3)
        return -1.618f;
    float var2 = (float)self->field_878_e / 2.0f;
    float var3 = (float)self->field_878_e / 2.0f - (float)var1;
    float var4;
    if (var3 == 0.0f) {
        var4 = var2;
    } else if (fabsf(var3) >= var2) {
        var4 = 0.0f;
    } else {
        var4 = (float)sqrt(pow((double)fabsf(var2), 2.0) - pow((double)fabsf(var3), 2.0));
    }
    var4 *= 0.5f;
    return var4;
}

static float func_526_b(WorldGenBigTree *self, int var1) {
    (void)self;
    if (var1 < 0 || var1 >= self->field_869_n)
        return -1.0f;
    return (var1 != 0 && var1 != self->field_869_n - 1) ? 3.0f : 2.0f;
}

static void func_520_a(WorldGenBigTree *self, int var1, int var2, int var3) {
    int var4 = var2;
    for (int var5 = var2 + self->field_869_n; var4 < var5; ++var4) {
        float var6 = func_526_b(self, var4 - var2);
        func_523_a(self, var1, var4, var3, var6, 1, 18);
    }
}

static void func_522_a(WorldGenBigTree *self, int *var1, int *var2, int var3) {
    int var4[3] = {0, 0, 0};
    int var5 = 0;
    int var6;
    for (var6 = 0; var5 < 3; ++var5) {
        var4[var5] = var2[var5] - var1[var5];
        if (abs(var4[var5]) > abs(var4[var6]))
            var6 = var5;
    }

    if (var4[var6] != 0) {
        int var7 = WorldGenBigTree_field_882_a[var6];
        int var8 = WorldGenBigTree_field_882_a[var6 + 3];
        int var9 = var4[var6] > 0 ? 1 : -1;
        double var10 = (double)var4[var7] / (double)var4[var6];
        double var12 = (double)var4[var8] / (double)var4[var6];
        int var14[3] = {0, 0, 0};
        int var15 = 0;
        for (int var16 = var4[var6] + var9; var15 != var16; var15 += var9) {
            var14[var6] = MathHelper_floor_double((double)(var1[var6] + var15) + 0.5);
            var14[var7] = MathHelper_floor_double((double)var1[var7] + (double)var15 * var10 + 0.5);
            var14[var8] = MathHelper_floor_double((double)var1[var8] + (double)var15 * var12 + 0.5);
            World_setBlock(self->worldObj, var14[0], var14[1], var14[2], var3);
        }
    }
}

static void func_518_b(WorldGenBigTree *self) {
    int var1 = 0;
    for (int var2 = self->field_868_o_count; var1 < var2; ++var1) {
        int var3 = self->field_868_o[var1][0];
        int var4 = self->field_868_o[var1][1];
        int var5 = self->field_868_o[var1][2];
        func_520_a(self, var3, var4, var5);
    }
}

static int func_527_c(WorldGenBigTree *self, int var1) {
    return (double)var1 >= (double)self->field_878_e * 0.2;
}

static void func_529_c(WorldGenBigTree *self) {
    int var1 = self->basePos[0];
    int var2 = self->basePos[1];
    int var3 = self->basePos[1] + self->height;
    int var4 = self->basePos[2];
    int var5[3] = {var1, var2, var4};
    int var6[3] = {var1, var3, var4};
    func_522_a(self, var5, var6, 17);

    if (self->field_871_l == 2) {
        ++var5[0];
        ++var6[0];
        func_522_a(self, var5, var6, 17);
        ++var5[2];
        ++var6[2];
        func_522_a(self, var5, var6, 17);
        var5[0] += -1;
        var6[0] += -1;
        func_522_a(self, var5, var6, 17);
    }
}

static void func_525_d(WorldGenBigTree *self) {
    int var1 = 0;
    int var2 = self->field_868_o_count;
    int var3[3] = {self->basePos[0], self->basePos[1], self->basePos[2]};
    for (; var1 < var2; ++var1) {
        int *var4 = self->field_868_o[var1];
        int var5[3] = {var4[0], var4[1], var4[2]};
        var3[1] = var4[3];
        int var6 = var3[1] - self->basePos[1];
        if (func_527_c(self, var6))
            func_522_a(self, var3, var5, 17);
    }
}

static int func_524_a(WorldGenBigTree *self, int *var1, int *var2) {
    int var3[3] = {0, 0, 0};
    int var4 = 0;
    int var5;
    for (var5 = 0; var4 < 3; ++var4) {
        var3[var4] = var2[var4] - var1[var4];
        if (abs(var3[var4]) > abs(var3[var5]))
            var5 = var4;
    }

    if (var3[var5] == 0) {
        return -1;
    } else {
        int var6 = WorldGenBigTree_field_882_a[var5];
        int var7 = WorldGenBigTree_field_882_a[var5 + 3];
        int var8 = var3[var5] > 0 ? 1 : -1;
        double var9 = (double)var3[var6] / (double)var3[var5];
        double var11 = (double)var3[var7] / (double)var3[var5];
        int var13[3] = {0, 0, 0};
        int var14 = 0;
        int var15;
        for (var15 = var3[var5] + var8; var14 != var15; var14 += var8) {
            var13[var5] = var1[var5] + var14;
            var13[var6] = MathHelper_floor_double((double)var1[var6] + (double)var14 * var9);
            var13[var7] = MathHelper_floor_double((double)var1[var7] + (double)var14 * var11);
            int var16 = World_getBlockId(self->worldObj, var13[0], var13[1], var13[2]);
            if (var16 != 0 && var16 != 18)
                break;
        }
        return var14 == var15 ? -1 : abs(var14);
    }
}

static int func_519_e(WorldGenBigTree *self) {
    int var1[3] = {self->basePos[0], self->basePos[1], self->basePos[2]};
    int var2[3] = {self->basePos[0], self->basePos[1] + self->field_878_e - 1, self->basePos[2]};
    int var3 =
        World_getBlockId(self->worldObj, self->basePos[0], self->basePos[1] - 1, self->basePos[2]);
    if (var3 != 2 && var3 != 3)
        return 0;

    int var4 = func_524_a(self, var1, var2);
    if (var4 == -1) {
        return 1;
    } else if (var4 < 6) {
        return 0;
    } else {
        self->field_878_e = var4;
        return 1;
    }
}

static void WorldGenBigTree_func_517_a(WorldGenerator *base, double var1, double var3,
                                       double var5) {
    WorldGenBigTree *self = (WorldGenBigTree *)base;
    self->field_870_m = (int)(var1 * 12.0);
    if (var1 > 0.5)
        self->field_869_n = 5;
    self->field_873_j = var3;
    self->field_872_k = var5;
}

static int WorldGenBigTree_generate(WorldGenerator *base, World *var1, JavaRandom *var2, int var3,
                                    int var4, int var5) {
    WorldGenBigTree *self = (WorldGenBigTree *)base;
    self->worldObj = var1;
    int64_t var6 = JavaRandom_nextLong(var2);
    JavaRandom_setSeed(&self->field_881_b, var6);
    self->basePos[0] = var3;
    self->basePos[1] = var4;
    self->basePos[2] = var5;

    if (self->field_878_e == 0)
        self->field_878_e = 5 + JavaRandom_nextInt(&self->field_881_b, self->field_870_m);

    if (!func_519_e(self))
        return 0;
    func_521_a(self);
    func_518_b(self);
    func_529_c(self);
    func_525_d(self);
    return 1;
}

static void WorldGenBigTree_destroy(WorldGenerator *base) {
    WorldGenBigTree *self = (WorldGenBigTree *)base;
    if (self->field_868_o)
        free(self->field_868_o);
    free(self);
}

static WorldGeneratorVtable WorldGenBigTree_vtable_inst = {
    WorldGenBigTree_generate, WorldGenBigTree_func_517_a, WorldGenBigTree_destroy};

WorldGenBigTree *WorldGenBigTree_create(void) {
    WorldGenBigTree *self = (WorldGenBigTree *)calloc(1, sizeof(WorldGenBigTree));
    self->base.vtable = &WorldGenBigTree_vtable_inst;
    JavaRandom_init(&self->field_881_b, 0);
    self->field_876_g = 0.618;
    self->field_875_h = 1.0;
    self->field_874_i = 0.381;
    self->field_873_j = 1.0;
    self->field_872_k = 1.0;
    self->field_871_l = 1;
    self->field_870_m = 12;
    self->field_869_n = 4;
    self->field_878_e = 0;
    return self;
}
