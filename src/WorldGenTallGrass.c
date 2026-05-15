#include "WorldGenTallGrass.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>

static int tallgrass_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3,
                              int par4, int par5) {
    WorldGenTallGrass *self = (WorldGenTallGrass *)base;
    int var6 = 0;
    (void)var6;

    while (1) {
        int var11 = World_getBlockId(world, par3, par4, par5);
        if ((var11 != 0 && var11 != Block_leaves->blockID) || par4 <= 0) {
            for (int var7 = 0; var7 < 128; var7++) {
                int var8 = par3 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
                int var9 = par4 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
                int var10 = par5 + JavaRandom_nextInt(rand, 8) - JavaRandom_nextInt(rand, 8);
                Block *block = Block_blocksList[self->field_44_a];
                if (World_isAirBlock(world, var8, var9, var10) &&
                    block->vtable->canBlockStay(block, world, var8, var9, var10)) {
                    World_setBlockAndMetadata(world, var8, var9, var10, self->field_44_a,
                                              self->field_43_b);
                }
            }
            return 1;
        }
        par4--;
    }
}

WorldGenTallGrass *WorldGenTallGrass_create(int blockId, int meta) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = tallgrass_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenTallGrass *self = (WorldGenTallGrass *)calloc(1, sizeof(WorldGenTallGrass));
    self->base.vtable = &vtable;
    self->field_44_a = blockId;
    self->field_43_b = meta;
    return self;
}

void WorldGenTallGrass_free(WorldGenTallGrass *self) { free(self); }
