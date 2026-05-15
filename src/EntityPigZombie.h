#ifndef ENTITY_PIG_ZOMBIE_H
#define ENTITY_PIG_ZOMBIE_H

#include "EntityZombie.h"
#include "ItemStack.h"

typedef struct EntityPigZombie {
    EntityZombie base;

    int angerLevel;

    int randomSoundDelay;
} EntityPigZombie;

extern ItemStack EntityPigZombie_defaultHeldItem;

void EntityPigZombie_construct(EntityPigZombie *self, World *var1);
EntityPigZombie *EntityPigZombie_create(World *var1);

void EntityPigZombie_initVtable(EntityMobVtable *vt);

void EntityPigZombie_onUpdate(Entity *self);
int EntityPigZombie_getCanSpawnHere(EntityLiving *self);
void EntityPigZombie_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityPigZombie_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
Entity *EntityPigZombie_findPlayerToAttack(EntityCreature *self);
void EntityPigZombie_onLivingUpdate(EntityLiving *self);
int EntityPigZombie_attackEntityFrom(Entity *self, Entity *var1, int var2);
const char *EntityPigZombie_getLivingSound(EntityLiving *self);
const char *EntityPigZombie_getHurtSound(EntityLiving *self);
const char *EntityPigZombie_getDeathSound(EntityLiving *self);
int EntityPigZombie_getDropItemId(EntityLiving *self);
ItemStack *EntityPigZombie_getHeldItem(EntityLiving *self);

void EntityPigZombie_becomeAngryAt(EntityPigZombie *self, Entity *var1);

#endif
