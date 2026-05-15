#ifndef RENDER_SHEEP_H
#define RENDER_SHEEP_H

#include "RenderLiving.h"
#include "EntitySheep.h"

typedef struct RenderSheep {
    RenderLiving base;
} RenderSheep;

RenderSheep *RenderSheep_create(ModelBase *var1, ModelBase *var2, float var3);

#endif
