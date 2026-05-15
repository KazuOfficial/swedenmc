#include "Block.h"
#include "Material.h"
#include "Item.h"
#include "Entity.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockWeb_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockWeb_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockWeb_getRenderType(Block *self) {
    (void)self;
    return 1;
}

static AxisAlignedBB *BlockWeb_getCollisionBoundingBoxFromPool(Block *self, World *w, int x, int y,
                                                               int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}

static void BlockWeb_onEntityCollidedWithBlock(Block *self, World *world, int x, int y, int z,
                                               struct Entity *entity) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
    entity->isInWeb = 1;
}

static int BlockWeb_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Item_silk->shiftedIndex;
}

Block *BlockWeb_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockWeb_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockWeb_renderAsNormalBlock;
        vtable.getRenderType = BlockWeb_getRenderType;
        vtable.getCollisionBoundingBoxFromPool = BlockWeb_getCollisionBoundingBoxFromPool;
        vtable.onEntityCollidedWithBlock = BlockWeb_onEntityCollidedWithBlock;
        vtable.idDropped = BlockWeb_idDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_field_31068_A);
    return self;
}
