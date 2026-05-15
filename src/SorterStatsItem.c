#include "SorterStatsItem.h"
#include "StatBase.h"
#include "StatFileWriter.h"
#include "StatCrafting.h"
#include "StatList.h"
#include "GuiStats.h"

void SorterStatsItem_init(SorterStatsItem *self, GuiSlotStatsItem *var1, GuiStats *var2) {
    self->field_27372_b = var1;
    self->field_27373_a = var2;
}

int SorterStatsItem_func_27371_a(SorterStatsItem *self, struct StatCrafting *var1,
                                 struct StatCrafting *var2) {
    int var3 = StatCrafting_func_25072_b(var1);
    int var4 = StatCrafting_func_25072_b(var2);
    StatBase *var5 = NULL;
    StatBase *var6 = NULL;
    if (self->field_27372_b->base.field_27271_e == 0) {
        var5 = StatList_field_25170_B[var3];
        var6 = StatList_field_25170_B[var4];
    } else if (self->field_27372_b->base.field_27271_e == 1) {
        var5 = StatList_field_25158_z[var3];
        var6 = StatList_field_25158_z[var4];
    } else if (self->field_27372_b->base.field_27271_e == 2) {
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
            GuiStats_func_27142_c(self->field_27372_b->field_27275_a), var5);
        int var8 = StatFileWriter_writeStat(
            GuiStats_func_27142_c(self->field_27372_b->field_27275_a), var6);
        if (var7 != var8) {
            return (var7 - var8) * self->field_27372_b->base.field_27270_f;
        }
    }
    return var3 - var4;
}

int SorterStatsItem_compare(SorterStatsItem *self, void *var1, void *var2) {
    return SorterStatsItem_func_27371_a(self, (struct StatCrafting *)var1,
                                        (struct StatCrafting *)var2);
}
