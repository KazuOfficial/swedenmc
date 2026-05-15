#ifndef RENDER_SQUID_H
#define RENDER_SQUID_H

#include "RenderLiving.h"
#include "EntitySquid.h"

typedef struct RenderSquid {
    RenderLiving base;
} RenderSquid;

RenderSquid *RenderSquid_create(ModelBase *var1, float var2);

#endif
