#ifndef SORTER_STATS_BLOCK_H
#define SORTER_STATS_BLOCK_H

#include "GuiStats.h"
#include "GuiSlotStatsBlock.h"

struct StatCrafting;

typedef struct SorterStatsBlock {

    GuiStats *field_27299_a;

    GuiSlotStatsBlock *field_27298_b;
} SorterStatsBlock;

void SorterStatsBlock_init(SorterStatsBlock *self, GuiSlotStatsBlock *var1, GuiStats *var2);

int SorterStatsBlock_func_27297_a(SorterStatsBlock *self, struct StatCrafting *var1,
                                  struct StatCrafting *var2);

int SorterStatsBlock_compare(SorterStatsBlock *self, void *var1, void *var2);

#endif
