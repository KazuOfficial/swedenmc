#ifndef RENDER_FALLING_SAND_H
#define RENDER_FALLING_SAND_H

#include "Render.h"
#include "EntityFallingSand.h"
#include "RenderBlocks.h"

typedef struct RenderFallingSand {
    Render base;
    RenderBlocks *field_197_d;
} RenderFallingSand;

RenderFallingSand *RenderFallingSand_create(void);

#endif
