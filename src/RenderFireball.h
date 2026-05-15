#ifndef RENDER_FIREBALL_H
#define RENDER_FIREBALL_H

#include "Render.h"
#include "EntityFireball.h"

typedef struct RenderFireball {
    Render base;
} RenderFireball;

RenderFireball *RenderFireball_create(void);

#endif
