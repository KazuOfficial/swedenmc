#include "BlockFence.h"
#include "Material.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

BlockVtable BlockFence_defaultVtable;

void BlockFence_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->canPlaceBlockAt = BlockFence_canPlaceBlockAt;
    vt->getCollisionBoundingBoxFromPool = BlockFence_getCollisionBoundingBoxFromPool;
    vt->isOpaqueCube = BlockFence_isOpaqueCube;
    vt->renderAsNormalBlock = BlockFence_renderAsNormalBlock;
    vt->getRenderType = BlockFence_getRenderType;
}

void BlockFence_construct(BlockFence *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFence_initVtable(&BlockFence_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockFence_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_wood);
}

BlockFence *BlockFence_create(int id, int tex) {
    BlockFence *self = (BlockFence *)calloc(1, sizeof(BlockFence));
    BlockFence_construct(self, id, tex);
    return self;
}

int BlockFence_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    return World_getBlockId(var1, var2, var3 - 1, var4) == self->blockID
               ? 1
               : (!Material_isSolid(World_getBlockMaterial(var1, var2, var3 - 1, var4))
                      ? 0
                      : Block_defaultVtable.canPlaceBlockAt(self, var1, var2, var3, var4));
}

AxisAlignedBB *BlockFence_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                          int var3, int var4) {
    (void)self;
    (void)var1;
    return AxisAlignedBB_getBoundingBoxFromPool((double)var2, (double)var3, (double)var4,
                                                (double)(var2 + 1), (double)((float)var3 + 1.5F),
                                                (double)(var4 + 1));
}

int BlockFence_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockFence_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockFence_getRenderType(Block *self) {
    (void)self;
    return 11;
}
