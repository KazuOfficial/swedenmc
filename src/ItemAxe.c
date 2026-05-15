#include "ItemAxe.h"
#include "ItemTool.h"
#include "Block.h"
#include <stdlib.h>

Item *ItemAxe_create(int id, EnumToolMaterial mat) {
    static struct Block *eff[4];
    static int eff_init = 0;
    if (!eff_init) {
        eff[0] = Block_planks;
        eff[1] = Block_bookShelf;
        eff[2] = Block_wood;
        eff[3] = Block_chest;
        eff_init = 1;
    }
    return ItemTool_create(id, 3, mat, eff, 4);
}
