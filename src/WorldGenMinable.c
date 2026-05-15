#include "WorldGenMinable.h"
#include "MathHelper.h"
#include "Block.h"
#include <stdlib.h>
#include <math.h>

static int minable_generate(WorldGenerator *base, World *world, JavaRandom *rand, int x, int y,
                            int z) {
    WorldGenMinable *self = (WorldGenMinable *)base;
    float var6 = JavaRandom_nextFloat(rand) * (float)M_PI;
    double var7 =
        (double)((float)(x + 8) + MathHelper_sin(var6) * (float)self->numberOfBlocks / 8.0f);
    double var9 =
        (double)((float)(x + 8) - MathHelper_sin(var6) * (float)self->numberOfBlocks / 8.0f);
    double var11 =
        (double)((float)(z + 8) + MathHelper_cos(var6) * (float)self->numberOfBlocks / 8.0f);
    double var13 =
        (double)((float)(z + 8) - MathHelper_cos(var6) * (float)self->numberOfBlocks / 8.0f);
    double var15 = (double)(y + JavaRandom_nextInt(rand, 3) + 2);
    double var17 = (double)(y + JavaRandom_nextInt(rand, 3) + 2);

    for (int var19 = 0; var19 <= self->numberOfBlocks; ++var19) {
        double var20 = var7 + (var9 - var7) * (double)var19 / (double)self->numberOfBlocks;
        double var22 = var15 + (var17 - var15) * (double)var19 / (double)self->numberOfBlocks;
        double var24 = var11 + (var13 - var11) * (double)var19 / (double)self->numberOfBlocks;
        double var26 = JavaRandom_nextDouble(rand) * (double)self->numberOfBlocks / 16.0;
        double var28 =
            (double)(MathHelper_sin((float)var19 * (float)M_PI / (float)self->numberOfBlocks) +
                     1.0f) *
                var26 +
            1.0;
        double var30 =
            (double)(MathHelper_sin((float)var19 * (float)M_PI / (float)self->numberOfBlocks) +
                     1.0f) *
                var26 +
            1.0;
        int var32 = MathHelper_floor_double(var20 - var28 / 2.0);
        int var33 = MathHelper_floor_double(var22 - var30 / 2.0);
        int var34 = MathHelper_floor_double(var24 - var28 / 2.0);
        int var35 = MathHelper_floor_double(var20 + var28 / 2.0);
        int var36 = MathHelper_floor_double(var22 + var30 / 2.0);
        int var37 = MathHelper_floor_double(var24 + var28 / 2.0);

        for (int var38 = var32; var38 <= var35; ++var38) {
            double var39 = ((double)var38 + 0.5 - var20) / (var28 / 2.0);
            if (var39 * var39 < 1.0) {
                for (int var41 = var33; var41 <= var36; ++var41) {
                    double var42 = ((double)var41 + 0.5 - var22) / (var30 / 2.0);
                    if (var39 * var39 + var42 * var42 < 1.0) {
                        for (int var44 = var34; var44 <= var37; ++var44) {
                            double var45 = ((double)var44 + 0.5 - var24) / (var28 / 2.0);
                            if (var39 * var39 + var42 * var42 + var45 * var45 < 1.0 &&
                                World_getBlockId(world, var38, var41, var44) ==
                                    Block_stone->blockID) {
                                World_setBlock(world, var38, var41, var44, self->minableBlockId);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}

static WorldGeneratorVtable minable_vtable = {minable_generate, WorldGenerator_noop_func_517_a,
                                              WorldGenerator_simple_destroy};

WorldGenMinable *WorldGenMinable_create(int blockID, int count) {
    WorldGenMinable *self = (WorldGenMinable *)calloc(1, sizeof(WorldGenMinable));
    self->base.vtable = &minable_vtable;
    self->base.scaleX = self->base.scaleY = self->base.scaleZ = 1.0;
    self->minableBlockId = blockID;
    self->numberOfBlocks = count;
    return self;
}

void WorldGenMinable_free(WorldGenMinable *self) { free(self); }
