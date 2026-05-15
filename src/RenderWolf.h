#ifndef RENDER_WOLF_H
#define RENDER_WOLF_H

#include "RenderLiving.h"
#include "EntityWolf.h"

typedef struct RenderWolf {
    RenderLiving base;
} RenderWolf;

RenderWolf *RenderWolf_create(ModelBase *var1, float var2);

#endif
