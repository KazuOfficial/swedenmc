#include "WorldGenLiquids.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int liquids_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3,
                            int par4, int par5) {
    WorldGenLiquids *self = (WorldGenLiquids *)base;

    if (World_getBlockId(world, par3, par4 + 1, par5) != Block_stone->blockID) {
        return 0;
    } else if (World_getBlockId(world, par3, par4 - 1, par5) != Block_stone->blockID) {
        return 0;
    } else if (World_getBlockId(world, par3, par4, par5) != 0 &&
               World_getBlockId(world, par3, par4, par5) != Block_stone->blockID) {
        return 0;
    } else {
        int var6 = 0;
        if (World_getBlockId(world, par3 - 1, par4, par5) == Block_stone->blockID) {
            ++var6;
        }
        if (World_getBlockId(world, par3 + 1, par4, par5) == Block_stone->blockID) {
            ++var6;
        }
        if (World_getBlockId(world, par3, par4, par5 - 1) == Block_stone->blockID) {
            ++var6;
        }
        if (World_getBlockId(world, par3, par4, par5 + 1) == Block_stone->blockID) {
            ++var6;
        }

        int var7 = 0;
        if (World_isAirBlock(world, par3 - 1, par4, par5)) {
            ++var7;
        }
        if (World_isAirBlock(world, par3 + 1, par4, par5)) {
            ++var7;
        }
        if (World_isAirBlock(world, par3, par4, par5 - 1)) {
            ++var7;
        }
        if (World_isAirBlock(world, par3, par4, par5 + 1)) {
            ++var7;
        }

        if (var6 == 3 && var7 == 1) {
            World_setBlockWithNotify(world, par3, par4, par5, self->field_36_a);
            world->scheduledUpdatesAreImmediate = 1;
            Block_updateTick(Block_blocksList[self->field_36_a], world, par3, par4, par5, rand);
            world->scheduledUpdatesAreImmediate = 0;
        }

        return 1;
    }
}

WorldGenLiquids *WorldGenLiquids_create(int blockId) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = liquids_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenLiquids *self = (WorldGenLiquids *)calloc(1, sizeof(WorldGenLiquids));
    self->base.vtable = &vtable;
    self->field_36_a = blockId;
    return self;
}

void WorldGenLiquids_free(WorldGenLiquids *self) { free(self); }
