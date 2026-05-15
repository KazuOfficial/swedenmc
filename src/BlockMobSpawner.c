#include "Block.h"
#include "BlockContainer.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "TileEntityMobSpawner.h"
#include <stdlib.h>

static int BlockMobSpawner_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static void BlockMobSpawner_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    World_setBlockTileEntity(world, x, y, z, (TileEntity *)TileEntityMobSpawner_create());
}
static int BlockMobSpawner_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return 0;
}
static int BlockMobSpawner_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}

Block *BlockMobSpawner_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable);
        vtable.isOpaqueCube = BlockMobSpawner_isOpaqueCube;
        vtable.onBlockAdded = BlockMobSpawner_onBlockAdded;
        vtable.idDropped = BlockMobSpawner_idDropped;
        vtable.quantityDropped = BlockMobSpawner_quantityDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    BlockContainer_constructWithTexture(self, id, tex, Material_rock);
    return self;
}
