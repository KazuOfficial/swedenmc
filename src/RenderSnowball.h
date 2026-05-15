#ifndef RENDER_SNOWBALL_H
#define RENDER_SNOWBALL_H

#include "Render.h"

typedef struct RenderSnowball {
    Render base;

    int itemIconIndex;
} RenderSnowball;

RenderSnowball *RenderSnowball_create(int var1);

#endif
