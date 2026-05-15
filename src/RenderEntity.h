#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "Render.h"

typedef struct RenderEntity {
    Render base;
} RenderEntity;

RenderEntity *RenderEntity_create(void);

#endif
