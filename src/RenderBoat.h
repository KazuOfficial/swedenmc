#ifndef RENDER_BOAT_H
#define RENDER_BOAT_H

#include "Render.h"
#include "EntityBoat.h"
#include "ModelBase.h"

typedef struct RenderBoat {
    Render base;
    ModelBase *modelBoat;
} RenderBoat;

RenderBoat *RenderBoat_create(void);

void RenderBoat_func_157_a(RenderBoat *self, EntityBoat *var1, double var2, double var4,
                           double var6, float var8, float var9);

#endif
