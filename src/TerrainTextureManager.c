#include "TerrainTextureManager.h"
#include "IsoImageBuffer.h"
#include "World.h"
#include "Chunk.h"
#include "Block.h"
#include "Material.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../lib/stb_image.h"

TerrainTextureManager *TerrainTextureManager_create(void) {
    int var3;
    TerrainTextureManager *self = (TerrainTextureManager *)calloc(1, sizeof(TerrainTextureManager));

    {
        int w, h, comp;
        unsigned char *img = stbi_load("terrain.png", &w, &h, &comp, 4);
        if (img && w >= 256 && h >= 256) {
            int var2[65536];
            for (int i = 0; i < 256; i++) {
                for (int j = 0; j < 256; j++) {
                    int si = (i * w + j) * 4;
                    int a = (unsigned char)img[si + 3];
                    int r = (unsigned char)img[si + 0];
                    int g = (unsigned char)img[si + 1];
                    int b = (unsigned char)img[si + 2];
                    var2[i * 256 + j] = (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) |
                                              ((unsigned int)g << 8) | (unsigned int)b);
                }
            }
            stbi_image_free(img);
            for (var3 = 0; var3 < 256; ++var3) {
                int var4 = 0, var5 = 0, var6 = 0;
                int var7 = var3 % 16 * 16;
                int var8 = var3 / 16 * 16;
                int var9 = 0;
                for (int var10 = 0; var10 < 16; ++var10) {
                    for (int var11 = 0; var11 < 16; ++var11) {
                        int var12 = var2[var11 + var7 + (var10 + var8) * 256];
                        int var13 = var12 >> 24 & 255;
                        if (var13 > 128) {
                            var4 += var12 >> 16 & 255;
                            var5 += var12 >> 8 & 255;
                            var6 += var12 & 255;
                            ++var9;
                        }
                    }
                    if (var9 == 0) {
                        ++var9;
                    }
                    self->field_1181_a[var3 * 3 + 0] = (float)(var4 / var9);
                    self->field_1181_a[var3 * 3 + 1] = (float)(var5 / var9);
                    self->field_1181_a[var3 * 3 + 2] = (float)(var6 / var9);
                }
            }
        } else if (img) {
            stbi_image_free(img);
        }
    }

    for (var3 = 0; var3 < 256; ++var3) {
        if (Block_blocksList[var3] != NULL) {
            self->field_1182_g[var3 * 3 + 0] =
                Block_blocksList[var3]->vtable->getBlockTextureFromSide(Block_blocksList[var3], 1);
            self->field_1182_g[var3 * 3 + 1] =
                Block_blocksList[var3]->vtable->getBlockTextureFromSide(Block_blocksList[var3], 2);
            self->field_1182_g[var3 * 3 + 2] =
                Block_blocksList[var3]->vtable->getBlockTextureFromSide(Block_blocksList[var3], 3);
        }
    }

    return self;
}

void TerrainTextureManager_destroy(TerrainTextureManager *self) { free(self); }

static void func_800_a(TerrainTextureManager *self) {
    int var1;
    for (var1 = 0; var1 < 32; ++var1) {
        int var2;
        for (var2 = 0; var2 < 160; ++var2) {
            int var3 = var1 + var2 * 32;
            if (self->field_1186_c[var3] == 0) {
                self->field_1180_b[var3] = 0;
            }
            if (self->field_1185_d[var3] > self->field_1186_c[var3]) {
                int var4 = self->field_1180_b[var3] >> 24 & 255;
                self->field_1180_b[var3] =
                    ((self->field_1180_b[var3] & 16711422) >> 1) + self->field_1184_e[var3];
                if (var4 < 128) {

                    self->field_1180_b[var3] = INT_MIN + self->field_1184_e[var3] * 2;
                } else {
                    self->field_1180_b[var3] |= -16777216;
                }
            }
        }
    }
}

void TerrainTextureManager_func_799_a(TerrainTextureManager *self, IsoImageBuffer *var1) {
    World *var2 = var1->worldObj;
    if (var2 == NULL) {
        var1->field_1351_f = 1;
        var1->field_1352_e = 1;
    } else {
        int var3 = var1->field_1354_c * 16;
        int var4 = var1->field_1353_d * 16;
        int var5 = var3 + 16;
        int var6 = var4 + 16;
        Chunk *var7 =
            var2->vtable->getChunkFromChunkCoords(var2, var1->field_1354_c, var1->field_1353_d);
        if (Chunk_func_21167_h(var7)) {
            var1->field_1351_f = 1;
            var1->field_1352_e = 1;
        } else {
            int var8, var9;
            var1->field_1351_f = 0;

            memset(self->field_1186_c, 0, sizeof(self->field_1186_c));

            memset(self->field_1185_d, 0, sizeof(self->field_1185_d));

            {
                int fi;
                for (fi = 0; fi < 34; ++fi)
                    self->field_1183_f[fi] = 160;
            }

            for (var8 = var6 - 1; var8 >= var4; --var8) {
                for (var9 = var5 - 1; var9 >= var3; --var9) {
                    int var10 = var9 - var3;
                    int var11 = var8 - var4;
                    int var12 = var10 + var11;
                    int var13 = 1;
                    int var14;

                    for (var14 = 0; var14 < 128; ++var14) {
                        int var15 = var11 - var10 - var14 + 160 - 16;
                        if (var15 < self->field_1183_f[var12] ||
                            var15 < self->field_1183_f[var12 + 1]) {
                            Block *var16 =
                                Block_blocksList[World_getBlockId(var2, var9, var14, var8)];
                            if (var16 == NULL) {
                                var13 = 0;
                            } else if (var16->blockMaterial == Material_water) {
                                int var24 = World_getBlockId(var2, var9, var14 + 1, var8);
                                if (var24 == 0 || !Block_blocksList[var24] ||
                                    Block_blocksList[var24]->blockMaterial != Material_water) {
                                    float var25 = (float)var14 / 127.0F * 0.6F + 0.4F;
                                    float var26 = (float)var2->vtable->getLightBrightness(
                                                      var2, var9, var14 + 1, var8) *
                                                  var25;
                                    if (var15 >= 0 && var15 < 160) {
                                        int var27 = var12 + var15 * 32;
                                        if (var12 >= 0 && var12 <= 32 &&
                                            self->field_1185_d[var27] <= var14) {
                                            self->field_1185_d[var27] = var14;
                                            self->field_1184_e[var27] = (int)(var26 * 127.0F);
                                        }
                                        if (var12 >= -1 && var12 <= 31 &&
                                            self->field_1185_d[var27 + 1] <= var14) {
                                            self->field_1185_d[var27 + 1] = var14;
                                            self->field_1184_e[var27 + 1] = (int)(var26 * 127.0F);
                                        }
                                        var13 = 0;
                                    }
                                }
                            } else {
                                if (var13) {
                                    if (var15 < self->field_1183_f[var12]) {
                                        self->field_1183_f[var12] = var15;
                                    }
                                    if (var15 < self->field_1183_f[var12 + 1]) {
                                        self->field_1183_f[var12 + 1] = var15;
                                    }
                                }
                                {
                                    float var17 = (float)var14 / 127.0F * 0.6F + 0.4F;
                                    int var18;
                                    int var19;
                                    float var20;
                                    float var22;
                                    if (var15 >= 0 && var15 < 160) {
                                        var18 = var12 + var15 * 32;
                                        var19 = self->field_1182_g[var16->blockID * 3 + 0];
                                        var20 = ((float)var2->vtable->getLightBrightness(
                                                     var2, var9, var14 + 1, var8) *
                                                     0.8F +
                                                 0.2F) *
                                                var17;
                                        if (var12 >= 0 && self->field_1186_c[var18] <= var14) {
                                            self->field_1186_c[var18] = var14;
                                            self->field_1180_b[var18] =
                                                -16777216 |
                                                (int)(self->field_1181_a[var19 * 3 + 0] * var20)
                                                    << 16 |
                                                (int)(self->field_1181_a[var19 * 3 + 1] * var20)
                                                    << 8 |
                                                (int)(self->field_1181_a[var19 * 3 + 2] * var20);
                                        }
                                        if (var12 < 31) {
                                            var22 = var20 * 0.9F;
                                            if (self->field_1186_c[var18 + 1] <= var14) {
                                                self->field_1186_c[var18 + 1] = var14;
                                                self->field_1180_b[var18 + 1] =
                                                    -16777216 |
                                                    (int)(self->field_1181_a[var19 * 3 + 0] * var22)
                                                        << 16 |
                                                    (int)(self->field_1181_a[var19 * 3 + 1] * var22)
                                                        << 8 |
                                                    (int)(self->field_1181_a[var19 * 3 + 2] *
                                                          var22);
                                            }
                                        }
                                    }
                                    if (var15 >= -1 && var15 < 159) {
                                        int var21;
                                        float var23;
                                        var18 = var12 + (var15 + 1) * 32;
                                        var19 = self->field_1182_g[var16->blockID * 3 + 1];
                                        var20 = (float)var2->vtable->getLightBrightness(
                                                    var2, var9 - 1, var14, var8) *
                                                    0.8F +
                                                0.2F;
                                        var21 = self->field_1182_g[var16->blockID * 3 + 2];
                                        var22 = (float)var2->vtable->getLightBrightness(
                                                    var2, var9, var14, var8 + 1) *
                                                    0.8F +
                                                0.2F;
                                        if (var12 >= 0) {
                                            var23 = var20 * var17 * 0.6F;
                                            if (self->field_1186_c[var18] <= var14 - 1) {
                                                self->field_1186_c[var18] = var14 - 1;
                                                self->field_1180_b[var18] =
                                                    -16777216 |
                                                    (int)(self->field_1181_a[var19 * 3 + 0] * var23)
                                                        << 16 |
                                                    (int)(self->field_1181_a[var19 * 3 + 1] * var23)
                                                        << 8 |
                                                    (int)(self->field_1181_a[var19 * 3 + 2] *
                                                          var23);
                                            }
                                        }
                                        if (var12 < 31) {
                                            var23 = var22 * 0.9F * var17 * 0.4F;
                                            if (self->field_1186_c[var18 + 1] <= var14 - 1) {
                                                self->field_1186_c[var18 + 1] = var14 - 1;
                                                self->field_1180_b[var18 + 1] =
                                                    -16777216 |
                                                    (int)(self->field_1181_a[var21 * 3 + 0] * var23)
                                                        << 16 |
                                                    (int)(self->field_1181_a[var21 * 3 + 1] * var23)
                                                        << 8 |
                                                    (int)(self->field_1181_a[var21 * 3 + 2] *
                                                          var23);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            func_800_a(self);

            if (var1->field_1348_a == NULL) {
                var1->field_1348_a = (int *)malloc(32 * 160 * sizeof(int));
            }
            memcpy(var1->field_1348_a, self->field_1180_b, 32 * 160 * sizeof(int));
            var1->field_1352_e = 1;
        }
    }
}
