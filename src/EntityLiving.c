#include "EntityLiving.h"
#include "World.h"
#include "Block.h"
#include "StepSound.h"
#include "Item.h"
#include "MathHelper.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include "Vec3D.h"
#include "JavaRandom.h"
#include "JavaMath.h"
#include "MovingObjectPosition.h"
#include <string.h>
#include <math.h>

static int def_getMaxHealth(EntityLiving *self) {
    (void)self;
    return 10;
}
static int def_getHealth(EntityLiving *self) { return self->health; }

static void def_setHealth(EntityLiving *self, int health) { self->health = health; }

static void def_heal(EntityLiving *self, int var1) {
    if (self->health > 0) {
        self->health += var1;
        if (self->health > 20) {
            self->health = 20;
        }
        self->base.heartsLife = self->heartsHalvesLife / 2;
    }
}

static void def_knockBack(EntityLiving *self, Entity *source, int damage, double vx, double vz) {
    (void)source;
    (void)damage;
    float var7 = MathHelper_sqrt_double(vx * vx + vz * vz);
    float var8 = 0.4f;
    Entity *e = &self->base;
    e->motionX /= 2.0;
    e->motionY /= 2.0;
    e->motionZ /= 2.0;
    e->motionX -= vx / (double)var7 * (double)var8;
    e->motionY += (double)0.4f;
    e->motionZ -= vz / (double)var7 * (double)var8;
    if (e->motionY > (double)0.4f)
        e->motionY = (double)0.4f;
}

static void def_fall_living(Entity *base, float var1) {
    Entity_defaultVtable.fall(base, var1);
    int var2 = (int)ceil((double)(var1 - 3.0f));
    if (var2 > 0) {
        base->vtable->attackEntityFrom(base, NULL, var2);
        int var3 =
            World_getBlockId(base->worldObj, MathHelper_floor_double(base->posX),
                             MathHelper_floor_double(base->posY - 0.2 - (double)base->yOffset),
                             MathHelper_floor_double(base->posZ));
        if (var3 > 0) {
            StepSound *var4 = Block_blocksList[var3]->stepSound;
            World_playSoundAtEntity(base->worldObj, base, StepSound_func_1145_d(var4),
                                    StepSound_getVolume(var4) * 0.5f,
                                    StepSound_getPitch(var4) * 0.75f);
        }
    }
}

static void def_damageEntity(EntityLiving *self, Entity *source, int var1) {
    (void)source;
    self->health -= var1;
}

static void def_onDeath(EntityLiving *self, Entity *var1) {
    Entity *base = &self->base;
    if (self->scoreValue >= 0 && var1 != NULL) {
        Entity_addToPlayerScore(var1, base, self->scoreValue);
    }
    if (var1 != NULL) {
        Entity_onKillEntity(var1, base);
    }
    self->unused_flag = 1;
    if (!base->worldObj->multiplayerWorld) {
        ((EntityLivingVtable *)base->vtable)->dropFewItems(self);
    }
    World_func_9425_a(base->worldObj, base, 3);
}

static int def_attackEntityFrom(Entity *base, Entity *var1, int var2) {
    EntityLiving *self = (EntityLiving *)base;
    EntityLivingVtable *vt = (EntityLivingVtable *)base->vtable;
    if (base->worldObj->multiplayerWorld) {
        return 0;
    }
    self->entityAge = 0;
    if (self->health <= 0) {
        return 0;
    }
    self->field_704_R = 1.5f;
    int var3 = 1;
    if ((float)base->heartsLife > (float)self->heartsHalvesLife / 2.0f) {
        if (var2 <= self->field_9346_af) {
            return 0;
        }
        vt->damageEntity(self, var1, var2 - self->field_9346_af);
        self->field_9346_af = var2;
        var3 = 0;
    } else {
        self->field_9346_af = var2;
        self->prevHealth = self->health;
        base->heartsLife = self->heartsHalvesLife;
        vt->damageEntity(self, var1, var2);
        self->hurtTime = self->maxHurtTime = 10;
    }
    self->attackedAtYaw = 0.0f;
    if (var3) {
        World_func_9425_a(base->worldObj, base, 2);
        Entity_setBeenAttacked(base);
        if (var1 != NULL) {
            double var4 = var1->posX - base->posX;
            double var6 = var1->posZ - base->posZ;
            for (; var4 * var4 + var6 * var6 < 1.0e-4;
                 var6 = (Math_random() - Math_random()) * 0.01) {
                var4 = (Math_random() - Math_random()) * 0.01;
            }
            self->attackedAtYaw =
                (float)(atan2(var6, var4) * 180.0 / (double)3.14159265358979323846f) -
                base->rotationYaw;
            vt->knockBack(self, var1, var2, var4, var6);
        } else {
            self->attackedAtYaw = (float)((int)(Math_random() * 2.0) * 180);
        }
    }
    if (self->health <= 0) {
        if (var3) {
            World_playSoundAtEntity(
                base->worldObj, base, vt->getDeathSound(self), vt->getSoundVolume(self),
                (JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand)) * 0.2f +
                    1.0f);
        }
        vt->onDeath(self, var1);
    } else if (var3) {
        World_playSoundAtEntity(
            base->worldObj, base, vt->getHurtSound(self), vt->getSoundVolume(self),
            (JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand)) * 0.2f + 1.0f);
    }
    return 1;
}

static ItemStack *def_getHeldItem(EntityLiving *self) {
    if (!self->equipmentHas[EQUIPMENT_SLOT_MAINHAND])
        return NULL;
    return &self->equipment[EQUIPMENT_SLOT_MAINHAND];
}

static ItemStack *def_getEquipmentInSlot(EntityLiving *self, int slot) {
    if (slot < 0 || slot >= EQUIPMENT_NUM_SLOTS)
        return NULL;
    if (!self->equipmentHas[slot])
        return NULL;
    return &self->equipment[slot];
}

static void def_setCurrentItemOrArmor(EntityLiving *self, int slot, ItemStack *stack) {
    if (slot < 0 || slot >= EQUIPMENT_NUM_SLOTS)
        return;
    if (stack) {
        self->equipment[slot] = *stack;
        self->equipmentHas[slot] = 1;
    } else {
        self->equipmentHas[slot] = 0;
    }
}

static void def_dropEquipment(EntityLiving *self, int recentlyHit, int lootingLevel) {
    (void)recentlyHit;
    (void)lootingLevel;
    for (int i = 0; i < EQUIPMENT_NUM_SLOTS; i++) {
        if (!self->equipmentHas[i])
            continue;
        Entity_entityDropItem(&self->base, self->equipment[i], 0.0f);
        self->equipmentHas[i] = 0;
    }
}

static int def_isEntityUndead(EntityLiving *self) {
    (void)self;
    return 0;
}

static int def_canBreatheUnderwater(EntityLiving *self) {
    (void)self;
    return 0;
}

static int def_getTalkInterval(EntityLiving *self) {
    (void)self;
    return 80;
}

static void def_dropFewItems(EntityLiving *self) {
    int var1 = ((EntityLivingVtable *)self->base.vtable)->getDropItemId(self);
    if (var1 > 0) {
        int var2 = JavaRandom_nextInt(&self->base.rand, 3);
        int var3;
        for (var3 = 0; var3 < var2; ++var3) {
            Entity_dropItem(&self->base, var1, 1);
        }
    }
}

static const char *def_getEntityTexture(EntityLiving *self) { return self->texture; }

static int def_canDespawn(EntityLiving *self) {
    (void)self;
    return 1;
}

static int def_func_25026_x(EntityLiving *self) {
    (void)self;
    return 40;
}

static const char *def_getEntityString(Entity *self) {
    (void)self;
    return NULL;
}

static void def_onEntityUpdate(Entity *base) {
    EntityLiving *self = (EntityLiving *)base;

    self->prevSwingProgress = self->swingProgress;

    Entity_defaultVtable.onEntityUpdate(base);

    if (JavaRandom_nextInt(&base->rand, 1000) < self->livingSoundTime++) {
        self->livingSoundTime = -((EntityLivingVtable *)base->vtable)->getTalkInterval(self);

        const char *var1 = ((EntityLivingVtable *)base->vtable)->getLivingSound(self);
        if (var1 != NULL) {
            World_playSoundAtEntity(
                base->worldObj, base, var1,
                ((EntityLivingVtable *)base->vtable)->getSoundVolume(self),
                (JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand)) * 0.2f +
                    1.0f);
        }
    }

    if (!base->isDead && self->health > 0 && Entity_isEntityInsideOpaqueBlock(base)) {
        base->vtable->attackEntityFrom(base, NULL, 1);
    }

    if (base->isImmuneToFire || (base->worldObj && base->worldObj->multiplayerWorld)) {
        base->fire = 0;
    }

    if (!base->isDead && self->health > 0 && Entity_isInsideOfMaterial(base, Material_water) &&
        !((EntityLivingVtable *)base->vtable)->canBreatheUnderwater(self)) {
        --base->air;
        if (base->air == -20) {
            base->air = 0;
            int var1;
            for (var1 = 0; var1 < 8; ++var1) {
                float var2 = JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand);
                float var3 = JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand);
                float var4 = JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand);
                World_spawnParticle(base->worldObj, "bubble", base->posX + (double)var2,
                                    base->posY + (double)var3, base->posZ + (double)var4,
                                    base->motionX, base->motionY, base->motionZ);
            }
            base->vtable->attackEntityFrom(base, NULL, 2);
        }
        base->fire = 0;
    } else {
        base->air = base->maxAir;
    }

    self->cameraPitch = self->field_9328_R;

    if (self->attackTime > 0)
        --self->attackTime;
    if (self->hurtTime > 0)
        --self->hurtTime;
    if (base->heartsLife > 0)
        --base->heartsLife;

    if (self->health <= 0) {
        ++self->deathTime;
        if (self->deathTime > 20) {

            ((EntityLivingVtable *)base->vtable)->onEntityDeath(self);
            Entity_setEntityDead(base);
            int var1;
            for (var1 = 0; var1 < 20; ++var1) {
                double var8 = JavaRandom_nextGaussian(&base->rand) * 0.02;
                double var9 = JavaRandom_nextGaussian(&base->rand) * 0.02;
                double var6 = JavaRandom_nextGaussian(&base->rand) * 0.02;
                World_spawnParticle(
                    base->worldObj, "explode",
                    base->posX + (double)(JavaRandom_nextFloat(&base->rand) * base->width * 2.0f) -
                        (double)base->width,
                    base->posY + (double)(JavaRandom_nextFloat(&base->rand) * base->height),
                    base->posZ + (double)(JavaRandom_nextFloat(&base->rand) * base->width * 2.0f) -
                        (double)base->width,
                    var8, var9, var6);
            }
        }
    }

    self->field_9359_x = self->field_9360_w;
    self->prevRenderYawOffset = self->renderYawOffset;
    base->prevRotationYaw = base->rotationYaw;
    base->prevRotationPitch = base->rotationPitch;
}

void EntityLiving_onLivingUpdateBase(EntityLiving *self) {
    Entity *base = &self->base;

    if (self->newPosRotationIncrements > 0) {
        double var1 =
            base->posX + (self->newPosX - base->posX) / (double)self->newPosRotationIncrements;
        double var3 =
            base->posY + (self->newPosY - base->posY) / (double)self->newPosRotationIncrements;
        double var5 =
            base->posZ + (self->newPosZ - base->posZ) / (double)self->newPosRotationIncrements;
        double var7;
        for (var7 = self->newRotationYaw - (double)base->rotationYaw; var7 < -180.0;
             var7 += 360.0) {
        }
        while (var7 >= 180.0) {
            var7 -= 360.0;
        }
        base->rotationYaw =
            (float)((double)base->rotationYaw + var7 / (double)self->newPosRotationIncrements);
        base->rotationPitch = (float)((double)base->rotationPitch +
                                      (self->newRotationPitch - (double)base->rotationPitch) /
                                          (double)self->newPosRotationIncrements);
        --self->newPosRotationIncrements;
        base->vtable->setPosition(base, var1, var3, var5);
        Entity_setRotation(base, base->rotationYaw, base->rotationPitch);
        if (base->worldObj) {

            AxisAlignedBB *expandedBB =
                AxisAlignedBB_func_28195_e(&base->boundingBox, 1.0 / 32.0, 0.0, 1.0 / 32.0);
#define MAX_COLLIDE 64
            AxisAlignedBB collideOut[MAX_COLLIDE];
            int nCollide = World_getCollidingBoundingBoxes(base->worldObj, base, expandedBB,
                                                           collideOut, MAX_COLLIDE);
            if (nCollide > 0) {
                double var10 = 0.0;
                int var12;
                for (var12 = 0; var12 < nCollide; ++var12) {
                    if (collideOut[var12].maxY > var10)
                        var10 = collideOut[var12].maxY;
                }
                var3 += var10 - base->boundingBox.minY;
                base->vtable->setPosition(base, var1, var3, var5);
            }
#undef MAX_COLLIDE
        }
    }

    if (((EntityLivingVtable *)base->vtable)->isMovementBlocked(self)) {
        self->isJumping = 0;
        self->moveStrafing = 0.0f;
        self->moveForward = 0.0f;
        self->randomYawVelocity = 0.0f;
    } else if (!self->isMultiplayerEntity) {
        ((EntityLivingVtable *)base->vtable)->updatePlayerActionState(self);
    }

    int var14 = Entity_isInWater(base);
    int var2 = Entity_handleLavaMovement(base);
    if (self->isJumping) {
        if (var14) {
            base->motionY += (double)0.04f;
        } else if (var2) {
            base->motionY += (double)0.04f;
        } else if (base->onGround) {
            ((EntityLivingVtable *)base->vtable)->jump(self);
        }
    }

    self->moveStrafing *= 0.98f;
    self->moveForward *= 0.98f;
    self->randomYawVelocity *= 0.9f;

    ((EntityLivingVtable *)base->vtable)
        ->moveEntityWithHeading(self, self->moveStrafing, self->moveForward);

    if (base->worldObj) {
        AxisAlignedBB *pushBB =
            AxisAlignedBB_expand(&base->boundingBox, (double)0.2f, 0.0, (double)0.2f);
#define MAX_PUSH 64
        Entity *pushList[MAX_PUSH];
        int nPush = World_getEntitiesWithinAABBExcludingEntity(base->worldObj, base, pushBB,
                                                               pushList, MAX_PUSH);
        if (nPush > 0) {
            int var4;
            for (var4 = 0; var4 < nPush; ++var4) {
                Entity *var16 = pushList[var4];
                if (var16->vtable->canBePushed(var16)) {
                    var16->vtable->applyEntityCollision(var16, base);
                }
            }
        }
#undef MAX_PUSH
    }
}

static void def_onLivingUpdate(EntityLiving *self) { EntityLiving_onLivingUpdateBase(self); }

static void def_moveEntityWithHeading(EntityLiving *self, float strafe, float forward) {
    EntityLiving_moveEntityWithHeading(self, strafe, forward);
}

static int def_isOnLadder(EntityLiving *self) {
    Entity *base = &self->base;
    if (!base->worldObj)
        return 0;
    int var1 = MathHelper_floor_double(base->posX);
    int var2 = MathHelper_floor_double(base->boundingBox.minY);
    int var3 = MathHelper_floor_double(base->posZ);
    int var4 = World_getBlockId(base->worldObj, var1, var2, var3);
    return var4 == Block_ladder->blockID;
}

static void def_updatePlayerActionState(EntityLiving *self) {
    Entity *base = &self->base;

    ++self->entityAge;

    Entity *var1 =
        (base->worldObj) ? World_getClosestPlayerToEntity(base->worldObj, base, -1.0) : NULL;

    EntityLiving_func_27021_X(self);
    self->moveStrafing = 0.0f;
    self->moveForward = 0.0f;

    float var2 = 8.0f;
    if (JavaRandom_nextFloat(&base->rand) < 0.02f) {
        var1 = (base->worldObj) ? World_getClosestPlayerToEntity(base->worldObj, base, (double)var2)
                                : NULL;
        if (var1 != NULL) {
            self->currentTarget = var1;
            self->numTicksToChaseTarget = 10 + JavaRandom_nextInt(&base->rand, 20);
        } else {
            self->randomYawVelocity = (JavaRandom_nextFloat(&base->rand) - 0.5f) * 20.0f;
        }
    }

    if (self->currentTarget != NULL) {
        EntityLiving_faceEntity(self, self->currentTarget, 10.0f,
                                (float)((EntityLivingVtable *)base->vtable)->func_25026_x(self));
        if (self->numTicksToChaseTarget-- <= 0 || self->currentTarget->isDead ||
            Entity_getDistanceSqToEntity(self->currentTarget, base) > (double)(var2 * var2)) {
            self->currentTarget = NULL;
        }
    } else {
        if (JavaRandom_nextFloat(&base->rand) < 0.05f) {
            self->randomYawVelocity = (JavaRandom_nextFloat(&base->rand) - 0.5f) * 20.0f;
        }
        base->rotationYaw += self->randomYawVelocity;
        base->rotationPitch = self->defaultPitch;
    }

    int var3 = Entity_isInWater(base);
    int var4 = Entity_handleLavaMovement(base);
    if (var3 || var4) {
        self->isJumping = (JavaRandom_nextFloat(&base->rand) < 0.8f);
    }
}

static const char *def_getLivingSound(EntityLiving *self) {
    (void)self;
    return NULL;
}

static const char *def_getHurtSound(EntityLiving *self) {
    (void)self;
    return "random.hurt";
}

static const char *def_getDeathSound(EntityLiving *self) {
    (void)self;
    return "random.hurt";
}

static int def_getDropItemId(EntityLiving *self) {
    (void)self;
    return 0;
}

static float def_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 1.0F;
}

static void def_handleHealthUpdate(Entity *base, int8_t var1) {
    EntityLiving *self = (EntityLiving *)base;
    EntityLivingVtable *vt = (EntityLivingVtable *)base->vtable;
    if (var1 == 2) {
        self->field_704_R = 1.5f;
        base->heartsLife = self->heartsHalvesLife;
        self->hurtTime = self->maxHurtTime = 10;
        self->attackedAtYaw = 0.0f;
        World_playSoundAtEntity(
            base->worldObj, base, vt->getHurtSound(self), vt->getSoundVolume(self),
            (JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand)) * 0.2f + 1.0f);
        base->vtable->attackEntityFrom(base, NULL, 0);
    } else if (var1 == 3) {
        World_playSoundAtEntity(
            base->worldObj, base, vt->getDeathSound(self), vt->getSoundVolume(self),
            (JavaRandom_nextFloat(&base->rand) - JavaRandom_nextFloat(&base->rand)) * 0.2f + 1.0f);
        self->health = 0;
        vt->onDeath(self, NULL);
    } else {
        Entity_defaultVtable.handleHealthUpdate(base, var1);
    }
}

static int def_getCanSpawnHere(EntityLiving *self) {
    Entity *base = &self->base;
    if (!base->worldObj)
        return 0;
    if (!World_checkIfAABBIsClear(base->worldObj, &base->boundingBox))
        return 0;
    AxisAlignedBB collideOut[1];
    if (World_getCollidingBoundingBoxes(base->worldObj, base, &base->boundingBox, collideOut, 1) >
        0)
        return 0;
    if (World_getIsAnyLiquid(base->worldObj, &base->boundingBox))
        return 0;
    return 1;
}

static int def_getMaxSpawnedInChunk(EntityLiving *self) {
    (void)self;
    return 4;
}

static void def_onUpdate(Entity *base) {
    EntityLiving *self = (EntityLiving *)base;

    base->vtable->onEntityUpdate(base);

    ((EntityLivingVtable *)base->vtable)->onLivingUpdate(self);

    double var1 = base->posX - base->prevPosX;
    double var3 = base->posZ - base->prevPosZ;
    float var5 = MathHelper_sqrt_double(var1 * var1 + var3 * var3);
    float var6 = self->renderYawOffset;
    float var7 = 0.0f;

    self->field_9362_u = self->field_9361_v;

    float var8 = 0.0f;
    if (var5 > 0.05f) {
        var8 = 1.0f;
        var7 = var5 * 3.0f;
        var6 = (float)atan2(var3, var1) * 180.0f / (float)M_PI - 90.0f;
    }

    if (self->swingProgress > 0.0f) {
        var6 = base->rotationYaw;
    }

    if (!base->onGround) {
        var8 = 0.0f;
    }

    self->field_9361_v += (var8 - self->field_9361_v) * 0.3f;

    float var9;
    for (var9 = var6 - self->renderYawOffset; var9 < -180.0f; var9 += 360.0f) {
    }
    while (var9 >= 180.0f) {
        var9 -= 360.0f;
    }
    self->renderYawOffset += var9 * 0.3f;

    float var10;
    for (var10 = base->rotationYaw - self->renderYawOffset; var10 < -180.0f; var10 += 360.0f) {
    }
    while (var10 >= 180.0f) {
        var10 -= 360.0f;
    }
    int var11 = (var10 < -90.0f || var10 >= 90.0f);
    if (var10 < -75.0f)
        var10 = -75.0f;
    if (var10 >= 75.0f)
        var10 = 75.0f;
    self->renderYawOffset = base->rotationYaw - var10;
    if (var10 * var10 > 2500.0f) {
        self->renderYawOffset += var10 * 0.2f;
    }

    if (var11) {
        var7 *= -1.0f;
    }

    while (base->rotationYaw - base->prevRotationYaw < -180.0f) {
        base->prevRotationYaw -= 360.0f;
    }
    while (base->rotationYaw - base->prevRotationYaw >= 180.0f) {
        base->prevRotationYaw += 360.0f;
    }
    while (self->renderYawOffset - self->prevRenderYawOffset < -180.0f) {
        self->prevRenderYawOffset -= 360.0f;
    }
    while (self->renderYawOffset - self->prevRenderYawOffset >= 180.0f) {
        self->prevRenderYawOffset += 360.0f;
    }
    while (base->rotationPitch - base->prevRotationPitch < -180.0f) {
        base->prevRotationPitch -= 360.0f;
    }
    while (base->rotationPitch - base->prevRotationPitch >= 180.0f) {
        base->prevRotationPitch += 360.0f;
    }

    self->field_9360_w += var7;
}

void EntityLiving_moveEntityWithHeading(EntityLiving *self, float var1, float var2) {

    Entity *base = &self->base;

    if (Entity_isInWater(base)) {
        double var3 = base->posY;
        Entity_moveFlying(base, var1, var2, 0.02f);
        base->vtable->moveEntity(base, base->motionX, base->motionY, base->motionZ);
        base->motionX *= (double)0.8f;
        base->motionY *= (double)0.8f;
        base->motionZ *= (double)0.8f;
        base->motionY -= 0.02;
        if (base->isCollidedHorizontally &&
            Entity_isOffsetPositionInLiquid(base, base->motionX,
                                            base->motionY + (double)0.6f - base->posY + var3,
                                            base->motionZ)) {
            base->motionY = (double)0.3f;
        }
    } else if (Entity_handleLavaMovement(base)) {
        double var3 = base->posY;
        Entity_moveFlying(base, var1, var2, 0.02f);
        base->vtable->moveEntity(base, base->motionX, base->motionY, base->motionZ);
        base->motionX *= 0.5;
        base->motionY *= 0.5;
        base->motionZ *= 0.5;
        base->motionY -= 0.02;
        if (base->isCollidedHorizontally &&
            Entity_isOffsetPositionInLiquid(base, base->motionX,
                                            base->motionY + (double)0.6f - base->posY + var3,
                                            base->motionZ)) {
            base->motionY = (double)0.3f;
        }
    } else {
        float var8 = 0.91f;
        if (base->onGround) {
            var8 = 546.0f * 0.1f * 0.1f * 0.1f;
            if (base->worldObj) {
                int var4 = World_getBlockId(base->worldObj, MathHelper_floor_double(base->posX),
                                            MathHelper_floor_double(base->boundingBox.minY) - 1,
                                            MathHelper_floor_double(base->posZ));
                if (var4 > 0 && Block_blocksList[var4])
                    var8 = Block_blocksList[var4]->slipperiness * 0.91f;
            }
        }

        float var9 = 0.16277136f / (var8 * var8 * var8);
        Entity_moveFlying(base, var1, var2, base->onGround ? 0.1f * var9 : 0.02f);

        var8 = 0.91f;
        if (base->onGround) {
            var8 = 546.0f * 0.1f * 0.1f * 0.1f;
            if (base->worldObj) {
                int var5 = World_getBlockId(base->worldObj, MathHelper_floor_double(base->posX),
                                            MathHelper_floor_double(base->boundingBox.minY) - 1,
                                            MathHelper_floor_double(base->posZ));
                if (var5 > 0 && Block_blocksList[var5])
                    var8 = Block_blocksList[var5]->slipperiness * 0.91f;
            }
        }

        if (((EntityLivingVtable *)base->vtable)->isOnLadder(self)) {
            float var10 = 0.15f;
            if (base->motionX < (double)(-var10))
                base->motionX = (double)(-var10);
            if (base->motionX > (double)var10)
                base->motionX = (double)var10;
            if (base->motionZ < (double)(-var10))
                base->motionZ = (double)(-var10);
            if (base->motionZ > (double)var10)
                base->motionZ = (double)var10;
            base->fallDistance = 0.0f;
            if (base->motionY < -0.15)
                base->motionY = -0.15;
            if (base->vtable->isSneaking(base) && base->motionY < 0.0)
                base->motionY = 0.0;
        }

        base->vtable->moveEntity(base, base->motionX, base->motionY, base->motionZ);

        if (base->isCollidedHorizontally &&
            ((EntityLivingVtable *)base->vtable)->isOnLadder(self)) {
            base->motionY = 0.2;
        }

        base->motionY -= 0.08;
        base->motionY *= (double)0.98f;
        base->motionX *= (double)var8;
        base->motionZ *= (double)var8;
    }

    self->field_705_Q = self->field_704_R;
    double var3x = base->posX - base->prevPosX;
    double var11 = base->posZ - base->prevPosZ;

    float var7 = (float)(sqrt(var3x * var3x + var11 * var11) * 4.0);
    if (var7 > 1.0f)
        var7 = 1.0f;
    self->field_704_R += (var7 - self->field_704_R) * 0.4f;
    self->field_703_S += self->field_704_R;
}

float EntityLiving_getAIMoveSpeed(EntityLiving *self) { return self->field_35143_bh; }

static int def_isMovementBlocked(EntityLiving *self) { return self->health <= 0; }

static void def_jump(EntityLiving *self) { ((Entity *)self)->motionY = 0.42; }

void EntityLiving_jump(EntityLiving *self) {
    ((EntityLivingVtable *)((Entity *)self)->vtable)->jump(self);
}

static int def_isEntityAlive(Entity *base) {
    EntityLiving *self = (EntityLiving *)base;
    return !base->isDead && self->health > 0;
}

static void def_setPositionAndRotation2(Entity *base, double var1, double var3, double var5,
                                        float var7, float var8, int var9) {
    EntityLiving *self = (EntityLiving *)base;
    base->yOffset = 0.0f;
    self->newPosX = var1;
    self->newPosY = var3;
    self->newPosZ = var5;
    self->newRotationYaw = (double)var7;
    self->newRotationPitch = (double)var8;
    self->newPosRotationIncrements = var9;
}

static int def_canBeCollidedWith(Entity *self) { return !self->isDead; }

static int def_canBePushed(Entity *self) { return !self->isDead; }

static void def_onEntityDeath(EntityLiving *self) { (void)self; }

static void def_writeEntityToNBT(Entity *base, NBTTagCompound *var1) {
    EntityLiving *self = (EntityLiving *)base;
    NBTTagCompound_setShort(var1, "Health", (int16_t)self->health);
    NBTTagCompound_setShort(var1, "HurtTime", (int16_t)self->hurtTime);
    NBTTagCompound_setShort(var1, "DeathTime", (int16_t)self->deathTime);
    NBTTagCompound_setShort(var1, "AttackTime", (int16_t)self->attackTime);
}

static void def_readEntityFromNBT(Entity *base, NBTTagCompound *var1) {
    EntityLiving *self = (EntityLiving *)base;
    self->health = NBTTagCompound_getShort(var1, "Health");
    if (!NBTTagCompound_hasKey(var1, "Health")) {
        self->health = 10;
    }
    self->hurtTime = NBTTagCompound_getShort(var1, "HurtTime");
    self->deathTime = NBTTagCompound_getShort(var1, "DeathTime");
    self->attackTime = NBTTagCompound_getShort(var1, "AttackTime");
}

static void def_performHurtAnimation_living(Entity *base) {
    EntityLiving *self = (EntityLiving *)base;
    self->hurtTime = self->maxHurtTime = 10;
    self->attackedAtYaw = 0.0f;
}

static void def_kill_living(Entity *base) { base->vtable->attackEntityFrom(base, NULL, 4); }

static float def_getEyeHeight_living(Entity *base) { return base->height * 0.85f; }

static Vec3D *def_getLookVec_living(Entity *base) {
    return EntityLiving_getLook((EntityLiving *)base, 1.0f);
}

static int def_isPlayerSleeping(EntityLiving *self) {
    (void)self;
    return 0;
}

static int def_getItemIcon(EntityLiving *self, ItemStack *var1) {
    (void)self;
    return ItemStack_getIconIndex(var1);
}

static void def_updateRidden_living(Entity *base) {
    EntityLiving *self = (EntityLiving *)base;
    Entity_defaultVtable.updateRidden(base);
    self->field_9362_u = self->field_9361_v;
    self->field_9361_v = 0.0F;
}

EntityLivingVtable EntityLiving_defaultVtable;

void EntityLiving_initVtable(EntityLivingVtable *vt) {
    Entity_initVtable(&vt->base);

    vt->base.updateRidden = def_updateRidden_living;
    vt->base.fall = def_fall_living;
    vt->base.onUpdate = def_onUpdate;
    vt->base.onEntityUpdate = def_onEntityUpdate;
    vt->base.isEntityAlive = def_isEntityAlive;
    vt->base.setPositionAndRotation2 = def_setPositionAndRotation2;
    vt->base.canBeCollidedWith = def_canBeCollidedWith;
    vt->base.canBePushed = def_canBePushed;
    vt->base.attackEntityFrom = def_attackEntityFrom;
    vt->base.handleHealthUpdate = def_handleHealthUpdate;
    vt->base.getEntityString = def_getEntityString;
    vt->base.writeEntityToNBT = def_writeEntityToNBT;
    vt->base.readEntityFromNBT = def_readEntityFromNBT;
    vt->base.performHurtAnimation = def_performHurtAnimation_living;
    vt->base.kill = def_kill_living;
    vt->base.getEyeHeight = def_getEyeHeight_living;
    vt->base.getLookVec = def_getLookVec_living;

    vt->getMaxHealth = def_getMaxHealth;
    vt->heal = def_heal;
    vt->getHealth = def_getHealth;
    vt->setHealth = def_setHealth;
    vt->knockBack = def_knockBack;
    vt->damageEntity = def_damageEntity;
    vt->onDeath = def_onDeath;
    vt->onLivingUpdate = def_onLivingUpdate;
    vt->moveEntityWithHeading = def_moveEntityWithHeading;
    vt->isOnLadder = def_isOnLadder;
    vt->updatePlayerActionState = def_updatePlayerActionState;
    vt->getLivingSound = def_getLivingSound;
    vt->getHurtSound = def_getHurtSound;
    vt->getDeathSound = def_getDeathSound;
    vt->getDropItemId = def_getDropItemId;
    vt->getSoundVolume = def_getSoundVolume;
    vt->getCanSpawnHere = def_getCanSpawnHere;
    vt->getMaxSpawnedInChunk = def_getMaxSpawnedInChunk;
    vt->getHeldItem = def_getHeldItem;
    vt->getEquipmentInSlot = def_getEquipmentInSlot;
    vt->setCurrentItemOrArmor = def_setCurrentItemOrArmor;
    vt->dropEquipment = def_dropEquipment;
    vt->isEntityUndead = def_isEntityUndead;
    vt->canBreatheUnderwater = def_canBreatheUnderwater;
    vt->getTalkInterval = def_getTalkInterval;
    vt->dropFewItems = def_dropFewItems;
    vt->getEntityTexture = def_getEntityTexture;
    vt->canDespawn = def_canDespawn;
    vt->func_25026_x = def_func_25026_x;
    vt->jump = def_jump;
    vt->isMovementBlocked = def_isMovementBlocked;
    vt->onEntityDeath = def_onEntityDeath;
    vt->isPlayerSleeping = def_isPlayerSleeping;
    vt->getItemIcon = def_getItemIcon;
}

void EntityLiving_construct(EntityLiving *self, World *world) {
    Entity_construct(&self->base, world);
    static int once = 0;
    if (!once) {
        EntityLiving_initVtable(&EntityLiving_defaultVtable);
        once = 1;
    }
    self->base.vtable = &EntityLiving_defaultVtable.base;
    self->base.isLiving = 1;

    self->base.preventEntitySpawning = 1;

    self->field_9363_r = (float)(Math_random() + 1.0) * 0.01f;

    self->base.vtable->setPosition(&self->base, self->base.posX, self->base.posY, self->base.posZ);

    self->field_9365_p = (float)Math_random() * 12398.0f;

    self->base.rotationYaw = (float)(Math_random() * M_PI * 2.0);

    self->base.stepHeight = 0.5f;

    self->health = 10;
    self->heartsHalvesLife = 20;
    self->field_9346_af = 0;
    self->maxHurtTime = 10;
    self->jumpMovementFactor = 0.02f;
    self->field_35143_bh = 0.7f;
    self->moveSpeed = 0.7f;

    self->field_9358_y = 1;

    self->field_9355_A = 1;

    self->field_9349_D = 1.0f;

    self->field_9326_T = -1;

    self->field_9325_U = (float)(Math_random() * (double)0.9f + (double)0.1f);

    self->texture = "/mob/char.png";

    DataWatcher_addByte(&self->base.dataWatcher, 6, 0);
    self->isMultiplayerEntity = 0;
    self->currentTarget = NULL;
    self->numTicksToChaseTarget = 0;
}

int EntityLiving_getHealth(EntityLiving *self) { return self->health; }
void EntityLiving_setHealth(EntityLiving *self, int h) {
    ((EntityLivingVtable *)self->base.vtable)->setHealth(self, h);
}
int EntityLiving_getMaxHealth(EntityLiving *self) {
    return ((EntityLivingVtable *)self->base.vtable)->getMaxHealth(self);
}
void EntityLiving_heal(EntityLiving *self, int amount) {
    ((EntityLivingVtable *)self->base.vtable)->heal(self, amount);
}
void EntityLiving_knockBack(EntityLiving *self, Entity *src, int dmg, double vx, double vz) {
    ((EntityLivingVtable *)self->base.vtable)->knockBack(self, src, dmg, vx, vz);
}
ItemStack *EntityLiving_getHeldItem(EntityLiving *self) {
    return ((EntityLivingVtable *)self->base.vtable)->getHeldItem(self);
}
ItemStack *EntityLiving_getEquipmentInSlot(EntityLiving *self, int slot) {
    return ((EntityLivingVtable *)self->base.vtable)->getEquipmentInSlot(self, slot);
}
void EntityLiving_setCurrentItemOrArmor(EntityLiving *self, int slot, ItemStack *stack) {
    ((EntityLivingVtable *)self->base.vtable)->setCurrentItemOrArmor(self, slot, stack);
}

int EntityLiving_canEntityBeSeen(EntityLiving *self, Entity *other) {
    Entity *base = (Entity *)self;
    MovingObjectPosition *hit = World_rayTraceBlocks(
        base->worldObj,
        Vec3D_createVector(base->posX, base->posY + (double)base->vtable->getEyeHeight(base),
                           base->posZ),
        Vec3D_createVector(other->posX, other->posY + (double)other->vtable->getEyeHeight(other),
                           other->posZ));
    MovingObjectPosition_free(hit);
    return hit == NULL;
}

void EntityLiving_func_27021_X(EntityLiving *self) {
    Entity *base = &self->base;
    if (!base->worldObj)
        return;

    Entity *var1 = World_getClosestPlayerToEntity(base->worldObj, base, -1.0);
    if (((EntityLivingVtable *)base->vtable)->canDespawn(self) && var1 != NULL) {
        double var2 = var1->posX - base->posX;
        double var4 = var1->posY - base->posY;
        double var6 = var1->posZ - base->posZ;
        double var8 = var2 * var2 + var4 * var4 + var6 * var6;

        if (var8 > 16384.0) {
            Entity_setEntityDead(base);
        }

        if (self->entityAge > 600 && JavaRandom_nextInt(&base->rand, 800) == 0) {
            if (var8 < 1024.0) {
                self->entityAge = 0;
            } else {
                Entity_setEntityDead(base);
            }
        }
    }
}

void EntityLiving_faceEntity(EntityLiving *self, Entity *entity, float yawSpeed, float pitchSpeed) {
    Entity *base = &self->base;
    double var4 = entity->posX - base->posX;
    double var8 = entity->posZ - base->posZ;
    double var6;
    if (entity->isLiving) {
        EntityLiving *var10 = (EntityLiving *)entity;
        var6 = (base->posY + (double)base->vtable->getEyeHeight(base)) -
               (var10->base.posY + (double)var10->base.vtable->getEyeHeight(&var10->base));
    } else {
        var6 = (entity->boundingBox.minY + entity->boundingBox.maxY) / 2.0 -
               (base->posY + (double)base->vtable->getEyeHeight(base));
    }
    double var14 = (double)MathHelper_sqrt_double(var4 * var4 + var8 * var8);
    float var12 = (float)(atan2(var8, var4) * 180.0 / M_PI) - 90.0f;
    float var13 = (float)(-(atan2(var6, var14) * 180.0 / M_PI));

    {
        float var4f;
        for (var4f = var13 - base->rotationPitch; var4f < -180.0f; var4f += 360.0f) {
        }
        while (var4f >= 180.0f) {
            var4f -= 360.0f;
        }
        if (var4f > pitchSpeed)
            var4f = pitchSpeed;
        if (var4f < -pitchSpeed)
            var4f = -pitchSpeed;
        base->rotationPitch = -(base->rotationPitch + var4f);
    }

    {
        float var4f;
        for (var4f = var12 - base->rotationYaw; var4f < -180.0f; var4f += 360.0f) {
        }
        while (var4f >= 180.0f) {
            var4f -= 360.0f;
        }
        if (var4f > yawSpeed)
            var4f = yawSpeed;
        if (var4f < -yawSpeed)
            var4f = -yawSpeed;
        base->rotationYaw = base->rotationYaw + var4f;
    }
}

int EntityLiving_hasCurrentTarget(EntityLiving *self) { return self->currentTarget != NULL; }

Entity *EntityLiving_getCurrentTarget(EntityLiving *self) { return self->currentTarget; }

void EntityLiving_playLivingSound(EntityLiving *self) {
    const char *var1 = ((EntityLivingVtable *)self->base.vtable)->getLivingSound(self);
    if (var1 != NULL) {
        float pitch =
            (JavaRandom_nextFloat(&self->base.rand) - JavaRandom_nextFloat(&self->base.rand)) *
                0.2f +
            1.0f;
        World_playSoundAtEntity(self->base.worldObj, (Entity *)self, var1,
                                ((EntityLivingVtable *)self->base.vtable)->getSoundVolume(self),
                                pitch);
    }
}

float EntityLiving_getSwingProgress(EntityLiving *self, float var1) {
    float var2 = self->swingProgress - self->prevSwingProgress;
    if (var2 < 0.0f) {
        ++var2;
    }
    return self->prevSwingProgress + var2 * var1;
}

Vec3D *EntityLiving_getLook(EntityLiving *self, float var1) {
    Entity *base = &self->base;
    float var2, var3, var4, var5;
    if (var1 == 1.0f) {
        var2 = MathHelper_cos(-base->rotationYaw * ((float)M_PI / 180.0f) - (float)M_PI);
        var3 = MathHelper_sin(-base->rotationYaw * ((float)M_PI / 180.0f) - (float)M_PI);
        var4 = -MathHelper_cos(-base->rotationPitch * ((float)M_PI / 180.0f));
        var5 = MathHelper_sin(-base->rotationPitch * ((float)M_PI / 180.0f));
        return Vec3D_createVector((double)(var3 * var4), (double)var5, (double)(var2 * var4));
    } else {
        var2 = base->prevRotationPitch + (base->rotationPitch - base->prevRotationPitch) * var1;
        var3 = base->prevRotationYaw + (base->rotationYaw - base->prevRotationYaw) * var1;
        var4 = MathHelper_cos(-var3 * ((float)M_PI / 180.0f) - (float)M_PI);
        var5 = MathHelper_sin(-var3 * ((float)M_PI / 180.0f) - (float)M_PI);
        float var6 = -MathHelper_cos(-var2 * ((float)M_PI / 180.0f));
        float var7 = MathHelper_sin(-var2 * ((float)M_PI / 180.0f));
        return Vec3D_createVector((double)(var5 * var6), (double)var7, (double)(var4 * var6));
    }
}

Vec3D *EntityLiving_getPosition(EntityLiving *self, float var1) {
    Entity *base = &self->base;
    if (var1 == 1.0f) {
        return Vec3D_createVector(base->posX, base->posY, base->posZ);
    } else {
        double var2 = base->prevPosX + (base->posX - base->prevPosX) * (double)var1;
        double var4 = base->prevPosY + (base->posY - base->prevPosY) * (double)var1;
        double var6 = base->prevPosZ + (base->posZ - base->prevPosZ) * (double)var1;
        return Vec3D_createVector(var2, var4, var6);
    }
}

MovingObjectPosition *EntityLiving_rayTrace(EntityLiving *self, double var1, float var3) {
    Vec3D *var4 = EntityLiving_getPosition(self, var3);
    Vec3D *var5 = EntityLiving_getLook(self, var3);
    Vec3D *var6 =
        Vec3D_addVector(var4, var5->xCoord * var1, var5->yCoord * var1, var5->zCoord * var1);
    return World_rayTraceBlocks(self->base.worldObj, var4, var6);
}

void EntityLiving_kill(EntityLiving *self) { self->base.vtable->kill(&self->base); }

void EntityLiving_spawnExplosionParticle(EntityLiving *self) {
    for (int var1 = 0; var1 < 20; var1++) {
        double var2 = JavaRandom_nextGaussian(&self->base.rand) * 0.02;
        double var4 = JavaRandom_nextGaussian(&self->base.rand) * 0.02;
        double var6 = JavaRandom_nextGaussian(&self->base.rand) * 0.02;
        double var8 = 10.0;
        World_spawnParticle(
            self->base.worldObj, "explode",
            self->base.posX +
                (double)(JavaRandom_nextFloat(&self->base.rand) * self->base.width * 2.0f) -
                (double)self->base.width - var2 * var8,
            self->base.posY + (double)(JavaRandom_nextFloat(&self->base.rand) * self->base.height) -
                var4 * var8,
            self->base.posZ +
                (double)(JavaRandom_nextFloat(&self->base.rand) * self->base.width * 2.0f) -
                (double)self->base.width - var6 * var8,
            var2, var4, var6);
    }
}
