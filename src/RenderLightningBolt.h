#ifndef RENDER_LIGHTNING_BOLT_H
#define RENDER_LIGHTNING_BOLT_H

#include "Render.h"
#include "EntityLightningBolt.h"

typedef struct RenderLightningBolt {
    Render base;
} RenderLightningBolt;

RenderLightningBolt *RenderLightningBolt_create(void);

#endif
