#ifndef GUI_TEXTURE_PACK_SLOT_H
#define GUI_TEXTURE_PACK_SLOT_H

#include "GuiSlot.h"
#include "Tessellator.h"

struct GuiTexturePacks;

typedef struct GuiTexturePackSlot {
    GuiSlot base;

    struct GuiTexturePacks *parentTexturePackGui;
} GuiTexturePackSlot;

void GuiTexturePackSlot_construct(GuiTexturePackSlot *self, struct GuiTexturePacks *var1);

int GuiTexturePackSlot_getSize(GuiSlot *self);
void GuiTexturePackSlot_elementClicked(GuiSlot *self, int var1, int var2);
int GuiTexturePackSlot_isSelected(GuiSlot *self, int var1);
int GuiTexturePackSlot_getContentHeight(GuiSlot *self);
void GuiTexturePackSlot_drawBackground(GuiSlot *self);
void GuiTexturePackSlot_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                 Tessellator *var5);

#endif
