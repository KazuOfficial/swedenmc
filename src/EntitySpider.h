#ifndef ENTITY_SPIDER_H
#define ENTITY_SPIDER_H

#include "EntityMob.h"

typedef struct EntitySpider {
    EntityMob base;
} EntitySpider;

EntitySpider *EntitySpider_create(World *world);

#endif
