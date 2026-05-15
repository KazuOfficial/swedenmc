#ifndef ITEM_RENDERER_H
#define ITEM_RENDERER_H

#include "Minecraft.h"
#include "ItemStack.h"
#include "RenderBlocks.h"
#include "MapItemRenderer.h"
#include "EntityLiving.h"

typedef struct ItemRenderer {
    Minecraft *mc;
    ItemStack *itemToRender;
    float equippedProgress;
    float prevEquippedProgress;
    RenderBlocks renderBlocksInstance;
    MapItemRenderer field_28131_f;
    int field_20099_f;
} ItemRenderer;

void ItemRenderer_init(ItemRenderer *self, Minecraft *mc);

void ItemRenderer_renderItem(ItemRenderer *self, EntityLiving *entity, ItemStack *stack);

void ItemRenderer_renderItemInFirstPerson(ItemRenderer *self, float partialTick);

void ItemRenderer_renderOverlays(ItemRenderer *self, float partialTick);

void ItemRenderer_updateEquippedItem(ItemRenderer *self);

void ItemRenderer_func_9449_b(ItemRenderer *self);
void ItemRenderer_func_9450_c(ItemRenderer *self);

#endif
