#include "GuiSelectWorld.h"
#include "GuiWorldSlot.h"
#include "GuiCreateWorld.h"
#include "GuiRenameWorld.h"
#include "GuiYesNo.h"
#include "Minecraft.h"
#include "MathHelper.h"
#include "PlayerControllerSP.h"
#include "StringTranslate.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ISaveFormat.h"
#include <time.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void gsw_destroy(GuiScreen *base) {
    GuiSelectWorld *self = (GuiSelectWorld *)base;
    SaveFormatComparatorList_free(&self->saveList);
    free(self->worldSlotContainer);
    self->worldSlotContainer = NULL;
}

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiSelectWorld_initGui;
    s_vtable.destroy = gsw_destroy;
    s_vtable.actionPerformed = GuiSelectWorld_actionPerformed;
    s_vtable.drawScreen = GuiSelectWorld_drawScreen;
    s_vtable.deleteWorld = GuiSelectWorld_deleteWorld;
    s_vtable_init = 1;
}

void GuiSelectWorld_construct(GuiSelectWorld *self, GuiScreen *var1) {
    ensure_vtable();
    self->parentScreen = var1;
    self->base.parentScreen = var1;
    self->selected = 0;
    self->selectedWorld = -1;
    SaveFormatComparatorList_init(&self->saveList);
    self->worldSlotContainer = NULL;
    self->deleting = 0;
    self->buttonRename = NULL;
    self->buttonSelect = NULL;
    self->buttonDelete = NULL;
    strncpy(self->screenTitle, "Select world", sizeof(self->screenTitle) - 1);
    self->screenTitle[sizeof(self->screenTitle) - 1] = '\0';
    self->field_22098_o[0] = '\0';
    self->field_22097_p[0] = '\0';
    self->dateFormatter[0] = '\0';
    self->base.vtable = &s_vtable;
}

void GuiSelectWorld_initGui(GuiScreen *self_) {
    GuiSelectWorld *self = (GuiSelectWorld *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();
    strncpy(self->screenTitle, StringTranslate_translateKey(var1, "selectWorld.title"),
            sizeof(self->screenTitle) - 1);
    strncpy(self->field_22098_o, StringTranslate_translateKey(var1, "selectWorld.world"),
            sizeof(self->field_22098_o) - 1);
    strncpy(self->field_22097_p, StringTranslate_translateKey(var1, "selectWorld.conversion"),
            sizeof(self->field_22097_p) - 1);
    GuiSelectWorld_loadSaves(self);
    free(self->worldSlotContainer);
    self->worldSlotContainer = (GuiWorldSlot *)calloc(1, sizeof(GuiWorldSlot));
    GuiWorldSlot_construct(self->worldSlotContainer, self);
    GuiSlot_registerScrollButtons(&self->worldSlotContainer->base, self->base.buttonList, 4, 5);
    GuiSelectWorld_initButtons(self);
}

void GuiSelectWorld_loadSaves(GuiSelectWorld *self) {
    ISaveFormat *var1 = Minecraft_getSaveLoader(self->base.mc);
    SaveFormatComparatorList_free(&self->saveList);
    SaveFormatComparatorList_init(&self->saveList);
    if (var1 != NULL) {
        SaveFormatComparatorList *list = ISaveFormat_func_22176_b(var1);
        if (list != NULL) {

            SaveFormatComparatorList_sort(list);
            self->saveList = *list;
            free(list);
        }
    }
    self->selectedWorld = -1;
}

const char *GuiSelectWorld_getSaveFileName(GuiSelectWorld *self, int var1) {
    if (var1 < 0 || var1 >= self->saveList.size)
        return NULL;
    return SaveFormatComparator_getFileName(self->saveList.data[var1]);
}

const char *GuiSelectWorld_getSaveName(GuiSelectWorld *self, int var1) {
    if (var1 < 0 || var1 >= self->saveList.size)
        return NULL;
    const char *var2 = SaveFormatComparator_getDisplayName(self->saveList.data[var1]);
    if (var2 == NULL || MathHelper_stringNullOrLengthZero(var2)) {
        StringTranslate *var3 = StringTranslate_getInstance();
        static char buf[512];
        snprintf(buf, sizeof(buf), "%s %d", StringTranslate_translateKey(var3, "selectWorld.world"),
                 var1 + 1);
        return buf;
    }
    return var2;
}

void GuiSelectWorld_initButtons(GuiSelectWorld *self) {
    StringTranslate *var1 = StringTranslate_getInstance();
    self->buttonSelect =
        GuiButton_createSized(1, self->base.width / 2 - 154, self->base.height - 52, 150, 20,
                              StringTranslate_translateKey(var1, "selectWorld.select"));
    self->buttonRename =
        GuiButton_createSized(6, self->base.width / 2 - 154, self->base.height - 28, 70, 20,
                              StringTranslate_translateKey(var1, "selectWorld.rename"));
    self->buttonDelete =
        GuiButton_createSized(2, self->base.width / 2 - 74, self->base.height - 28, 70, 20,
                              StringTranslate_translateKey(var1, "selectWorld.delete"));
    GuiScreen_addButton((GuiScreen *)self, self->buttonSelect);
    GuiScreen_addButton((GuiScreen *)self, self->buttonRename);
    GuiScreen_addButton((GuiScreen *)self, self->buttonDelete);
    GuiScreen_addButton(
        (GuiScreen *)self,
        GuiButton_createSized(3, self->base.width / 2 + 4, self->base.height - 52, 150, 20,
                              StringTranslate_translateKey(var1, "selectWorld.create")));
    GuiScreen_addButton((GuiScreen *)self,
                        GuiButton_createSized(0, self->base.width / 2 + 4, self->base.height - 28,
                                              150, 20,
                                              StringTranslate_translateKey(var1, "gui.cancel")));
    self->buttonSelect->enabled = 0;
    self->buttonRename->enabled = 0;
    self->buttonDelete->enabled = 0;
}

void GuiSelectWorld_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiSelectWorld *self = (GuiSelectWorld *)self_;
    if (var1->enabled) {
        if (var1->id == 2) {
            const char *var2 = GuiSelectWorld_getSaveName(self, self->selectedWorld);
            if (var2 != NULL) {
                self->deleting = 1;
                StringTranslate *var3 = StringTranslate_getInstance();
                const char *var4 = StringTranslate_translateKey(var3, "selectWorld.deleteQuestion");

                static char var5buf[512];
                snprintf(var5buf, sizeof(var5buf), "'%s' %s", var2,
                         StringTranslate_translateKey(var3, "selectWorld.deleteWarning"));
                const char *var6 = StringTranslate_translateKey(var3, "selectWorld.deleteButton");
                const char *var7 = StringTranslate_translateKey(var3, "gui.cancel");
                GuiYesNo *var8 = (GuiYesNo *)calloc(1, sizeof(GuiYesNo));
                GuiYesNo_construct(var8, (GuiScreen *)self, var4, var5buf, var6, var7,
                                   self->selectedWorld);
                Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)var8);
            }
        } else if (var1->id == 1) {
            GuiSelectWorld_selectWorld(self, self->selectedWorld);
        } else if (var1->id == 3) {
            GuiCreateWorld *cw = (GuiCreateWorld *)calloc(1, sizeof(GuiCreateWorld));
            GuiCreateWorld_construct(cw, (GuiScreen *)self);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)cw);
        } else if (var1->id == 6) {
            GuiRenameWorld *rw = (GuiRenameWorld *)calloc(1, sizeof(GuiRenameWorld));
            GuiRenameWorld_construct(rw, (GuiScreen *)self,
                                     GuiSelectWorld_getSaveFileName(self, self->selectedWorld));
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)rw);
        } else if (var1->id == 0) {
            Minecraft_displayGuiScreen(self->base.mc, self->parentScreen);
        } else {
            GuiSlot_actionPerformed(&self->worldSlotContainer->base, var1);
        }
    }
}

void GuiSelectWorld_selectWorld(GuiSelectWorld *self, int var1) {
    Minecraft_displayGuiScreen(self->base.mc, NULL);
    if (!self->selected) {
        self->selected = 1;
        PlayerController_destroy(self->base.mc->playerController);
        self->base.mc->playerController =
            (PlayerController *)PlayerControllerSP_create(self->base.mc);
        const char *var2 = GuiSelectWorld_getSaveFileName(self, var1);
        static char fallback[32];
        if (var2 == NULL) {
            snprintf(fallback, sizeof(fallback), "World%d", var1);
            var2 = fallback;
        }
        Minecraft_startWorld(self->base.mc, var2, GuiSelectWorld_getSaveName(self, var1), 0L);
        Minecraft_displayGuiScreen(self->base.mc, NULL);
    }
}

void GuiSelectWorld_deleteWorld(GuiScreen *self_, int var1, int var2) {
    GuiSelectWorld *self = (GuiSelectWorld *)self_;
    if (self->deleting) {
        self->deleting = 0;
        if (var1) {
            ISaveFormat *var3 = Minecraft_getSaveLoader(self->base.mc);
            if (var3 != NULL) {
                ISaveFormat_flushCache(var3);
                ISaveFormat_func_22172_c(var3, GuiSelectWorld_getSaveFileName(self, var2));
            }
            GuiSelectWorld_loadSaves(self);
        }
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)self);
    }
}

void GuiSelectWorld_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiSelectWorld *self = (GuiSelectWorld *)self_;
    struct Minecraft *mc = self->base.mc;
    GuiScreen *savedScreen = mc->currentScreen;
    GuiSlot_drawScreen(&self->worldSlotContainer->base, var1, var2, var3);

    if (mc->currentScreen != savedScreen)
        return;
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->screenTitle,
                                 self->base.width / 2, 20, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

int GuiSelectWorld_getSize(GuiSelectWorld *var0) { return var0->saveList.size; }

int GuiSelectWorld_onElementSelected(GuiSelectWorld *var0, int var1) {
    return var0->selectedWorld = var1;
}

int GuiSelectWorld_getSelectedWorld(GuiSelectWorld *var0) { return var0->selectedWorld; }

GuiButton *GuiSelectWorld_getSelectButton(GuiSelectWorld *var0) { return var0->buttonSelect; }

GuiButton *GuiSelectWorld_getRenameButton(GuiSelectWorld *var0) { return var0->buttonRename; }

GuiButton *GuiSelectWorld_getDeleteButton(GuiSelectWorld *var0) { return var0->buttonDelete; }

const char *GuiSelectWorld_func_22087_f(GuiSelectWorld *var0) { return var0->field_22098_o; }

const char *GuiSelectWorld_getDateFormatter(GuiSelectWorld *var0) { return var0->dateFormatter; }

const char *GuiSelectWorld_func_22088_h(GuiSelectWorld *var0) { return var0->field_22097_p; }
