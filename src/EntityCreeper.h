#ifndef ENTITY_CREEPER_H
#define ENTITY_CREEPER_H

#include "EntityMob.h"

typedef struct EntityCreeper {
    EntityMob base;

    int timeSinceIgnited;

    int lastActiveTime;
} EntityCreeper;

EntityCreeper *EntityCreeper_create(World *world);

int EntityCreeper_getPowered(EntityCreeper *self);

float EntityCreeper_setCreeperFlashTime(EntityCreeper *self, float var1);

#endif
