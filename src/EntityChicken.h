#ifndef ENTITY_CHICKEN_H
#define ENTITY_CHICKEN_H

#include "EntityAnimal.h"

typedef struct EntityChicken {
    EntityAnimal base;

    int field_753_a;

    float field_752_b;

    float destPos;

    float field_757_d;

    float field_756_e;

    float field_755_h;

    int timeUntilNextEgg;
} EntityChicken;

EntityChicken *EntityChicken_create(World *world);

#endif
