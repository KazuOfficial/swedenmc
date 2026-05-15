#ifndef RENDER_FISH_H
#define RENDER_FISH_H

#include "Render.h"
#include "EntityFish.h"

typedef struct RenderFish {
    Render base;
} RenderFish;

RenderFish *RenderFish_create(void);

#endif
