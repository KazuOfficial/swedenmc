#ifndef RENDER_ITEM_H
#define RENDER_ITEM_H

#include "Render.h"
#include "RenderBlocks.h"
#include "JavaRandom.h"
#include "EntityItem.h"
#include "FontRenderer.h"
#include "ItemStack.h"

typedef struct RenderItem {
    Render base;
    RenderBlocks *renderBlocks;
    JavaRandom random;
    int field_27004_a;
} RenderItem;

RenderItem *RenderItem_create(void);

void RenderItem_doRenderItem(RenderItem *self, EntityItem *var1, double var2, double var4,
                             double var6, float var8, float var9);

void RenderItem_drawItemIntoGui(RenderItem *self, FontRenderer *var1, RenderEngine *var2, int var3,
                                int var4, int var5, int var6, int var7);

void RenderItem_renderItemIntoGUI(RenderItem *self, FontRenderer *var1, RenderEngine *var2,
                                  ItemStack *var3, int var4, int var5);

void RenderItem_renderItemOverlayIntoGUI(RenderItem *self, FontRenderer *var1, RenderEngine *var2,
                                         ItemStack *var3, int var4, int var5);

void RenderItem_renderTexturedQuad(RenderItem *self, int var1, int var2, int var3, int var4,
                                   int var5, int var6);

#endif
