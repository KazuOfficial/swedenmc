#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include <stdlib.h>

static AxisAlignedBB *reed_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                           int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

static int reed_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int reed_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int reed_getRenderType(Block *self) {
    (void)self;
    return 1;
}

static int reed_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 338;
}

static void reed_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                            JavaRandom *var5) {
    (void)var5;
    if (World_isAirBlock(var1, var2, var3 + 1, var4)) {
        int var6;
        for (var6 = 1; World_getBlockId(var1, var2, var3 - var6, var4) == self->blockID; ++var6) {
        }
        if (var6 < 3) {
            int var7 = World_getBlockMetadata(var1, var2, var3, var4);
            if (var7 == 15) {
                World_setBlockWithNotify(var1, var2, var3 + 1, var4, self->blockID);
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, 0);
            } else {
                World_setBlockMetadataWithNotify(var1, var2, var3, var4, var7 + 1);
            }
        }
    }
}

static int reed_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockId(var1, var2, var3 - 1, var4);
    return var5 == self->blockID
               ? 1
               : (var5 != Block_grass->blockID && var5 != Block_dirt->blockID
                      ? 0
                      : (World_getBlockMaterial(var1, var2 - 1, var3 - 1, var4) == Material_water
                             ? 1
                             : (World_getBlockMaterial(var1, var2 + 1, var3 - 1, var4) ==
                                        Material_water
                                    ? 1
                                    : (World_getBlockMaterial(var1, var2, var3 - 1, var4 - 1) ==
                                               Material_water
                                           ? 1
                                           : (World_getBlockMaterial(var1, var2, var3 - 1,
                                                                     var4 + 1) ==
                                              Material_water)))));
}

static void reed_checkBlockCoordValid(Block *self, World *var1, int var2, int var3, int var4) {
    if (!self->vtable->canBlockStay(self, var1, var2, var3, var4)) {
        Block_dropBlockAsItem(self, var1, var2, var3, var4,
                              World_getBlockMetadata(var1, var2, var3, var4));
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

static void reed_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                       int var5) {
    (void)var5;
    reed_checkBlockCoordValid(self, var1, var2, var3, var4);
}

static int reed_canBlockStay(Block *self, World *var1, int var2, int var3, int var4) {
    return reed_canPlaceBlockAt(self, var1, var2, var3, var4);
}

Block *BlockReed_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getCollisionBoundingBoxFromPool = reed_getCollisionBoundingBoxFromPool;
        vtable.isOpaqueCube = reed_isOpaqueCube;
        vtable.renderAsNormalBlock = reed_renderAsNormalBlock;
        vtable.getRenderType = reed_getRenderType;
        vtable.idDropped = reed_idDropped;
        vtable.updateTick = reed_updateTick;
        vtable.canPlaceBlockAt = reed_canPlaceBlockAt;
        vtable.onNeighborBlockChange = reed_onNeighborBlockChange;
        vtable.canBlockStay = reed_canBlockStay;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_plants);
    float var3 = 6.0f / 16.0f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, 1.0f, 0.5f + var3);
    Block_setTickOnLoad(self, 1);
    return self;
}
