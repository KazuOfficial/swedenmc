#ifndef RENDER_PIG_H
#define RENDER_PIG_H

#include "RenderLiving.h"
#include "EntityPig.h"

typedef struct RenderPig {
    RenderLiving base;
} RenderPig;

RenderPig *RenderPig_create(ModelBase *var1, ModelBase *var2, float var3);

#endif
