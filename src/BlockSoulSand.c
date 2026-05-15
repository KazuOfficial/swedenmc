#include "Block.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "Material.h"
#include <stdlib.h>

static AxisAlignedBB *BlockSoulSand_getCollisionBoundingBoxFromPool(Block *self, World *var1,
                                                                    int var2, int var3, int var4) {
    (void)self;
    (void)var1;
    float var5 = 2.0F / 16.0F;
    return AxisAlignedBB_getBoundingBoxFromPool(
        (double)var2, (double)var3, (double)var4, (double)(var2 + 1),
        (double)((float)(var3 + 1) - var5), (double)(var4 + 1));
}

static void BlockSoulSand_onEntityCollidedWithBlock(Block *self, World *var1, int var2, int var3,
                                                    int var4, Entity *var5) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    var5->motionX *= 0.4;
    var5->motionZ *= 0.4;
}

Block *BlockSoulSand_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getCollisionBoundingBoxFromPool = BlockSoulSand_getCollisionBoundingBoxFromPool;
        vtable.onEntityCollidedWithBlock = BlockSoulSand_onEntityCollidedWithBlock;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_sand);
    return self;
}
