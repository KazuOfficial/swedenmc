#ifndef GUI_STATS_H
#define GUI_STATS_H

#include "GuiScreen.h"
#include "FontRenderer.h"

struct StatFileWriter;
struct GuiSlotStatsGeneral;
struct GuiSlotStatsItem;
struct GuiSlotStatsBlock;
struct GuiSlot;

typedef struct GuiStats {
    GuiScreen base;

    GuiScreen *field_27152_a;

    char field_27154_i[256];

    struct GuiSlotStatsGeneral *field_27151_l;

    struct GuiSlotStatsItem *field_27150_m;

    struct GuiSlotStatsBlock *field_27157_n;

    struct StatFileWriter *field_27156_o;

    struct GuiSlot *field_27155_p;
} GuiStats;

void GuiStats_construct(GuiStats *self, GuiScreen *var1, struct StatFileWriter *var2);

void GuiStats_initGui(GuiStats *self);

void GuiStats_func_27130_k(GuiStats *self);

void GuiStats_actionPerformed(GuiStats *self, GuiButton *var1);

void GuiStats_drawScreen(GuiStats *self, int var1, int var2, float var3);

void GuiStats_func_27138_c(GuiStats *self, int var1, int var2, int var3);

void GuiStats_func_27147_a(GuiStats *self, int var1, int var2);

void GuiStats_func_27136_c(GuiStats *self, int var1, int var2, int var3, int var4);

struct Minecraft *GuiStats_func_27141_a(GuiStats *var0);

FontRenderer *GuiStats_func_27145_b(GuiStats *var0);

struct StatFileWriter *GuiStats_func_27142_c(GuiStats *var0);

FontRenderer *GuiStats_func_27140_d(GuiStats *var0);

FontRenderer *GuiStats_func_27146_e(GuiStats *var0);

struct Minecraft *GuiStats_func_27143_f(GuiStats *var0);

void GuiStats_func_27128_a(GuiStats *var0, int var1, int var2, int var3, int var4);

struct Minecraft *GuiStats_func_27149_g(GuiStats *var0);

FontRenderer *GuiStats_func_27133_h(GuiStats *var0);

FontRenderer *GuiStats_func_27137_i(GuiStats *var0);

FontRenderer *GuiStats_func_27132_j(GuiStats *var0);

FontRenderer *GuiStats_func_27134_k(GuiStats *var0);

FontRenderer *GuiStats_func_27139_l(GuiStats *var0);

void GuiStats_func_27129_a(GuiStats *var0, int var1, int var2, int var3, int var4, int var5,
                           int var6);

FontRenderer *GuiStats_func_27144_m(GuiStats *var0);

FontRenderer *GuiStats_func_27127_n(GuiStats *var0);

void GuiStats_func_27135_b(GuiStats *var0, int var1, int var2, int var3, int var4, int var5,
                           int var6);

FontRenderer *GuiStats_func_27131_o(GuiStats *var0);

void GuiStats_func_27148_a(GuiStats *var0, int var1, int var2, int var3);

#endif
