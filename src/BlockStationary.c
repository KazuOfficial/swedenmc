#include "BlockFluid.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

static void BlockStationary_func_30004_j(Block *self, World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    var1->editingBlocks = 1;
    World_setBlockAndMetadata(var1, var2, var3, var4, self->blockID - 1, var5);
    World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
    World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID - 1,
                              self->vtable->tickRate(self));
    var1->editingBlocks = 0;
}

static void BlockStationary_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                                  int var4, int var5) {
    BlockFluid_onNeighborBlockChange(self, var1, var2, var3, var4, var5);
    if (World_getBlockId(var1, var2, var3, var4) == self->blockID) {
        BlockStationary_func_30004_j(self, var1, var2, var3, var4);
    }
}

static int BlockStationary_func_301_k(World *var1, int var2, int var3, int var4) {
    return Material_getBurning(World_getBlockMaterial(var1, var2, var3, var4));
}

static void BlockStationary_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                       JavaRandom *var5) {
    if (self->blockMaterial == Material_lava) {
        int var6 = JavaRandom_nextInt(var5, 3);
        for (int var7 = 0; var7 < var6; ++var7) {
            var2 += JavaRandom_nextInt(var5, 3) - 1;
            ++var3;
            var4 += JavaRandom_nextInt(var5, 3) - 1;
            int var8 = World_getBlockId(var1, var2, var3, var4);
            if (var8 == 0) {
                if (BlockStationary_func_301_k(var1, var2 - 1, var3, var4) ||
                    BlockStationary_func_301_k(var1, var2 + 1, var3, var4) ||
                    BlockStationary_func_301_k(var1, var2, var3, var4 - 1) ||
                    BlockStationary_func_301_k(var1, var2, var3, var4 + 1) ||
                    BlockStationary_func_301_k(var1, var2, var3 - 1, var4) ||
                    BlockStationary_func_301_k(var1, var2, var3 + 1, var4)) {
                    World_setBlockWithNotify(var1, var2, var3, var4, Block_fire->blockID);
                    return;
                }
            } else if (Material_getIsSolid(Block_blocksList[var8]->blockMaterial)) {
                return;
            }
        }
    }
}

Block *BlockStationary_create(int id, Material *mat) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFluid_initVtable(&vtable);
        vtable.onNeighborBlockChange = BlockStationary_onNeighborBlockChange;
        vtable.updateTick = BlockStationary_updateTick;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    int tex = (mat == Material_lava ? 14 : 12) * 16 + 13;
    Block_constructWithTexture(self, id, tex, mat);
    Block_setTickOnLoad(self, (mat == Material_lava) ? 1 : 0);
    return self;
}
