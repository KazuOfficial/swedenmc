#include "Block.h"
#include "World.h"
#include "Material.h"
#include "Item.h"
#include "JavaRandom.h"
#include "EnumSkyBlock.h"
#include <stdlib.h>

static int BlockSnowBlock_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Item_snowball->shiftedIndex;
}

static int BlockSnowBlock_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 4;
}

static void BlockSnowBlock_updateTick(Block *self, World *world, int x, int y, int z,
                                      JavaRandom *rand) {
    (void)rand;
    if (World_getSavedLightValue(world, EnumSkyBlock_Block, x, y, z) > 11) {
        Block_dropBlockAsItem(self, world, x, y, z, World_getBlockMetadata(world, x, y, z));
        World_setBlockWithNotify(world, x, y, z, 0);
    }
}

Block *BlockSnowBlock_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.idDropped = BlockSnowBlock_idDropped;
        vtable.quantityDropped = BlockSnowBlock_quantityDropped;
        vtable.updateTick = BlockSnowBlock_updateTick;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_builtSnow);
    Block_setTickOnLoad(self, 1);
    return self;
}
