#include "Block.h"
#include "BlockFlower.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include <stdlib.h>

static int BlockMushroom_canThisPlantGrowOnThisBlockID(Block *self, int var1) {
    (void)self;
    return Block_opaqueCubeLookup[var1];
}

static int BlockMushroom_canBlockStay(Block *self, World *world, int x, int y, int z) {
    if (y < 0 || y >= 128)
        return 0;
    return world->vtable->getFullBlockLightValue(world, x, y, z) < 13 &&
           self->vtable->canThisPlantGrowOnThisBlockID(self, World_getBlockId(world, x, y - 1, z));
}

static void BlockMushroom_updateTick(Block *self, World *world, int x, int y, int z,
                                     JavaRandom *rand) {
    if (JavaRandom_nextInt(rand, 100) == 0) {
        int var6 = x + JavaRandom_nextInt(rand, 3) - 1;
        int var7 = y + JavaRandom_nextInt(rand, 2) - JavaRandom_nextInt(rand, 2);
        int var8 = z + JavaRandom_nextInt(rand, 3) - 1;
        if (World_isAirBlock(world, var6, var7, var8) &&
            self->vtable->canBlockStay(self, world, var6, var7, var8)) {
            int var10000 = x + (JavaRandom_nextInt(rand, 3) - 1);
            var10000 = z + (JavaRandom_nextInt(rand, 3) - 1);
            (void)var10000;
            if (World_isAirBlock(world, var6, var7, var8) &&
                self->vtable->canBlockStay(self, world, var6, var7, var8)) {
                World_setBlockWithNotify(world, var6, var7, var8, self->blockID);
            }
        }
    }
}

Block *BlockMushroom_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFlower_initVtable(&vtable);
        vtable.canThisPlantGrowOnThisBlockID = BlockMushroom_canThisPlantGrowOnThisBlockID;
        vtable.canBlockStay = BlockMushroom_canBlockStay;
        vtable.updateTick = BlockMushroom_updateTick;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_plants);
    self->blockIndexInTexture = tex;
    Block_setTickOnLoad(self, 1);
    float var3 = 0.2f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, var3 * 2.0f,
                         0.5f + var3);
    return self;
}
