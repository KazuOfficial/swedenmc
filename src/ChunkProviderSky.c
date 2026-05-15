#include "ChunkProviderSky.h"
#include "Block.h"
#include "Material.h"
#include "WorldGenLakes.h"
#include "WorldGenDungeons.h"
#include "WorldGenClay.h"
#include "WorldGenMinable.h"
#include "WorldGenFlowers.h"
#include "WorldGenReed.h"
#include "WorldGenPumpkin.h"
#include "WorldGenCactus.h"
#include "WorldGenLiquids.h"
#include "WorldGenTrees.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int cps_chunkExists(IChunkProvider *self, int x, int z) {
    (void)self;
    (void)x;
    (void)z;
    return 1;
}
static Chunk *cps_provideChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderSky_provideChunk((ChunkProviderSky *)self, x, z);
}
static Chunk *cps_prepareChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderSky_provideChunk((ChunkProviderSky *)self, x, z);
}
static void cps_populate(IChunkProvider *self, IChunkProvider *chunkProvider, int x, int z) {
    (void)chunkProvider;
    ChunkProviderSky_populate((ChunkProviderSky *)self, x, z);
}
static int cps_saveChunks(IChunkProvider *self, int saveAll, IProgressUpdate *progress) {
    (void)self;
    (void)saveAll;
    (void)progress;
    return 1;
}
static int cps_unload100OldestChunks(IChunkProvider *self) {
    (void)self;
    return 0;
}
static int cps_canSave(IChunkProvider *self) {
    (void)self;
    return 1;
}
static const char *cps_makeString(IChunkProvider *self) {
    (void)self;
    return "RandomLevelSource";
}
static const IChunkProviderVtable ChunkProviderSky_vtable = {
    cps_chunkExists, cps_provideChunk,          cps_prepareChunk, cps_populate,
    cps_saveChunks,  cps_unload100OldestChunks, cps_canSave,      cps_makeString,
};

ChunkProviderSky *ChunkProviderSky_create(World *world, long seed) {
    ChunkProviderSky *self = (ChunkProviderSky *)calloc(1, sizeof(ChunkProviderSky));
    self->base.vtable = &ChunkProviderSky_vtable;
    self->field_28081_p = world;
    JavaRandom_init(&self->field_28087_j, (int64_t)seed);
    NoiseGeneratorOctaves_init(&self->field_28086_k, &self->field_28087_j, 16);
    NoiseGeneratorOctaves_init(&self->field_28085_l, &self->field_28087_j, 16);
    NoiseGeneratorOctaves_init(&self->field_28084_m, &self->field_28087_j, 8);
    NoiseGeneratorOctaves_init(&self->field_28083_n, &self->field_28087_j, 4);
    NoiseGeneratorOctaves_init(&self->field_28082_o, &self->field_28087_j, 4);
    NoiseGeneratorOctaves_init(&self->field_28096_a, &self->field_28087_j, 10);
    NoiseGeneratorOctaves_init(&self->field_28095_b, &self->field_28087_j, 16);
    NoiseGeneratorOctaves_init(&self->field_28094_c, &self->field_28087_j, 8);

    {
        MapGenCaves *mc = MapGenCaves_create();
        self->field_28076_u = *mc;
        free(mc);
    }
    return self;
}

void ChunkProviderSky_free(ChunkProviderSky *self) {
    NoiseGeneratorOctaves_free(&self->field_28086_k);
    NoiseGeneratorOctaves_free(&self->field_28085_l);
    NoiseGeneratorOctaves_free(&self->field_28084_m);
    NoiseGeneratorOctaves_free(&self->field_28083_n);
    NoiseGeneratorOctaves_free(&self->field_28082_o);
    NoiseGeneratorOctaves_free(&self->field_28096_a);
    NoiseGeneratorOctaves_free(&self->field_28095_b);
    NoiseGeneratorOctaves_free(&self->field_28094_c);
    free(self->field_28080_q);
    free(self->field_28075_v);
    free(self->field_28093_d);
    free(self->field_28092_e);
    free(self->field_28091_f);
    free(self->field_28090_g);
    free(self->field_28089_h);
    free(self->field_28074_w);
    free(self);
}

static double *func_28073_a(ChunkProviderSky *self, double *var1, int var2, int var3, int var4,
                            int var5, int var6, int var7) {
    if (var1 == NULL) {
        var1 = (double *)malloc((size_t)var5 * var6 * var7 * sizeof(double));
    }

    WorldChunkManager *wcm = World_getWorldChunkManager(self->field_28081_p);
    double *var12 = wcm->temperature;
    double *var13 = wcm->humidity;

    double var8 = 684.412;
    double var10 = 684.412;

    self->field_28090_g = NoiseGeneratorOctaves_generateNoiseOctaves2D(
        &self->field_28096_a, self->field_28090_g, var2, var4, var5, var7, 1.121, 1.121, 0.5);
    self->field_28089_h = NoiseGeneratorOctaves_generateNoiseOctaves2D(
        &self->field_28095_b, self->field_28089_h, var2, var4, var5, var7, 200.0, 200.0, 0.5);
    var8 *= 2.0;
    self->field_28093_d = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_28084_m, self->field_28093_d, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8 / 80.0, var10 / 160.0, var8 / 80.0);
    self->field_28092_e = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_28086_k, self->field_28092_e, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8, var10, var8);
    self->field_28091_f = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_28085_l, self->field_28091_f, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, var8, var10, var8);

    int var14 = 0;
    int var15 = 0;
    int var16 = 16 / var5;

    for (int var17 = 0; var17 < var5; ++var17) {
        int var18 = var17 * var16 + var16 / 2;

        for (int var19 = 0; var19 < var7; ++var19) {
            int var20 = var19 * var16 + var16 / 2;

            double var21 = var12[var18 * 16 + var20];
            double var23 = var13[var18 * 16 + var20] * var21;
            double var25 = 1.0 - var23;
            var25 *= var25;
            var25 *= var25;
            var25 = 1.0 - var25;
            double var27 = (self->field_28090_g[var15] + 256.0) / 512.0;
            var27 *= var25;
            if (var27 > 1.0)
                var27 = 1.0;

            double var29 = self->field_28089_h[var15] / 8000.0;
            if (var29 < 0.0)
                var29 = -var29 * 0.3;
            var29 = var29 * 3.0 - 2.0;
            if (var29 > 1.0)
                var29 = 1.0;
            var29 /= 8.0;
            var29 = 0.0;
            if (var27 < 0.0)
                var27 = 0.0;
            var27 += 0.5;
            var29 = var29 * (double)var6 / 16.0;
            ++var15;

            double var31 = (double)var6 / 2.0;
            for (int var33 = 0; var33 < var6; ++var33) {
                double var34 = 0.0;
                double var36 = ((double)var33 - var31) * 8.0 / var27;
                if (var36 < 0.0)
                    var36 *= -1.0;
                double var38 = self->field_28092_e[var14] / 512.0;
                double var40 = self->field_28091_f[var14] / 512.0;
                double var42 = (self->field_28093_d[var14] / 10.0 + 1.0) / 2.0;
                if (var42 < 0.0)
                    var34 = var38;
                else if (var42 > 1.0)
                    var34 = var40;
                else
                    var34 = var38 + (var40 - var38) * var42;

                var34 -= 8.0;

                int var44 = 32;
                double var45;
                if (var33 > var6 - var44) {
                    var45 = (double)((float)(var33 - (var6 - var44)) / ((float)var44 - 1.0f));
                    var34 = var34 * (1.0 - var45) + -30.0 * var45;
                }
                var44 = 8;
                if (var33 < var44) {
                    var45 = (double)((float)(var44 - var33) / ((float)var44 - 1.0f));
                    var34 = var34 * (1.0 - var45) + -30.0 * var45;
                }

                var1[var14] = var34;
                ++var14;
            }
        }
    }

    return var1;
}

static void func_28071_a(ChunkProviderSky *self, int var1, int var2, uint8_t *var3,
                         BiomeGenBase **var4, double *var5) {
    (void)var4;
    (void)var5;
    int var6 = 2;
    int var7 = var6 + 1;
    int var8 = 33;
    int var9 = var6 + 1;

    self->field_28080_q =
        func_28073_a(self, self->field_28080_q, var1 * var6, 0, var2 * var6, var7, var8, var9);

    for (int var10 = 0; var10 < var6; ++var10) {
        for (int var11 = 0; var11 < var6; ++var11) {
            for (int var12 = 0; var12 < 32; ++var12) {
                double var13 = 0.25;
                double var15 =
                    self->field_28080_q[((var10 + 0) * var9 + var11 + 0) * var8 + var12 + 0];
                double var17 =
                    self->field_28080_q[((var10 + 0) * var9 + var11 + 1) * var8 + var12 + 0];
                double var19 =
                    self->field_28080_q[((var10 + 1) * var9 + var11 + 0) * var8 + var12 + 0];
                double var21 =
                    self->field_28080_q[((var10 + 1) * var9 + var11 + 1) * var8 + var12 + 0];
                double var23 =
                    (self->field_28080_q[((var10 + 0) * var9 + var11 + 0) * var8 + var12 + 1] -
                     var15) *
                    var13;
                double var25 =
                    (self->field_28080_q[((var10 + 0) * var9 + var11 + 1) * var8 + var12 + 1] -
                     var17) *
                    var13;
                double var27 =
                    (self->field_28080_q[((var10 + 1) * var9 + var11 + 0) * var8 + var12 + 1] -
                     var19) *
                    var13;
                double var29 =
                    (self->field_28080_q[((var10 + 1) * var9 + var11 + 1) * var8 + var12 + 1] -
                     var21) *
                    var13;

                for (int var31 = 0; var31 < 4; ++var31) {
                    double var32 = 0.125;
                    double var34 = var15;
                    double var36 = var17;
                    double var38 = (var19 - var15) * var32;
                    double var40 = (var21 - var17) * var32;

                    for (int var42 = 0; var42 < 8; ++var42) {
                        int var43 =
                            (var42 + var10 * 8) << 11 | (0 + var11 * 8) << 7 | (var12 * 4 + var31);
                        short var44 = 128;
                        double var45 = 0.125;
                        double var47 = var34;
                        double var49 = (var36 - var34) * var45;

                        for (int var51 = 0; var51 < 8; ++var51) {
                            uint8_t var52 = 0;
                            if (var47 > 0.0)
                                var52 = (uint8_t)Block_stone->blockID;
                            var3[var43] = var52;
                            var43 += var44;
                            var47 += var49;
                        }
                        var34 += var38;
                        var36 += var40;
                    }

                    var15 += var23;
                    var17 += var25;
                    var19 += var27;
                    var21 += var29;
                }
            }
        }
    }
}

static void func_28072_a(ChunkProviderSky *self, int var1, int var2, uint8_t *var3,
                         BiomeGenBase **var4) {
    double var5 = 1.0 / 32.0;
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_28083_n, self->field_28079_r,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var5, var5, 1.0);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_28083_n, self->field_28078_s,
                                               (double)(var1 * 16), 109.0134, (double)(var2 * 16),
                                               16, 1, 16, var5, 1.0, var5);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_28082_o, self->field_28077_t,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var5 * 2.0, var5 * 2.0, var5 * 2.0);

    for (int var7 = 0; var7 < 16; ++var7) {
        for (int var8 = 0; var8 < 16; ++var8) {
            BiomeGenBase *var9 = var4[var7 + var8 * 16];
            int var10 = (int)(self->field_28077_t[var7 + var8 * 16] / 3.0 + 3.0 +
                              JavaRandom_nextDouble(&self->field_28087_j) * 0.25);
            int var11 = -1;
            uint8_t var12 = var9->topBlock;
            uint8_t var13 = var9->fillerBlock;

            for (int var14 = 127; var14 >= 0; --var14) {
                int var15 = (var8 * 16 + var7) * 128 + var14;
                uint8_t var16 = var3[var15];
                if (var16 == 0) {
                    var11 = -1;
                } else if (var16 == Block_stone->blockID) {
                    if (var11 == -1) {
                        if (var10 <= 0) {
                            var12 = 0;
                            var13 = (uint8_t)Block_stone->blockID;
                        }
                        var11 = var10;
                        if (var14 >= 0) {
                            var3[var15] = var12;
                        } else {
                            var3[var15] = var13;
                        }
                    } else if (var11 > 0) {
                        --var11;
                        var3[var15] = var13;
                        if (var11 == 0 && var13 == Block_sand->blockID) {
                            var11 = JavaRandom_nextInt(&self->field_28087_j, 4);
                            var13 = (uint8_t)Block_sandStone->blockID;
                        }
                    }
                }
            }
        }
    }
}

Chunk *ChunkProviderSky_provideChunk(ChunkProviderSky *self, int cx, int cz) {
    JavaRandom_setSeed(&self->field_28087_j,
                       (int64_t)cx * 341873128712LL + (int64_t)cz * 132897987541LL);
    uint8_t *var3 = (uint8_t *)calloc(32768, 1);
    Chunk *var4 = Chunk_create(self->field_28081_p, cx, cz);

    WorldChunkManager *wcm = World_getWorldChunkManager(self->field_28081_p);
    self->field_28075_v =
        WorldChunkManager_loadBiomeData(wcm, self->field_28075_v, cx * 16, cz * 16, 16, 16);

    double *var5 = wcm->temperature;

    func_28071_a(self, cx, cz, var3, self->field_28075_v, var5);
    func_28072_a(self, cx, cz, var3, self->field_28075_v);
    MapGenBase_generate(&self->field_28076_u.base, self->field_28081_p, cx, cz, var3);
    memcpy(var4->blocks, var3, 32768);
    free(var3);

    Chunk_func_1024_c(var4);
    return var4;
}

void ChunkProviderSky_populate(ChunkProviderSky *self, int cx, int cz) {
    Block_sand->fallInstantly = 1;
    int var4 = cx * 16;
    int var5 = cz * 16;

    WorldChunkManager *wcm = World_getWorldChunkManager(self->field_28081_p);
    BiomeGenBase *var6 = WorldChunkManager_getBiomeGenAt(wcm, var4 + 16, var5 + 16);

    JavaRandom_setSeed(&self->field_28087_j, World_getRandomSeed(self->field_28081_p));
    long var7 = JavaRandom_nextLong(&self->field_28087_j) / 2L * 2L + 1L;
    long var9 = JavaRandom_nextLong(&self->field_28087_j) / 2L * 2L + 1L;
    JavaRandom_setSeed(
        &self->field_28087_j,
        (long)(((unsigned long)cx * (unsigned long)var7 + (unsigned long)cz * (unsigned long)var9) ^
               (unsigned long)World_getRandomSeed(self->field_28081_p)));

    if (JavaRandom_nextInt(&self->field_28087_j, 4) == 0) {
        int var13 = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int var14 = JavaRandom_nextInt(&self->field_28087_j, 128);
        int var15 = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenLakes *gen = WorldGenLakes_create(Block_waterStill->blockID);
        WorldGenerator_generate(&gen->base, self->field_28081_p, &self->field_28087_j, var13, var14,
                                var15);
        WorldGenerator_destroy(&gen->base);
    }
    if (JavaRandom_nextInt(&self->field_28087_j, 8) == 0) {
        int var13 = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int var14 = JavaRandom_nextInt(&self->field_28087_j,
                                       JavaRandom_nextInt(&self->field_28087_j, 120) + 8);
        int var15 = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        if (var14 < 64 || JavaRandom_nextInt(&self->field_28087_j, 10) == 0) {
            WorldGenLakes *gen = WorldGenLakes_create(Block_lavaStill->blockID);
            WorldGenerator_generate(&gen->base, self->field_28081_p, &self->field_28087_j, var13,
                                    var14, var15);
            WorldGenerator_destroy(&gen->base);
        }
    }

    for (int var13 = 0; var13 < 8; ++var13) {
        int var14 = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int var15 = JavaRandom_nextInt(&self->field_28087_j, 128);
        int var16 = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenDungeons *gen = WorldGenDungeons_create();
        WorldGenerator_generate(&gen->base, self->field_28081_p, &self->field_28087_j, var14, var15,
                                var16);
        WorldGenerator_destroy(&gen->base);
    }

    for (int var13 = 0; var13 < 10; ++var13) {
        int var14 = var4 + JavaRandom_nextInt(&self->field_28087_j, 16);
        int var15 = JavaRandom_nextInt(&self->field_28087_j, 128);
        int var16 = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
        WorldGenClay *gen = WorldGenClay_create(32);
        WorldGenerator_generate(&gen->base, self->field_28081_p, &self->field_28087_j, var14, var15,
                                var16);
        WorldGenerator_destroy(&gen->base);
    }

    {
        WorldGenMinable *g = WorldGenMinable_create(Block_dirt->blockID, 32);
        for (int i = 0; i < 20; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 128),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_gravel->blockID, 32);
        for (int i = 0; i < 10; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 128),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreCoal->blockID, 16);
        for (int i = 0; i < 20; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 128),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreIron->blockID, 8);
        for (int i = 0; i < 20; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 64),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreGold->blockID, 8);
        for (int i = 0; i < 2; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 32),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreRedstone->blockID, 7);
        for (int i = 0; i < 8; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 16),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreDiamond->blockID, 7);
        for (int i = 0; i < 1; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 16),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }
    {
        WorldGenMinable *g = WorldGenMinable_create(Block_oreLapis->blockID, 6);
        for (int i = 0; i < 1; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16),
                y = JavaRandom_nextInt(&self->field_28087_j, 16) +
                    JavaRandom_nextInt(&self->field_28087_j, 16),
                z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16);
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        }
        WorldGenerator_destroy(&g->base);
    }

    {
        double var11 = 0.5;
        int var13 = (int)(NoiseGeneratorOctaves_sample2D(&self->field_28094_c, (double)var4 * var11,
                                                         (double)var5 * var11) /
                              8.0 +
                          JavaRandom_nextDouble(&self->field_28087_j) * 4.0 + 4.0) /
                    3;
        int var14 = 0;
        if (JavaRandom_nextInt(&self->field_28087_j, 10) == 0)
            ++var14;
        if (var6 == &BiomeGenBase_biomes[BIOME_FOREST])
            var14 += var13 + 5;
        if (var6 == &BiomeGenBase_biomes[BIOME_RAINFOREST])
            var14 += var13 + 5;
        if (var6 == &BiomeGenBase_biomes[BIOME_SEASONAL_FOREST])
            var14 += var13 + 2;
        if (var6 == &BiomeGenBase_biomes[BIOME_TAIGA])
            var14 += var13 + 5;
        if (var6 == &BiomeGenBase_biomes[BIOME_DESERT])
            var14 -= 20;
        if (var6 == &BiomeGenBase_biomes[BIOME_TUNDRA])
            var14 -= 20;
        if (var6 == &BiomeGenBase_biomes[BIOME_PLAINS])
            var14 -= 20;

        for (int var15 = 0; var15 < var14; ++var15) {
            int var16 = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
            int var17 = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
            WorldGenerator *var18 =
                BiomeGenBase_getRandomWorldGenForTrees(var6, &self->field_28087_j);
            WorldGenerator_func_517_a(var18, 1.0, 1.0, 1.0);
            WorldGenerator_generate(var18, self->field_28081_p, &self->field_28087_j, var16,
                                    World_getHeightValue(self->field_28081_p, var16, var17), var17);
            WorldGenerator_destroy(var18);
        }
    }

    for (int i = 0; i < 2; ++i) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenFlowers *g = WorldGenFlowers_create(Block_plantYellow->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    if (JavaRandom_nextInt(&self->field_28087_j, 2) == 0) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenFlowers *g = WorldGenFlowers_create(Block_plantRed->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    if (JavaRandom_nextInt(&self->field_28087_j, 4) == 0) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenFlowers *g = WorldGenFlowers_create(Block_mushroomBrown->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    if (JavaRandom_nextInt(&self->field_28087_j, 8) == 0) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenFlowers *g = WorldGenFlowers_create(Block_mushroomRed->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    for (int i = 0; i < 10; ++i) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenReed *g = WorldGenReed_create();
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    if (JavaRandom_nextInt(&self->field_28087_j, 32) == 0) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j, 128);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenPumpkin *g = WorldGenPumpkin_create();
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    {
        int var15 = 0;
        if (var6 == &BiomeGenBase_biomes[BIOME_DESERT])
            var15 += 10;
        for (int i = 0; i < var15; ++i) {
            int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
            int y = JavaRandom_nextInt(&self->field_28087_j, 128);
            int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
            WorldGenCactus *g = WorldGenCactus_create();
            WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
            WorldGenerator_destroy(&g->base);
        }
    }

    for (int i = 0; i < 50; ++i) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(&self->field_28087_j,
                                   JavaRandom_nextInt(&self->field_28087_j, 120) + 8);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenLiquids *g = WorldGenLiquids_create(Block_waterMoving->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }
    for (int i = 0; i < 20; ++i) {
        int x = var4 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        int y = JavaRandom_nextInt(
            &self->field_28087_j,
            JavaRandom_nextInt(&self->field_28087_j,
                               JavaRandom_nextInt(&self->field_28087_j, 112) + 8) +
                8);
        int z = var5 + JavaRandom_nextInt(&self->field_28087_j, 16) + 8;
        WorldGenLiquids *g = WorldGenLiquids_create(Block_lavaMoving->blockID);
        WorldGenerator_generate(&g->base, self->field_28081_p, &self->field_28087_j, x, y, z);
        WorldGenerator_destroy(&g->base);
    }

    {

        self->field_28074_w =
            WorldChunkManager_getTemperatures(wcm, self->field_28074_w, var4 + 8, var5 + 8, 16, 16);
        for (int var16 = var4 + 8; var16 < var4 + 8 + 16; ++var16) {
            for (int var17 = var5 + 8; var17 < var5 + 8 + 16; ++var17) {
                int var23 = var16 - (var4 + 8);
                int var19 = var17 - (var5 + 8);
                int var20 = World_findTopSolidBlock(self->field_28081_p, var16, var17);
                double var21 =
                    self->field_28074_w[var23 * 16 + var19] - (double)(var20 - 64) / 64.0 * 0.3;

                Material *var22 =
                    World_getBlockMaterial(self->field_28081_p, var16, var20 - 1, var17);
                if (var21 < 0.5 && var20 > 0 && var20 < 128 &&
                    World_isAirBlock(self->field_28081_p, var16, var20, var17) &&
                    Material_getIsSolid(var22) && var22 != Material_ice) {
                    World_setBlockWithNotify(self->field_28081_p, var16, var20, var17,
                                             Block_snow->blockID);
                }
            }
        }
    }

    Block_sand->fallInstantly = 0;
}
