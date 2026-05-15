#include "Block.h"
#include "Material.h"
#include "World.h"
#include <stdlib.h>

static void BlockSponge_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    int r = 2;
    for (int i = x - r; i <= x + r; ++i) {
        for (int j = y - r; j <= y + r; ++j) {
            for (int k = z - r; k <= z + r; ++k) {
                if (World_getBlockMaterial(world, i, j, k) == Material_water) {
                }
            }
        }
    }
}

static void BlockSponge_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    (void)self;
    int r = 2;
    for (int i = x - r; i <= x + r; ++i) {
        for (int j = y - r; j <= y + r; ++j) {
            for (int k = z - r; k <= z + r; ++k) {
                World_notifyBlocksOfNeighborChange(world, i, j, k,
                                                   World_getBlockId(world, i, j, k));
            }
        }
    }
}

Block *BlockSponge_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.onBlockAdded = BlockSponge_onBlockAdded;
        vtable.onBlockRemoval = BlockSponge_onBlockRemoval;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_sponge);
    self->blockIndexInTexture = 48;
    return self;
}
