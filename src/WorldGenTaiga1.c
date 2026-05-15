#include "WorldGenTaiga1.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <math.h>
#include <stdlib.h>

static int WorldGenTaiga1_generate(WorldGenerator *self, World *var1, JavaRandom *var2, int var3,
                                   int var4, int var5) {
    (void)self;
    int var6 = JavaRandom_nextInt(var2, 5) + 7;
    int var7 = var6 - JavaRandom_nextInt(var2, 2) - 3;
    int var8 = var6 - var7;
    int var9 = 1 + JavaRandom_nextInt(var2, var8 + 1);
    int var10 = 1;

    if (var4 >= 1 && var4 + var6 + 1 <= 128) {
        int var11, var13, var14, var15, var18;
        for (var11 = var4; var11 <= var4 + 1 + var6 && var10; ++var11) {
            if (var11 - var4 < var7)
                var18 = 0;
            else
                var18 = var9;

            for (var13 = var3 - var18; var13 <= var3 + var18 && var10; ++var13) {
                for (var14 = var5 - var18; var14 <= var5 + var18 && var10; ++var14) {
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
            var18 = 0;

            for (var13 = var4 + var6; var13 >= var4 + var7; --var13) {
                for (var14 = var3 - var18; var14 <= var3 + var18; ++var14) {
                    var15 = var14 - var3;

                    for (int var16 = var5 - var18; var16 <= var5 + var18; ++var16) {
                        int var17 = var16 - var5;
                        if ((abs(var15) != var18 || abs(var17) != var18 || var18 <= 0) &&
                            !Block_opaqueCubeLookup[World_getBlockId(var1, var14, var13, var16)]) {

                            World_setBlockAndMetadata(var1, var14, var13, var16,
                                                      Block_leaves->blockID, 1);
                        }
                    }
                }

                if (var18 >= 1 && var13 == var4 + var7 + 1)
                    --var18;
                else if (var18 < var9)
                    ++var18;
            }

            for (var13 = 0; var13 < var6 - 1; ++var13) {
                var14 = World_getBlockId(var1, var3, var4 + var13, var5);
                if (var14 == 0 || var14 == Block_leaves->blockID) {

                    World_setBlockAndMetadata(var1, var3, var4 + var13, var5, Block_wood->blockID,
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

static WorldGeneratorVtable WorldGenTaiga1_vtable = {
    WorldGenTaiga1_generate, WorldGenerator_noop_func_517_a, WorldGenerator_simple_destroy};

WorldGenTaiga1 *WorldGenTaiga1_create(void) {
    WorldGenTaiga1 *self = (WorldGenTaiga1 *)calloc(1, sizeof(WorldGenTaiga1));
    self->base.vtable = &WorldGenTaiga1_vtable;
    return self;
}

void WorldGenTaiga1_free(WorldGenTaiga1 *self) { free(self); }
