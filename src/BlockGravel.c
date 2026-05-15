#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

extern BlockVtable BlockSand_vtable;
extern void BlockSand_initVtable(void);

static int BlockGravel_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)meta;
    if (JavaRandom_nextInt(rand, 10) == 0)
        return Item_flint->shiftedIndex;
    return self->blockID;
}

Block *BlockGravel_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockSand_initVtable();
        vtable = BlockSand_vtable;
        vtable.idDropped = BlockGravel_idDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_sand);
    return self;
}
