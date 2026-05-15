#ifndef RENDER_CHICKEN_H
#define RENDER_CHICKEN_H

#include "RenderLiving.h"
#include "EntityChicken.h"

typedef struct RenderChicken {
    RenderLiving base;
} RenderChicken;

RenderChicken *RenderChicken_create(ModelBase *var1, float var2);

#endif
