#include "GuiSlotStatsBlock.h"
#include "GuiStats.h"
#include "StatList.h"
#include "StatCrafting.h"
#include "StatFileWriter.h"
#include "SorterStatsBlock.h"
#include <stdlib.h>

static const GuiSlotVtable s_GuiSlotStatsBlock_vtable = {
    GuiSlotStats_getSize,           GuiSlotStats_elementClicked,
    GuiSlotStats_isSelected,        GuiSlot_getContentHeight_default,
    GuiSlotStats_drawBackground,    GuiSlotStatsBlock_drawSlot,
    GuiSlotStatsBlock_func_27260_a, GuiSlotStats_func_27255_a,
    GuiSlotStats_func_27257_b};

static int s_sorter_block_cmp(void *sorter, void *a, void *b) {
    return SorterStatsBlock_compare((SorterStatsBlock *)sorter, (struct StatCrafting *)a,
                                    (struct StatCrafting *)b);
}

void GuiSlotStatsBlock_construct(GuiSlotStatsBlock *self, struct GuiStats *var1) {
    GuiSlotStats_construct(&self->base, var1);
    self->base.base.vtable = &s_GuiSlotStatsBlock_vtable;
    self->base.func_27263_a = GuiSlotStatsBlock_func_27263_a;
    self->field_27274_a = var1;

    int cap = StatList_field_25185_d_count;
    if (cap > 0) {
        self->base.field_27273_c = (void **)malloc(cap * sizeof(void *));
    }
    for (int i = 0; i < StatList_field_25185_d_count; i++) {
        StatCrafting *var3 = StatList_field_25185_d[i];
        int var4 = 0;
        int var5 = StatCrafting_func_25072_b(var3);
        if (StatFileWriter_writeStat(GuiStats_func_27142_c(var1), &var3->base) > 0) {
            var4 = 1;
        } else if (StatList_field_25172_A[var5] != NULL &&
                   StatFileWriter_writeStat(GuiStats_func_27142_c(var1),
                                            StatList_field_25172_A[var5]) > 0) {
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

    SorterStatsBlock *sorter = (SorterStatsBlock *)calloc(1, sizeof(SorterStatsBlock));
    SorterStatsBlock_init(sorter, self, var1);
    self->base.field_27272_d = sorter;
    self->base.field_27272_d_cmp = s_sorter_block_cmp;
}

void GuiSlotStatsBlock_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3) {
    GuiSlotStatsBlock *blk = (GuiSlotStatsBlock *)self;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    GuiSlotStats_func_27260_a(self, var1, var2, var3);
    if (gs->field_27268_b == 0) {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 115 - 18 + 1, var2 + 1 + 1, 18, 18);
    } else {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 115 - 18, var2 + 1, 18, 18);
    }
    if (gs->field_27268_b == 1) {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 165 - 18 + 1, var2 + 1 + 1, 36, 18);
    } else {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 165 - 18, var2 + 1, 36, 18);
    }
    if (gs->field_27268_b == 2) {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 215 - 18 + 1, var2 + 1 + 1, 54, 18);
    } else {
        GuiStats_func_27128_a(blk->field_27274_a, var1 + 215 - 18, var2 + 1, 54, 18);
    }
}

void GuiSlotStatsBlock_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                Tessellator *var5) {
    (void)var4;
    (void)var5;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    struct StatCrafting *var6 = (struct StatCrafting *)GuiSlotStats_func_27264_b(gs, var1);
    if (var6 == NULL)
        return;
    int var7 = StatCrafting_func_25072_b(var6);
    GuiStats_func_27148_a(((GuiSlotStatsBlock *)gs)->field_27274_a, var2 + 40, var3, var7);
    GuiSlotStats_func_27265_a(gs, (struct StatCrafting *)StatList_field_25158_z[var7], var2 + 115,
                              var3, var1 % 2 == 0);
    GuiSlotStats_func_27265_a(gs, (struct StatCrafting *)StatList_field_25172_A[var7], var2 + 165,
                              var3, var1 % 2 == 0);
    GuiSlotStats_func_27265_a(gs, var6, var2 + 215, var3, var1 % 2 == 0);
}

const char *GuiSlotStatsBlock_func_27263_a(GuiSlotStats *self, int var1) {
    (void)self;
    return var1 == 0 ? "stat.crafted" : (var1 == 1 ? "stat.used" : "stat.mined");
}
