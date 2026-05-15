#ifndef RENDER_TNT_PRIMED_H
#define RENDER_TNT_PRIMED_H

#include "Render.h"
#include "RenderBlocks.h"
#include "EntityTNTPrimed.h"

typedef struct RenderTNTPrimed {
    Render base;

    RenderBlocks blockRenderer;
} RenderTNTPrimed;

RenderTNTPrimed *RenderTNTPrimed_create(void);

#endif
