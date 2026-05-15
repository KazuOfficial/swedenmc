#ifndef ENTITY_GHAST_H
#define ENTITY_GHAST_H

#include "EntityFlying.h"

typedef struct EntityGhast {
    EntityFlying base;

    int courseChangeCooldown;

    double waypointX;
    double waypointY;
    double waypointZ;

    Entity *targetedEntity;

    int aggroCooldown;

    int prevAttackCounter;

    int attackCounter;
} EntityGhast;

void EntityGhast_construct(EntityGhast *self, World *var1);
EntityGhast *EntityGhast_create(World *var1);

void EntityGhast_entityInit(Entity *self);
void EntityGhast_onUpdate(Entity *self);
void EntityGhast_updatePlayerActionState(EntityLiving *self);
const char *EntityGhast_getLivingSound(EntityLiving *self);
const char *EntityGhast_getHurtSound(EntityLiving *self);
const char *EntityGhast_getDeathSound(EntityLiving *self);
int EntityGhast_getDropItemId(EntityLiving *self);
float EntityGhast_getSoundVolume(EntityLiving *self);
int EntityGhast_getCanSpawnHere(EntityLiving *self);
int EntityGhast_getMaxSpawnedInChunk(EntityLiving *self);

#endif
