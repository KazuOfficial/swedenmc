#include "Block.h"
#include "World.h"
#include "IBlockAccess.h"
#include "Entity.h"
#include "AxisAlignedBB.h"
#include "EnumMobType.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

typedef struct {
    Block base;
    EnumMobType triggerMobType;
} BlockPressurePlate;

static int match_EntityLiving(struct Entity *e) { return e->isLiving; }
static int match_EntityPlayer(struct Entity *e) { return e->isPlayer; }

static void setStateIfMobInteractsWithPlate(BlockPressurePlate *self, World *var1, int var2,
                                            int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4) == 1;
    int var6 = 0;
    float var7 = 2.0f / 16.0f;
    Entity *list[64];
    int listSize = 0;
    AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var7), (double)var3, (double)((float)var4 + var7),
        (double)((float)(var2 + 1) - var7), (double)var3 + 0.25,
        (double)((float)(var4 + 1) - var7));

    if (self->triggerMobType == EnumMobType_everything) {
        listSize = World_getEntitiesWithinAABBExcludingEntity(var1, NULL, bb, list, 64);
    }
    if (self->triggerMobType == EnumMobType_mobs) {
        listSize = World_getEntitiesWithinAABB(var1, match_EntityLiving, bb, list, 64);
    }
    if (self->triggerMobType == EnumMobType_players) {
        listSize = World_getEntitiesWithinAABB(var1, match_EntityPlayer, bb, list, 64);
    }

    if (listSize > 0) {
        var6 = 1;
    }

    if (var6 && !var5) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 1);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->base.blockID);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.1, (double)var4 + 0.5,
                              "random.click", 0.3f, 0.6f);
    }

    if (!var6 && var5) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 0);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->base.blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->base.blockID);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.1, (double)var4 + 0.5,
                              "random.click", 0.3f, 0.5f);
    }

    if (var6) {
        World_scheduleBlockUpdate(var1, var2, var3, var4, self->base.blockID,
                                  self->base.vtable->tickRate(&self->base));
    }
}

static int pp_tickRate(Block *self) {
    (void)self;
    return 20;
}

static AxisAlignedBB *pp_getCollisionBoundingBoxFromPool(Block *self, World *w, int x, int y,
                                                         int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}
static int pp_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int pp_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int pp_canProvidePower(Block *self) {
    (void)self;
    return 1;
}
static int pp_getMobilityFlag(Block *self) {
    (void)self;
    return 1;
}

static int pp_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4);
}

static void pp_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
}

static void pp_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                     int var5) {
    (void)var5;
    int var6 = 0;
    if (!World_isBlockNormalCube(var1, var2, var3 - 1, var4)) {
        var6 = 1;
    }
    if (var6) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

static void pp_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                          JavaRandom *var5) {
    (void)var5;
    if (!var1->multiplayerWorld) {
        if (World_getBlockMetadata(var1, var2, var3, var4) != 0) {
            setStateIfMobInteractsWithPlate((BlockPressurePlate *)self, var1, var2, var3, var4);
        }
    }
}

static void pp_onEntityCollidedWithBlock(Block *self, World *var1, int var2, int var3, int var4,
                                         Entity *var5) {
    (void)var5;
    if (!var1->multiplayerWorld) {
        if (World_getBlockMetadata(var1, var2, var3, var4) != 1) {
            setStateIfMobInteractsWithPlate((BlockPressurePlate *)self, var1, var2, var3, var4);
        }
    }
}

static void pp_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    if (var5 > 0) {
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
    }
    Block_defaultVtable.onBlockRemoval(self, var1, var2, var3, var4);
}

static void pp_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4) {
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4) == 1;
    float var6 = 1.0f / 16.0f;
    if (var5) {
        Block_setBlockBounds(self, var6, 0.0f, var6, 1.0f - var6, 0.03125f, 1.0f - var6);
    } else {
        Block_setBlockBounds(self, var6, 0.0f, var6, 1.0f - var6, 1.0f / 16.0f, 1.0f - var6);
    }
}

static int pp_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                           int var5) {
    (void)self;
    (void)var5;
    return var1->vtable->getBlockMetadata(var1, var2, var3, var4) > 0;
}

static int pp_isIndirectlyPoweringTo(Block *self, World *var1, int var2, int var3, int var4,
                                     int var5) {
    (void)self;
    return World_getBlockMetadata(var1, var2, var3, var4) == 0 ? 0 : (var5 == 1);
}

static void pp_setBlockBoundsForItemRender(Block *self) {
    float var1 = 0.5f, var2 = 2.0f / 16.0f, var3 = 0.5f;
    Block_setBlockBounds(self, 0.5f - var1, 0.5f - var2, 0.5f - var3, 0.5f + var1, 0.5f + var2,
                         0.5f + var3);
}

Block *BlockPressurePlate_create(int id, int tex, EnumMobType mobType, Material *mat) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.tickRate = pp_tickRate;
        vtable.getCollisionBoundingBoxFromPool = pp_getCollisionBoundingBoxFromPool;
        vtable.isOpaqueCube = pp_isOpaqueCube;
        vtable.renderAsNormalBlock = pp_renderAsNormalBlock;
        vtable.canProvidePower = pp_canProvidePower;
        vtable.getMobilityFlag = pp_getMobilityFlag;
        vtable.canPlaceBlockAt = pp_canPlaceBlockAt;
        vtable.onBlockAdded = pp_onBlockAdded;
        vtable.onNeighborBlockChange = pp_onNeighborBlockChange;
        vtable.updateTick = pp_updateTick;
        vtable.onEntityCollidedWithBlock = pp_onEntityCollidedWithBlock;
        vtable.onBlockRemoval = pp_onBlockRemoval;
        vtable.setBlockBoundsBasedOnState = pp_setBlockBoundsBasedOnState;
        vtable.isPoweringTo = pp_isPoweringTo;
        vtable.isIndirectlyPoweringTo = pp_isIndirectlyPoweringTo;
        vtable.setBlockBoundsForItemRender = pp_setBlockBoundsForItemRender;
        vtable_init = 1;
    }
    BlockPressurePlate *self = (BlockPressurePlate *)calloc(1, sizeof(BlockPressurePlate));
    self->triggerMobType = mobType;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, mat);
    Block_setTickOnLoad(&self->base, 1);
    float var5 = 1.0f / 16.0f;
    Block_setBlockBounds(&self->base, var5, 0.0f, var5, 1.0f - var5, 0.03125f, 1.0f - var5);
    return &self->base;
}
