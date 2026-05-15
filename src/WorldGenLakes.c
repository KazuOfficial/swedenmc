#include "WorldGenLakes.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "EnumSkyBlock.h"
#include <stdlib.h>
#include <string.h>

static int lakes_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3, int par4,
                          int par5) {
    WorldGenLakes *self = (WorldGenLakes *)base;

    par3 -= 8;

    for (par5 -= 8; par4 > 0 && World_isAirBlock(world, par3, par4, par5); --par4) {
    }

    par4 -= 4;

    int var6[2048];
    memset(var6, 0, sizeof(var6));

    int var7 = JavaRandom_nextInt(rand, 4) + 4;

    for (int var8 = 0; var8 < var7; var8++) {
        double var9 = JavaRandom_nextDouble(rand) * 6.0 + 3.0;
        double var11 = JavaRandom_nextDouble(rand) * 4.0 + 2.0;
        double var13 = JavaRandom_nextDouble(rand) * 6.0 + 3.0;
        double var15 = JavaRandom_nextDouble(rand) * (16.0 - var9 - 2.0) + 1.0 + var9 / 2.0;
        double var17 = JavaRandom_nextDouble(rand) * (8.0 - var11 - 4.0) + 2.0 + var11 / 2.0;
        double var19 = JavaRandom_nextDouble(rand) * (16.0 - var13 - 2.0) + 1.0 + var13 / 2.0;

        for (int var21 = 1; var21 < 15; var21++) {
            for (int var22 = 1; var22 < 15; var22++) {
                for (int var23 = 1; var23 < 7; var23++) {
                    double var24 = ((double)var21 - var15) / (var9 / 2.0);
                    double var26 = ((double)var23 - var17) / (var11 / 2.0);
                    double var28 = ((double)var22 - var19) / (var13 / 2.0);
                    if (var24 * var24 + var26 * var26 + var28 * var28 < 1.0) {
                        var6[(var21 * 16 + var22) * 8 + var23] = 1;
                    }
                }
            }
        }
    }

    int var8b, var32, var10;
    int var33;
    for (var8b = 0; var8b < 16; var8b++) {
        for (var32 = 0; var32 < 16; var32++) {
            for (var10 = 0; var10 < 8; var10++) {
                var33 = !var6[(var8b * 16 + var32) * 8 + var10] &&
                        ((var8b < 15 && var6[((var8b + 1) * 16 + var32) * 8 + var10]) ||
                         (var8b > 0 && var6[((var8b - 1) * 16 + var32) * 8 + var10]) ||
                         (var32 < 15 && var6[(var8b * 16 + var32 + 1) * 8 + var10]) ||
                         (var32 > 0 && var6[(var8b * 16 + (var32 - 1)) * 8 + var10]) ||
                         (var10 < 7 && var6[(var8b * 16 + var32) * 8 + var10 + 1]) ||
                         (var10 > 0 && var6[(var8b * 16 + var32) * 8 + (var10 - 1)]));
                if (var33) {
                    Material *var12 =
                        World_getBlockMaterial(world, par3 + var8b, par4 + var10, par5 + var32);
                    if (var10 >= 4 && Material_getIsLiquid(var12)) {
                        return 0;
                    }
                    if (var10 < 4 && !Material_isSolid(var12) &&
                        World_getBlockId(world, par3 + var8b, par4 + var10, par5 + var32) !=
                            self->field_40_a) {
                        return 0;
                    }
                }
            }
        }
    }

    for (var8b = 0; var8b < 16; var8b++) {
        for (var32 = 0; var32 < 16; var32++) {
            for (var10 = 0; var10 < 8; var10++) {
                if (var6[(var8b * 16 + var32) * 8 + var10]) {
                    World_setBlock(world, par3 + var8b, par4 + var10, par5 + var32,
                                   var10 >= 4 ? 0 : self->field_40_a);
                }
            }
        }
    }

    for (var8b = 0; var8b < 16; var8b++) {
        for (var32 = 0; var32 < 16; var32++) {
            for (var10 = 4; var10 < 8; var10++) {
                if (var6[(var8b * 16 + var32) * 8 + var10] &&
                    World_getBlockId(world, par3 + var8b, par4 + var10 - 1, par5 + var32) ==
                        Block_dirt->blockID &&
                    World_getSavedLightValue(world, EnumSkyBlock_Sky, par3 + var8b, par4 + var10,
                                             par5 + var32) > 0) {
                    World_setBlock(world, par3 + var8b, par4 + var10 - 1, par5 + var32,
                                   Block_grass->blockID);
                }
            }
        }
    }

    if (Block_blocksList[self->field_40_a]->blockMaterial == Material_lava) {
        for (var8b = 0; var8b < 16; var8b++) {
            for (var32 = 0; var32 < 16; var32++) {
                for (var10 = 0; var10 < 8; var10++) {
                    var33 = !var6[(var8b * 16 + var32) * 8 + var10] &&
                            ((var8b < 15 && var6[((var8b + 1) * 16 + var32) * 8 + var10]) ||
                             (var8b > 0 && var6[((var8b - 1) * 16 + var32) * 8 + var10]) ||
                             (var32 < 15 && var6[(var8b * 16 + var32 + 1) * 8 + var10]) ||
                             (var32 > 0 && var6[(var8b * 16 + (var32 - 1)) * 8 + var10]) ||
                             (var10 < 7 && var6[(var8b * 16 + var32) * 8 + var10 + 1]) ||
                             (var10 > 0 && var6[(var8b * 16 + var32) * 8 + (var10 - 1)]));
                    if (var33 && (var10 < 4 || JavaRandom_nextInt(rand, 2) != 0) &&
                        Material_isSolid(World_getBlockMaterial(world, par3 + var8b, par4 + var10,
                                                                par5 + var32))) {
                        World_setBlock(world, par3 + var8b, par4 + var10, par5 + var32,
                                       Block_stone->blockID);
                    }
                }
            }
        }
    }

    return 1;
}

WorldGenLakes *WorldGenLakes_create(int blockId) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = lakes_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenLakes *self = (WorldGenLakes *)calloc(1, sizeof(WorldGenLakes));
    self->base.vtable = &vtable;
    self->field_40_a = blockId;
    return self;
}

void WorldGenLakes_free(WorldGenLakes *self) { free(self); }
