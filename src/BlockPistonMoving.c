#include "Block.h"
#include "BlockContainer.h"
#include "TileEntityPiston.h"
#include "TileEntity.h"
#include "World.h"
#include "Material.h"
#include "PistonBlockTextures.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "JavaRandom.h"
#include <stdlib.h>

AxisAlignedBB *BlockPistonMoving_func_31035_a(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5, float var6, int var7);

static int BlockPistonMoving_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonMoving_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonMoving_getRenderType(Block *self) {
    (void)self;
    return -1;
}

static int BlockPistonMoving_canPlaceBlockAt(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return 0;
}
static int BlockPistonMoving_canPlaceBlockOnSide(Block *self, World *w, int x, int y, int z,
                                                 int side) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
    return 0;
}

static void BlockPistonMoving_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
}

static void BlockPistonMoving_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    TileEntity *var5 = World_getBlockTileEntity(world, x, y, z);
    if (var5 != NULL && var5->tileEntityTypeId == TE_TYPE_PISTON) {
        TileEntityPiston_func_31011_l((TileEntityPiston *)var5);
    } else {
        BlockContainer_onBlockRemoval(self, world, x, y, z);
    }
}

static int BlockPistonMoving_blockActivated(Block *self, World *world, int x, int y, int z,
                                            EntityPlayer *player) {
    (void)self;
    (void)player;
    if (!world->isRemote && World_getBlockTileEntity(world, x, y, z) == NULL) {
        World_setBlockWithNotify(world, x, y, z, 0);
        return 1;
    }
    return 0;
}

static int BlockPistonMoving_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return 0;
}

static TileEntityPiston *func_31034_c(IBlockAccess *var1, int var2, int var3, int var4) {
    TileEntity *var5 = var1->vtable->getBlockTileEntity(var1, var2, var3, var4);
    return (var5 != NULL && var5->tileEntityTypeId == TE_TYPE_PISTON) ? (TileEntityPiston *)var5
                                                                      : NULL;
}

static void BlockPistonMoving_dropBlockAsItemWithChance(Block *self, World *world, int x, int y,
                                                        int z, int meta, float chance) {
    (void)self;
    (void)meta;
    (void)chance;
    if (!world->isRemote) {
        TileEntityPiston *var7 = func_31034_c((IBlockAccess *)world, x, y, z);
        if (var7 != NULL) {
            int storedID = TileEntityPiston_getStoredBlockID(var7);
            Block *b = Block_blocksList[storedID];
            if (b)
                Block_dropBlockAsItem(b, world, x, y, z, TileEntityPiston_getBlockMetadata(var7));
        }
    }
}

static void BlockPistonMoving_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                                    int neighborID) {
    (void)self;
    (void)neighborID;
    if (!world->isRemote && World_getBlockTileEntity(world, x, y, z) == NULL) {
    }
}

static AxisAlignedBB *BlockPistonMoving_getCollisionBoundingBoxFromPool(Block *self, World *world,
                                                                        int x, int y, int z) {
    TileEntityPiston *var5 = func_31034_c((IBlockAccess *)world, x, y, z);
    if (var5 == NULL)
        return NULL;
    float var6 = TileEntityPiston_func_31008_a(var5, 0.0f);
    if (TileEntityPiston_func_31015_b(var5))
        var6 = 1.0f - var6;
    return (AxisAlignedBB *)BlockPistonMoving_func_31035_a(
        self, world, x, y, z, TileEntityPiston_getStoredBlockID(var5), var6,
        TileEntityPiston_func_31009_d(var5));
}

static void BlockPistonMoving_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                         int var3, int var4) {
    TileEntityPiston *var5 = func_31034_c(var1, var2, var3, var4);
    if (var5 != NULL) {
        int storedID = TileEntityPiston_getStoredBlockID(var5);
        Block *var6 = Block_blocksList[storedID];
        if (var6 == NULL || var6 == self)
            return;
        var6->vtable->setBlockBoundsBasedOnState(var6, var1, var2, var3, var4);
        float var7 = TileEntityPiston_func_31008_a(var5, 0.0f);
        if (TileEntityPiston_func_31015_b(var5))
            var7 = 1.0f - var7;
        int var8 = TileEntityPiston_func_31009_d(var5);
        self->minX = var6->minX - (double)((float)PistonBlockTextures_field_31056_b[var8] * var7);
        self->minY = var6->minY - (double)((float)PistonBlockTextures_field_31059_c[var8] * var7);
        self->minZ = var6->minZ - (double)((float)PistonBlockTextures_field_31058_d[var8] * var7);
        self->maxX = var6->maxX - (double)((float)PistonBlockTextures_field_31056_b[var8] * var7);
        self->maxY = var6->maxY - (double)((float)PistonBlockTextures_field_31059_c[var8] * var7);
        self->maxZ = var6->maxZ - (double)((float)PistonBlockTextures_field_31058_d[var8] * var7);
    }
}

AxisAlignedBB *BlockPistonMoving_func_31035_a(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5, float var6, int var7) {
    if (var5 != 0 && var5 != self->blockID && Block_blocksList[var5]) {
        Block *b = Block_blocksList[var5];
        AxisAlignedBB *var8 = b->vtable->getCollisionBoundingBoxFromPool(b, var1, var2, var3, var4);
        if (var8 == NULL)
            return NULL;
        var8->minX -= (double)((float)PistonBlockTextures_field_31056_b[var7] * var6);
        var8->maxX -= (double)((float)PistonBlockTextures_field_31056_b[var7] * var6);
        var8->minY -= (double)((float)PistonBlockTextures_field_31059_c[var7] * var6);
        var8->maxY -= (double)((float)PistonBlockTextures_field_31059_c[var7] * var6);
        var8->minZ -= (double)((float)PistonBlockTextures_field_31058_d[var7] * var6);
        var8->maxZ -= (double)((float)PistonBlockTextures_field_31058_d[var7] * var6);
        return var8;
    } else {
        return NULL;
    }
}

TileEntity *BlockPistonMoving_func_31036_a(int var0, int var1, int var2, int var3, int var4) {
    return (TileEntity *)TileEntityPiston_createWith(var0, var1, var2, var3, var4);
}

Block *BlockPistonMoving_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable);
        vtable.isOpaqueCube = BlockPistonMoving_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockPistonMoving_renderAsNormalBlock;
        vtable.getRenderType = BlockPistonMoving_getRenderType;
        vtable.canPlaceBlockAt = BlockPistonMoving_canPlaceBlockAt;
        vtable.canPlaceBlockOnSide = BlockPistonMoving_canPlaceBlockOnSide;
        vtable.idDropped = BlockPistonMoving_idDropped;
        vtable.onBlockAdded = BlockPistonMoving_onBlockAdded;
        vtable.onBlockRemoval = BlockPistonMoving_onBlockRemoval;
        vtable.blockActivated = BlockPistonMoving_blockActivated;
        vtable.dropBlockAsItemWithChance = BlockPistonMoving_dropBlockAsItemWithChance;
        vtable.onNeighborBlockChange = BlockPistonMoving_onNeighborBlockChange;
        vtable.getCollisionBoundingBoxFromPool = BlockPistonMoving_getCollisionBoundingBoxFromPool;
        vtable.setBlockBoundsBasedOnState = BlockPistonMoving_setBlockBoundsBasedOnState;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    BlockContainer_construct(self, id, Material_field_31067_B);
    self->blockHardness = -1.0f;
    return self;
}
