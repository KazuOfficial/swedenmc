#ifndef GUI_SLOT_STATS_ITEM_H
#define GUI_SLOT_STATS_ITEM_H

#include "GuiSlotStats.h"

struct GuiStats;

typedef struct GuiSlotStatsItem {
    GuiSlotStats base;

    struct GuiStats *field_27275_a;
} GuiSlotStatsItem;

void GuiSlotStatsItem_construct(GuiSlotStatsItem *self, struct GuiStats *var1);

void GuiSlotStatsItem_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3);
void GuiSlotStatsItem_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                               Tessellator *var5);

const char *GuiSlotStatsItem_func_27263_a(GuiSlotStats *self, int var1);

#endif
