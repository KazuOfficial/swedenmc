#ifndef ENTITY_SKELETON_H
#define ENTITY_SKELETON_H

#include "EntityMob.h"

typedef struct EntitySkeleton {
    EntityMob base;
} EntitySkeleton;

EntitySkeleton *EntitySkeleton_create(World *world);

#endif
