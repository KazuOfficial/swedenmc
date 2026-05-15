#include "SorterStatsBlock.h"
#include "StatBase.h"
#include "StatFileWriter.h"
#include "StatCrafting.h"
#include "StatList.h"
#include "GuiStats.h"

void SorterStatsBlock_init(SorterStatsBlock *self, GuiSlotStatsBlock *var1, GuiStats *var2) {
    self->field_27298_b = var1;
    self->field_27299_a = var2;
}

int SorterStatsBlock_func_27297_a(SorterStatsBlock *self, struct StatCrafting *var1,
                                  struct StatCrafting *var2) {
    int var3 = StatCrafting_func_25072_b(var1);
    int var4 = StatCrafting_func_25072_b(var2);
    StatBase *var5 = NULL;
    StatBase *var6 = NULL;
    if (self->field_27298_b->base.field_27271_e == 2) {
        var5 = StatList_mineBlockStatArray[var3];
        var6 = StatList_mineBlockStatArray[var4];
    } else if (self->field_27298_b->base.field_27271_e == 0) {
        var5 = StatList_field_25158_z[var3];
        var6 = StatList_field_25158_z[var4];
    } else if (self->field_27298_b->base.field_27271_e == 1) {
        var5 = StatList_field_25172_A[var3];
        var6 = StatList_field_25172_A[var4];
    }
    if (var5 != NULL || var6 != NULL) {
        if (var5 == NULL) {
            return 1;
        }
        if (var6 == NULL) {
            return -1;
        }
        int var7 = StatFileWriter_writeStat(
            GuiStats_func_27142_c(self->field_27298_b->field_27274_a), var5);
        int var8 = StatFileWriter_writeStat(
            GuiStats_func_27142_c(self->field_27298_b->field_27274_a), var6);
        if (var7 != var8) {
            return (var7 - var8) * self->field_27298_b->base.field_27270_f;
        }
    }
    return var3 - var4;
}

int SorterStatsBlock_compare(SorterStatsBlock *self, void *var1, void *var2) {
    return SorterStatsBlock_func_27297_a(self, (struct StatCrafting *)var1,
                                         (struct StatCrafting *)var2);
}
