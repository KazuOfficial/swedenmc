#include "WorldGenTrees.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>
#include <math.h>

static int WorldGenTrees_generate(WorldGenerator *self, World *var1, JavaRandom *var2, int var3,
                                  int var4, int var5) {
    (void)self;
    int var6 = JavaRandom_nextInt(var2, 3) + 4;
    int var7 = 1;

    if (var4 >= 1 && var4 + var6 + 1 <= 128) {
        int var8;
        int var10;
        int var11;
        int var12;

        for (var8 = var4; var8 <= var4 + 1 + var6; ++var8) {
            int var9 = 1;
            if (var8 == var4)
                var9 = 0;
            if (var8 >= var4 + 1 + var6 - 2)
                var9 = 2;

            for (var10 = var3 - var9; var10 <= var3 + var9 && var7; ++var10) {
                for (var11 = var5 - var9; var11 <= var5 + var9 && var7; ++var11) {
                    if (var8 >= 0 && var8 < 128) {
                        var12 = World_getBlockId(var1, var10, var8, var11);
                        if (var12 != 0 && var12 != Block_leaves->blockID)
                            var7 = 0;
                    } else {
                        var7 = 0;
                    }
                }
            }
        }

        if (!var7) {
            return 0;
        } else {
            var8 = World_getBlockId(var1, var3, var4 - 1, var5);
            if ((var8 == Block_grass->blockID || var8 == Block_dirt->blockID) &&
                var4 < 128 - var6 - 1) {

                World_setBlock(var1, var3, var4 - 1, var5, Block_dirt->blockID);

                int var16;
                for (var16 = var4 - 3 + var6; var16 <= var4 + var6; ++var16) {
                    var10 = var16 - (var4 + var6);
                    var11 = 1 - var10 / 2;

                    for (var12 = var3 - var11; var12 <= var3 + var11; ++var12) {
                        int var13 = var12 - var3;

                        for (int var14 = var5 - var11; var14 <= var5 + var11; ++var14) {
                            int var15 = var14 - var5;
                            if ((abs(var13) != var11 || abs(var15) != var11 ||
                                 (JavaRandom_nextInt(var2, 2) != 0 && var10 != 0)) &&
                                !Block_opaqueCubeLookup[World_getBlockId(var1, var12, var16,
                                                                         var14)]) {
                                World_setBlock(var1, var12, var16, var14, Block_leaves->blockID);
                            }
                        }
                    }
                }

                for (var16 = 0; var16 < var6; ++var16) {
                    var10 = World_getBlockId(var1, var3, var4 + var16, var5);
                    if (var10 == 0 || var10 == Block_leaves->blockID) {
                        World_setBlock(var1, var3, var4 + var16, var5, Block_wood->blockID);
                    }
                }

                return 1;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }
}

static WorldGeneratorVtable WorldGenTrees_vtable = {
    WorldGenTrees_generate, WorldGenerator_noop_func_517_a, WorldGenerator_simple_destroy};

WorldGenTrees *WorldGenTrees_create(void) {
    WorldGenTrees *self = (WorldGenTrees *)calloc(1, sizeof(WorldGenTrees));
    self->base.vtable = &WorldGenTrees_vtable;
    return self;
}

void WorldGenTrees_free(WorldGenTrees *self) { free(self); }
