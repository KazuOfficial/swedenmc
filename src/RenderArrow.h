#ifndef RENDER_ARROW_H
#define RENDER_ARROW_H

#include "Render.h"
#include "EntityArrow.h"

typedef struct RenderArrow {
    Render base;
} RenderArrow;

RenderArrow *RenderArrow_create(void);

void RenderArrow_renderArrow(RenderArrow *self, EntityArrow *var1, double var2, double var4,
                             double var6, float var8, float var9);

#endif
