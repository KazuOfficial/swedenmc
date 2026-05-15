#include "GuiSlotStatsItem.h"
#include "GuiStats.h"
#include "StatList.h"
#include "StatCrafting.h"
#include "StatFileWriter.h"
#include "SorterStatsItem.h"
#include <stdlib.h>

static const GuiSlotVtable s_GuiSlotStatsItem_vtable = {
    GuiSlotStats_getSize,          GuiSlotStats_elementClicked,
    GuiSlotStats_isSelected,       GuiSlot_getContentHeight_default,
    GuiSlotStats_drawBackground,   GuiSlotStatsItem_drawSlot,
    GuiSlotStatsItem_func_27260_a, GuiSlotStats_func_27255_a,
    GuiSlotStats_func_27257_b};

static int s_sorter_item_cmp(void *sorter, void *a, void *b) {
    return SorterStatsItem_compare((SorterStatsItem *)sorter, (struct StatCrafting *)a,
                                   (struct StatCrafting *)b);
}

void GuiSlotStatsItem_construct(GuiSlotStatsItem *self, struct GuiStats *var1) {
    GuiSlotStats_construct(&self->base, var1);
    self->base.base.vtable = &s_GuiSlotStatsItem_vtable;
    self->base.func_27263_a = GuiSlotStatsItem_func_27263_a;
    self->field_27275_a = var1;

    int cap = StatList_field_25186_c_count;
    if (cap > 0) {
        self->base.field_27273_c = (void **)malloc(cap * sizeof(void *));
    }
    for (int i = 0; i < StatList_field_25186_c_count; i++) {
        StatCrafting *var3 = StatList_field_25186_c[i];
        int var4 = 0;
        int var5 = StatCrafting_func_25072_b(var3);
        if (StatFileWriter_writeStat(GuiStats_func_27142_c(var1), &var3->base) > 0) {
            var4 = 1;
        } else if (StatList_field_25170_B[var5] != NULL &&
                   StatFileWriter_writeStat(GuiStats_func_27142_c(var1),
                                            StatList_field_25170_B[var5]) > 0) {
            var4 = 1;
        } else if (StatList_field_25158_z[var5] != NULL &&
                   StatFileWriter_writeStat(GuiStats_func_27142_c(var1),
                                            StatList_field_25158_z[var5]) > 0) {
            var4 = 1;
        }
        if (var4) {
            self->base.field_27273_c[self->base.field_27273_c_count++] = var3;
        }
    }

    SorterStatsItem *sorter = (SorterStatsItem *)calloc(1, sizeof(SorterStatsItem));
    SorterStatsItem_init(sorter, self, var1);
    self->base.field_27272_d = sorter;
    self->base.field_27272_d_cmp = s_sorter_item_cmp;
}

void GuiSlotStatsItem_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3) {
    GuiSlotStatsItem *item = (GuiSlotStatsItem *)self;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    GuiSlotStats_func_27260_a(self, var1, var2, var3);
    if (gs->field_27268_b == 0) {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 115 - 18 + 1, var2 + 1 + 1, 72, 18);
    } else {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 115 - 18, var2 + 1, 72, 18);
    }
    if (gs->field_27268_b == 1) {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 165 - 18 + 1, var2 + 1 + 1, 18, 18);
    } else {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 165 - 18, var2 + 1, 18, 18);
    }
    if (gs->field_27268_b == 2) {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 215 - 18 + 1, var2 + 1 + 1, 36, 18);
    } else {
        GuiStats_func_27128_a(item->field_27275_a, var1 + 215 - 18, var2 + 1, 36, 18);
    }
}

void GuiSlotStatsItem_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                               Tessellator *var5) {
    (void)var4;
    (void)var5;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    struct StatCrafting *var6 = (struct StatCrafting *)GuiSlotStats_func_27264_b(gs, var1);
    if (var6 == NULL)
        return;
    int var7 = StatCrafting_func_25072_b(var6);
    GuiStats_func_27148_a(((GuiSlotStatsItem *)gs)->field_27275_a, var2 + 40, var3, var7);
    GuiSlotStats_func_27265_a(gs, (struct StatCrafting *)StatList_field_25170_B[var7], var2 + 115,
                              var3, var1 % 2 == 0);
    GuiSlotStats_func_27265_a(gs, (struct StatCrafting *)StatList_field_25158_z[var7], var2 + 165,
                              var3, var1 % 2 == 0);
    GuiSlotStats_func_27265_a(gs, var6, var2 + 215, var3, var1 % 2 == 0);
}

const char *GuiSlotStatsItem_func_27263_a(GuiSlotStats *self, int var1) {
    (void)self;
    return var1 == 1 ? "stat.crafted" : (var1 == 2 ? "stat.used" : "stat.depleted");
}
