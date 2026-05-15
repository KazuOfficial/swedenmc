#ifndef RENDER_GIANT_ZOMBIE_H
#define RENDER_GIANT_ZOMBIE_H

#include "RenderLiving.h"
#include "EntityGiantZombie.h"

typedef struct RenderGiantZombie {
    RenderLiving base;
    float scale;
} RenderGiantZombie;

RenderGiantZombie *RenderGiantZombie_create(ModelBase *var1, float var2, float var3);

#endif
