#include "WorldGenClay.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>

static int clay_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3, int par4,
                         int par5) {
    WorldGenClay *self = (WorldGenClay *)base;

    if (World_getBlockMaterial(world, par3, par4, par5) != Material_water) {
        return 0;
    }

    float var6 = JavaRandom_nextFloat(rand) * (float)M_PI;
    double var7 =
        (double)((float)(par3 + 8) + MathHelper_sin(var6) * (float)self->field_35_a / 8.0f);
    double var9 =
        (double)((float)(par3 + 8) - MathHelper_sin(var6) * (float)self->field_35_a / 8.0f);
    double var11 =
        (double)((float)(par5 + 8) + MathHelper_cos(var6) * (float)self->field_35_a / 8.0f);
    double var13 =
        (double)((float)(par5 + 8) - MathHelper_cos(var6) * (float)self->field_35_a / 8.0f);
    double var15 = (double)(par4 + JavaRandom_nextInt(rand, 3) + 2);
    double var17 = (double)(par4 + JavaRandom_nextInt(rand, 3) + 2);

    for (int var19 = 0; var19 <= self->field_35_a; var19++) {
        double var20 = var7 + (var9 - var7) * (double)var19 / (double)self->field_35_a;
        double var22 = var15 + (var17 - var15) * (double)var19 / (double)self->field_35_a;
        double var24 = var11 + (var13 - var11) * (double)var19 / (double)self->field_35_a;
        double var26 = JavaRandom_nextDouble(rand) * (double)self->field_35_a / 16.0;
        double var28 =
            (double)(MathHelper_sin((float)var19 * (float)M_PI / (float)self->field_35_a) + 1.0f) *
                var26 +
            1.0;
        double var30 =
            (double)(MathHelper_sin((float)var19 * (float)M_PI / (float)self->field_35_a) + 1.0f) *
                var26 +
            1.0;

        int var32 = MathHelper_floor_double(var20 - var28 / 2.0);
        int var33 = MathHelper_floor_double(var20 + var28 / 2.0);
        int var34 = MathHelper_floor_double(var22 - var30 / 2.0);
        int var35 = MathHelper_floor_double(var22 + var30 / 2.0);
        int var36 = MathHelper_floor_double(var24 - var28 / 2.0);
        int var37 = MathHelper_floor_double(var24 + var28 / 2.0);

        for (int var38 = var32; var38 <= var33; var38++) {
            for (int var39 = var34; var39 <= var35; var39++) {
                for (int var40 = var36; var40 <= var37; var40++) {
                    double var41 = ((double)var38 + 0.5 - var20) / (var28 / 2.0);
                    double var43 = ((double)var39 + 0.5 - var22) / (var30 / 2.0);
                    double var45 = ((double)var40 + 0.5 - var24) / (var28 / 2.0);
                    if (var41 * var41 + var43 * var43 + var45 * var45 < 1.0) {
                        int var47 = World_getBlockId(world, var38, var39, var40);
                        if (var47 == Block_sand->blockID) {
                            World_setBlock(world, var38, var39, var40, Block_blockClay->blockID);
                        }
                    }
                }
            }
        }
    }

    return 1;
}

WorldGenClay *WorldGenClay_create(int numberOfBlocks) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = clay_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenClay *self = (WorldGenClay *)calloc(1, sizeof(WorldGenClay));
    self->base.vtable = &vtable;
    self->field_35_a = numberOfBlocks;
    return self;
}

void WorldGenClay_free(WorldGenClay *self) { free(self); }
