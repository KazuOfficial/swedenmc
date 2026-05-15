#ifndef TILE_ENTITY_SPECIAL_RENDERER_H
#define TILE_ENTITY_SPECIAL_RENDERER_H

#include "TileEntity.h"
#include "FontRenderer.h"

struct TileEntityRenderer;
struct World;

typedef struct TileEntitySpecialRenderer TileEntitySpecialRenderer;

typedef struct TileEntitySpecialRendererVtable {

    void (*renderTileEntityAt)(TileEntitySpecialRenderer *self, TileEntity *var1, double var2,
                               double var4, double var6, float var8);

    void (*func_31069_a)(TileEntitySpecialRenderer *self, struct World *var1);
} TileEntitySpecialRendererVtable;

struct TileEntitySpecialRenderer {
    const TileEntitySpecialRendererVtable *vtable;

    struct TileEntityRenderer *tileEntityRenderer;
};

void TileEntitySpecialRenderer_bindTextureByName(TileEntitySpecialRenderer *self, const char *var1);

void TileEntitySpecialRenderer_setTileEntityRenderer(TileEntitySpecialRenderer *self,
                                                     struct TileEntityRenderer *var1);

FontRenderer *TileEntitySpecialRenderer_getFontRenderer(TileEntitySpecialRenderer *self);

void TileEntitySpecialRenderer_func_31069_a_default(TileEntitySpecialRenderer *self,
                                                    struct World *var1);

#endif
