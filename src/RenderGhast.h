#ifndef RENDER_GHAST_H
#define RENDER_GHAST_H

#include "RenderLiving.h"
#include "EntityGhast.h"
#include "ModelGhast.h"

typedef struct RenderGhast {
    RenderLiving base;
} RenderGhast;

RenderGhast *RenderGhast_create(void);

#endif
