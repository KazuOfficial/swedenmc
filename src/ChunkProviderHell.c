#include "ChunkProviderHell.h"
#include "Chunk.h"
#include "Block.h"
#include "WorldGenHellLava.h"
#include "WorldGenFire.h"
#include "WorldGenGlowStone1.h"
#include "WorldGenGlowStone2.h"
#include "WorldGenFlowers.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int cph_chunkExists(IChunkProvider *self, int x, int z) {
    (void)self;
    (void)x;
    (void)z;

    return 1;
}
static Chunk *cph_provideChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderHell_provideChunk((ChunkProviderHell *)self, x, z);
}
static Chunk *cph_prepareChunk(IChunkProvider *self, int x, int z) {

    return ChunkProviderHell_provideChunk((ChunkProviderHell *)self, x, z);
}
static void cph_populate(IChunkProvider *self, IChunkProvider *chunkProvider, int x, int z) {
    (void)chunkProvider;
    ChunkProviderHell_populate((ChunkProviderHell *)self, x, z);
}
static int cph_saveChunks(IChunkProvider *self, int saveAll, IProgressUpdate *progress) {
    (void)self;
    (void)saveAll;
    (void)progress;

    return 1;
}
static int cph_unload100OldestChunks(IChunkProvider *self) {
    (void)self;

    return 0;
}
static int cph_canSave(IChunkProvider *self) {
    (void)self;

    return 1;
}
static const char *cph_makeString(IChunkProvider *self) {
    (void)self;

    return "HellRandomLevelSource";
}
const IChunkProviderVtable ChunkProviderHell_vtable = {
    cph_chunkExists, cph_provideChunk,          cph_prepareChunk, cph_populate,
    cph_saveChunks,  cph_unload100OldestChunks, cph_canSave,      cph_makeString,
};

ChunkProviderHell *ChunkProviderHell_create(World *world, long seed) {
    ChunkProviderHell *self = (ChunkProviderHell *)calloc(1, sizeof(ChunkProviderHell));
    self->base.vtable = &ChunkProviderHell_vtable;
    self->worldObj = world;
    JavaRandom_init(&self->hellRNG, (int64_t)seed);
    NoiseGeneratorOctaves_init(&self->field_4169_i, &self->hellRNG, 16);
    NoiseGeneratorOctaves_init(&self->field_4168_j, &self->hellRNG, 16);
    NoiseGeneratorOctaves_init(&self->field_4167_k, &self->hellRNG, 8);
    NoiseGeneratorOctaves_init(&self->field_4166_l, &self->hellRNG, 4);
    NoiseGeneratorOctaves_init(&self->field_4165_m, &self->hellRNG, 4);
    NoiseGeneratorOctaves_init(&self->field_4177_a, &self->hellRNG, 10);
    NoiseGeneratorOctaves_init(&self->field_4176_b, &self->hellRNG, 16);

    MapGenCavesHell_init(&self->field_4159_s);
    return self;
}

void ChunkProviderHell_free(ChunkProviderHell *self) {
    NoiseGeneratorOctaves_free(&self->field_4169_i);
    NoiseGeneratorOctaves_free(&self->field_4168_j);
    NoiseGeneratorOctaves_free(&self->field_4167_k);
    NoiseGeneratorOctaves_free(&self->field_4166_l);
    NoiseGeneratorOctaves_free(&self->field_4165_m);
    NoiseGeneratorOctaves_free(&self->field_4177_a);
    NoiseGeneratorOctaves_free(&self->field_4176_b);
    free(self->field_4163_o);
    free(self->field_4175_c);
    free(self->field_4174_d);
    free(self->field_4173_e);
    free(self->field_4172_f);
    free(self->field_4171_g);
    free(self);
}

static double *func_4057_a(ChunkProviderHell *self, double *var1, int var2, int var3, int var4,
                           int var5, int var6, int var7) {
    if (var1 == NULL) {
        var1 = (double *)malloc((size_t)var5 * var6 * var7 * sizeof(double));
    }

    double var8 = 684.412;
    double var10 = 2053.236;

    self->field_4172_f = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_4177_a, self->field_4172_f, (double)var2, (double)var3, (double)var4, var5, 1,
        var7, 1.0, 0.0, 1.0);
    self->field_4171_g = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_4176_b, self->field_4171_g, (double)var2, (double)var3, (double)var4, var5, 1,
        var7, 100.0, 0.0, 100.0);
    self->field_4175_c = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_4167_k, self->field_4175_c, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8 / 80.0, var10 / 60.0, var8 / 80.0);
    self->field_4174_d = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_4169_i, self->field_4174_d, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8, var10, var8);
    self->field_4173_e = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_4168_j, self->field_4173_e, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8, var10, var8);

    int var12 = 0;
    int var13 = 0;

    double var14[17];
    for (int v15 = 0; v15 < var6; ++v15) {
        var14[v15] = cos((double)v15 * M_PI * 6.0 / (double)var6) * 2.0;
        double v16 = (double)v15;
        if (v15 > var6 / 2)
            v16 = (double)(var6 - 1 - v15);
        if (v16 < 4.0) {
            v16 = 4.0 - v16;
            var14[v15] -= v16 * v16 * v16 * 10.0;
        }
    }

    for (int v15 = 0; v15 < var5; ++v15) {
        for (int var36 = 0; var36 < var7; ++var36) {
            double var17 = (self->field_4172_f[var13] + 256.0) / 512.0;
            if (var17 > 1.0)
                var17 = 1.0;

            double var19 = 0.0;
            double var21 = self->field_4171_g[var13] / 8000.0;
            if (var21 < 0.0)
                var21 = -var21;
            var21 = var21 * 3.0 - 3.0;
            if (var21 < 0.0) {
                var21 /= 2.0;
                if (var21 < -1.0)
                    var21 = -1.0;
                var21 /= 1.4;
                var21 /= 2.0;
                var17 = 0.0;
            } else {
                if (var21 > 1.0)
                    var21 = 1.0;
                var21 /= 6.0;
            }

            var17 += 0.5;
            var21 = var21 * (double)var6 / 16.0;
            ++var13;

            for (int var23 = 0; var23 < var6; ++var23) {
                double var24 = 0.0;
                double var26 = var14[var23];
                double var28 = self->field_4174_d[var12] / 512.0;
                double var30 = self->field_4173_e[var12] / 512.0;
                double var32 = (self->field_4175_c[var12] / 10.0 + 1.0) / 2.0;
                if (var32 < 0.0)
                    var24 = var28;
                else if (var32 > 1.0)
                    var24 = var30;
                else
                    var24 = var28 + (var30 - var28) * var32;

                var24 -= var26;

                double var34;
                if (var23 > var6 - 4) {
                    var34 = (double)((float)(var23 - (var6 - 4)) / 3.0f);
                    var24 = var24 * (1.0 - var34) + -10.0 * var34;
                }
                if ((double)var23 < var19) {
                    var34 = (var19 - (double)var23) / 4.0;
                    if (var34 < 0.0)
                        var34 = 0.0;
                    if (var34 > 1.0)
                        var34 = 1.0;
                    var24 = var24 * (1.0 - var34) + -10.0 * var34;
                }

                var1[var12] = var24;
                ++var12;
            }
        }
    }

    return var1;
}

static void func_4059_a(ChunkProviderHell *self, int var1, int var2, uint8_t *var3) {
    int var4 = 4;
    int var5 = 32;
    int var6 = var4 + 1;
    int var7 = 17;
    int var8 = var4 + 1;

    self->field_4163_o =
        func_4057_a(self, self->field_4163_o, var1 * var4, 0, var2 * var4, var6, var7, var8);

    for (int var9 = 0; var9 < var4; ++var9) {
        for (int var10 = 0; var10 < var4; ++var10) {
            for (int var11 = 0; var11 < 16; ++var11) {
                double var12 = 0.125;
                double var14 =
                    self->field_4163_o[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 0];
                double var16 =
                    self->field_4163_o[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 0];
                double var18 =
                    self->field_4163_o[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 0];
                double var20 =
                    self->field_4163_o[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 0];
                double var22 =
                    (self->field_4163_o[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 1] -
                     var14) *
                    var12;
                double var24 =
                    (self->field_4163_o[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 1] -
                     var16) *
                    var12;
                double var26 =
                    (self->field_4163_o[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 1] -
                     var18) *
                    var12;
                double var28 =
                    (self->field_4163_o[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 1] -
                     var20) *
                    var12;

                for (int var30 = 0; var30 < 8; ++var30) {
                    double var31 = 0.25;
                    double var33 = var14;
                    double var35 = var16;
                    double var37 = (var18 - var14) * var31;
                    double var39 = (var20 - var16) * var31;

                    for (int var41 = 0; var41 < 4; ++var41) {
                        int var42 =
                            (var41 + var9 * 4) << 11 | (0 + var10 * 4) << 7 | (var11 * 8 + var30);
                        short var43 = 128;
                        double var44 = 0.25;
                        double var46 = var33;
                        double var48 = (var35 - var33) * var44;

                        for (int var50 = 0; var50 < 4; ++var50) {
                            int var51 = 0;
                            if (var11 * 8 + var30 < var5) {
                                var51 = Block_lavaStill->blockID;
                            }
                            if (var46 > 0.0) {
                                var51 = Block_netherrack->blockID;
                            }
                            var3[var42] = (uint8_t)var51;
                            var42 += var43;
                            var46 += var48;
                        }
                        var33 += var37;
                        var35 += var39;
                    }

                    var14 += var22;
                    var16 += var24;
                    var18 += var26;
                    var20 += var28;
                }
            }
        }
    }
}

static void func_4058_b(ChunkProviderHell *self, int var1, int var2, uint8_t *var3) {
    int var4 = 64;
    double var5 = 1.0 / 32.0;

    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_4166_l, self->field_4162_p,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var5, var5, 1.0);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_4166_l, self->field_4161_q,
                                               (double)(var1 * 16), 109.0134, (double)(var2 * 16),
                                               16, 1, 16, var5, 1.0, var5);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_4165_m, self->field_4160_r,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var5 * 2.0, var5 * 2.0, var5 * 2.0);

    for (int var7 = 0; var7 < 16; ++var7) {
        for (int var8 = 0; var8 < 16; ++var8) {
            int var9 = (self->field_4162_p[var7 + var8 * 16] +
                        JavaRandom_nextDouble(&self->hellRNG) * 0.2) > 0.0
                           ? 1
                           : 0;
            int var10 = (self->field_4161_q[var7 + var8 * 16] +
                         JavaRandom_nextDouble(&self->hellRNG) * 0.2) > 0.0
                            ? 1
                            : 0;
            int var11 = (int)(self->field_4160_r[var7 + var8 * 16] / 3.0 + 3.0 +
                              JavaRandom_nextDouble(&self->hellRNG) * 0.25);
            int var12 = -1;
            uint8_t var13 = (uint8_t)Block_netherrack->blockID;
            uint8_t var14 = (uint8_t)Block_netherrack->blockID;

            for (int var15 = 127; var15 >= 0; --var15) {
                int var16 = (var8 * 16 + var7) * 128 + var15;
                if (var15 >= 127 - JavaRandom_nextInt(&self->hellRNG, 5)) {
                    var3[var16] = (uint8_t)Block_bedrock->blockID;
                } else if (var15 <= 0 + JavaRandom_nextInt(&self->hellRNG, 5)) {
                    var3[var16] = (uint8_t)Block_bedrock->blockID;
                } else {
                    uint8_t var17 = var3[var16];
                    if (var17 == 0) {
                        var12 = -1;
                    } else if (var17 == Block_netherrack->blockID) {
                        if (var12 == -1) {
                            if (var11 <= 0) {
                                var13 = 0;
                                var14 = (uint8_t)Block_netherrack->blockID;
                            } else if (var15 >= var4 - 4 && var15 <= var4 + 1) {
                                var13 = (uint8_t)Block_netherrack->blockID;
                                var14 = (uint8_t)Block_netherrack->blockID;
                                if (var10)
                                    var13 = (uint8_t)Block_gravel->blockID;
                                if (var10)
                                    var14 = (uint8_t)Block_netherrack->blockID;
                                if (var9)
                                    var13 = (uint8_t)Block_slowSand->blockID;
                                if (var9)
                                    var14 = (uint8_t)Block_slowSand->blockID;
                            }
                            if (var15 < var4 && var13 == 0) {
                                var13 = (uint8_t)Block_lavaStill->blockID;
                            }
                            var12 = var11;
                            if (var15 >= var4 - 1) {
                                var3[var16] = var13;
                            } else {
                                var3[var16] = var14;
                            }
                        } else if (var12 > 0) {
                            --var12;
                            var3[var16] = var14;
                        }
                    }
                }
            }
        }
    }
}

Chunk *ChunkProviderHell_provideChunk(ChunkProviderHell *self, int cx, int cz) {
    JavaRandom_setSeed(&self->hellRNG, (int64_t)cx * 341873128712LL + (int64_t)cz * 132897987541LL);
    uint8_t *var3 = (uint8_t *)calloc(32768, 1);
    func_4059_a(self, cx, cz, var3);
    func_4058_b(self, cx, cz, var3);

    if (self->field_4159_s.base.vtable) {

        MapGenBase_generate(&self->field_4159_s.base, self->worldObj, cx, cz, var3);
    }

    Chunk *var4 = Chunk_create(self->worldObj, cx, cz);
    memcpy(var4->blocks, var3, 32768);
    free(var3);

    Chunk_func_1024_c(var4);
    return var4;
}

void ChunkProviderHell_populate(ChunkProviderHell *self, int cx, int cz) {
    Block_sand->fallInstantly = 1;
    int var4 = cx * 16;
    int var5 = cz * 16;

    for (int var6 = 0; var6 < 8; ++var6) {
        int var7 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        int var8 = JavaRandom_nextInt(&self->hellRNG, 120) + 4;
        int var9 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        WorldGenHellLava *gen = WorldGenHellLava_create(Block_lavaMoving->blockID);
        WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var7, var8, var9);
        WorldGenerator_destroy(&gen->base);
    }

    {
        int var6 =
            JavaRandom_nextInt(&self->hellRNG, JavaRandom_nextInt(&self->hellRNG, 10) + 1) + 1;
        for (int var7 = 0; var7 < var6; ++var7) {
            int var8 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
            int var9 = JavaRandom_nextInt(&self->hellRNG, 120) + 4;
            int var10 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
            WorldGenFire *gen = WorldGenFire_create();
            WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var8, var9, var10);
            WorldGenerator_destroy(&gen->base);
        }
    }

    {
        int var6 = JavaRandom_nextInt(&self->hellRNG, JavaRandom_nextInt(&self->hellRNG, 10) + 1);
        for (int var7 = 0; var7 < var6; ++var7) {
            int var8 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
            int var9 = JavaRandom_nextInt(&self->hellRNG, 120) + 4;
            int var10 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
            WorldGenGlowStone1 *gen = WorldGenGlowStone1_create();
            WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var8, var9, var10);
            WorldGenerator_destroy(&gen->base);
        }
    }

    for (int var7 = 0; var7 < 10; ++var7) {
        int var8 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        int var9 = JavaRandom_nextInt(&self->hellRNG, 128);
        int var10 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        WorldGenGlowStone2 *gen = WorldGenGlowStone2_create();
        WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var8, var9, var10);
        WorldGenerator_destroy(&gen->base);
    }

    if (JavaRandom_nextInt(&self->hellRNG, 1) == 0) {
        int var7 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        int var8 = JavaRandom_nextInt(&self->hellRNG, 128);
        int var9 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        WorldGenFlowers *gen = WorldGenFlowers_create(Block_mushroomBrown->blockID);
        WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var7, var8, var9);
        WorldGenerator_destroy(&gen->base);
    }
    if (JavaRandom_nextInt(&self->hellRNG, 1) == 0) {
        int var7 = var4 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        int var8 = JavaRandom_nextInt(&self->hellRNG, 128);
        int var9 = var5 + JavaRandom_nextInt(&self->hellRNG, 16) + 8;
        WorldGenFlowers *gen = WorldGenFlowers_create(Block_mushroomRed->blockID);
        WorldGenerator_generate(&gen->base, self->worldObj, &self->hellRNG, var7, var8, var9);
        WorldGenerator_destroy(&gen->base);
    }

    Block_sand->fallInstantly = 0;
}
