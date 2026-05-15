#include "ChunkProviderGenerate.h"
#include "WorldChunkManager.h"
#include "WorldGenMinable.h"
#include "WorldGenTrees.h"
#include "WorldGenDungeons.h"
#include "WorldGenLakes.h"
#include "WorldGenClay.h"
#include "WorldGenFlowers.h"
#include "WorldGenTallGrass.h"
#include "WorldGenDeadBush.h"
#include "WorldGenReed.h"
#include "WorldGenPumpkin.h"
#include "WorldGenCactus.h"
#include "WorldGenLiquids.h"
#include "BiomeGenBase.h"
#include "Block.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static double *func_4061_a(ChunkProviderGenerate *self, double *var1, int var2, int var3, int var4,
                           int var5, int var6, int var7) {
    if (var1 == NULL) {
        var1 = (double *)malloc(var5 * var6 * var7 * sizeof(double));
    }

    WorldChunkManager *wcm = World_getWorldChunkManager(self->worldObj);
    double *var12f = wcm->temperature;
    double *var13f = wcm->humidity;

    self->field_4182_g = NoiseGeneratorOctaves_generateNoiseOctaves2D(
        &self->field_922_a, self->field_4182_g, var2, var4, var5, var7, 1.121, 1.121, 0.5);
    self->field_4181_h = NoiseGeneratorOctaves_generateNoiseOctaves2D(
        &self->field_921_b, self->field_4181_h, var2, var4, var5, var7, 200.0, 200.0, 0.5);
    self->field_4185_d = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_910_m, self->field_4185_d, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, 684.412 / 80.0, 684.412 / 160.0, 684.412 / 80.0);
    self->field_4184_e = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_912_k, self->field_4184_e, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, 684.412, 684.412, 684.412);
    self->field_4183_f = NoiseGeneratorOctaves_generateNoiseOctaves(
        &self->field_911_l, self->field_4183_f, (double)var2, (double)var3, (double)var4, var5,
        var6, var7, 684.412, 684.412, 684.412);

    int var14 = 0;
    int var15 = 0;
    int var16 = 16 / var5;

    for (int var17 = 0; var17 < var5; var17++) {
        int var18 = var17 * var16 + var16 / 2;

        for (int var19 = 0; var19 < var7; var19++) {
            int var20 = var19 * var16 + var16 / 2;

            double var21 = var12f[var18 * 16 + var20];
            double var23 = var13f[var18 * 16 + var20] * var21;

            double var25 = 1.0 - var23;
            var25 *= var25;
            var25 *= var25;
            var25 = 1.0 - var25;

            double var27 = (self->field_4182_g[var15] + 256.0) / 512.0;
            var27 *= var25;
            if (var27 > 1.0)
                var27 = 1.0;

            double var29 = self->field_4181_h[var15] / 8000.0;
            if (var29 < 0.0)
                var29 = -var29 * 0.3;
            var29 = var29 * 3.0 - 2.0;

            if (var29 < 0.0) {
                var29 /= 2.0;
                if (var29 < -1.0)
                    var29 = -1.0;
                var29 /= 1.4;
                var29 /= 2.0;
                var27 = 0.0;
            } else {
                if (var29 > 1.0)
                    var29 = 1.0;
                var29 /= 8.0;
            }

            if (var27 < 0.0)
                var27 = 0.0;
            var27 += 0.5;
            var29 = var29 * (double)var6 / 16.0;
            double var31 = (double)var6 / 2.0 + var29 * 4.0;
            var15++;

            for (int var33 = 0; var33 < var6; var33++) {
                double var34 = 0.0;
                double var36 = ((double)var33 - var31) * 12.0 / var27;
                if (var36 < 0.0)
                    var36 *= 4.0;

                double var38 = self->field_4184_e[var14] / 512.0;
                double var40 = self->field_4183_f[var14] / 512.0;
                double var42 = (self->field_4185_d[var14] / 10.0 + 1.0) / 2.0;

                if (var42 < 0.0)
                    var34 = var38;
                else if (var42 > 1.0)
                    var34 = var40;
                else
                    var34 = var38 + (var40 - var38) * var42;

                var34 -= var36;

                if (var33 > var6 - 4) {
                    double var44 = (double)((float)(var33 - (var6 - 4)) / 3.0f);
                    var34 = var34 * (1.0 - var44) + -10.0 * var44;
                }

                var1[var14] = var34;
                var14++;
            }
        }
    }

    return var1;
}

void ChunkProviderGenerate_generateTerrain(ChunkProviderGenerate *self, int var1, int var2,
                                           uint8_t *var3, BiomeGenBase **var4, double *var5) {
    int var6 = 4;
    int var7 = 64;
    int var8 = var6 + 1;
    int var9 = 17;
    int var10 = var6 + 1;

    self->field_4180_q =
        func_4061_a(self, self->field_4180_q, var1 * var6, 0, var2 * var6, var8, var9, var10);

    for (int var11 = 0; var11 < var6; var11++) {
        for (int var12 = 0; var12 < var6; var12++) {
            for (int var13 = 0; var13 < 16; var13++) {
                double var14 = 0.125;
                double var16 =
                    self->field_4180_q[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 0];
                double var18 =
                    self->field_4180_q[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 0];
                double var20 =
                    self->field_4180_q[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 0];
                double var22 =
                    self->field_4180_q[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 0];
                double var24 =
                    (self->field_4180_q[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 1] -
                     var16) *
                    var14;
                double var26 =
                    (self->field_4180_q[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 1] -
                     var18) *
                    var14;
                double var28 =
                    (self->field_4180_q[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 1] -
                     var20) *
                    var14;
                double var30 =
                    (self->field_4180_q[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 1] -
                     var22) *
                    var14;

                for (int var32 = 0; var32 < 8; var32++) {
                    double var33 = 0.25;
                    double var35 = var16;
                    double var37 = var18;
                    double var39 = (var20 - var16) * var33;
                    double var41 = (var22 - var18) * var33;

                    for (int var43 = 0; var43 < 4; var43++) {
                        int var44 =
                            (var43 + var11 * 4) << 11 | (0 + var12 * 4) << 7 | var13 * 8 + var32;
                        int var45 = 128;
                        double var46 = 0.25;
                        double var48 = var35;
                        double var50 = (var37 - var35) * var46;

                        for (int var52 = 0; var52 < 4; var52++) {
                            double var53 = var5[(var11 * 4 + var43) * 16 + var12 * 4 + var52];
                            int var55 = 0;

                            if (var13 * 8 + var32 < var7) {
                                if (var53 < 0.5 && var13 * 8 + var32 >= var7 - 1) {
                                    var55 = Block_ice->blockID;
                                } else {
                                    var55 = Block_waterStill->blockID;
                                }
                            }

                            if (var48 > 0.0) {
                                var55 = Block_stone->blockID;
                            }

                            var3[var44] = (uint8_t)var55;
                            var44 += var45;
                            var48 += var50;
                        }

                        var35 += var39;
                        var37 += var41;
                    }

                    var16 += var24;
                    var18 += var26;
                    var20 += var28;
                    var22 += var30;
                }
            }
        }
    }
    (void)var4;
}

void ChunkProviderGenerate_replaceBlocksForBiome(ChunkProviderGenerate *self, int var1, int var2,
                                                 uint8_t *var3, BiomeGenBase **var4) {
    int var5 = 64;
    double var6 = 1.0 / 32.0;

    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_909_n, self->sandNoise,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var6, var6, 1.0);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_909_n, self->gravelNoise,
                                               (double)(var1 * 16), 109.0134, (double)(var2 * 16),
                                               16, 1, 16, var6, 1.0, var6);
    NoiseGeneratorOctaves_generateNoiseOctaves(&self->field_908_o, self->stoneNoise,
                                               (double)(var1 * 16), (double)(var2 * 16), 0.0, 16,
                                               16, 1, var6 * 2.0, var6 * 2.0, var6 * 2.0);

    for (int var8 = 0; var8 < 16; var8++) {
        for (int var9 = 0; var9 < 16; var9++) {
            BiomeGenBase *var10 = var4[var8 + var9 * 16];
            int var11 = (self->sandNoise[var8 + var9 * 16] +
                         JavaRandom_nextDouble(&self->rand) * 0.2) > 0.0;
            int var12 = (self->gravelNoise[var8 + var9 * 16] +
                         JavaRandom_nextDouble(&self->rand) * 0.2) > 3.0;
            int var13 = (int)(self->stoneNoise[var8 + var9 * 16] / 3.0 + 3.0 +
                              JavaRandom_nextDouble(&self->rand) * 0.25);
            int var14 = -1;
            int var15 = var10->topBlock;
            int var16 = var10->fillerBlock;

            for (int var17 = 127; var17 >= 0; var17--) {
                int var18 = (var9 * 16 + var8) * 128 + var17;

                if (var17 <= 0 + JavaRandom_nextInt(&self->rand, 5)) {
                    var3[var18] = (uint8_t)Block_bedrock->blockID;
                } else {
                    uint8_t var19 = var3[var18];

                    if (var19 == 0) {
                        var14 = -1;
                    } else if (var19 == Block_stone->blockID) {
                        if (var14 == -1) {
                            if (var13 <= 0) {
                                var15 = 0;
                                var16 = Block_stone->blockID;
                            } else if (var17 >= var5 - 4 && var17 <= var5 + 1) {
                                var15 = var10->topBlock;
                                var16 = var10->fillerBlock;
                                if (var12) {
                                    var15 = 0;
                                }
                                if (var12) {
                                    var16 = Block_gravel->blockID;
                                }
                                if (var11) {
                                    var15 = Block_sand->blockID;
                                }
                                if (var11) {
                                    var16 = Block_sand->blockID;
                                }
                            }

                            if (var17 < var5 && var15 == 0) {
                                var15 = Block_waterStill->blockID;
                            }

                            var14 = var13;
                            if (var17 >= var5 - 1) {
                                var3[var18] = (uint8_t)var15;
                            } else {
                                var3[var18] = (uint8_t)var16;
                            }
                        } else if (var14 > 0) {
                            var14--;
                            var3[var18] = (uint8_t)var16;
                            if (var14 == 0 && var16 == Block_sand->blockID) {
                                var14 = JavaRandom_nextInt(&self->rand, 4);
                                var16 = Block_sandStone->blockID;
                            }
                        }
                    }
                }
            }
        }
    }
}

Chunk *ChunkProviderGenerate_provideChunk(ChunkProviderGenerate *self, int var1, int var2) {
    JavaRandom_setSeed(&self->rand, (long)var1 * 341873128712L + (long)var2 * 132897987541L);

    uint8_t var3[16 * 128 * 16];
    memset(var3, 0, sizeof(var3));

    WorldChunkManager *var_wcm = World_getWorldChunkManager(self->worldObj);
    BiomeGenBase **biomes = WorldChunkManager_loadBiomeData(var_wcm, self->biomesForGeneration,
                                                            var1 * 16, var2 * 16, 16, 16);
    double *var5 = var_wcm->temperature;

    ChunkProviderGenerate_generateTerrain(self, var1, var2, var3, biomes, var5);
    ChunkProviderGenerate_replaceBlocksForBiome(self, var1, var2, var3, biomes);

    MapGenBase_generate(&self->caveGen.base, self->worldObj, var1, var2, var3);

    Chunk *var4 = Chunk_create(self->worldObj, var1, var2);
    memcpy(var4->blocks, var3, sizeof(var4->blocks));

    Chunk_func_1024_c(var4);
    return var4;
}

void ChunkProviderGenerate_populate(ChunkProviderGenerate *self, int var2, int var3) {
    Block_sand->fallInstantly = 1;
    int var4 = var2 * 16;
    int var5 = var3 * 16;

    BiomeGenBase *var6 = WorldChunkManager_getBiomeGenAt(World_getWorldChunkManager(self->worldObj),
                                                         var4 + 16, var5 + 16);

    JavaRandom_setSeed(&self->rand, World_getRandomSeed(self->worldObj));
    long var7 = (JavaRandom_nextLong(&self->rand) / 2L) * 2L + 1L;
    long var9 = (JavaRandom_nextLong(&self->rand) / 2L) * 2L + 1L;
    JavaRandom_setSeed(&self->rand, (long)(((unsigned long)var2 * (unsigned long)var7 +
                                            (unsigned long)var3 * (unsigned long)var9) ^
                                           (unsigned long)World_getRandomSeed(self->worldObj)));

    double var11 = 0.25;
    int var13, var14, var15, var16;

    if (JavaRandom_nextInt(&self->rand, 4) == 0) {
        var13 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var14 = JavaRandom_nextInt(&self->rand, 128);
        var15 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenLakes *wgl = WorldGenLakes_create(Block_waterStill->blockID);
        WorldGenerator_generate(&wgl->base, self->worldObj, &self->rand, var13, var14, var15);
        WorldGenLakes_free(wgl);
    }

    if (JavaRandom_nextInt(&self->rand, 8) == 0) {
        var13 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var14 = JavaRandom_nextInt(&self->rand, JavaRandom_nextInt(&self->rand, 120) + 8);
        var15 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        if (var14 < 64 || JavaRandom_nextInt(&self->rand, 10) == 0) {
            WorldGenLakes *wgll = WorldGenLakes_create(Block_lavaStill->blockID);
            WorldGenerator_generate(&wgll->base, self->worldObj, &self->rand, var13, var14, var15);
            WorldGenLakes_free(wgll);
        }
    }

    for (var13 = 0; var13 < 8; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var15 = JavaRandom_nextInt(&self->rand, 128);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenDungeons *dgen = WorldGenDungeons_create();
        WorldGenerator_generate(&dgen->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenDungeons_free(dgen);
    }

    for (var13 = 0; var13 < 10; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 128);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenClay *wgc = WorldGenClay_create(32);
        WorldGenerator_generate(&wgc->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenClay_free(wgc);
    }

    for (var13 = 0; var13 < 20; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 128);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_dirt->blockID, 32);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 10; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 128);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_gravel->blockID, 32);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 20; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 128);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreCoal->blockID, 16);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 20; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 64);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreIron->blockID, 8);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 2; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 32);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreGold->blockID, 8);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 8; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 16);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreRedstone->blockID, 7);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 1; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 16);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreDiamond->blockID, 7);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    for (var13 = 0; var13 < 1; var13++) {
        var14 = var4 + JavaRandom_nextInt(&self->rand, 16);
        var15 = JavaRandom_nextInt(&self->rand, 16) + JavaRandom_nextInt(&self->rand, 16);
        var16 = var5 + JavaRandom_nextInt(&self->rand, 16);
        WorldGenMinable *mg = WorldGenMinable_create(Block_oreLapis->blockID, 6);
        WorldGenerator_generate(&mg->base, self->worldObj, &self->rand, var14, var15, var16);
        WorldGenMinable_free(mg);
    }

    var11 = 0.5;
    var13 = (int)((NoiseGeneratorOctaves_sample2D(&self->mobSpawnerNoise, (double)var4 * var11,
                                                  (double)var5 * var11) /
                       8.0 +
                   JavaRandom_nextDouble(&self->rand) * 4.0 + 4.0) /
                  3.0);
    var14 = 0;
    if (JavaRandom_nextInt(&self->rand, 10) == 0)
        var14++;

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

    int var17;
    for (var15 = 0; var15 < var14; var15++) {
        var16 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var17 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;

        WorldGenerator *tgen = BiomeGenBase_getRandomWorldGenForTrees(var6, &self->rand);
        WorldGenerator_func_517_a(tgen, 1.0, 1.0, 1.0);
        WorldGenerator_generate(tgen, self->worldObj, &self->rand, var16,
                                World_getHeightValue(self->worldObj, var16, var17), var17);
        WorldGenerator_destroy(tgen);
    }

    uint8_t var27 = 0;
    if (var6 == &BiomeGenBase_biomes[BIOME_FOREST])
        var27 = 2;
    if (var6 == &BiomeGenBase_biomes[BIOME_SEASONAL_FOREST])
        var27 = 4;
    if (var6 == &BiomeGenBase_biomes[BIOME_TAIGA])
        var27 = 2;
    if (var6 == &BiomeGenBase_biomes[BIOME_PLAINS])
        var27 = 3;

    int var19, var25;
    for (var16 = 0; var16 < var27; var16++) {
        var17 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var25 = JavaRandom_nextInt(&self->rand, 128);
        var19 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenFlowers *wgfy = WorldGenFlowers_create(Block_plantYellow->blockID);
        WorldGenerator_generate(&wgfy->base, self->worldObj, &self->rand, var17, var25, var19);
        WorldGenFlowers_free(wgfy);
    }

    uint8_t var28 = 0;
    if (var6 == &BiomeGenBase_biomes[BIOME_FOREST])
        var28 = 2;
    if (var6 == &BiomeGenBase_biomes[BIOME_RAINFOREST])
        var28 = 10;
    if (var6 == &BiomeGenBase_biomes[BIOME_SEASONAL_FOREST])
        var28 = 2;
    if (var6 == &BiomeGenBase_biomes[BIOME_TAIGA])
        var28 = 1;
    if (var6 == &BiomeGenBase_biomes[BIOME_PLAINS])
        var28 = 10;

    int var20, var21;
    for (var17 = 0; var17 < var28; var17++) {
        uint8_t var26 = 1;
        if (var6 == &BiomeGenBase_biomes[BIOME_RAINFOREST] &&
            JavaRandom_nextInt(&self->rand, 3) != 0)
            var26 = 2;
        var19 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var20 = JavaRandom_nextInt(&self->rand, 128);
        var21 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenTallGrass *wgtg = WorldGenTallGrass_create(Block_tallGrass->blockID, var26);
        WorldGenerator_generate(&wgtg->base, self->worldObj, &self->rand, var19, var20, var21);
        WorldGenTallGrass_free(wgtg);
    }

    var28 = 0;
    if (var6 == &BiomeGenBase_biomes[BIOME_DESERT])
        var28 = 2;
    for (var17 = 0; var17 < var28; var17++) {
        var25 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var19 = JavaRandom_nextInt(&self->rand, 128);
        var20 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenDeadBush *wgdb = WorldGenDeadBush_create(Block_deadBush->blockID);
        WorldGenerator_generate(&wgdb->base, self->worldObj, &self->rand, var25, var19, var20);
        WorldGenDeadBush_free(wgdb);
    }

    if (JavaRandom_nextInt(&self->rand, 2) == 0) {
        var17 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var25 = JavaRandom_nextInt(&self->rand, 128);
        var19 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenFlowers *wgfr = WorldGenFlowers_create(Block_plantRed->blockID);
        WorldGenerator_generate(&wgfr->base, self->worldObj, &self->rand, var17, var25, var19);
        WorldGenFlowers_free(wgfr);
    }

    if (JavaRandom_nextInt(&self->rand, 4) == 0) {
        var17 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var25 = JavaRandom_nextInt(&self->rand, 128);
        var19 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenFlowers *wgfmb = WorldGenFlowers_create(Block_mushroomBrown->blockID);
        WorldGenerator_generate(&wgfmb->base, self->worldObj, &self->rand, var17, var25, var19);
        WorldGenFlowers_free(wgfmb);
    }

    if (JavaRandom_nextInt(&self->rand, 8) == 0) {
        var17 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var25 = JavaRandom_nextInt(&self->rand, 128);
        var19 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenFlowers *wgfmr = WorldGenFlowers_create(Block_mushroomRed->blockID);
        WorldGenerator_generate(&wgfmr->base, self->worldObj, &self->rand, var17, var25, var19);
        WorldGenFlowers_free(wgfmr);
    }

    for (var17 = 0; var17 < 10; var17++) {
        var25 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var19 = JavaRandom_nextInt(&self->rand, 128);
        var20 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenReed *wgr = WorldGenReed_create();
        WorldGenerator_generate(&wgr->base, self->worldObj, &self->rand, var25, var19, var20);
        WorldGenReed_free(wgr);
    }

    if (JavaRandom_nextInt(&self->rand, 32) == 0) {
        var17 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var25 = JavaRandom_nextInt(&self->rand, 128);
        var19 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenPumpkin *wgp = WorldGenPumpkin_create();
        WorldGenerator_generate(&wgp->base, self->worldObj, &self->rand, var17, var25, var19);
        WorldGenPumpkin_free(wgp);
    }

    var17 = 0;
    if (var6 == &BiomeGenBase_biomes[BIOME_DESERT])
        var17 += 10;
    for (var25 = 0; var25 < var17; var25++) {
        var19 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var20 = JavaRandom_nextInt(&self->rand, 128);
        var21 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenCactus *wgca = WorldGenCactus_create();
        WorldGenerator_generate(&wgca->base, self->worldObj, &self->rand, var19, var20, var21);
        WorldGenCactus_free(wgca);
    }

    for (var25 = 0; var25 < 50; var25++) {
        var19 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var20 = JavaRandom_nextInt(&self->rand, JavaRandom_nextInt(&self->rand, 120) + 8);
        var21 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenLiquids *wglw = WorldGenLiquids_create(Block_waterMoving->blockID);
        WorldGenerator_generate(&wglw->base, self->worldObj, &self->rand, var19, var20, var21);
        WorldGenLiquids_free(wglw);
    }

    for (var25 = 0; var25 < 20; var25++) {
        var19 = var4 + JavaRandom_nextInt(&self->rand, 16) + 8;
        var20 = JavaRandom_nextInt(
            &self->rand,
            JavaRandom_nextInt(&self->rand, JavaRandom_nextInt(&self->rand, 112) + 8) + 8);
        var21 = var5 + JavaRandom_nextInt(&self->rand, 16) + 8;
        WorldGenLiquids *wgll2 = WorldGenLiquids_create(Block_lavaMoving->blockID);
        WorldGenerator_generate(&wgll2->base, self->worldObj, &self->rand, var19, var20, var21);
        WorldGenLiquids_free(wgll2);
    }

    {
        WorldChunkManager *wcm2 = World_getWorldChunkManager(self->worldObj);
        self->generatedTemperatures = WorldChunkManager_getTemperatures(
            wcm2, self->generatedTemperatures, var4 + 8, var5 + 8, 16, 16);
        for (int var25 = var4 + 8; var25 < var4 + 8 + 16; var25++) {
            for (int var19 = var5 + 8; var19 < var5 + 8 + 16; var19++) {
                int var20 = var25 - (var4 + 8);
                int var21 = var19 - (var5 + 8);
                int var22 = World_findTopSolidBlock(self->worldObj, var25, var19);
                double var23 = self->generatedTemperatures[var20 * 16 + var21] -
                               (double)(var22 - 64) / 64.0 * 0.3;
                if (var23 < 0.5 && var22 > 0 && var22 < 128 &&
                    World_isAirBlock(self->worldObj, var25, var22, var19) &&
                    Material_getIsSolid(
                        World_getBlockMaterial(self->worldObj, var25, var22 - 1, var19)) &&
                    World_getBlockMaterial(self->worldObj, var25, var22 - 1, var19) !=
                        Material_ice) {
                    World_setBlockWithNotify(self->worldObj, var25, var22, var19,
                                             Block_snow->blockID);
                }
            }
        }
    }

    Block_sand->fallInstantly = 0;
}

static int cpg_chunkExists(IChunkProvider *self, int x, int z) {
    (void)self;
    (void)x;
    (void)z;

    return 1;
}
static Chunk *cpg_provideChunk(IChunkProvider *self, int x, int z) {
    return ChunkProviderGenerate_provideChunk((ChunkProviderGenerate *)self, x, z);
}
static Chunk *cpg_prepareChunk(IChunkProvider *self, int x, int z) {

    return ChunkProviderGenerate_provideChunk((ChunkProviderGenerate *)self, x, z);
}
static void cpg_populate(IChunkProvider *self, IChunkProvider *chunkProvider, int x, int z) {
    (void)chunkProvider;
    ChunkProviderGenerate_populate((ChunkProviderGenerate *)self, x, z);
}
static int cpg_saveChunks(IChunkProvider *self, int saveAll, IProgressUpdate *progress) {
    (void)self;
    (void)saveAll;
    (void)progress;

    return 1;
}
static int cpg_unload100OldestChunks(IChunkProvider *self) {
    (void)self;

    return 0;
}
static int cpg_canSave(IChunkProvider *self) {
    (void)self;

    return 1;
}
static const char *cpg_makeString(IChunkProvider *self) {
    (void)self;

    return "RandomLevelSource";
}
const IChunkProviderVtable ChunkProviderGenerate_vtable = {
    cpg_chunkExists, cpg_provideChunk,          cpg_prepareChunk, cpg_populate,
    cpg_saveChunks,  cpg_unload100OldestChunks, cpg_canSave,      cpg_makeString,
};

ChunkProviderGenerate *ChunkProviderGenerate_create(World *world, long seed) {
    ChunkProviderGenerate *self = (ChunkProviderGenerate *)calloc(1, sizeof(ChunkProviderGenerate));
    self->base.vtable = &ChunkProviderGenerate_vtable;
    self->worldObj = world;
    JavaRandom_setSeed(&self->rand, seed);

    NoiseGeneratorOctaves_init(&self->field_912_k, &self->rand, 16);
    NoiseGeneratorOctaves_init(&self->field_911_l, &self->rand, 16);
    NoiseGeneratorOctaves_init(&self->field_910_m, &self->rand, 8);
    NoiseGeneratorOctaves_init(&self->field_909_n, &self->rand, 4);
    NoiseGeneratorOctaves_init(&self->field_908_o, &self->rand, 4);
    NoiseGeneratorOctaves_init(&self->field_922_a, &self->rand, 10);
    NoiseGeneratorOctaves_init(&self->field_921_b, &self->rand, 16);
    NoiseGeneratorOctaves_init(&self->mobSpawnerNoise, &self->rand, 8);

    MapGenCaves *cg = MapGenCaves_create();
    self->caveGen = *cg;
    free(cg);

    BiomeGenBase_generateBiomeLookup();
    return self;
}

void ChunkProviderGenerate_free(ChunkProviderGenerate *self) {
    NoiseGeneratorOctaves_free(&self->field_912_k);
    NoiseGeneratorOctaves_free(&self->field_911_l);
    NoiseGeneratorOctaves_free(&self->field_910_m);
    NoiseGeneratorOctaves_free(&self->field_909_n);
    NoiseGeneratorOctaves_free(&self->field_908_o);
    NoiseGeneratorOctaves_free(&self->field_922_a);
    NoiseGeneratorOctaves_free(&self->field_921_b);
    NoiseGeneratorOctaves_free(&self->mobSpawnerNoise);
    free(self->field_4180_q);
    free(self->field_4185_d);
    free(self->field_4184_e);
    free(self->field_4183_f);
    free(self->field_4182_g);
    free(self->field_4181_h);
    free(self->generatedTemperatures);
    free(self);
}
