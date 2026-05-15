#include "MetadataChunkBlock.h"
#include "World.h"
#include "Block.h"
#include "Chunk.h"
#include <stdio.h>

void MetadataChunkBlock_init(MetadataChunkBlock *self, EnumSkyBlock var1, int var2, int var3,
                             int var4, int var5, int var6, int var7) {
    self->field_1299_a = var1;
    self->field_1298_b = var2;
    self->field_1304_c = var3;
    self->field_1303_d = var4;
    self->field_1302_e = var5;
    self->field_1301_f = var6;
    self->field_1300_g = var7;
}

void MetadataChunkBlock_func_4127_a(MetadataChunkBlock *self, World *var1) {
    int var2 = self->field_1302_e - self->field_1298_b + 1;
    int var3 = self->field_1301_f - self->field_1304_c + 1;
    int var4 = self->field_1300_g - self->field_1303_d + 1;
    int var5 = var2 * var3 * var4;
    if (var5 > 32768) {

        printf("Light too large, skipping!\n");
        return;
    }

    int var6 = 0, var7 = 0;
    int var8 = 0;
    int var9 = 0;

    for (int var10 = self->field_1298_b; var10 <= self->field_1302_e; ++var10) {
        for (int var11 = self->field_1303_d; var11 <= self->field_1300_g; ++var11) {
            int var12 = var10 >> 4;
            int var13 = var11 >> 4;
            int var14 = 0;
            if (var8 && var12 == var6 && var13 == var7) {
                var14 = var9;
            } else {
                var14 = World_doChunksNearChunkExist(var1, var10, 0, var11, 1);
                if (var14) {
                    Chunk *var15 =
                        var1->vtable->getChunkFromChunkCoords(var1, var10 >> 4, var11 >> 4);
                    if (Chunk_func_21167_h(var15)) {
                        var14 = 0;
                    }
                }
                var9 = var14;
                var6 = var12;
                var7 = var13;
            }

            if (var14) {
                if (self->field_1304_c < 0)
                    self->field_1304_c = 0;
                if (self->field_1301_f >= 128)
                    self->field_1301_f = 127;

                for (int var27 = self->field_1304_c; var27 <= self->field_1301_f; ++var27) {
                    int var16 =
                        World_getSavedLightValue(var1, self->field_1299_a, var10, var27, var11);
                    int var18 = World_getBlockId(var1, var10, var27, var11);
                    int var19 = Block_lightOpacity[var18];
                    if (var19 == 0)
                        var19 = 1;

                    int var20 = 0;
                    if (self->field_1299_a == EnumSkyBlock_Sky) {
                        if (World_canExistingBlockSeeTheSky(var1, var10, var27, var11)) {
                            var20 = 15;
                        }
                    } else if (self->field_1299_a == EnumSkyBlock_Block) {
                        var20 = Block_lightValue[var18];
                    }

                    int var28;
                    if (var19 >= 15 && var20 == 0) {
                        var28 = 0;
                    } else {
                        int var21 = World_getSavedLightValue(var1, self->field_1299_a, var10 - 1,
                                                             var27, var11);
                        int var22 = World_getSavedLightValue(var1, self->field_1299_a, var10 + 1,
                                                             var27, var11);
                        int var23 = World_getSavedLightValue(var1, self->field_1299_a, var10,
                                                             var27 - 1, var11);
                        int var24 = World_getSavedLightValue(var1, self->field_1299_a, var10,
                                                             var27 + 1, var11);
                        int var25 = World_getSavedLightValue(var1, self->field_1299_a, var10, var27,
                                                             var11 - 1);
                        int var26 = World_getSavedLightValue(var1, self->field_1299_a, var10, var27,
                                                             var11 + 1);
                        var28 = var21;
                        if (var22 > var21)
                            var28 = var22;
                        if (var23 > var28)
                            var28 = var23;
                        if (var24 > var28)
                            var28 = var24;
                        if (var25 > var28)
                            var28 = var25;
                        if (var26 > var28)
                            var28 = var26;
                        var28 -= var19;
                        if (var28 < 0)
                            var28 = 0;
                        if (var20 > var28)
                            var28 = var20;
                    }

                    if (var16 != var28) {
                        World_setLightValue(var1, self->field_1299_a, var10, var27, var11, var28);
                        int var21_b = var28 - 1;
                        if (var21_b < 0)
                            var21_b = 0;

                        World_neighborLightPropagationChanged(var1, self->field_1299_a, var10 - 1,
                                                              var27, var11, var21_b);
                        World_neighborLightPropagationChanged(var1, self->field_1299_a, var10,
                                                              var27 - 1, var11, var21_b);
                        World_neighborLightPropagationChanged(var1, self->field_1299_a, var10,
                                                              var27, var11 - 1, var21_b);
                        if (var10 + 1 >= self->field_1302_e) {
                            World_neighborLightPropagationChanged(var1, self->field_1299_a,
                                                                  var10 + 1, var27, var11, var21_b);
                        }
                        if (var27 + 1 >= self->field_1301_f) {
                            World_neighborLightPropagationChanged(var1, self->field_1299_a, var10,
                                                                  var27 + 1, var11, var21_b);
                        }
                        if (var11 + 1 >= self->field_1300_g) {
                            World_neighborLightPropagationChanged(var1, self->field_1299_a, var10,
                                                                  var27, var11 + 1, var21_b);
                        }
                    }
                }
            }
        }
    }
}

int MetadataChunkBlock_func_866_a(MetadataChunkBlock *self, int var1, int var2, int var3, int var4,
                                  int var5, int var6) {
    if (var1 >= self->field_1298_b && var2 >= self->field_1304_c && var3 >= self->field_1303_d &&
        var4 <= self->field_1302_e && var5 <= self->field_1301_f && var6 <= self->field_1300_g) {
        return 1;
    }

    int var7 = 1;
    if (var1 >= self->field_1298_b - var7 && var2 >= self->field_1304_c - var7 &&
        var3 >= self->field_1303_d - var7 && var4 <= self->field_1302_e + var7 &&
        var5 <= self->field_1301_f + var7 && var6 <= self->field_1300_g + var7) {
        int var8 = self->field_1302_e - self->field_1298_b;
        int var9 = self->field_1301_f - self->field_1304_c;
        int var10 = self->field_1300_g - self->field_1303_d;
        if (var1 > self->field_1298_b)
            var1 = self->field_1298_b;
        if (var2 > self->field_1304_c)
            var2 = self->field_1304_c;
        if (var3 > self->field_1303_d)
            var3 = self->field_1303_d;
        if (var4 < self->field_1302_e)
            var4 = self->field_1302_e;
        if (var5 < self->field_1301_f)
            var5 = self->field_1301_f;
        if (var6 < self->field_1300_g)
            var6 = self->field_1300_g;

        int var11 = var4 - var1;
        int var12 = var5 - var2;
        int var13 = var6 - var3;
        int var14 = var8 * var9 * var10;
        int var15 = var11 * var12 * var13;
        if (var15 - var14 <= 2) {
            self->field_1298_b = var1;
            self->field_1304_c = var2;
            self->field_1303_d = var3;
            self->field_1302_e = var4;
            self->field_1301_f = var5;
            self->field_1300_g = var6;
            return 1;
        }
    }

    return 0;
}
