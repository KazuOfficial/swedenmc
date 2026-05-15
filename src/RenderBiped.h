#ifndef RENDER_BIPED_H
#define RENDER_BIPED_H

#include "RenderLiving.h"
#include "ModelBiped.h"

typedef struct RenderBiped {
    RenderLiving base;

    ModelBiped *modelBipedMain;
} RenderBiped;

RenderBiped *RenderBiped_create(ModelBiped *var1, float var2);

#endif
