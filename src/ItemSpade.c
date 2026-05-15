#include "ItemTool.h"
#include "Block.h"
#include <stdlib.h>

static int ItemSpade_canHarvestBlock(Item *self, struct Block *block) {
    (void)self;
    return block == Block_snow || block == Block_blockSnow;
}

Item *ItemSpade_create(int id, EnumToolMaterial mat) {
    static struct Block *eff[8];
    static int eff_init = 0;
    if (!eff_init) {
        eff[0] = Block_grass;
        eff[1] = Block_dirt;
        eff[2] = Block_sand;
        eff[3] = Block_gravel;
        eff[4] = Block_snow;
        eff[5] = Block_blockSnow;
        eff[6] = Block_blockClay;
        eff[7] = Block_tilledField;
        eff_init = 1;
    }

    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemTool_initVtable(&vtable);
        vtable.canHarvestBlock = ItemSpade_canHarvestBlock;
        vi = 1;
    }

    Item *self = ItemTool_create(id, 1, mat, eff, 8);
    self->vtable = &vtable;
    ((ItemTool *)self)->damageVsEntity = 1 + EnumToolMaterial_getDamageVsEntity(mat);
    return self;
}
