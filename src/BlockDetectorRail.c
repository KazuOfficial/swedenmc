#include "BlockDetectorRail.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include <stdlib.h>

static int match_EntityMinecart(Entity *e) { return e->isMinecart; }

BlockVtable BlockDetectorRail_defaultVtable;

void BlockDetectorRail_initVtable(BlockVtable *vt) {
    BlockRail_initVtable(vt);
    vt->tickRate = BlockDetectorRail_tickRate;
    vt->canProvidePower = BlockDetectorRail_canProvidePower;
    vt->onEntityCollidedWithBlock = BlockDetectorRail_onEntityCollidedWithBlock;
    vt->updateTick = BlockDetectorRail_updateTick;
    vt->isPoweringTo = BlockDetectorRail_isPoweringTo;
    vt->isIndirectlyPoweringTo = BlockDetectorRail_isIndirectlyPoweringTo;
}

void BlockDetectorRail_construct(BlockDetectorRail *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockDetectorRail_initVtable(&BlockDetectorRail_defaultVtable);
        vtable_init = 1;
    }
    BlockRail_construct(&self->base, id, tex, 1);
    self->base.base.vtable = &BlockDetectorRail_defaultVtable;
    Block_setTickOnLoad(&self->base.base, 1);
}

BlockDetectorRail *BlockDetectorRail_create(int id, int tex) {
    BlockDetectorRail *self = (BlockDetectorRail *)calloc(1, sizeof(BlockDetectorRail));
    BlockDetectorRail_construct(self, id, tex);
    return self;
}

int BlockDetectorRail_tickRate(Block *self) {
    (void)self;
    return 20;
}

int BlockDetectorRail_canProvidePower(Block *self) {
    (void)self;
    return 1;
}

static void setStateIfMinecartInteractsWithRail(Block *self, World *var1, int var2, int var3,
                                                int var4, int var5) {
    int var6 = (var5 & 8) != 0;
    int var7 = 0;
    float var8 = 2.0F / 16.0F;
    AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(
        (double)((float)var2 + var8), (double)var3, (double)((float)var4 + var8),
        (double)((float)(var2 + 1) - var8), (double)var3 + 0.25,
        (double)((float)(var4 + 1) - var8));
    Entity *buf[16];
    int count = World_getEntitiesWithinAABB(var1, match_EntityMinecart, bb, buf, 16);
    if (count > 0) {
        var7 = 1;
    }

    if (var7 && !var6) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var5 | 8);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
    }

    if (!var7 && var6) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var5 & 7);
        World_notifyBlocksOfNeighborChange(var1, var2, var3, var4, self->blockID);
        World_notifyBlocksOfNeighborChange(var1, var2, var3 - 1, var4, self->blockID);
        World_markBlocksDirty(var1, var2, var3, var4, var2, var3, var4);
    }

    if (var7) {
        World_scheduleBlockUpdate(var1, var2, var3, var4, self->blockID,
                                  self->vtable->tickRate(self));
    }
}

void BlockDetectorRail_onEntityCollidedWithBlock(Block *self, World *var1, int var2, int var3,
                                                 int var4, Entity *var5) {
    (void)var5;
    if (!var1->isRemote) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if ((var6 & 8) == 0) {
            setStateIfMinecartInteractsWithRail(self, var1, var2, var3, var4, var6);
        }
    }
}

void BlockDetectorRail_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                  JavaRandom *var5) {
    (void)var5;
    if (!var1->isRemote) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if ((var6 & 8) != 0) {
            setStateIfMinecartInteractsWithRail(self, var1, var2, var3, var4, var6);
        }
    }
}

int BlockDetectorRail_isPoweringTo(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                                   int var5) {
    (void)self;
    (void)var5;
    return (var1->vtable->getBlockMetadata(var1, var2, var3, var4) & 8) != 0;
}

int BlockDetectorRail_isIndirectlyPoweringTo(Block *self, World *var1, int var2, int var3, int var4,
                                             int var5) {
    (void)self;
    return (World_getBlockMetadata(var1, var2, var3, var4) & 8) == 0 ? 0 : var5 == 1;
}
