#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "MovingObjectPosition.h"
#include "World.h"
#include <stdlib.h>

static int BlockTorch_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockTorch_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockTorch_getRenderType(Block *self) {
    (void)self;
    return 2;
}

static AxisAlignedBB *BlockTorch_getCollisionBoundingBoxFromPool(Block *self, World *w, int x,
                                                                 int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}

static int BlockTorch_func_31032_h(World *world, int x, int y, int z) {
    if (World_isBlockNormalCube(world, x, y, z))
        return 1;
    extern Block *Block_fence;
    if (Block_fence && World_getBlockId(world, x, y, z) == Block_fence->blockID)
        return 1;
    return 0;
}

static int BlockTorch_canPlaceBlockAt(Block *self, World *world, int x, int y, int z) {
    (void)self;
    if (World_isBlockNormalCube(world, x - 1, y, z))
        return 1;
    if (World_isBlockNormalCube(world, x + 1, y, z))
        return 1;
    if (World_isBlockNormalCube(world, x, y, z - 1))
        return 1;
    if (World_isBlockNormalCube(world, x, y, z + 1))
        return 1;
    return BlockTorch_func_31032_h(world, x, y - 1, z);
}

static int BlockTorch_dropTorchIfCantStay(Block *self, World *world, int x, int y, int z) {
    if (!BlockTorch_canPlaceBlockAt(self, world, x, y, z)) {
        Block_dropBlockAsItem(self, world, x, y, z, World_getBlockMetadata(world, x, y, z));
        World_setBlockWithNotify(world, x, y, z, 0);
        return 0;
    }
    return 1;
}

static void BlockTorch_onBlockPlaced(Block *self, World *world, int x, int y, int z, int side) {
    (void)self;
    int meta = World_getBlockMetadata(world, x, y, z);
    if (side == 1 && BlockTorch_func_31032_h(world, x, y - 1, z))
        meta = 5;
    if (side == 2 && World_isBlockNormalCube(world, x, y, z + 1))
        meta = 4;
    if (side == 3 && World_isBlockNormalCube(world, x, y, z - 1))
        meta = 3;
    if (side == 4 && World_isBlockNormalCube(world, x + 1, y, z))
        meta = 2;
    if (side == 5 && World_isBlockNormalCube(world, x - 1, y, z))
        meta = 1;
    World_setBlockMetadataWithNotify(world, x, y, z, meta);
}

static void BlockTorch_updateTick(Block *self, World *world, int x, int y, int z,
                                  JavaRandom *rand) {
    Block_defaultVtable.updateTick(self, world, x, y, z, rand);
    if (World_getBlockMetadata(world, x, y, z) == 0)
        self->vtable->onBlockAdded(self, world, x, y, z);
}

void BlockTorch_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    if (World_isBlockNormalCube(world, x - 1, y, z))
        World_setBlockMetadataWithNotify(world, x, y, z, 1);
    else if (World_isBlockNormalCube(world, x + 1, y, z))
        World_setBlockMetadataWithNotify(world, x, y, z, 2);
    else if (World_isBlockNormalCube(world, x, y, z - 1))
        World_setBlockMetadataWithNotify(world, x, y, z, 3);
    else if (World_isBlockNormalCube(world, x, y, z + 1))
        World_setBlockMetadataWithNotify(world, x, y, z, 4);
    else if (BlockTorch_func_31032_h(world, x, y - 1, z))
        World_setBlockMetadataWithNotify(world, x, y, z, 5);
    BlockTorch_dropTorchIfCantStay(self, world, x, y, z);
}

void BlockTorch_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                      int neighborID) {
    (void)neighborID;
    if (BlockTorch_dropTorchIfCantStay(self, world, x, y, z)) {
        int meta = World_getBlockMetadata(world, x, y, z);
        int drop = 0;
        if (!World_isBlockNormalCube(world, x - 1, y, z) && meta == 1)
            drop = 1;
        if (!World_isBlockNormalCube(world, x + 1, y, z) && meta == 2)
            drop = 1;
        if (!World_isBlockNormalCube(world, x, y, z - 1) && meta == 3)
            drop = 1;
        if (!World_isBlockNormalCube(world, x, y, z + 1) && meta == 4)
            drop = 1;
        if (!BlockTorch_func_31032_h(world, x, y - 1, z) && meta == 5)
            drop = 1;
        if (drop) {
            Block_dropBlockAsItem(self, world, x, y, z, World_getBlockMetadata(world, x, y, z));
            World_setBlockWithNotify(world, x, y, z, 0);
        }
    }
}

static MovingObjectPosition *BlockTorch_collisionRayTrace(Block *self, World *world, int x, int y,
                                                          int z, Vec3D *start, Vec3D *end) {
    int meta = World_getBlockMetadata(world, x, y, z) & 7;
    float var8 = 0.15f;
    if (meta == 1)
        Block_setBlockBounds(self, 0.0f, 0.2f, 0.5f - var8, var8 * 2.0f, 0.8f, 0.5f + var8);
    else if (meta == 2)
        Block_setBlockBounds(self, 1.0f - var8 * 2.0f, 0.2f, 0.5f - var8, 1.0f, 0.8f, 0.5f + var8);
    else if (meta == 3)
        Block_setBlockBounds(self, 0.5f - var8, 0.2f, 0.0f, 0.5f + var8, 0.8f, var8 * 2.0f);
    else if (meta == 4)
        Block_setBlockBounds(self, 0.5f - var8, 0.2f, 1.0f - var8 * 2.0f, 0.5f + var8, 0.8f, 1.0f);
    else {
        var8 = 0.1f;
        Block_setBlockBounds(self, 0.5f - var8, 0.0f, 0.5f - var8, 0.5f + var8, 0.6f, 0.5f + var8);
    }
    return Block_defaultVtable.collisionRayTrace(self, world, x, y, z, start, end);
}

static void BlockTorch_randomDisplayTick(Block *self, World *var1, int var2, int var3, int var4,
                                         JavaRandom *var5) {
    (void)self;
    (void)var5;
    int var6 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    double var7 = (double)((float)var2 + 0.5f);
    double var9 = (double)((float)var3 + 0.7f);
    double var11 = (double)((float)var4 + 0.5f);
    double var13 = (double)0.22f;
    double var15 = (double)0.27f;
    if (var6 == 1) {
        World_spawnParticle(var1, "smoke", var7 - var15, var9 + var13, var11, 0.0, 0.0, 0.0);
        World_spawnParticle(var1, "flame", var7 - var15, var9 + var13, var11, 0.0, 0.0, 0.0);
    } else if (var6 == 2) {
        World_spawnParticle(var1, "smoke", var7 + var15, var9 + var13, var11, 0.0, 0.0, 0.0);
        World_spawnParticle(var1, "flame", var7 + var15, var9 + var13, var11, 0.0, 0.0, 0.0);
    } else if (var6 == 3) {
        World_spawnParticle(var1, "smoke", var7, var9 + var13, var11 - var15, 0.0, 0.0, 0.0);
        World_spawnParticle(var1, "flame", var7, var9 + var13, var11 - var15, 0.0, 0.0, 0.0);
    } else if (var6 == 4) {
        World_spawnParticle(var1, "smoke", var7, var9 + var13, var11 + var15, 0.0, 0.0, 0.0);
        World_spawnParticle(var1, "flame", var7, var9 + var13, var11 + var15, 0.0, 0.0, 0.0);
    } else {
        World_spawnParticle(var1, "smoke", var7, var9, var11, 0.0, 0.0, 0.0);
        World_spawnParticle(var1, "flame", var7, var9, var11, 0.0, 0.0, 0.0);
    }
}

void BlockTorch_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->isOpaqueCube = BlockTorch_isOpaqueCube;
    vt->renderAsNormalBlock = BlockTorch_renderAsNormalBlock;
    vt->getRenderType = BlockTorch_getRenderType;
    vt->getCollisionBoundingBoxFromPool = BlockTorch_getCollisionBoundingBoxFromPool;
    vt->canPlaceBlockAt = BlockTorch_canPlaceBlockAt;
    vt->onBlockAdded = BlockTorch_onBlockAdded;
    vt->onNeighborBlockChange = BlockTorch_onNeighborBlockChange;
    vt->onBlockPlaced = BlockTorch_onBlockPlaced;
    vt->updateTick = BlockTorch_updateTick;
    vt->collisionRayTrace = BlockTorch_collisionRayTrace;
    vt->randomDisplayTick = BlockTorch_randomDisplayTick;
}

Block *BlockTorch_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockTorch_initVtable(&vtable);
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_circuits);
    Block_setTickOnLoad(self, 1);
    return self;
}
