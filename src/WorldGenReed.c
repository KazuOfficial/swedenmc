#include "WorldGenReed.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include <stdlib.h>

static int reed_generate(WorldGenerator *base, World *world, JavaRandom *rand, int par3, int par4,
                         int par5) {
    (void)base;

    for (int var6 = 0; var6 < 20; ++var6) {
        int var7 = par3 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);
        int var8 = par4;
        int var9 = par5 + JavaRandom_nextInt(rand, 4) - JavaRandom_nextInt(rand, 4);

        if (World_isAirBlock(world, var7, par4, var9) &&
            (World_getBlockMaterial(world, var7 - 1, par4 - 1, var9) == Material_water ||
             World_getBlockMaterial(world, var7 + 1, par4 - 1, var9) == Material_water ||
             World_getBlockMaterial(world, var7, par4 - 1, var9 - 1) == Material_water ||
             World_getBlockMaterial(world, var7, par4 - 1, var9 + 1) == Material_water)) {
            int var10 = 2 + JavaRandom_nextInt(rand, JavaRandom_nextInt(rand, 3) + 1);

            for (int var11 = 0; var11 < var10; ++var11) {
                if (Block_reed->vtable->canBlockStay(Block_reed, world, var7, var8 + var11, var9)) {
                    World_setBlock(world, var7, var8 + var11, var9, Block_reed->blockID);
                }
            }
        }
    }

    return 1;
}

WorldGenReed *WorldGenReed_create(void) {
    static WorldGeneratorVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable.generate = reed_generate;
        vtable.func_517_a = WorldGenerator_noop_func_517_a;
        vtable.destroy = WorldGenerator_simple_destroy;
        vtable_init = 1;
    }
    WorldGenReed *self = (WorldGenReed *)calloc(1, sizeof(WorldGenReed));
    self->base.vtable = &vtable;
    return self;
}

void WorldGenReed_free(WorldGenReed *self) { free(self); }
