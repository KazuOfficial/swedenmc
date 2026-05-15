#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "MathHelper.h"
#include "World.h"
#include "EntityLiving.h"
#include <stdlib.h>

typedef struct {
    Block base;
    Block *modelBlock;
} BlockStairs;

static int BlockStairs_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockStairs_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockStairs_getRenderType(Block *self) {
    (void)self;
    return 10;
}

static void BlockStairs_setBlockBoundsBasedOnState(Block *self, IBlockAccess *world, int x, int y,
                                                   int z) {
    (void)world;
    (void)x;
    (void)y;
    (void)z;
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

static AxisAlignedBB *BlockStairs_getCollisionBoundingBoxFromPool(Block *self, World *world, int x,
                                                                  int y, int z) {
    return Block_defaultVtable.getCollisionBoundingBoxFromPool(self, world, x, y, z);
}

static int BlockStairs_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                            int side) {
    return Block_defaultVtable.shouldSideBeRendered(self, world, x, y, z, side);
}

static void BlockStairs_getCollidingBoundingBoxes(Block *self, World *world, int x, int y, int z,
                                                  AxisAlignedBB *bb, void *list) {
    CollidingList *cl = (CollidingList *)list;
    int meta = world->vtable->getBlockMetadata(world, x, y, z);

    float x0a, y0a, z0a, x1a, y1a, z1a;
    float x0b, y0b, z0b, x1b, y1b, z1b;
    if (meta == 0) {
        x0a = 0.0f;
        y0a = 0.0f;
        z0a = 0.0f;
        x1a = 0.5f;
        y1a = 0.5f;
        z1a = 1.0f;
        x0b = 0.5f;
        y0b = 0.0f;
        z0b = 0.0f;
        x1b = 1.0f;
        y1b = 1.0f;
        z1b = 1.0f;
    } else if (meta == 1) {
        x0a = 0.0f;
        y0a = 0.0f;
        z0a = 0.0f;
        x1a = 0.5f;
        y1a = 1.0f;
        z1a = 1.0f;
        x0b = 0.5f;
        y0b = 0.0f;
        z0b = 0.0f;
        x1b = 1.0f;
        y1b = 0.5f;
        z1b = 1.0f;
    } else if (meta == 2) {
        x0a = 0.0f;
        y0a = 0.0f;
        z0a = 0.0f;
        x1a = 1.0f;
        y1a = 0.5f;
        z1a = 0.5f;
        x0b = 0.0f;
        y0b = 0.0f;
        z0b = 0.5f;
        x1b = 1.0f;
        y1b = 1.0f;
        z1b = 1.0f;
    } else if (meta == 3) {
        x0a = 0.0f;
        y0a = 0.0f;
        z0a = 0.0f;
        x1a = 1.0f;
        y1a = 1.0f;
        z1a = 0.5f;
        x0b = 0.0f;
        y0b = 0.0f;
        z0b = 0.5f;
        x1b = 1.0f;
        y1b = 0.5f;
        z1b = 1.0f;
    } else {
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    Block_setBlockBounds(self, x0a, y0a, z0a, x1a, y1a, z1a);
    AxisAlignedBB *col = self->vtable->getCollisionBoundingBoxFromPool(self, world, x, y, z);
    if (col != NULL && AxisAlignedBB_intersectsWith(bb, col) && cl->count < cl->max)
        cl->arr[cl->count++] = *col;

    Block_setBlockBounds(self, x0b, y0b, z0b, x1b, y1b, z1b);
    col = self->vtable->getCollisionBoundingBoxFromPool(self, world, x, y, z);
    if (col != NULL && AxisAlignedBB_intersectsWith(bb, col) && cl->count < cl->max)
        cl->arr[cl->count++] = *col;

    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

static void BlockStairs_randomDisplayTick(Block *self, World *world, int x, int y, int z,
                                          JavaRandom *rand) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->randomDisplayTick(b->modelBlock, world, x, y, z, rand);
}

static void BlockStairs_onBlockClicked(Block *self, World *world, int x, int y, int z,
                                       EntityPlayer *player) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->onBlockClicked(b->modelBlock, world, x, y, z, player);
}

static void BlockStairs_onBlockDestroyedByPlayer(Block *self, World *world, int x, int y, int z,
                                                 int meta) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->onBlockDestroyedByPlayer(b->modelBlock, world, x, y, z, meta);
}

static float BlockStairs_getBlockBrightness(Block *self, IBlockAccess *world, int x, int y, int z) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getBlockBrightness(b->modelBlock, world, x, y, z);
}

static float BlockStairs_getExplosionResistance(Block *self, struct Entity *entity) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getExplosionResistance(b->modelBlock, entity);
}

static int BlockStairs_getRenderBlockPass(Block *self) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getRenderBlockPass(b->modelBlock);
}

static int BlockStairs_idDropped(Block *self, int meta, JavaRandom *rand) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->idDropped(b->modelBlock, meta, rand);
}

static int BlockStairs_quantityDropped(Block *self, JavaRandom *rand) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->quantityDropped(b->modelBlock, rand);
}

static int BlockStairs_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getBlockTextureFromSideAndMetadata(b->modelBlock, side, meta);
}

static int BlockStairs_getBlockTextureFromSide(Block *self, int side) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getBlockTextureFromSide(b->modelBlock, side);
}

static int BlockStairs_getBlockTexture(Block *self, IBlockAccess *world, int x, int y, int z,
                                       int side) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getBlockTexture(b->modelBlock, world, x, y, z, side);
}

static int BlockStairs_tickRate(Block *self) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->tickRate(b->modelBlock);
}

static AxisAlignedBB *BlockStairs_getSelectedBoundingBoxFromPool(Block *self, World *world, int x,
                                                                 int y, int z) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->getSelectedBoundingBoxFromPool(b->modelBlock, world, x, y, z);
}

static void BlockStairs_velocityToAddToEntity(Block *self, World *world, int x, int y, int z,
                                              struct Entity *entity, Vec3D *vel) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->velocityToAddToEntity(b->modelBlock, world, x, y, z, entity, vel);
}

static int BlockStairs_isCollidable(Block *self) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->isCollidable(b->modelBlock);
}

static int BlockStairs_canCollideCheck(Block *self, int meta, int flag) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->canCollideCheck(b->modelBlock, meta, flag);
}

static int BlockStairs_canPlaceBlockAt(Block *self, World *world, int x, int y, int z) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->canPlaceBlockAt(b->modelBlock, world, x, y, z);
}

static void BlockStairs_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    BlockStairs *b = (BlockStairs *)self;
    self->vtable->onNeighborBlockChange(self, world, x, y, z, 0);
    b->modelBlock->vtable->onBlockAdded(b->modelBlock, world, x, y, z);
}

static void BlockStairs_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->onBlockRemoval(b->modelBlock, world, x, y, z);
}

static void BlockStairs_dropBlockAsItemWithChance(Block *self, World *world, int x, int y, int z,
                                                  int meta, float chance) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->dropBlockAsItemWithChance(b->modelBlock, world, x, y, z, meta, chance);
}

static void BlockStairs_onEntityWalking(Block *self, World *world, int x, int y, int z,
                                        struct Entity *entity) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->onEntityWalking(b->modelBlock, world, x, y, z, entity);
}

static void BlockStairs_updateTick(Block *self, World *world, int x, int y, int z,
                                   JavaRandom *rand) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->updateTick(b->modelBlock, world, x, y, z, rand);
}

static int BlockStairs_blockActivated(Block *self, World *world, int x, int y, int z,
                                      EntityPlayer *player) {
    BlockStairs *b = (BlockStairs *)self;
    return b->modelBlock->vtable->blockActivated(b->modelBlock, world, x, y, z, player);
}

static void BlockStairs_onBlockDestroyedByExplosion(Block *self, World *world, int x, int y,
                                                    int z) {
    BlockStairs *b = (BlockStairs *)self;
    b->modelBlock->vtable->onBlockDestroyedByExplosion(b->modelBlock, world, x, y, z);
}

static void BlockStairs_onBlockPlacedBy(Block *self, World *world, int x, int y, int z,
                                        EntityLiving *entity) {
    (void)self;
    int var6 =
        MathHelper_floor_double((double)(entity->base.rotationYaw * 4.0f / 360.0f) + 0.5) & 3;
    if (var6 == 0) {
        world->vtable->setBlockMetadataWithNotify(world, x, y, z, 2);
    }
    if (var6 == 1) {
        world->vtable->setBlockMetadataWithNotify(world, x, y, z, 1);
    }
    if (var6 == 2) {
        world->vtable->setBlockMetadataWithNotify(world, x, y, z, 3);
    }
    if (var6 == 3) {
        world->vtable->setBlockMetadataWithNotify(world, x, y, z, 0);
    }
}

Block *BlockStairs_create(int id, Block *modelBlock) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockStairs_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockStairs_renderAsNormalBlock;
        vtable.getRenderType = BlockStairs_getRenderType;
        vtable.setBlockBoundsBasedOnState = BlockStairs_setBlockBoundsBasedOnState;
        vtable.getCollisionBoundingBoxFromPool = BlockStairs_getCollisionBoundingBoxFromPool;
        vtable.shouldSideBeRendered = BlockStairs_shouldSideBeRendered;
        vtable.getCollidingBoundingBoxes = BlockStairs_getCollidingBoundingBoxes;
        vtable.randomDisplayTick = BlockStairs_randomDisplayTick;
        vtable.onBlockClicked = BlockStairs_onBlockClicked;
        vtable.onBlockDestroyedByPlayer = BlockStairs_onBlockDestroyedByPlayer;
        vtable.getBlockBrightness = BlockStairs_getBlockBrightness;
        vtable.getExplosionResistance = BlockStairs_getExplosionResistance;
        vtable.getRenderBlockPass = BlockStairs_getRenderBlockPass;
        vtable.idDropped = BlockStairs_idDropped;
        vtable.quantityDropped = BlockStairs_quantityDropped;
        vtable.getBlockTextureFromSideAndMetadata = BlockStairs_getBlockTextureFromSideAndMetadata;
        vtable.getBlockTextureFromSide = BlockStairs_getBlockTextureFromSide;
        vtable.getBlockTexture = BlockStairs_getBlockTexture;
        vtable.tickRate = BlockStairs_tickRate;
        vtable.getSelectedBoundingBoxFromPool = BlockStairs_getSelectedBoundingBoxFromPool;
        vtable.velocityToAddToEntity = BlockStairs_velocityToAddToEntity;
        vtable.isCollidable = BlockStairs_isCollidable;
        vtable.canCollideCheck = BlockStairs_canCollideCheck;
        vtable.canPlaceBlockAt = BlockStairs_canPlaceBlockAt;
        vtable.onBlockAdded = BlockStairs_onBlockAdded;
        vtable.onBlockRemoval = BlockStairs_onBlockRemoval;
        vtable.dropBlockAsItemWithChance = BlockStairs_dropBlockAsItemWithChance;
        vtable.onEntityWalking = BlockStairs_onEntityWalking;
        vtable.updateTick = BlockStairs_updateTick;
        vtable.blockActivated = BlockStairs_blockActivated;
        vtable.onBlockDestroyedByExplosion = BlockStairs_onBlockDestroyedByExplosion;
        vtable.onBlockPlacedBy = BlockStairs_onBlockPlacedBy;
        vtable_init = 1;
    }
    BlockStairs *self = (BlockStairs *)calloc(1, sizeof(BlockStairs));
    self->base.vtable = &vtable;
    self->modelBlock = modelBlock;
    Block_constructWithTexture(&self->base, id, modelBlock->blockIndexInTexture,
                               modelBlock->blockMaterial);
    Block_setHardness(&self->base, modelBlock->blockHardness);
    Block_setResistance(&self->base, modelBlock->blockResistance / 3.0f);
    Block_setStepSound(&self->base, modelBlock->stepSound);
    Block_setLightOpacity(&self->base, 255);
    return &self->base;
}
