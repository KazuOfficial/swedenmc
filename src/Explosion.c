#include "Explosion.h"
#include "World.h"
#include "Entity.h"
#include "Block.h"
#include "AxisAlignedBB.h"
#include "Vec3D.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

static void destroyed_add(Explosion *self, int x, int y, int z) {

    for (int i = 0; i < self->numDestroyed; ++i) {
        if (self->destroyedBlockPositions[i].x == x && self->destroyedBlockPositions[i].y == y &&
            self->destroyedBlockPositions[i].z == z) {
            return;
        }
    }
    if (self->numDestroyed >= self->destroyedCap) {
        int nc = self->destroyedCap ? self->destroyedCap * 2 : 64;
        self->destroyedBlockPositions =
            (ChunkPosition *)realloc(self->destroyedBlockPositions, nc * sizeof(ChunkPosition));
        self->destroyedCap = nc;
    }
    self->destroyedBlockPositions[self->numDestroyed].x = x;
    self->destroyedBlockPositions[self->numDestroyed].y = y;
    self->destroyedBlockPositions[self->numDestroyed].z = z;
    ++self->numDestroyed;
}

Explosion *Explosion_create(World *world, Entity *exploder, double x, double y, double z,
                            float size) {
    Explosion *self = (Explosion *)calloc(1, sizeof(Explosion));
    self->worldObj = world;
    self->exploder = exploder;
    self->explosionSize = size;
    self->explosionX = x;
    self->explosionY = y;
    self->explosionZ = z;

    static int64_t s_uniquifier = 8682522807148012LL;
    s_uniquifier ^= s_uniquifier << 8;
    s_uniquifier ^= (int64_t)((uint64_t)s_uniquifier >> 5);
    s_uniquifier ^= s_uniquifier << 15;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    int64_t nanoTime = (int64_t)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    JavaRandom_init(&self->ExplosionRNG, s_uniquifier ^ nanoTime);
    return self;
}

void Explosion_destroy(Explosion *self) {
    free(self->destroyedBlockPositions);
    free(self);
}

void Explosion_doExplosionA(Explosion *self) {
    float var1 = self->explosionSize;

    int var2 = 16;

    int var3, var4, var5;
    double var15, var17, var19;
    for (var3 = 0; var3 < var2; ++var3) {
        for (var4 = 0; var4 < var2; ++var4) {
            for (var5 = 0; var5 < var2; ++var5) {
                if (var3 == 0 || var3 == var2 - 1 || var4 == 0 || var4 == var2 - 1 || var5 == 0 ||
                    var5 == var2 - 1) {
                    double var6 = (double)((float)var3 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
                    double var8 = (double)((float)var4 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
                    double var10 = (double)((float)var5 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
                    double var12 = sqrt(var6 * var6 + var8 * var8 + var10 * var10);
                    var6 /= var12;
                    var8 /= var12;
                    var10 /= var12;

                    float var14 = self->explosionSize *
                                  (0.7F + JavaRandom_nextFloat(&self->worldObj->rand) * 0.6F);
                    var15 = self->explosionX;
                    var17 = self->explosionY;
                    var19 = self->explosionZ;

                    for (float var21 = 0.3F; var14 > 0.0F; var14 -= var21 * (12.0F / 16.0F)) {
                        int var22 = MathHelper_floor_double(var15);
                        int var23 = MathHelper_floor_double(var17);
                        int var24 = MathHelper_floor_double(var19);
                        int var25 = World_getBlockId(self->worldObj, var22, var23, var24);
                        if (var25 > 0) {
                            var14 -= (Block_blocksList[var25]->vtable->getExplosionResistance(
                                          Block_blocksList[var25], self->exploder) +
                                      0.3F) *
                                     var21;
                        }

                        if (var14 > 0.0F) {

                            destroyed_add(self, var22, var23, var24);
                        }

                        var15 += var6 * (double)var21;
                        var17 += var8 * (double)var21;
                        var19 += var10 * (double)var21;
                    }
                }
            }
        }
    }

    self->explosionSize *= 2.0F;
    var3 = MathHelper_floor_double(self->explosionX - (double)self->explosionSize - 1.0);
    var4 = MathHelper_floor_double(self->explosionX + (double)self->explosionSize + 1.0);
    var5 = MathHelper_floor_double(self->explosionY - (double)self->explosionSize - 1.0);
    int var29 = MathHelper_floor_double(self->explosionY + (double)self->explosionSize + 1.0);
    int var7 = MathHelper_floor_double(self->explosionZ - (double)self->explosionSize - 1.0);
    int var30 = MathHelper_floor_double(self->explosionZ + (double)self->explosionSize + 1.0);

    AxisAlignedBB *var9bb = AxisAlignedBB_getBoundingBoxFromPool(
        (double)var3, (double)var5, (double)var7, (double)var4, (double)var29, (double)var30);
    Entity *var9_buf[256];
    int var9Count = World_getEntitiesWithinAABBExcludingEntity(self->worldObj, self->exploder,
                                                               var9bb, var9_buf, 256);
    Entity **var9 = var9_buf;

    Vec3D *var31 = Vec3D_createVector(self->explosionX, self->explosionY, self->explosionZ);

    for (int var11 = 0; var11 < var9Count; ++var11) {
        Entity *var33 = var9[var11];
        double var13 =
            Entity_getDistance(var33, self->explosionX, self->explosionY, self->explosionZ) /
            (double)self->explosionSize;
        if (var13 <= 1.0) {
            var15 = var33->posX - self->explosionX;
            var17 = var33->posY - self->explosionY;
            var19 = var33->posZ - self->explosionZ;
            double var39 =
                (double)MathHelper_sqrt_double(var15 * var15 + var17 * var17 + var19 * var19);
            var15 /= var39;
            var17 /= var39;
            var19 /= var39;

            double var40 = (double)World_func_675_a(self->worldObj, var31, &var33->boundingBox);
            double var41 = (1.0 - var13) * var40;
            var33->vtable->attackEntityFrom(
                var33, self->exploder,
                (int)((var41 * var41 + var41) / 2.0 * 8.0 * (double)self->explosionSize + 1.0));
            var33->motionX += var15 * var41;
            var33->motionY += var17 * var41;
            var33->motionZ += var19 * var41;
        }
    }

    self->explosionSize = var1;

    if (self->isFlaming) {
        for (int var34 = self->numDestroyed - 1; var34 >= 0; --var34) {
            int var36 = self->destroyedBlockPositions[var34].x;
            int var37 = self->destroyedBlockPositions[var34].y;
            int var16 = self->destroyedBlockPositions[var34].z;
            int var38 = World_getBlockId(self->worldObj, var36, var37, var16);
            int var18 = World_getBlockId(self->worldObj, var36, var37 - 1, var16);
            if (var38 == 0 && Block_opaqueCubeLookup[var18] &&
                JavaRandom_nextInt(&self->ExplosionRNG, 3) == 0) {
                extern Block *Block_fire;
                World_setBlockWithNotify(self->worldObj, var36, var37, var16, Block_fire->blockID);
            }
        }
    }
}

void Explosion_doExplosionB(Explosion *self, int spawnParticles) {

    World_playSoundEffect(self->worldObj, self->explosionX, self->explosionY, self->explosionZ,
                          "random.explode", 4.0F,
                          (1.0F + (JavaRandom_nextFloat(&self->worldObj->rand) -
                                   JavaRandom_nextFloat(&self->worldObj->rand)) *
                                      0.2F) *
                              0.7F);

    for (int var3 = self->numDestroyed - 1; var3 >= 0; --var3) {
        int var5 = self->destroyedBlockPositions[var3].x;
        int var6 = self->destroyedBlockPositions[var3].y;
        int var7 = self->destroyedBlockPositions[var3].z;
        int var8 = World_getBlockId(self->worldObj, var5, var6, var7);

        if (spawnParticles) {
            double var9 = (double)((float)var5 + JavaRandom_nextFloat(&self->worldObj->rand));
            double var11 = (double)((float)var6 + JavaRandom_nextFloat(&self->worldObj->rand));
            double var13 = (double)((float)var7 + JavaRandom_nextFloat(&self->worldObj->rand));
            double var15 = var9 - self->explosionX;
            double var17 = var11 - self->explosionY;
            double var19 = var13 - self->explosionZ;
            double var21 =
                (double)MathHelper_sqrt_double(var15 * var15 + var17 * var17 + var19 * var19);
            var15 /= var21;
            var17 /= var21;
            var19 /= var21;
            double var23 = 0.5 / (var21 / (double)self->explosionSize + 0.1);
            var23 *= (double)(JavaRandom_nextFloat(&self->worldObj->rand) *
                                  JavaRandom_nextFloat(&self->worldObj->rand) +
                              0.3F);
            var15 *= var23;
            var17 *= var23;
            var19 *= var23;

            World_spawnParticle(self->worldObj, "explode", (var9 + self->explosionX * 1.0) / 2.0,
                                (var11 + self->explosionY * 1.0) / 2.0,
                                (var13 + self->explosionZ * 1.0) / 2.0, var15, var17, var19);
            World_spawnParticle(self->worldObj, "smoke", var9, var11, var13, var15, var17, var19);
        }

        if (var8 > 0) {
            Block_blocksList[var8]->vtable->dropBlockAsItemWithChance(
                Block_blocksList[var8], self->worldObj, var5, var6, var7,
                World_getBlockMetadata(self->worldObj, var5, var6, var7), 0.3F);
            World_setBlockWithNotify(self->worldObj, var5, var6, var7, 0);
            Block_blocksList[var8]->vtable->onBlockDestroyedByExplosion(
                Block_blocksList[var8], self->worldObj, var5, var6, var7);
        }
    }
}
