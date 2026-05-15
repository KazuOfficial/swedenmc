#ifndef GUI_SELECT_WORLD_H
#define GUI_SELECT_WORLD_H

#include "GuiScreen.h"
#include "GuiButton.h"
#include "SaveFormatComparator.h"

struct GuiWorldSlot;

typedef struct GuiSelectWorld {
    GuiScreen base;

    char dateFormatter[64];

    GuiScreen *parentScreen;

    char screenTitle[256];

    int selected;

    int selectedWorld;

    SaveFormatComparatorList saveList;

    struct GuiWorldSlot *worldSlotContainer;

    char field_22098_o[256];

    char field_22097_p[256];

    int deleting;

    GuiButton *buttonRename;

    GuiButton *buttonSelect;

    GuiButton *buttonDelete;
} GuiSelectWorld;

void GuiSelectWorld_construct(GuiSelectWorld *self, GuiScreen *var1);

void GuiSelectWorld_initGui(GuiScreen *self_);

void GuiSelectWorld_loadSaves(GuiSelectWorld *self);

const char *GuiSelectWorld_getSaveFileName(GuiSelectWorld *self, int var1);

const char *GuiSelectWorld_getSaveName(GuiSelectWorld *self, int var1);

void GuiSelectWorld_initButtons(GuiSelectWorld *self);

void GuiSelectWorld_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiSelectWorld_selectWorld(GuiSelectWorld *self, int var1);

void GuiSelectWorld_deleteWorld(GuiScreen *self_, int var1, int var2);

void GuiSelectWorld_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

int GuiSelectWorld_getSize(GuiSelectWorld *var0);

int GuiSelectWorld_onElementSelected(GuiSelectWorld *var0, int var1);

int GuiSelectWorld_getSelectedWorld(GuiSelectWorld *var0);

GuiButton *GuiSelectWorld_getSelectButton(GuiSelectWorld *var0);

GuiButton *GuiSelectWorld_getRenameButton(GuiSelectWorld *var0);

GuiButton *GuiSelectWorld_getDeleteButton(GuiSelectWorld *var0);

const char *GuiSelectWorld_func_22087_f(GuiSelectWorld *var0);

const char *GuiSelectWorld_getDateFormatter(GuiSelectWorld *var0);

const char *GuiSelectWorld_func_22088_h(GuiSelectWorld *var0);

#endif
