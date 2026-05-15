#ifndef ENTITY_WOLF_H
#define ENTITY_WOLF_H

#include "EntityAnimal.h"
#include "NBTTagCompound.h"

typedef struct EntityWolf {
    EntityAnimal base;

    int looksWithInterest;

    float field_25048_b;

    float field_25054_c;

    int isWolfShaking;

    int field_25052_g;

    float timeWolfIsShaking;

    float prevTimeWolfIsShaking;
} EntityWolf;

void EntityWolf_construct(EntityWolf *self, World *var1);
void EntityWolf_initVtable(EntityAnimalVtable *vt);

EntityWolf *EntityWolf_create(World *var1);

void EntityWolf_entityInit(Entity *self);
int EntityWolf_canTriggerWalking(Entity *self);
const char *EntityWolf_getEntityTexture(EntityLiving *self);
void EntityWolf_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityWolf_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
int EntityWolf_canDespawn(EntityLiving *self);
const char *EntityWolf_getLivingSound(EntityLiving *self);
const char *EntityWolf_getHurtSound(EntityLiving *self);
const char *EntityWolf_getDeathSound(EntityLiving *self);
float EntityWolf_getSoundVolume(EntityLiving *self);
int EntityWolf_getDropItemId(EntityLiving *self);
void EntityWolf_updatePlayerActionState(EntityLiving *self);
void EntityWolf_onLivingUpdate(EntityLiving *self);
void EntityWolf_onUpdate(Entity *self);
int EntityWolf_attackEntityFrom(Entity *self, Entity *var1, int var2);
Entity *EntityWolf_findPlayerToAttack(EntityCreature *self);
void EntityWolf_attackEntity(EntityCreature *self, Entity *var1, float var2);
int EntityWolf_interact(Entity *self, EntityPlayer *var1);
float EntityWolf_getEyeHeight(Entity *self);
int EntityWolf_func_25026_x(EntityLiving *self);
int EntityWolf_isMovementCeased(EntityCreature *self);
void EntityWolf_handleHealthUpdate(Entity *self, int8_t var1);
int EntityWolf_getMaxSpawnedInChunk(EntityLiving *self);

int EntityWolf_getWolfShaking(EntityWolf *self);
float EntityWolf_getShadingWhileShaking(EntityWolf *self, float var1);
float EntityWolf_getShakeAngle(EntityWolf *self, float var1, float var2);
float EntityWolf_getInterestedAngle(EntityWolf *self, float var1);
float EntityWolf_setTailRotation(EntityWolf *self);
const char *EntityWolf_getWolfOwner(EntityWolf *self);
void EntityWolf_setWolfOwner(EntityWolf *self, const char *var1);
int EntityWolf_isWolfSitting(EntityWolf *self);
void EntityWolf_setWolfSitting(EntityWolf *self, int var1);
int EntityWolf_isWolfAngry(EntityWolf *self);
void EntityWolf_setWolfAngry(EntityWolf *self, int var1);
int EntityWolf_isWolfTamed(EntityWolf *self);
void EntityWolf_setWolfTamed(EntityWolf *self, int var1);

#endif
