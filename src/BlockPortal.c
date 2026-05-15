#include "BlockBreakable.h"
#include "Block.h"
#include "Material.h"
#include "World.h"
#include "IBlockAccess.h"
#include "Entity.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockPortal_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}
static int BlockPortal_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockPortal_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}
static int BlockPortal_getRenderBlockPass(Block *self) {
    (void)self;
    return 1;
}

static AxisAlignedBB *BlockPortal_getCollisionBoundingBoxFromPool(Block *self, World *w, int x,
                                                                  int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return NULL;
}

static void BlockPortal_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2,
                                                   int var3, int var4) {
    float var5, var6;
    if (var1->vtable->getBlockId(var1, var2 - 1, var3, var4) != self->blockID &&
        var1->vtable->getBlockId(var1, var2 + 1, var3, var4) != self->blockID) {
        var5 = 2.0f / 16.0f;
        var6 = 0.5f;
    } else {
        var5 = 0.5f;
        var6 = 2.0f / 16.0f;
    }
    Block_setBlockBounds(self, 0.5f - var5, 0.0f, 0.5f - var6, 0.5f + var5, 1.0f, 0.5f + var6);
}

static int BlockPortal_shouldSideBeRendered(Block *self, IBlockAccess *var1, int var2, int var3,
                                            int var4, int var5) {
    if (var1->vtable->getBlockId(var1, var2, var3, var4) == self->blockID)
        return 0;
    int var6 = var1->vtable->getBlockId(var1, var2 - 1, var3, var4) == self->blockID &&
               var1->vtable->getBlockId(var1, var2 - 2, var3, var4) != self->blockID;
    int var7 = var1->vtable->getBlockId(var1, var2 + 1, var3, var4) == self->blockID &&
               var1->vtable->getBlockId(var1, var2 + 2, var3, var4) != self->blockID;
    int var8 = var1->vtable->getBlockId(var1, var2, var3, var4 - 1) == self->blockID &&
               var1->vtable->getBlockId(var1, var2, var3, var4 - 2) != self->blockID;
    int var9 = var1->vtable->getBlockId(var1, var2, var3, var4 + 1) == self->blockID &&
               var1->vtable->getBlockId(var1, var2, var3, var4 + 2) != self->blockID;
    int var10 = var6 || var7;
    int var11 = var8 || var9;
    return var10 && var5 == 4
               ? 1
               : (var10 && var5 == 5 ? 1 : (var11 && var5 == 2 ? 1 : var11 && var5 == 3));
}

int BlockPortal_tryToCreatePortal(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    int var5 = 0, var6 = 0;
    if (World_getBlockId(var1, var2 - 1, var3, var4) == Block_obsidian->blockID ||
        World_getBlockId(var1, var2 + 1, var3, var4) == Block_obsidian->blockID) {
        var5 = 1;
    }
    if (World_getBlockId(var1, var2, var3, var4 - 1) == Block_obsidian->blockID ||
        World_getBlockId(var1, var2, var3, var4 + 1) == Block_obsidian->blockID) {
        var6 = 1;
    }
    if (var5 == var6) {
        return 0;
    }
    if (World_getBlockId(var1, var2 - var5, var3, var4 - var6) == 0) {
        var2 -= var5;
        var4 -= var6;
    }
    int var7, var8;
    for (var7 = -1; var7 <= 2; ++var7) {
        for (var8 = -1; var8 <= 3; ++var8) {
            int var9 = var7 == -1 || var7 == 2 || var8 == -1 || var8 == 3;
            if ((var7 != -1 && var7 != 2) || (var8 != -1 && var8 != 3)) {
                int var10 =
                    World_getBlockId(var1, var2 + var5 * var7, var3 + var8, var4 + var6 * var7);
                if (var9) {
                    if (var10 != Block_obsidian->blockID)
                        return 0;
                } else if (var10 != 0 && var10 != Block_fire->blockID)
                    return 0;
            }
        }
    }
    var1->editingBlocks = 1;
    for (var7 = 0; var7 < 2; ++var7) {
        for (var8 = 0; var8 < 3; ++var8) {
            World_setBlockWithNotify(var1, var2 + var5 * var7, var3 + var8, var4 + var6 * var7,
                                     Block_portal->blockID);
        }
    }
    var1->editingBlocks = 0;
    return 1;
}

static void BlockPortal_onNeighborBlockChange(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5) {
    (void)var5;
    int var6 = 0, var7 = 1;
    if (World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID ||
        World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID) {
        var6 = 1;
        var7 = 0;
    }
    int var8;
    for (var8 = var3; World_getBlockId(var1, var2, var8 - 1, var4) == self->blockID; --var8) {
    }
    if (World_getBlockId(var1, var2, var8 - 1, var4) != Block_obsidian->blockID) {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    } else {
        int var9;
        for (var9 = 1; var9 < 4 && World_getBlockId(var1, var2, var8 + var9, var4) == self->blockID;
             ++var9) {
        }
        if (var9 == 3 &&
            World_getBlockId(var1, var2, var8 + var9, var4) == Block_obsidian->blockID) {
            int var10 = World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID ||
                        World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID;
            int var11 = World_getBlockId(var1, var2, var3, var4 - 1) == self->blockID ||
                        World_getBlockId(var1, var2, var3, var4 + 1) == self->blockID;
            if (var10 && var11) {
                World_setBlockWithNotify(var1, var2, var3, var4, 0);
            } else if ((World_getBlockId(var1, var2 + var6, var3, var4 + var7) !=
                            Block_obsidian->blockID ||
                        World_getBlockId(var1, var2 - var6, var3, var4 - var7) != self->blockID) &&
                       (World_getBlockId(var1, var2 - var6, var3, var4 - var7) !=
                            Block_obsidian->blockID ||
                        World_getBlockId(var1, var2 + var6, var3, var4 + var7) != self->blockID)) {
                World_setBlockWithNotify(var1, var2, var3, var4, 0);
            }
        } else {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
    }
}

static void BlockPortal_onEntityCollidedWithBlock(Block *self, World *world, int x, int y, int z,
                                                  Entity *entity) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
    if (entity->ridingEntity == NULL && entity->riddenByEntity == NULL) {
        entity->vtable->setInPortal(entity);
    }
}

static void BlockPortal_randomDisplayTick(Block *self, World *var1, int var2, int var3, int var4,
                                          JavaRandom *var5) {
    if (JavaRandom_nextInt(var5, 100) == 0) {
        World_playSoundEffect(var1, (double)var2 + 0.5, (double)var3 + 0.5, (double)var4 + 0.5,
                              "portal.portal", 1.0f, JavaRandom_nextFloat(var5) * 0.4f + 0.8f);
    }
    int var6;
    for (var6 = 0; var6 < 4; ++var6) {
        double var7 = (double)((float)var2 + JavaRandom_nextFloat(var5));
        double var9 = (double)((float)var3 + JavaRandom_nextFloat(var5));
        double var11 = (double)((float)var4 + JavaRandom_nextFloat(var5));
        double var13 = 0.0, var15 = 0.0, var17 = 0.0;
        int var19 = JavaRandom_nextInt(var5, 2) * 2 - 1;
        var13 = ((double)JavaRandom_nextFloat(var5) - 0.5) * 0.5;
        var15 = ((double)JavaRandom_nextFloat(var5) - 0.5) * 0.5;
        var17 = ((double)JavaRandom_nextFloat(var5) - 0.5) * 0.5;
        if (World_getBlockId(var1, var2 - 1, var3, var4) != self->blockID &&
            World_getBlockId(var1, var2 + 1, var3, var4) != self->blockID) {
            var7 = (double)var2 + 0.5 + 0.25 * (double)var19;
            var13 = (double)(JavaRandom_nextFloat(var5) * 2.0f * (float)var19);
        } else {
            var11 = (double)var4 + 0.5 + 0.25 * (double)var19;
            var17 = (double)(JavaRandom_nextFloat(var5) * 2.0f * (float)var19);
        }
        World_spawnParticle(var1, "portal", var7, var9, var11, var13, var15, var17);
    }
}

Block *BlockPortal_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.isOpaqueCube = BlockPortal_isOpaqueCube;
        vtable.renderAsNormalBlock = BlockPortal_renderAsNormalBlock;
        vtable.getRenderBlockPass = BlockPortal_getRenderBlockPass;
        vtable.quantityDropped = BlockPortal_quantityDropped;
        vtable.getCollisionBoundingBoxFromPool = BlockPortal_getCollisionBoundingBoxFromPool;
        vtable.setBlockBoundsBasedOnState = BlockPortal_setBlockBoundsBasedOnState;
        vtable.shouldSideBeRendered = BlockPortal_shouldSideBeRendered;
        vtable.onNeighborBlockChange = BlockPortal_onNeighborBlockChange;
        vtable.onEntityCollidedWithBlock = BlockPortal_onEntityCollidedWithBlock;
        vtable.randomDisplayTick = BlockPortal_randomDisplayTick;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_portal);
    Block_setLightOpacity(self, 0);
    return self;
}
