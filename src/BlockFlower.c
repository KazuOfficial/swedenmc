#include "Block.h"
#include "BlockFlower.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockFlower_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

static int BlockFlower_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static int BlockFlower_getRenderType(Block *self) {
    (void)self;
    return 1;
}

static AxisAlignedBB *BlockFlower_getCollisionBoundingBoxFromPool(Block *self, World *w, int x,
                                                                  int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}

static int BlockFlower_canThisPlantGrowOnThisBlockID(Block *self, int var1) {
    (void)self;
    return var1 == Block_grass->blockID || var1 == Block_dirt->blockID ||
           var1 == Block_tilledField->blockID;
}

static int BlockFlower_canPlaceBlockAt(Block *self, World *world, int x, int y, int z) {
    return Block_defaultVtable.canPlaceBlockAt(self, world, x, y, z) &&
           self->vtable->canThisPlantGrowOnThisBlockID(self, World_getBlockId(world, x, y - 1, z));
}

static int BlockFlower_canBlockStay(Block *self, World *world, int x, int y, int z) {
    return (world->vtable->getFullBlockLightValue(world, x, y, z) >= 8 ||
            World_canBlockSeeTheSky(world, x, y, z)) &&
           self->vtable->canThisPlantGrowOnThisBlockID(self, World_getBlockId(world, x, y - 1, z));
}

static void BlockFlower_func_268_h(Block *self, World *world, int x, int y, int z) {
    if (!self->vtable->canBlockStay(self, world, x, y, z)) {
        Block_dropBlockAsItem(self, world, x, y, z, World_getBlockMetadata(world, x, y, z));
        World_setBlockWithNotify(world, x, y, z, 0);
    }
}

static void BlockFlower_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                              int neighborID) {
    Block_defaultVtable.onNeighborBlockChange(self, world, x, y, z, neighborID);
    BlockFlower_func_268_h(self, world, x, y, z);
}

void BlockFlower_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand) {
    (void)rand;
    BlockFlower_func_268_h(self, world, x, y, z);
}

void BlockFlower_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->renderAsNormalBlock = BlockFlower_renderAsNormalBlock;
    vt->isOpaqueCube = BlockFlower_isOpaqueCube;
    vt->getRenderType = BlockFlower_getRenderType;
    vt->getCollisionBoundingBoxFromPool = BlockFlower_getCollisionBoundingBoxFromPool;
    vt->canThisPlantGrowOnThisBlockID = BlockFlower_canThisPlantGrowOnThisBlockID;
    vt->canPlaceBlockAt = BlockFlower_canPlaceBlockAt;
    vt->onNeighborBlockChange = BlockFlower_onNeighborBlockChange;
    vt->updateTick = BlockFlower_updateTick;
    vt->canBlockStay = BlockFlower_canBlockStay;
}

Block *BlockFlower_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFlower_initVtable(&vtable);
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_plants);
    self->blockIndexInTexture = tex;
    Block_setTickOnLoad(self, 1);

    float var3 = 0.2f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, var3 * 3.0f,
                         0.5f + var3);
    return self;
}
