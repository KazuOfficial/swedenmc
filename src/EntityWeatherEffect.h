#ifndef ENTITY_WEATHER_EFFECT_H
#define ENTITY_WEATHER_EFFECT_H

#include "Entity.h"

typedef struct EntityWeatherEffect {
    Entity base;
} EntityWeatherEffect;

void EntityWeatherEffect_construct(EntityWeatherEffect *self, World *var1);

void EntityWeatherEffect_initVtable(EntityVtable *vt);

#endif
