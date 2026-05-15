#ifndef SORTER_STATS_ITEM_H
#define SORTER_STATS_ITEM_H

#include "GuiStats.h"
#include "GuiSlotStatsItem.h"

struct StatCrafting;

typedef struct SorterStatsItem {

    GuiStats *field_27373_a;

    GuiSlotStatsItem *field_27372_b;
} SorterStatsItem;

void SorterStatsItem_init(SorterStatsItem *self, GuiSlotStatsItem *var1, GuiStats *var2);

int SorterStatsItem_func_27371_a(SorterStatsItem *self, struct StatCrafting *var1,
                                 struct StatCrafting *var2);

int SorterStatsItem_compare(SorterStatsItem *self, void *var1, void *var2);

#endif
