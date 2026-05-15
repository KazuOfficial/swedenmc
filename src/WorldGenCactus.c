#include "WorldGenCactus.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int cactus_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3, int par4,
                           int par5) {
    (void)base;

    for (int var6 = 0; var6 < 10; var6++) {
        int var7 = par3 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
        int var8 = par4 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
        int var9 = par5 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);

        if (World_isAirBlock(world, var7, var8, var9)) {
            int var10 = 1 + JavaRandom_nextInt(rand, JavaRandom_nextInt(rand, 3) + 1);

            for (int var11 = 0; var11 < var10; var11++) {
                if (Block_cactus->vtable->canBlockStay(Block_cactus, world, var7, var8 + var11,
                                                       var9)) {
                    World_setBlock(world, var7, var8 + var11, var9, Block_cactus->blockID);
                }
            }
        }
    }

    return 1;
}

WorldGenCactus *WorldGenCactus_create(void) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = cactus_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenCactus *self = (WorldGenCactus *)calloc(1, sizeof(WorldGenCactus));
    self->base.vtable = &vtable;
    return self;
}

void WorldGenCactus_free(WorldGenCactus *self) { free(self); }
