#ifndef RENDER_CREEPER_H
#define RENDER_CREEPER_H

#include "RenderLiving.h"
#include "EntityCreeper.h"
#include "ModelCreeper.h"

typedef struct RenderCreeper {
    RenderLiving base;
    ModelBase *field_27008_a;
} RenderCreeper;

RenderCreeper *RenderCreeper_create(void);

#endif
