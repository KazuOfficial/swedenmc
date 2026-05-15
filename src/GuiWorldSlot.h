#ifndef GUI_WORLD_SLOT_H
#define GUI_WORLD_SLOT_H

#include "GuiSlot.h"

struct GuiSelectWorld;

typedef struct GuiWorldSlot {
    GuiSlot base;

    struct GuiSelectWorld *parentWorldGui;
} GuiWorldSlot;

void GuiWorldSlot_construct(GuiWorldSlot *self, struct GuiSelectWorld *var1);

int GuiWorldSlot_getSize(GuiSlot *self);
void GuiWorldSlot_elementClicked(GuiSlot *self, int var1, int var2);
int GuiWorldSlot_isSelected(GuiSlot *self, int var1);
int GuiWorldSlot_getContentHeight(GuiSlot *self);
void GuiWorldSlot_drawBackground(GuiSlot *self);
void GuiWorldSlot_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                           Tessellator *var5);

#endif
