#include "Block.h"
#include "PistonBlockTextures.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int field_31053_a;
} BlockPistonExtension;

int BlockPistonExtension_func_31050_c(int meta) { return meta & 7; }
static int PistonExt_facing(int meta) { return BlockPistonExtension_func_31050_c(meta); }

static int BlockPistonExtension_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonExtension_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockPistonExtension_getRenderType(Block *self) {
    (void)self;
    return 17;
}

static int BlockPistonExtension_canPlaceBlockAt(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return 0;
}
static int BlockPistonExtension_canPlaceBlockOnSide(Block *self, World *w, int x, int y, int z,
                                                    int side) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
    return 0;
}

static int BlockPistonExtension_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}

static int BlockPistonExtension_getBlockTextureFromSideAndMetadata(Block *self, int var1,
                                                                   int var2) {
    BlockPistonExtension *b = (BlockPistonExtension *)self;
    int var3 = PistonExt_facing(var2);
    return var1 == var3 ? (b->field_31053_a >= 0 ? b->field_31053_a
                                                 : ((var2 & 8) != 0 ? self->blockIndexInTexture - 1
                                                                    : self->blockIndexInTexture))
                        : (var1 == PistonBlockTextures_field_31057_a[var3] ? 107 : 108);
}

static void BlockPistonExtension_onBlockRemoval(Block *self, World *var1, int var2, int var3,
                                                int var4) {
    Block_defaultVtable.onBlockRemoval(self, var1, var2, var3, var4);
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    int var6 = PistonBlockTextures_field_31057_a[BlockPistonExtension_func_31050_c(var5)];
    var2 += PistonBlockTextures_field_31056_b[var6];
    var3 += PistonBlockTextures_field_31059_c[var6];
    var4 += PistonBlockTextures_field_31058_d[var6];
    int var7 = World_getBlockId(var1, var2, var3, var4);
    if (var7 == Block_pistonBase->blockID || var7 == Block_pistonStickyBase->blockID) {
        var5 = World_getBlockMetadata(var1, var2, var3, var4);
        if (BlockPistonBase_isPowered(var5)) {
            Block_dropBlockAsItem(Block_blocksList[var7], var1, var2, var3, var4, var5);
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
    }
}

static void BlockPistonExtension_getCollidingBoundingBoxes(Block *self, World *var1, int var2,
                                                           int var3, int var4, AxisAlignedBB *var5,
                                                           void *var6) {
    int var7 = World_getBlockMetadata(var1, var2, var3, var4);
    switch (BlockPistonExtension_func_31050_c(var7)) {
    case 0:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 6.0f / 16.0f, 0.25f, 6.0f / 16.0f, 10.0f / 16.0f, 1.0f,
                             10.0f / 16.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    case 1:
        Block_setBlockBounds(self, 0.0f, 12.0f / 16.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 6.0f / 16.0f, 0.0f, 6.0f / 16.0f, 10.0f / 16.0f, 12.0f / 16.0f,
                             10.0f / 16.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    case 2:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 0.25f, 6.0f / 16.0f, 0.25f, 12.0f / 16.0f, 10.0f / 16.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    case 3:
        Block_setBlockBounds(self, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 0.25f, 6.0f / 16.0f, 0.0f, 12.0f / 16.0f, 10.0f / 16.0f,
                             12.0f / 16.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    case 4:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 6.0f / 16.0f, 0.25f, 0.25f, 10.0f / 16.0f, 12.0f / 16.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    case 5:
        Block_setBlockBounds(self, 12.0f / 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        Block_setBlockBounds(self, 0.0f, 6.0f / 16.0f, 0.25f, 12.0f / 16.0f, 10.0f / 16.0f,
                             12.0f / 16.0f);
        Block_defaultVtable.getCollidingBoundingBoxes(self, var1, var2, var3, var4, var5, var6);
        break;
    }
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

static void BlockPistonExtension_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1,
                                                            int var2, int var3, int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    switch (BlockPistonExtension_func_31050_c(var5)) {
    case 0:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
        break;
    case 1:
        Block_setBlockBounds(self, 0.0f, 12.0f / 16.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 2:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f);
        break;
    case 3:
        Block_setBlockBounds(self, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 4:
        Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f);
        break;
    case 5:
        Block_setBlockBounds(self, 12.0f / 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        break;
    }
}

static void BlockPistonExtension_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                                       int var4, int var5) {
    (void)self;
    int var6 = BlockPistonExtension_func_31050_c(World_getBlockMetadata(var1, var2, var3, var4));
    int var7 = World_getBlockId(var1, var2 - PistonBlockTextures_field_31056_b[var6],
                                var3 - PistonBlockTextures_field_31059_c[var6],
                                var4 - PistonBlockTextures_field_31058_d[var6]);
    if (var7 != Block_pistonBase->blockID && var7 != Block_pistonStickyBase->blockID) {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    } else {
        Block_blocksList[var7]->vtable->onNeighborBlockChange(
            Block_blocksList[var7], var1, var2 - PistonBlockTextures_field_31056_b[var6],
            var3 - PistonBlockTextures_field_31059_c[var6],
            var4 - PistonBlockTextures_field_31058_d[var6], var5);
    }
}

Block *BlockPistonExtension_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockPistonExtension_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockPistonExtension_renderAsNormalBlock;
        vtable.getRenderType = BlockPistonExtension_getRenderType;
        vtable.canPlaceBlockAt = BlockPistonExtension_canPlaceBlockAt;
        vtable.canPlaceBlockOnSide = BlockPistonExtension_canPlaceBlockOnSide;
        vtable.quantityDropped = BlockPistonExtension_quantityDropped;
        vtable.getBlockTextureFromSideAndMetadata =
            BlockPistonExtension_getBlockTextureFromSideAndMetadata;
        vtable.onBlockRemoval = BlockPistonExtension_onBlockRemoval;
        vtable.getCollidingBoundingBoxes = BlockPistonExtension_getCollidingBoundingBoxes;
        vtable.setBlockBoundsBasedOnState = BlockPistonExtension_setBlockBoundsBasedOnState;
        vtable.onNeighborBlockChange = BlockPistonExtension_onNeighborBlockChange;
        vtable_init = 1;
    }
    BlockPistonExtension *self = (BlockPistonExtension *)calloc(1, sizeof(BlockPistonExtension));
    self->field_31053_a = -1;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, Material_field_31067_B);
    self->base.stepSound = Block_soundStoneFootstep;
    self->base.blockHardness = 0.5f;
    self->base.blockResistance = 0.5f / 5.0f;
    return &self->base;
}

void BlockPistonExtension_func_31052_a_(Block *self, int var1) {
    ((BlockPistonExtension *)self)->field_31053_a = var1;
}

void BlockPistonExtension_func_31051_a(Block *self) {
    ((BlockPistonExtension *)self)->field_31053_a = -1;
}
