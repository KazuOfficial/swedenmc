#ifndef RENDER_COW_H
#define RENDER_COW_H

#include "RenderLiving.h"
#include "EntityCow.h"

typedef struct RenderCow {
    RenderLiving base;
} RenderCow;

RenderCow *RenderCow_create(ModelBase *var1, float var2);

#endif
