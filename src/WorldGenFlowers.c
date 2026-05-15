#include "WorldGenFlowers.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int flowers_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3,
                            int par4, int par5) {
    WorldGenFlowers *self = (WorldGenFlowers *)base;

    for (int var6 = 0; var6 < 64; var6++) {
        int var7 = par3 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
        int var8 = par4 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
        int var9 = par5 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
        Block *block = Block_blocksList[self->field_39_a];
        if (World_isAirBlock(world, var7, var8, var9) &&
            block->vtable->canBlockStay(block, world, var7, var8, var9)) {
            World_setBlock(world, var7, var8, var9, self->field_39_a);
        }
    }

    return 1;
}

WorldGenFlowers *WorldGenFlowers_create(int blockId) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = flowers_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenFlowers *self = (WorldGenFlowers *)calloc(1, sizeof(WorldGenFlowers));
    self->base.vtable = &vtable;
    self->field_39_a = blockId;
    return self;
}

void WorldGenFlowers_free(WorldGenFlowers *self) { free(self); }
