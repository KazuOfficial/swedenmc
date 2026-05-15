#ifndef ENTITY_BOAT_H
#define ENTITY_BOAT_H

#include "Entity.h"
#include "EntityPlayer.h"

typedef struct EntityBoat {
    Entity base;

    int boatCurrentDamage;

    int boatTimeSinceHit;

    int boatRockDirection;

    int field_9394_d;

    double field_9393_e;

    double field_9392_f;

    double field_9391_g;

    double field_9390_h;

    double field_9389_i;

    double field_9388_j;

    double field_9387_k;

    double field_9386_l;
} EntityBoat;

void EntityBoat_construct1(EntityBoat *self, World *var1);

void EntityBoat_construct2(EntityBoat *self, World *var1, double var2, double var4, double var6);

int EntityBoat_canTriggerWalking(Entity *self);

void EntityBoat_entityInit(Entity *self);

AxisAlignedBB *EntityBoat_getCollisionBox(Entity *self, Entity *var1);

AxisAlignedBB *EntityBoat_getBoundingBox(Entity *self);

int EntityBoat_canBePushed(Entity *self);

double EntityBoat_getMountedYOffset(Entity *self);

int EntityBoat_attackEntityFrom(Entity *self, Entity *var1, int var2);

void EntityBoat_performHurtAnimation(Entity *self);

int EntityBoat_canBeCollidedWith(Entity *self);

void EntityBoat_setPositionAndRotation2(EntityBoat *self, double var1, double var3, double var5,
                                        float var7, float var8, int var9);

void EntityBoat_setVelocity(EntityBoat *self, double var1, double var3, double var5);

void EntityBoat_onUpdate(Entity *self);

void EntityBoat_updateRiderPosition(Entity *self);

void EntityBoat_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntityBoat_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

float EntityBoat_getShadowSize(Entity *self);

int EntityBoat_interact(Entity *self, EntityPlayer *var1);

#endif
