#include "GuiSlotStatsGeneral.h"
#include "GuiStats.h"
#include "GuiScreen.h"
#include "FontRenderer.h"
#include "StatList.h"
#include "StatBase.h"
#include "StatFileWriter.h"

static const GuiSlotVtable s_GuiSlotStatsGeneral_vtable = {GuiSlotStatsGeneral_getSize,
                                                           GuiSlotStatsGeneral_elementClicked,
                                                           GuiSlotStatsGeneral_isSelected,
                                                           GuiSlotStatsGeneral_getContentHeight,
                                                           GuiSlotStatsGeneral_drawBackground,
                                                           GuiSlotStatsGeneral_drawSlot,
                                                           NULL,
                                                           NULL,
                                                           NULL};

void GuiSlotStatsGeneral_construct(GuiSlotStatsGeneral *self, struct GuiStats *var1) {
    GuiSlot_construct(&self->base, GuiStats_func_27141_a(var1), var1->base.width, var1->base.height,
                      32, var1->base.height - 64, 10);
    self->base.vtable = &s_GuiSlotStatsGeneral_vtable;
    self->field_27276_a = var1;
    GuiSlot_func_27258_a(&self->base, 0);
}

int GuiSlotStatsGeneral_getSize(GuiSlot *self) {
    (void)self;
    return StatList_field_25187_b_count;
}

void GuiSlotStatsGeneral_elementClicked(GuiSlot *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

int GuiSlotStatsGeneral_isSelected(GuiSlot *self, int var1) {
    (void)self;
    (void)var1;
    return 0;
}

int GuiSlotStatsGeneral_getContentHeight(GuiSlot *self) { return self->vtable->getSize(self) * 10; }

void GuiSlotStatsGeneral_drawBackground(GuiSlot *self) {
    GuiSlotStatsGeneral *gs = (GuiSlotStatsGeneral *)self;
    GuiScreen_drawDefaultBackground((GuiScreen *)gs->field_27276_a);
}

void GuiSlotStatsGeneral_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                  Tessellator *var5) {
    (void)var4;
    (void)var5;
    GuiSlotStatsGeneral *gs = (GuiSlotStatsGeneral *)self;
    StatBase *var6 = StatList_field_25187_b[var1];
    FontRenderer *fr = GuiStats_func_27145_b(gs->field_27276_a);
    GuiScreen_drawString((GuiScreen *)gs->field_27276_a, fr, var6->statName, var2 + 2, var3 + 1,
                         var1 % 2 == 0 ? 16777215 : 9474192);
    int statVal = StatFileWriter_writeStat(GuiStats_func_27142_c(gs->field_27276_a), var6);
    const char *var7 = StatBase_func_27084_a(var6, statVal);
    FontRenderer *fr2 = GuiStats_func_27140_d(gs->field_27276_a);
    FontRenderer *fr3 = GuiStats_func_27146_e(gs->field_27276_a);
    int sw = fr3 ? FontRenderer_getStringWidth(fr3, var7) : 0;
    GuiScreen_drawString((GuiScreen *)gs->field_27276_a, fr2, var7, var2 + 2 + 213 - sw, var3 + 1,
                         var1 % 2 == 0 ? 16777215 : 9474192);
}
