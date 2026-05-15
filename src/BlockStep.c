#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "IBlockAccess.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int blockType;
} BlockStep;

static int BlockStep_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    (void)self;
    if (meta == 0)
        return (side <= 1) ? 6 : 5;
    if (meta == 1) {
        if (side == 0)
            return 208;
        if (side == 1)
            return 176;
        return 192;
    }
    if (meta == 2)
        return 4;
    if (meta == 3)
        return 16;
    return 6;
}

static int BlockStep_getBlockTextureFromSide(Block *self, int side) {
    return BlockStep_getBlockTextureFromSideAndMetadata(self, side, 0);
}

static int BlockStep_isOpaqueCube(Block *self) {
    BlockStep *b = (BlockStep *)self;
    return b->blockType;
}

static int BlockStep_renderAsNormalBlock(Block *self) {
    BlockStep *b = (BlockStep *)self;
    return b->blockType;
}

static int BlockStep_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Block_stairSingle->blockID;
}

static int BlockStep_quantityDropped(Block *self, JavaRandom *rand) {
    (void)rand;
    BlockStep *b = (BlockStep *)self;
    return b->blockType ? 2 : 1;
}

static int BlockStep_damageDropped(Block *self, int meta) {
    (void)self;
    return meta;
}

static void BlockStep_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    if (self != Block_stairSingle) {
        Block_defaultVtable.onBlockAdded(self, world, x, y, z);
    }
    int var5 = World_getBlockId(world, x, y - 1, z);
    int var6 = World_getBlockMetadata(world, x, y, z);
    int var7 = World_getBlockMetadata(world, x, y - 1, z);
    if (var6 == var7) {
        if (var5 == Block_stairSingle->blockID) {
            World_setBlockWithNotify(world, x, y, z, 0);
            World_setBlockAndMetadataWithNotify(world, x, y - 1, z, Block_stairDouble->blockID,
                                                var6);
        }
    }
}

static int BlockStep_shouldSideBeRendered(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4, int var5) {
    if (self != Block_stairSingle) {
        Block_defaultVtable.shouldSideBeRendered(self, var1, var2, var3, var4, var5);
    }
    if (var5 == 1)
        return 1;
    if (!Block_defaultVtable.shouldSideBeRendered(self, var1, var2, var3, var4, var5))
        return 0;
    if (var5 == 0)
        return 1;
    return var1->vtable->getBlockId(var1, var2, var3, var4) != self->blockID;
}

Block *BlockStep_create(int id, int isDouble) {
    static BlockVtable vtable_half;
    static BlockVtable vtable_double;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable_half = Block_defaultVtable;
        vtable_half.getBlockTextureFromSideAndMetadata =
            BlockStep_getBlockTextureFromSideAndMetadata;
        vtable_half.getBlockTextureFromSide = BlockStep_getBlockTextureFromSide;
        vtable_half.isOpaqueCube = BlockStep_isOpaqueCube;
        vtable_half.renderAsNormalBlock = BlockStep_renderAsNormalBlock;
        vtable_half.idDropped = BlockStep_idDropped;
        vtable_half.quantityDropped = BlockStep_quantityDropped;
        vtable_half.damageDropped = BlockStep_damageDropped;
        vtable_half.onBlockAdded = BlockStep_onBlockAdded;
        vtable_half.shouldSideBeRendered = BlockStep_shouldSideBeRendered;
        vtable_double = vtable_half;
        vtable_init = 1;
    }

    BlockStep *self = (BlockStep *)calloc(1, sizeof(BlockStep));
    self->blockType = isDouble;
    self->base.vtable = isDouble ? &vtable_double : &vtable_half;
    Block_constructWithTexture(&self->base, id, 6, Material_rock);

    if (!isDouble) {
        Block_setBlockBounds(&self->base, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f);
    }
    Block_lightOpacity[id] = 255;
    return &self->base;
}
