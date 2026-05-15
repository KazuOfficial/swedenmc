#include "ItemTool.h"
#include "Block.h"
#include "Material.h"

static int ItemPickaxe_canHarvestBlock(Item *self, struct Block *block) {
    ItemTool *t = (ItemTool *)self;
    if (block == Block_obsidian)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) == 3;
    if (block == Block_blockDiamond || block == Block_oreDiamond)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) >= 2;
    if (block == Block_blockGold || block == Block_oreGold)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) >= 2;
    if (block == Block_blockSteel || block == Block_oreIron)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) >= 1;
    if (block == Block_blockLapis || block == Block_oreLapis)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) >= 1;
    if (block == Block_oreRedstone || block == Block_oreRedstoneGlowing)
        return EnumToolMaterial_getHarvestLevel(t->toolMaterial) >= 2;
    return block->blockMaterial == Material_rock || block->blockMaterial == Material_iron;
}

Item *ItemPickaxe_create(int id, EnumToolMaterial mat) {
    static struct Block *eff[17];
    static int eff_init = 0;
    if (!eff_init) {
        eff[0] = Block_cobblestone;
        eff[1] = Block_stairDouble;
        eff[2] = Block_stairSingle;
        eff[3] = Block_stone;
        eff[4] = Block_sandStone;
        eff[5] = Block_cobblestoneMossy;
        eff[6] = Block_oreIron;
        eff[7] = Block_blockSteel;
        eff[8] = Block_oreCoal;
        eff[9] = Block_blockGold;
        eff[10] = Block_oreGold;
        eff[11] = Block_oreDiamond;
        eff[12] = Block_blockDiamond;
        eff[13] = Block_ice;
        eff[14] = Block_netherrack;
        eff[15] = Block_oreLapis;
        eff[16] = Block_blockLapis;
        eff_init = 1;
    }

    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemTool_initVtable(&vtable);
        vtable.canHarvestBlock = ItemPickaxe_canHarvestBlock;
        vi = 1;
    }

    Item *self = ItemTool_create(id, 2, mat, eff, 17);
    self->vtable = &vtable;
    return self;
}
