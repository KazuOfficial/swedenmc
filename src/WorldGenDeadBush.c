#include "WorldGenDeadBush.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int deadbush_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3,
                             int par4, int par5) {
    WorldGenDeadBush *self = (WorldGenDeadBush *)base;
    int var6 = 0;
    (void)var6;

    while (1) {
        int var11 = World_getBlockId(world, par3, par4, par5);
        if ((var11 != 0 && var11 != Block_leaves->blockID) || par4 <= 0) {
            for (int var7 = 0; var7 < 4; var7++) {
                int var8 = par3 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
                int var9 = par4 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
                int var10 = par5 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
                Block *block = Block_blocksList[self->field_28058_a];
                if (World_isAirBlock(world, var8, var9, var10) &&
                    block->vtable->canBlockStay(block, world, var8, var9, var10)) {
                    World_setBlock(world, var8, var9, var10, self->field_28058_a);
                }
            }
            return 1;
        }
        par4--;
    }
}

WorldGenDeadBush *WorldGenDeadBush_create(int blockId) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = deadbush_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenDeadBush *self = (WorldGenDeadBush *)calloc(1, sizeof(WorldGenDeadBush));
    self->base.vtable = &vtable;
    self->field_28058_a = blockId;
    return self;
}

void WorldGenDeadBush_free(WorldGenDeadBush *self) { free(self); }
