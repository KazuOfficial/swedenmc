#include "BlockBreakable.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "EntityPlayer.h"
#include "JavaRandom.h"
#include "EnumSkyBlock.h"
#include <stdlib.h>

static int BlockIce_getRenderBlockPass(Block *self) {
    (void)self;
    return 1;
}

static int BlockIce_getMobilityFlag(Block *self) {
    (void)self;
    return 0;
}

static int BlockIce_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}

static int BlockIce_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                         int side) {
    return BlockBreakable_shouldSideBeRendered(self, world, x, y, z, 1 - side);
}

static void BlockIce_harvestBlock(Block *self, World *world, EntityPlayer *player, int x, int y,
                                  int z, int meta) {
    Block_defaultVtable.harvestBlock(self, world, player, x, y, z, meta);
    Material *var7 = World_getBlockMaterial(world, x, y - 1, z);
    if (Material_getIsSolid(var7) || Material_getIsLiquid(var7)) {
        extern Block *Block_waterMoving;
        World_setBlockWithNotify(world, x, y, z, Block_waterMoving->blockID);
    }
}

static void BlockIce_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand) {
    (void)rand;
    if (World_getSavedLightValue(world, EnumSkyBlock_Block, x, y, z) >
        11 - Block_lightOpacity[self->blockID]) {
        Block_dropBlockAsItem(self, world, x, y, z, World_getBlockMetadata(world, x, y, z));
        World_setBlockWithNotify(world, x, y, z, Block_waterStill->blockID);
    }
}

Block *BlockIce_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockBreakable_isOpaqueCube;
        vtable.shouldSideBeRendered = BlockIce_shouldSideBeRendered;
        vtable.getRenderBlockPass = BlockIce_getRenderBlockPass;
        vtable.getMobilityFlag = BlockIce_getMobilityFlag;
        vtable.quantityDropped = BlockIce_quantityDropped;
        vtable.harvestBlock = BlockIce_harvestBlock;
        vtable.updateTick = BlockIce_updateTick;
        vtable_init = 1;
    }
    BlockBreakable *self = (BlockBreakable *)calloc(1, sizeof(BlockBreakable));
    self->base.vtable = &vtable;
    BlockBreakable_construct(self, id, tex, Material_ice, 0);
    self->base.slipperiness = 0.98f;
    Block_setTickOnLoad(&self->base, 1);
    return &self->base;
}
