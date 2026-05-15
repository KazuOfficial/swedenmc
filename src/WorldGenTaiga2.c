#include "WorldGenTaiga2.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <math.h>
#include <stdlib.h>

static int WorldGenTaiga2_generate(WorldGenerator *self, World *var1, JavaRandom *var2, int var3,
                                   int var4, int var5) {
    (void)self;
    int var6 = JavaRandom_nextInt(var2, 4) + 6;
    int var7 = 1 + JavaRandom_nextInt(var2, 2);
    int var8 = var6 - var7;
    int var9 = 2 + JavaRandom_nextInt(var2, 2);
    int var10 = 1;

    if (var4 >= 1 && var4 + var6 + 1 <= 128) {
        int var11, var13, var15, var21;
        for (var11 = var4; var11 <= var4 + 1 + var6 && var10; ++var11) {
            if (var11 - var4 < var7)
                var21 = 0;
            else
                var21 = var9;

            for (var13 = var3 - var21; var13 <= var3 + var21 && var10; ++var13) {
                for (int var14 = var5 - var21; var14 <= var5 + var21 && var10; ++var14) {
                    if (var11 >= 0 && var11 < 128) {
                        var15 = World_getBlockId(var1, var13, var11, var14);
                        if (var15 != 0 && var15 != Block_leaves->blockID)
                            var10 = 0;
                    } else {
                        var10 = 0;
                    }
                }
            }
        }

        if (!var10)
            return 0;

        var11 = World_getBlockId(var1, var3, var4 - 1, var5);
        if ((var11 == Block_grass->blockID || var11 == Block_dirt->blockID) &&
            var4 < 128 - var6 - 1) {

            World_setBlock(var1, var3, var4 - 1, var5, Block_dirt->blockID);
            var21 = JavaRandom_nextInt(var2, 2);
            var13 = 1;
            int var22 = 0;

            int var16, var17;
            for (var15 = 0; var15 <= var8; ++var15) {
                var16 = var4 + var6 - var15;

                for (var17 = var3 - var21; var17 <= var3 + var21; ++var17) {
                    int var18 = var17 - var3;

                    for (int var19 = var5 - var21; var19 <= var5 + var21; ++var19) {
                        int var20 = var19 - var5;
                        if ((abs(var18) != var21 || abs(var20) != var21 || var21 <= 0) &&
                            !Block_opaqueCubeLookup[World_getBlockId(var1, var17, var16, var19)]) {

                            World_setBlockAndMetadata(var1, var17, var16, var19,
                                                      Block_leaves->blockID, 1);
                        }
                    }
                }

                if (var21 >= var13) {
                    var21 = var22;
                    var22 = 1;
                    ++var13;
                    if (var13 > var9)
                        var13 = var9;
                } else {
                    ++var21;
                }
            }

            var15 = JavaRandom_nextInt(var2, 3);
            for (var16 = 0; var16 < var6 - var15; ++var16) {
                var17 = World_getBlockId(var1, var3, var4 + var16, var5);
                if (var17 == 0 || var17 == Block_leaves->blockID) {

                    World_setBlockAndMetadata(var1, var3, var4 + var16, var5, Block_wood->blockID,
                                              1);
                }
            }

            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

static WorldGeneratorVtable WorldGenTaiga2_vtable = {
    WorldGenTaiga2_generate, WorldGenerator_noop_func_517_a, WorldGenerator_simple_destroy};

WorldGenTaiga2 *WorldGenTaiga2_create(void) {
    WorldGenTaiga2 *self = (WorldGenTaiga2 *)calloc(1, sizeof(WorldGenTaiga2));
    self->base.vtable = &WorldGenTaiga2_vtable;
    return self;
}

void WorldGenTaiga2_free(WorldGenTaiga2 *self) { free(self); }
