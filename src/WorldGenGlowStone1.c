#include "WorldGenGlowStone1.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int WorldGenGlowStone1_generate(WorldGenerator *self, World *var1, JavaRandom *var2,
                                       int var3, int var4, int var5) {
    (void)self;
    if (!World_isAirBlock(var1, var3, var4, var5))
        return 0;
    if (World_getBlockId(var1, var3, var4 + 1, var5) != Block_glowStone->blockID)
        return 0;

    World_setBlockWithNotify(var1, var3, var4, var5, Block_glowStone->blockID);

    for (int var6 = 0; var6 < 1500; ++var6) {
        int var7 = var3 + JavaRandom_nextInt(var2, 8) - JavaRandom_nextInt(var2, 8);
        int var8 = var4 - JavaRandom_nextInt(var2, 12);
        int var9 = var5 + JavaRandom_nextInt(var2, 8) - JavaRandom_nextInt(var2, 8);
        if (World_getBlockId(var1, var7, var8, var9) == 0) {
            int var10 = 0;
            for (int var11 = 0; var11 < 6; ++var11) {
                int var12 = 0;
                if (var11 == 0)
                    var12 = World_getBlockId(var1, var7 - 1, var8, var9);
                if (var11 == 1)
                    var12 = World_getBlockId(var1, var7 + 1, var8, var9);
                if (var11 == 2)
                    var12 = World_getBlockId(var1, var7, var8 - 1, var9);
                if (var11 == 3)
                    var12 = World_getBlockId(var1, var7, var8 + 1, var9);
                if (var11 == 4)
                    var12 = World_getBlockId(var1, var7, var8, var9 - 1);
                if (var11 == 5)
                    var12 = World_getBlockId(var1, var7, var8, var9 + 1);
                if (var12 == Block_glowStone->blockID)
                    ++var10;
            }
            if (var10 == 1) {
                World_setBlockWithNotify(var1, var7, var8, var9, Block_glowStone->blockID);
            }
        }
    }
    return 1;
}

static WorldGeneratorVtable WorldGenGlowStone1_vtable = {
    WorldGenGlowStone1_generate,
    WorldGenerator_noop_func_517_a,
    WorldGenerator_simple_destroy,
};

WorldGenGlowStone1 *WorldGenGlowStone1_create(void) {
    WorldGenGlowStone1 *self = (WorldGenGlowStone1 *)calloc(1, sizeof(WorldGenGlowStone1));
    self->base.vtable = &WorldGenGlowStone1_vtable;
    return self;
}
