#include "Block.h"
#include "BlockFluid.h"
#include "Material.h"
#include "JavaRandom.h"
#include "IBlockAccess.h"
#include "World.h"
#include "Vec3D.h"
#include "JavaMath.h"
#include <stdlib.h>
#include <math.h>

int BlockFluid_colorMultiplier(Block *self, IBlockAccess *var1, int var2, int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return 16777215;
}

float BlockFluid_getPercentAir(int var0) {
    if (var0 >= 8) {
        var0 = 0;
    }
    float var1 = (float)(var0 + 1) / 9.0f;
    return var1;
}

static int BlockFluid_getBlockTextureFromSide(Block *self, int var1) {
    return var1 != 0 && var1 != 1 ? self->blockIndexInTexture + 1 : self->blockIndexInTexture;
}

int BlockFluid_getFlowDecay(Block *self, World *var1, int var2, int var3, int var4) {
    return World_getBlockMaterial(var1, var2, var3, var4) != self->blockMaterial
               ? -1
               : World_getBlockMetadata(var1, var2, var3, var4);
}

int BlockFluid_getEffectiveFlowDecay(Block *self, IBlockAccess *var1, int var2, int var3,
                                     int var4) {
    if (var1->vtable->getBlockMaterial(var1, var2, var3, var4) != self->blockMaterial) {
        return -1;
    } else {
        int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
        if (var5 >= 8) {
            var5 = 0;
        }
        return var5;
    }
}

static int BlockFluid_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}
static int BlockFluid_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

static int BlockFluid_canCollideCheck(Block *self, int var1, int var2) {
    (void)self;
    return var2 && var1 == 0;
}

static int BlockFluid_getIsBlockSolid(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                                      int var5) {
    Material *var6 = var1->vtable->getBlockMaterial(var1, var2, var3, var4);
    return var6 == self->blockMaterial
               ? 0
               : (var6 == Material_ice ? 0
                                       : (var5 == 1 ? 1
                                                    : Block_defaultVtable.getIsBlockSolid(
                                                          self, var1, var2, var3, var4, var5)));
}

static int BlockFluid_shouldSideBeRendered(Block *self, IBlockAccess *var1, int var2, int var3,
                                           int var4, int var5) {
    Material *var6 = var1->vtable->getBlockMaterial(var1, var2, var3, var4);
    return var6 == self->blockMaterial
               ? 0
               : (var6 == Material_ice ? 0
                                       : (var5 == 1 ? 1
                                                    : Block_defaultVtable.shouldSideBeRendered(
                                                          self, var1, var2, var3, var4, var5)));
}

static AxisAlignedBB *BlockFluid_getCollisionBoundingBoxFromPool(Block *self, World *var1, int var2,
                                                                 int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return NULL;
}

static int BlockFluid_getRenderType(Block *self) {
    (void)self;
    return 4;
}

static int BlockFluid_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 0;
}

static int BlockFluid_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 0;
}

static Vec3D *BlockFluid_getFlowVector(Block *self, IBlockAccess *var1, int var2, int var3,
                                       int var4) {
    Vec3D *var5 = Vec3D_createVector(0.0, 0.0, 0.0);
    int var6 = BlockFluid_getEffectiveFlowDecay(self, var1, var2, var3, var4);

    for (int var7 = 0; var7 < 4; ++var7) {
        int var8 = var2;
        int var10 = var4;
        if (var7 == 0) {
            var8 = var2 - 1;
        }
        if (var7 == 1) {
            var10 = var4 - 1;
        }
        if (var7 == 2) {
            ++var8;
        }
        if (var7 == 3) {
            ++var10;
        }

        int var11 = BlockFluid_getEffectiveFlowDecay(self, var1, var8, var3, var10);
        int var12;
        if (var11 < 0) {
            if (!Material_getIsSolid(var1->vtable->getBlockMaterial(var1, var8, var3, var10))) {
                var11 = BlockFluid_getEffectiveFlowDecay(self, var1, var8, var3 - 1, var10);
                if (var11 >= 0) {
                    var12 = var11 - (var6 - 8);
                    var5 = Vec3D_addVector(var5, (double)((var8 - var2) * var12),
                                           (double)((var3 - var3) * var12),
                                           (double)((var10 - var4) * var12));
                }
            }
        } else if (var11 >= 0) {
            var12 = var11 - var6;
            var5 =
                Vec3D_addVector(var5, (double)((var8 - var2) * var12),
                                (double)((var3 - var3) * var12), (double)((var10 - var4) * var12));
        }
    }

    if (var1->vtable->getBlockMetadata(var1, var2, var3, var4) >= 8) {
        int var13 = 0;
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2, var3, var4 - 1, 2)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2, var3, var4 + 1, 3)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2 - 1, var3, var4, 4)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2 + 1, var3, var4, 5)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2, var3 + 1, var4 - 1, 2)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2, var3 + 1, var4 + 1, 3)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2 - 1, var3 + 1, var4, 4)) {
            var13 = 1;
        }
        if (var13 || self->vtable->getIsBlockSolid(self, var1, var2 + 1, var3 + 1, var4, 5)) {
            var13 = 1;
        }
        if (var13) {
            var5 = Vec3D_addVector(Vec3D_normalize(var5), 0.0, -6.0, 0.0);
        }
    }

    var5 = Vec3D_normalize(var5);
    return var5;
}

static void BlockFluid_velocityToAddToEntity(Block *self, World *var1, int var2, int var3, int var4,
                                             struct Entity *var5, Vec3D *var6) {
    (void)var5;
    Vec3D *var7 = BlockFluid_getFlowVector(self, (IBlockAccess *)var1, var2, var3, var4);
    var6->xCoord += var7->xCoord;
    var6->yCoord += var7->yCoord;
    var6->zCoord += var7->zCoord;
}

static int BlockFluid_tickRate(Block *self) {
    if (self->blockMaterial == Material_water)
        return 5;
    if (self->blockMaterial == Material_lava)
        return 30;
    return 0;
}

float BlockFluid_getBlockBrightness(Block *self, IBlockAccess *var1, int var2, int var3, int var4) {
    (void)self;
    float var5 = var1->vtable->getLightBrightness(var1, var2, var3, var4);
    float var6 = var1->vtable->getLightBrightness(var1, var2, var3 + 1, var4);
    return var5 > var6 ? var5 : var6;
}

static void BlockFluid_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                  JavaRandom *var5) {
    Block_defaultVtable.updateTick(self, var1, var2, var3, var4, var5);
}

static int BlockFluid_getRenderBlockPass(Block *self) {
    return (self->blockMaterial == Material_water) ? 1 : 0;
}

static void BlockFluid_randomDisplayTick(Block *self, World *var1, int var2, int var3, int var4,
                                         JavaRandom *var5) {
    if (self->blockMaterial == Material_water && JavaRandom_nextInt(var5, 64) == 0) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if (var6 > 0 && var6 < 8) {
            World_playSoundEffect(var1, (double)((float)var2 + 0.5f), (double)((float)var3 + 0.5f),
                                  (double)((float)var4 + 0.5f), "liquid.water",
                                  JavaRandom_nextFloat(var5) * 0.25f + 12.0f / 16.0f,
                                  JavaRandom_nextFloat(var5) * 1.0f + 0.5f);
        }
    }
    if (self->blockMaterial == Material_lava &&
        World_getBlockMaterial(var1, var2, var3 + 1, var4) == Material_air &&
        !World_isBlockOpaqueCube(var1, var2, var3 + 1, var4) &&
        JavaRandom_nextInt(var5, 100) == 0) {
        double var12 = (double)((float)var2 + JavaRandom_nextFloat(var5));
        double var8 = (double)var3 + self->maxY;
        double var10 = (double)((float)var4 + JavaRandom_nextFloat(var5));
        World_spawnParticle(var1, "lava", var12, var8, var10, 0.0, 0.0, 0.0);
    }
}

double BlockFluid_func_293_a(IBlockAccess *var0, int var1, int var2, int var3, Material *var4) {
    Vec3D *var5 = NULL;
    if (var4 == Material_water) {
        var5 = BlockFluid_getFlowVector(Block_waterMoving, var0, var1, var2, var3);
    }
    if (var4 == Material_lava) {
        var5 = BlockFluid_getFlowVector(Block_lavaMoving, var0, var1, var2, var3);
    }
    return var5->xCoord == 0.0 && var5->zCoord == 0.0
               ? -1000.0
               : atan2(var5->zCoord, var5->xCoord) - M_PI * 0.5;
}

static void BlockFluid_checkForHarden(Block *self, World *var1, int var2, int var3, int var4) {
    if (World_getBlockId(var1, var2, var3, var4) == self->blockID) {
        if (self->blockMaterial == Material_lava) {
            int var5 = 0;
            if (var5 || World_getBlockMaterial(var1, var2, var3, var4 - 1) == Material_water) {
                var5 = 1;
            }
            if (var5 || World_getBlockMaterial(var1, var2, var3, var4 + 1) == Material_water) {
                var5 = 1;
            }
            if (var5 || World_getBlockMaterial(var1, var2 - 1, var3, var4) == Material_water) {
                var5 = 1;
            }
            if (var5 || World_getBlockMaterial(var1, var2 + 1, var3, var4) == Material_water) {
                var5 = 1;
            }
            if (var5 || World_getBlockMaterial(var1, var2, var3 + 1, var4) == Material_water) {
                var5 = 1;
            }
            if (var5) {
                int var6 = World_getBlockMetadata(var1, var2, var3, var4);
                if (var6 == 0) {
                    World_setBlockWithNotify(var1, var2, var3, var4, Block_obsidian->blockID);
                } else if (var6 <= 4) {
                    World_setBlockWithNotify(var1, var2, var3, var4, Block_cobblestone->blockID);
                }
                BlockFluid_triggerLavaMixEffects(self, var1, var2, var3, var4);
            }
        }
    }
}

void BlockFluid_triggerLavaMixEffects(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    World_playSoundEffect(
        var1, (double)((float)var2 + 0.5f), (double)((float)var3 + 0.5f),
        (double)((float)var4 + 0.5f), "random.fizz", 0.5f,
        2.6f + (JavaRandom_nextFloat(&var1->rand) - JavaRandom_nextFloat(&var1->rand)) * 0.8f);
    for (int var5 = 0; var5 < 8; ++var5) {
        World_spawnParticle(var1, "largesmoke", (double)var2 + Math_random(), (double)var3 + 1.2,
                            (double)var4 + Math_random(), 0.0, 0.0, 0.0);
    }
}

void BlockFluid_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    BlockFluid_checkForHarden(self, var1, var2, var3, var4);
}

void BlockFluid_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5) {
    (void)var5;
    BlockFluid_checkForHarden(self, var1, var2, var3, var4);
}

void BlockFluid_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->colorMultiplier = BlockFluid_colorMultiplier;
    vt->getBlockTextureFromSide = BlockFluid_getBlockTextureFromSide;
    vt->renderAsNormalBlock = BlockFluid_renderAsNormalBlock;
    vt->isOpaqueCube = BlockFluid_isOpaqueCube;
    vt->canCollideCheck = BlockFluid_canCollideCheck;
    vt->getIsBlockSolid = BlockFluid_getIsBlockSolid;
    vt->shouldSideBeRendered = BlockFluid_shouldSideBeRendered;
    vt->getCollisionBoundingBoxFromPool = BlockFluid_getCollisionBoundingBoxFromPool;
    vt->getRenderType = BlockFluid_getRenderType;
    vt->idDropped = BlockFluid_idDropped;
    vt->quantityDropped = BlockFluid_quantityDropped;
    vt->velocityToAddToEntity = BlockFluid_velocityToAddToEntity;
    vt->tickRate = BlockFluid_tickRate;
    vt->updateTick = BlockFluid_updateTick;
    vt->getRenderBlockPass = BlockFluid_getRenderBlockPass;
    vt->randomDisplayTick = BlockFluid_randomDisplayTick;
    vt->onBlockAdded = BlockFluid_onBlockAdded;
    vt->onNeighborBlockChange = BlockFluid_onNeighborBlockChange;
    vt->getBlockBrightness = BlockFluid_getBlockBrightness;
}
