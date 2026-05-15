#ifndef RENDER_PAINTING_H
#define RENDER_PAINTING_H

#include "Render.h"
#include "JavaRandom.h"
#include "EntityPainting.h"

typedef struct RenderPainting {
    Render base;
    JavaRandom rand;
} RenderPainting;

RenderPainting *RenderPainting_create(void);

#endif
