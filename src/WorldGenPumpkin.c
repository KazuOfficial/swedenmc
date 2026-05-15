#include "WorldGenPumpkin.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int pumpkin_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3,
                            int par4, int par5) {
    (void)base;

    for (int var6 = 0; var6 < 64; var6++) {
        int var7 = par3 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
        int var8 = par4 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
        int var9 = par5 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);

        if (World_isAirBlock(world, var7, var8, var9) &&
            World_getBlockId(world, var7, var8 - 1, var9) == Block_grass->blockID &&
            Block_pumpkin->vtable->canPlaceBlockAt(Block_pumpkin, world, var7, var8, var9)) {
            World_setBlockAndMetadata(world, var7, var8, var9, Block_pumpkin->blockID,
                                      JavaRandom_nextInt(rand, 4));
        }
    }

    return 1;
}

WorldGenPumpkin *WorldGenPumpkin_create(void) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = pumpkin_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenPumpkin *self = (WorldGenPumpkin *)calloc(1, sizeof(WorldGenPumpkin));
    self->base.vtable = &vtable;
    return self;
}

void WorldGenPumpkin_free(WorldGenPumpkin *self) { free(self); }
