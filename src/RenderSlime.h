#ifndef RENDER_SLIME_H
#define RENDER_SLIME_H

#include "RenderLiving.h"
#include "EntitySlime.h"

typedef struct RenderSlime {
    RenderLiving base;

    ModelBase *scaleAmount;
} RenderSlime;

RenderSlime *RenderSlime_create(ModelBase *var1, ModelBase *var2, float var3);

#endif
