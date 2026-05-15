#ifndef ENTITY_BUBBLE_FX_H
#define ENTITY_BUBBLE_FX_H

#include "EntityFX.h"

typedef struct EntityBubbleFX {
    EntityFX base;
} EntityBubbleFX;

void EntityBubbleFX_construct(EntityBubbleFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12);

void EntityBubbleFX_onUpdate(EntityBubbleFX *self);

#endif
