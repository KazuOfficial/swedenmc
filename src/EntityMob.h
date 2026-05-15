#ifndef ENTITY_MOB_H
#define ENTITY_MOB_H

#include "EntityCreature.h"

struct EntityMob;
typedef struct EntityMob EntityMob;

typedef struct EntityMobVtable {
    EntityCreatureVtable base;

} EntityMobVtable;

struct EntityMob {
    EntityCreature base;

    int attackStrength;
};

void EntityMob_construct(EntityMob *self, World *world);
void EntityMob_initVtable(EntityMobVtable *vt);

void EntityMob_onLivingUpdate(EntityLiving *self);

void EntityMob_attackEntity(EntityCreature *base, Entity *var1, float var2);

#endif
