#ifndef TILE_ENTITY_RENDERER_PISTON_H
#define TILE_ENTITY_RENDERER_PISTON_H

#include "TileEntitySpecialRenderer.h"
#include "TileEntityPiston.h"
#include "RenderBlocks.h"

typedef struct TileEntityRendererPiston {
    TileEntitySpecialRenderer base;

    RenderBlocks *field_31071_b;
} TileEntityRendererPiston;

TileEntitySpecialRenderer *TileEntityRendererPiston_create(void);

#endif
