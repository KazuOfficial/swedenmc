#ifndef TILE_ENTITY_SIGN_RENDERER_H
#define TILE_ENTITY_SIGN_RENDERER_H

#include "TileEntitySpecialRenderer.h"
#include "TileEntitySign.h"
#include "SignModel.h"

typedef struct TileEntitySignRenderer {
    TileEntitySpecialRenderer base;

    SignModel signModel;
} TileEntitySignRenderer;

TileEntitySpecialRenderer *TileEntitySignRenderer_create(void);

#endif
