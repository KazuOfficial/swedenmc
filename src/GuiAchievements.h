#ifndef GUI_ACHIEVEMENTS_H
#define GUI_ACHIEVEMENTS_H

#include "GuiScreen.h"
#include "StatFileWriter.h"

#include "GuiSmallButton.h"

typedef struct GuiAchievements {
    GuiScreen base;

    int field_27121_a;

    int field_27119_i;

    int field_27118_j;

    int field_27117_l;

    double field_27116_m;

    double field_27115_n;

    double field_27114_o;

    double field_27113_p;

    double field_27112_q;

    double field_27111_r;

    int field_27122_w;

    StatFileWriter *field_27120_x;
} GuiAchievements;

void GuiAchievements_construct(GuiAchievements *self, StatFileWriter *var1);

void GuiAchievements_initGui(GuiAchievements *self);

void GuiAchievements_actionPerformed(GuiAchievements *self, struct GuiButton *var1);

void GuiAchievements_keyTyped(GuiAchievements *self, char var1, int var2);

void GuiAchievements_drawScreen(GuiAchievements *self, int var1, int var2, float var3);

void GuiAchievements_updateScreen(GuiAchievements *self);

void GuiAchievements_func_27110_k(GuiAchievements *self);

void GuiAchievements_func_27109_b(GuiAchievements *self, int var1, int var2, float var3);

int GuiAchievements_doesGuiPauseGame(GuiAchievements *self);

#endif
