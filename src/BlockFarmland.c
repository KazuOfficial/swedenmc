#include "BlockFarmland.h"
#include "Material.h"
#include "World.h"
#include "Block.h"
#include "AxisAlignedBB.h"
#include "JavaRandom.h"
#include <stdlib.h>

BlockVtable BlockFarmland_defaultVtable;

void BlockFarmland_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->getCollisionBoundingBoxFromPool = BlockFarmland_getCollisionBoundingBoxFromPool;
    vt->isOpaqueCube = BlockFarmland_isOpaqueCube;
    vt->renderAsNormalBlock = BlockFarmland_renderAsNormalBlock;
    vt->getBlockTextureFromSideAndMetadata = BlockFarmland_getBlockTextureFromSideAndMetadata;
    vt->updateTick = BlockFarmland_updateTick;
    vt->onEntityWalking = BlockFarmland_onEntityWalking;
    vt->onNeighborBlockChange = BlockFarmland_onNeighborBlockChange;
    vt->idDropped = BlockFarmland_idDropped;
}

void BlockFarmland_construct(BlockFarmland *self, int id) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFarmland_initVtable(&BlockFarmland_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockFarmland_defaultVtable;
    Block_construct(&self->base, id, Material_ground);
    self->base.blockIndexInTexture = 87;
    Block_setTickOnLoad(&self->base, 1);
    Block_setBlockBounds(&self->base, 0.0F, 0.0F, 0.0F, 1.0F, 15.0F / 16.0F, 1.0F);
    Block_setLightOpacity(&self->base, 255);
}

BlockFarmland *BlockFarmland_create(int id) {
    BlockFarmland *self = (BlockFarmland *)calloc(1, sizeof(BlockFarmland));
    BlockFarmland_construct(self, id);
    return self;
}

AxisAlignedBB *BlockFarmland_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                             int var3, int var4) {
    (void)self;
    (void)var1;
    return AxisAlignedBB_getBoundingBoxFromPool((double)(var2 + 0), (double)(var3 + 0),
                                                (double)(var4 + 0), (double)(var2 + 1),
                                                (double)(var3 + 1), (double)(var4 + 1));
}

int BlockFarmland_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockFarmland_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockFarmland_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    return var1 == 1 && var2 > 0 ? self->blockIndexInTexture - 1
                                 : (var1 == 1 ? self->blockIndexInTexture : 2);
}

static int isCropsNearby(World *var1, int var2, int var3, int var4) {
    int var5 = 0;
    int var6, var7;
    for (var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
        for (var7 = var4 - var5; var7 <= var4 + var5; ++var7) {
            if (World_getBlockId(var1, var6, var3 + 1, var7) == Block_crops->blockID) {
                return 1;
            }
        }
    }
    return 0;
}

static int isWaterNearby(World *var1, int var2, int var3, int var4) {
    int var5, var6, var7;
    for (var5 = var2 - 4; var5 <= var2 + 4; ++var5) {
        for (var6 = var3; var6 <= var3 + 1; ++var6) {
            for (var7 = var4 - 4; var7 <= var4 + 4; ++var7) {
                if (World_getBlockMaterial(var1, var5, var6, var7) == Material_water) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void BlockFarmland_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                              JavaRandom *var5) {
    (void)self;
    if (JavaRandom_nextInt(var5, 5) == 0) {
        if (!isWaterNearby(var1, var2, var3, var4) &&
            !World_canBlockBeRainedOn(var1, var2, var3 + 1, var4)) {
            int var6 = World_getBlockMetadata(var1, var2, var3, var4);
            if (var6 > 0) {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6 - 1);
            } else if (!isCropsNearby(var1, var2, var3, var4)) {
                World_setBlockWithNotify(var1, var2, var3, var4, Block_dirt->blockID);
            }
        } else {
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, 7);
        }
    }
}

void BlockFarmland_onEntityWalking(Block *self, World *var1, int var2, int var3, int var4,
                                   Entity *var5) {
    (void)self;
    (void)var5;
    if (JavaRandom_nextInt(&var1->rand, 4) == 0) {
        World_setBlockWithNotify(var1, var2, var3, var4, Block_dirt->blockID);
    }
}

void BlockFarmland_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                         int var5) {
    Block_defaultVtable.onNeighborBlockChange(self, var1, var2, var3, var4, var5);
    Material *var6 = World_getBlockMaterial(var1, var2, var3 + 1, var4);
    if (Material_isSolid(var6)) {
        World_setBlockWithNotify(var1, var2, var3, var4, Block_dirt->blockID);
    }
}

int BlockFarmland_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    return Block_dirt->vtable->idDropped(Block_dirt, 0, var2);
}
