#ifndef RENDER_MINECART_H
#define RENDER_MINECART_H

#include "Render.h"
#include "ModelBase.h"
#include "ModelMinecart.h"
#include "EntityMinecart.h"

typedef struct RenderMinecart {
    Render base;
    ModelBase *modelMinecart;
} RenderMinecart;

RenderMinecart *RenderMinecart_create(void);

#endif
