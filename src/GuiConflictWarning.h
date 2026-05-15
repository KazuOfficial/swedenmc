#ifndef GUI_CONFLICT_WARNING_H
#define GUI_CONFLICT_WARNING_H

#include "GuiScreen.h"

typedef struct GuiConflictWarning {
    GuiScreen base;

    int updateCounter;
} GuiConflictWarning;

void GuiConflictWarning_updateScreen(GuiConflictWarning *self);

void GuiConflictWarning_initGui(GuiConflictWarning *self);

void GuiConflictWarning_actionPerformed(GuiConflictWarning *self, GuiButton *var1);

void GuiConflictWarning_drawScreen(GuiConflictWarning *self, int var1, int var2, float var3);

#endif
