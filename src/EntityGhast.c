#include "EntityGhast.h"
#include "EntityFireball.h"
#include "World.h"
#include "Item.h"
#include "MathHelper.h"
#include "Vec3D.h"
#include "AxisAlignedBB.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

static EntityLivingVtable EntityGhast_vtable;
static int EntityGhast_vtable_init = 0;

static const char *ghast_getEntityString(Entity *self) {
    (void)self;
    return "Ghast";
}

static void ensure_vtable(void) {
    if (EntityGhast_vtable_init)
        return;
    EntityFlying_initVtable(&EntityGhast_vtable);
    EntityGhast_vtable.base.getEntityString = ghast_getEntityString;
    EntityGhast_vtable.base.entityInit = EntityGhast_entityInit;
    EntityGhast_vtable.base.onUpdate = EntityGhast_onUpdate;
    EntityGhast_vtable.updatePlayerActionState = EntityGhast_updatePlayerActionState;
    EntityGhast_vtable.getLivingSound = EntityGhast_getLivingSound;
    EntityGhast_vtable.getHurtSound = EntityGhast_getHurtSound;
    EntityGhast_vtable.getDeathSound = EntityGhast_getDeathSound;
    EntityGhast_vtable.getDropItemId = EntityGhast_getDropItemId;
    EntityGhast_vtable.getSoundVolume = EntityGhast_getSoundVolume;
    EntityGhast_vtable.getCanSpawnHere = EntityGhast_getCanSpawnHere;
    EntityGhast_vtable.getMaxSpawnedInChunk = EntityGhast_getMaxSpawnedInChunk;
    EntityGhast_vtable_init = 1;
}

void EntityGhast_construct(EntityGhast *self, World *var1) {
    ensure_vtable();
    EntityFlying_construct(&self->base, var1);
    self->base.base.base.vtable = (const EntityVtable *)&EntityGhast_vtable;
    self->base.base.base.isGhast = 1;
    self->base.base.texture = "/mob/ghast.png";
    Entity_setSize(&self->base.base.base, 4.0F, 4.0F);
    self->base.base.base.isImmuneToFire = 1;
}

EntityGhast *EntityGhast_create(World *var1) {
    EntityGhast *self = (EntityGhast *)calloc(1, sizeof(EntityGhast));
    EntityGhast_construct(self, var1);
    return self;
}

void EntityGhast_entityInit(Entity *selfE) {
    Entity_defaultVtable.entityInit(selfE);
    DataWatcher_addByte(&selfE->dataWatcher, 16, 0);
}

void EntityGhast_onUpdate(Entity *selfE) {
    EntityGhast *self = (EntityGhast *)selfE;
    EntityFlying_defaultVtable.base.onUpdate(selfE);
    int8_t var1 = DataWatcher_getWatchableObjectByte(&selfE->dataWatcher, 16);
    self->base.base.texture = (var1 == 1) ? "/mob/ghast_fire.png" : "/mob/ghast.png";
}

static int EntityGhast_isCourseTraversable(EntityGhast *self, double var1, double var3, double var5,
                                           double var7);

void EntityGhast_updatePlayerActionState(EntityLiving *selfL) {
    EntityGhast *self = (EntityGhast *)selfL;
    Entity *selfE = &selfL->base;

    if (!selfE->worldObj->isRemote && selfE->worldObj->difficultySetting == 0) {
        Entity_setEntityDead(selfE);
    }

    EntityLiving_func_27021_X(selfL);
    self->prevAttackCounter = self->attackCounter;
    double var1 = self->waypointX - selfE->posX;
    double var3 = self->waypointY - selfE->posY;
    double var5 = self->waypointZ - selfE->posZ;
    double var7 = (double)MathHelper_sqrt_double(var1 * var1 + var3 * var3 + var5 * var5);
    if (var7 < 1.0 || var7 > 60.0) {
        self->waypointX =
            selfE->posX + (double)((JavaRandom_nextFloat(&selfE->rand) * 2.0F - 1.0F) * 16.0F);
        self->waypointY =
            selfE->posY + (double)((JavaRandom_nextFloat(&selfE->rand) * 2.0F - 1.0F) * 16.0F);
        self->waypointZ =
            selfE->posZ + (double)((JavaRandom_nextFloat(&selfE->rand) * 2.0F - 1.0F) * 16.0F);
    }

    if (self->courseChangeCooldown-- <= 0) {
        self->courseChangeCooldown += JavaRandom_nextInt(&selfE->rand, 5) + 2;
        if (EntityGhast_isCourseTraversable(self, self->waypointX, self->waypointY, self->waypointZ,
                                            var7)) {
            selfE->motionX += var1 / var7 * 0.1;
            selfE->motionY += var3 / var7 * 0.1;
            selfE->motionZ += var5 / var7 * 0.1;
        } else {
            self->waypointX = selfE->posX;
            self->waypointY = selfE->posY;
            self->waypointZ = selfE->posZ;
        }
    }

    if (self->targetedEntity != NULL && self->targetedEntity->isDead) {
        self->targetedEntity = NULL;
    }

    if (self->targetedEntity == NULL || self->aggroCooldown-- <= 0) {
        self->targetedEntity = World_getClosestPlayerToEntity(selfE->worldObj, selfE, 100.0);
        if (self->targetedEntity != NULL) {
            self->aggroCooldown = 20;
        }
    }

    double var9 = 64.0;
    if (self->targetedEntity != NULL &&
        Entity_getDistanceSqToEntity(self->targetedEntity, selfE) < var9 * var9) {
        double var11 = self->targetedEntity->posX - selfE->posX;
        double var13 = self->targetedEntity->boundingBox.minY +
                       (double)(self->targetedEntity->height / 2.0F) -
                       (selfE->posY + (double)(selfE->height / 2.0F));
        double var15 = self->targetedEntity->posZ - selfE->posZ;
        selfL->renderYawOffset = selfL->base.rotationYaw =
            -((float)atan2(var11, var15)) * 180.0F / (float)M_PI;
        if (EntityLiving_canEntityBeSeen(selfL, self->targetedEntity)) {
            if (self->attackCounter == 10) {
                World_playSoundAtEntity(
                    selfE->worldObj, selfE, "mob.ghast.charge", EntityGhast_getSoundVolume(selfL),
                    (JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) *
                            0.2F +
                        1.0F);
            }

            ++self->attackCounter;
            if (self->attackCounter == 20) {
                World_playSoundAtEntity(
                    selfE->worldObj, selfE, "mob.ghast.fireball", EntityGhast_getSoundVolume(selfL),
                    (JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) *
                            0.2F +
                        1.0F);
                EntityFireball *var17 =
                    EntityFireball_create3(selfE->worldObj, selfL, var11, var13, var15);
                double var18 = 4.0;
                Vec3D *var20 = Entity_getLook(selfE, 1.0F);
                var17->base.posX = selfE->posX + var20->xCoord * var18;
                var17->base.posY = selfE->posY + (double)(selfE->height / 2.0F) + 0.5;
                var17->base.posZ = selfE->posZ + var20->zCoord * var18;
                World_entityJoinedWorld(selfE->worldObj, (Entity *)var17);
                self->attackCounter = -40;
            }
        } else if (self->attackCounter > 0) {
            --self->attackCounter;
        }
    } else {
        selfL->renderYawOffset = selfL->base.rotationYaw =
            -((float)atan2(selfE->motionX, selfE->motionZ)) * 180.0F / (float)M_PI;
        if (self->attackCounter > 0) {
            --self->attackCounter;
        }
    }

    if (!selfE->worldObj->isRemote) {
        int8_t var21 = DataWatcher_getWatchableObjectByte(&selfE->dataWatcher, 16);
        int8_t var12 = (int8_t)(self->attackCounter > 10 ? 1 : 0);
        if (var21 != var12) {
            DataWatcher_updateByte(&selfE->dataWatcher, 16, var12);
        }
    }
}

static int EntityGhast_isCourseTraversable(EntityGhast *self, double var1, double var3, double var5,
                                           double var7) {
    (void)var1;
    (void)var3;
    (void)var5;
    Entity *selfE = &self->base.base.base;
    double var9 = (self->waypointX - selfE->posX) / var7;
    double var11 = (self->waypointY - selfE->posY) / var7;
    double var13 = (self->waypointZ - selfE->posZ) / var7;
    AxisAlignedBB *var15 = AxisAlignedBB_copy(&selfE->boundingBox);

    for (int var16 = 1; (double)var16 < var7; ++var16) {
        AxisAlignedBB_offset(var15, var9, var11, var13);
        AxisAlignedBB outList[8];
        if (World_getCollidingBoundingBoxes(selfE->worldObj, selfE, var15, outList, 8) > 0) {
            return 0;
        }
    }

    return 1;
}

const char *EntityGhast_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.ghast.moan";
}

const char *EntityGhast_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.ghast.scream";
}

const char *EntityGhast_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.ghast.death";
}

int EntityGhast_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_gunpowder->shiftedIndex;
}

float EntityGhast_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 10.0F;
}

int EntityGhast_getCanSpawnHere(EntityLiving *self) {
    Entity *selfE = &self->base;
    return JavaRandom_nextInt(&selfE->rand, 20) == 0 &&
           EntityFlying_defaultVtable.getCanSpawnHere(self) &&
           selfE->worldObj->difficultySetting > 0;
}

int EntityGhast_getMaxSpawnedInChunk(EntityLiving *self) {
    (void)self;
    return 1;
}
