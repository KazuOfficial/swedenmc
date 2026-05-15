#ifndef GUI_SLOT_STATS_BLOCK_H
#define GUI_SLOT_STATS_BLOCK_H

#include "GuiSlotStats.h"

struct GuiStats;

typedef struct GuiSlotStatsBlock {
    GuiSlotStats base;

    struct GuiStats *field_27274_a;
} GuiSlotStatsBlock;

void GuiSlotStatsBlock_construct(GuiSlotStatsBlock *self, struct GuiStats *var1);

void GuiSlotStatsBlock_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3);
void GuiSlotStatsBlock_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                Tessellator *var5);

const char *GuiSlotStatsBlock_func_27263_a(GuiSlotStats *self, int var1);

#endif
