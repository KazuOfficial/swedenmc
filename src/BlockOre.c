#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockOre_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)meta;
    (void)rand;
    if (self->blockID == Block_oreCoal->blockID)
        return Item_coal->shiftedIndex;
    if (self->blockID == Block_oreDiamond->blockID)
        return Item_diamond->shiftedIndex;
    if (self->blockID == Block_oreLapis->blockID)
        return Item_dyePowder->shiftedIndex;
    return self->blockID;
}

static int BlockOre_quantityDropped(Block *self, JavaRandom *rand) {
    if (self->blockID == Block_oreLapis->blockID)
        return 4 + JavaRandom_nextInt(rand, 5);
    return 1;
}

static int BlockOre_damageDropped(Block *self, int meta) {
    (void)meta;
    if (self->blockID == Block_oreLapis->blockID)
        return 4;
    return 0;
}

Block *BlockOre_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.idDropped = BlockOre_idDropped;
        vtable.quantityDropped = BlockOre_quantityDropped;
        vtable.damageDropped = BlockOre_damageDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_rock);
    return self;
}
