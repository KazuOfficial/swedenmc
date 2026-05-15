#include "GuiRenameWorld.h"
#include "Minecraft.h"
#include "ISaveFormat.h"
#include "StringTranslate.h"
#include "GuiTextField.h"
#include <string.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void rw_destroy(GuiScreen *base) {
    GuiRenameWorld *self = (GuiRenameWorld *)base;
    free(self->field_22114_h);
    self->field_22114_h = NULL;
}

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.destroy = rw_destroy;
    s_vtable.updateScreen = GuiRenameWorld_updateScreen;
    s_vtable.initGui = GuiRenameWorld_initGui;
    s_vtable.onGuiClosed = GuiRenameWorld_onGuiClosed;
    s_vtable.actionPerformed = GuiRenameWorld_actionPerformed;
    s_vtable.keyTyped = GuiRenameWorld_keyTyped;
    s_vtable.mouseClicked = GuiRenameWorld_mouseClicked;
    s_vtable.drawScreen = GuiRenameWorld_drawScreen;
    s_vtable_init = 1;
}

void GuiRenameWorld_construct(GuiRenameWorld *self, GuiScreen *var1, const char *var2) {
    ensure_vtable();
    self->field_22112_a = var1;
    self->base.parentScreen = var1;
    self->field_22114_h = NULL;
    strncpy(self->field_22113_i, var2, sizeof(self->field_22113_i) - 1);
    self->field_22113_i[sizeof(self->field_22113_i) - 1] = '\0';
    self->base.vtable = &s_vtable;
}

void GuiRenameWorld_updateScreen(GuiScreen *self_) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    GuiTextField_updateCursorCounter(self->field_22114_h);
}

void GuiRenameWorld_initGui(GuiScreen *self_) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();

    Minecraft_enableKeyRepeatEvents(1);
    self->base.buttonCount = 0;
    GuiButton *btn0 =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 96 + 12,
                         StringTranslate_translateKey(var1, "selectWorld.renameButton"));
    GuiButton *btn1 =
        GuiButton_create(1, self->base.width / 2 - 100, self->base.height / 4 + 120 + 12,
                         StringTranslate_translateKey(var1, "gui.cancel"));
    GuiScreen_addButton((GuiScreen *)self, btn0);
    GuiScreen_addButton((GuiScreen *)self, btn1);

    const char *var4 = "";
    WorldInfo *var3 = NULL;
    struct ISaveFormat *var2 = Minecraft_getSaveLoader(self->base.mc);
    if (var2 != NULL) {
        var3 = ISaveFormat_func_22173_b(var2, self->field_22113_i);
        if (var3 != NULL) {
            var4 = WorldInfo_getWorldName(var3);
        }
    }
    self->field_22114_h = (GuiTextField *)calloc(1, sizeof(GuiTextField));
    GuiTextField_construct(self->field_22114_h, (GuiScreen *)self, self->base.fontRenderer,
                           self->base.width / 2 - 100, 60, 200, 20, var4);
    WorldInfo_free(var3);
    self->field_22114_h->isFocused = 1;
    GuiTextField_setMaxStringLength(self->field_22114_h, 32);
}

void GuiRenameWorld_onGuiClosed(GuiScreen *self_) {

    (void)self_;
    Minecraft_enableKeyRepeatEvents(0);
}

void GuiRenameWorld_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    if (var1->enabled) {
        if (var1->id == 1) {
            Minecraft_displayGuiScreen(self->base.mc, self->field_22112_a);
        } else if (var1->id == 0) {

            struct ISaveFormat *var2 = Minecraft_getSaveLoader(self->base.mc);
            if (var2 != NULL) {
                const char *rawText = GuiTextField_getText(self->field_22114_h);

                while (*rawText == ' ')
                    ++rawText;
                char trimmed[256];
                strncpy(trimmed, rawText, sizeof(trimmed) - 1);
                trimmed[sizeof(trimmed) - 1] = '\0';
                int tlen = (int)strlen(trimmed);
                while (tlen > 0 && trimmed[tlen - 1] == ' ')
                    trimmed[--tlen] = '\0';
                ISaveFormat_func_22170_a(var2, self->field_22113_i, trimmed);
            }
            Minecraft_displayGuiScreen(self->base.mc, self->field_22112_a);
        }
    }
}

void GuiRenameWorld_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    GuiTextField_textboxKeyTyped(self->field_22114_h, var1, var2);

    const char *txt = GuiTextField_getText(self->field_22114_h);
    while (*txt == ' ')
        ++txt;
    int tlen = (int)strlen(txt);
    while (tlen > 0 && txt[tlen - 1] == ' ')
        --tlen;
    self->base.buttonList[0]->enabled = (tlen > 0) ? 1 : 0;
    if (var1 == 13) {
        if (self->base.buttonCount > 0)
            GuiRenameWorld_actionPerformed((GuiScreen *)self, self->base.buttonList[0]);
    }
}

void GuiRenameWorld_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    GuiScreen_defaultVtable.mouseClicked((GuiScreen *)self, var1, var2, var3);
    GuiTextField_mouseClicked(self->field_22114_h, var1, var2, var3);
}

void GuiRenameWorld_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiRenameWorld *self = (GuiRenameWorld *)self_;
    StringTranslate *var4 = StringTranslate_getInstance();
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                           StringTranslate_translateKey(var4, "selectWorld.renameTitle"),
                           self->base.width / 2, self->base.height / 4 - 60 + 20, 16777215);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "selectWorld.enterName"),
                         self->base.width / 2 - 100, 47, 10526880);
    GuiTextField_drawTextBox(self->field_22114_h);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
