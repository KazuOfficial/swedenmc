#ifndef GUI_SLOT_STATS_GENERAL_H
#define GUI_SLOT_STATS_GENERAL_H

#include "GuiSlot.h"
#include "Tessellator.h"

struct GuiStats;

typedef struct GuiSlotStatsGeneral {
    GuiSlot base;

    struct GuiStats *field_27276_a;
} GuiSlotStatsGeneral;

void GuiSlotStatsGeneral_construct(GuiSlotStatsGeneral *self, struct GuiStats *var1);

int GuiSlotStatsGeneral_getSize(GuiSlot *self);
void GuiSlotStatsGeneral_elementClicked(GuiSlot *self, int var1, int var2);
int GuiSlotStatsGeneral_isSelected(GuiSlot *self, int var1);
int GuiSlotStatsGeneral_getContentHeight(GuiSlot *self);
void GuiSlotStatsGeneral_drawBackground(GuiSlot *self);
void GuiSlotStatsGeneral_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                  Tessellator *var5);

#endif
