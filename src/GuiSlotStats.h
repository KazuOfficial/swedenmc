#ifndef GUI_SLOT_STATS_H
#define GUI_SLOT_STATS_H

#include "GuiSlot.h"
#include "Tessellator.h"

struct GuiStats;
struct StatCrafting;

typedef struct GuiSlotStats {
    GuiSlot base;

    int field_27268_b;

    void **field_27273_c;
    int field_27273_c_count;

    void *field_27272_d;

    int (*field_27272_d_cmp)(void *sorter, void *a, void *b);

    int field_27271_e;

    int field_27270_f;

    struct GuiStats *field_27269_g;

    const char *(*func_27263_a)(struct GuiSlotStats *self, int var1);
} GuiSlotStats;

void GuiSlotStats_construct(GuiSlotStats *self, struct GuiStats *var1);

void GuiSlotStats_elementClicked(GuiSlot *self, int var1, int var2);
int GuiSlotStats_isSelected(GuiSlot *self, int var1);
void GuiSlotStats_drawBackground(GuiSlot *self);
void GuiSlotStats_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3);
void GuiSlotStats_func_27255_a(GuiSlot *self, int var1, int var2);
void GuiSlotStats_func_27257_b(GuiSlot *self, int var1, int var2);
int GuiSlotStats_getSize(GuiSlot *self);

void *GuiSlotStats_func_27264_b(GuiSlotStats *self, int var1);

void GuiSlotStats_func_27265_a(GuiSlotStats *self, struct StatCrafting *var1, int var2, int var3,
                               int var4);

void GuiSlotStats_func_27266_c(GuiSlotStats *self, int var1);

void GuiSlotStats_func_27267_a(GuiSlotStats *self, struct StatCrafting *var1, int var2, int var3);

#endif
