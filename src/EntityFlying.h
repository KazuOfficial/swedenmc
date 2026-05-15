#ifndef ENTITY_FLYING_H
#define ENTITY_FLYING_H

#include "EntityLiving.h"

typedef struct EntityFlying {
    EntityLiving base;
} EntityFlying;

void EntityFlying_construct(EntityFlying *self, World *var1);
EntityFlying *EntityFlying_create(World *var1);

void EntityFlying_fall(Entity *self, float var1);
void EntityFlying_moveEntityWithHeading(EntityLiving *self, float var1, float var2);
int EntityFlying_isOnLadder(EntityLiving *self);

void EntityFlying_initVtable(EntityLivingVtable *vt);
extern EntityLivingVtable EntityFlying_defaultVtable;

#endif
