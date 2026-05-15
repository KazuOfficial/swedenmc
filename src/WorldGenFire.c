#include "WorldGenFire.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int WorldGenFire_generate(WorldGenerator *self, World *var1, JavaRandom *var2, int var3,
                                 int var4, int var5) {
    (void)self;
    for (int var6 = 0; var6 < 64; ++var6) {
        int var7 = var3 + JavaRandom_nextInt(var2, 8) - JavaRandom_nextInt(var2, 8);
        int var8 = var4 + JavaRandom_nextInt(var2, 4) - JavaRandom_nextInt(var2, 4);
        int var9 = var5 + JavaRandom_nextInt(var2, 8) - JavaRandom_nextInt(var2, 8);
        if (World_isAirBlock(var1, var7, var8, var9) &&
            World_getBlockId(var1, var7, var8 - 1, var9) == Block_netherrack->blockID) {
            World_setBlockWithNotify(var1, var7, var8, var9, Block_fire->blockID);
        }
    }
    return 1;
}

static WorldGeneratorVtable WorldGenFire_vtable = {
    WorldGenFire_generate,
    WorldGenerator_noop_func_517_a,
    WorldGenerator_simple_destroy,
};

WorldGenFire *WorldGenFire_create(void) {
    WorldGenFire *self = (WorldGenFire *)calloc(1, sizeof(WorldGenFire));
    self->base.vtable = &WorldGenFire_vtable;
    return self;
}
