#ifndef ENTITY_CREATURE_H
#define ENTITY_CREATURE_H

#include "EntityLiving.h"

struct EntityCreature;
typedef struct EntityCreature EntityCreature;

struct PathEntity;

typedef struct EntityCreatureVtable {
    EntityLivingVtable base;

    float (*getBlockPathWeight)(EntityCreature *self, int x, int y, int z);

    Entity *(*findPlayerToAttack)(EntityCreature *self);

    void (*attackEntity)(EntityCreature *self, Entity *var1, float var2);

    void (*attackBlockedEntity)(EntityCreature *self, Entity *var1, float var2);

    int (*isMovementCeased)(EntityCreature *self);
} EntityCreatureVtable;

struct EntityCreature {
    EntityLiving base;

    struct PathEntity *pathToEntity;

    Entity *playerToAttack;

    int hasAttacked;
};

void EntityCreature_construct(EntityCreature *self, World *world);
void EntityCreature_initVtable(EntityCreatureVtable *vt);

void EntityCreature_updatePlayerActionState(EntityLiving *self);

int EntityCreature_hasPath(EntityCreature *self);

void EntityCreature_setPathToEntity(EntityCreature *self, struct PathEntity *var1);

Entity *EntityCreature_getTarget(EntityCreature *self);

void EntityCreature_setTarget(EntityCreature *self, Entity *var1);

int EntityCreature_getCanSpawnHere(EntityCreature *self);

#endif
