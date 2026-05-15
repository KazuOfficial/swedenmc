#include "GuiControls.h"
#include "Minecraft.h"
#include "StringTranslate.h"
#include "GuiSmallButton.h"
#include <string.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiControls_initGui;
    s_vtable.actionPerformed = GuiControls_actionPerformed;
    s_vtable.keyTyped = GuiControls_keyTyped;
    s_vtable.drawScreen = GuiControls_drawScreen;
    s_vtable_init = 1;
}

void GuiControls_construct(GuiControls *self, GuiScreen *var1, GameSettings *var2) {
    ensure_vtable();
    strncpy(self->screenTitle, "Controls", sizeof(self->screenTitle) - 1);
    self->screenTitle[sizeof(self->screenTitle) - 1] = '\0';
    self->buttonId = -1;
    self->parentScreen = var1;
    self->base.parentScreen = var1;
    self->options = var2;
    self->base.vtable = &s_vtable;
}

int GuiControls_func_20080_j(GuiControls *self) { return self->base.width / 2 - 155; }

void GuiControls_initGui(GuiScreen *self_) {
    GuiControls *self = (GuiControls *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();
    int var2 = GuiControls_func_20080_j(self);
    for (int var3 = 0; var3 < NUM_KEY_BINDINGS; ++var3) {
        GuiSmallButton *btn = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
        GuiSmallButton_constructSized(btn, var3, var2 + var3 % 2 * 160,
                                      self->base.height / 6 + 24 * (var3 >> 1), 70, 20,
                                      GameSettings_getOptionDisplayString(self->options, var3));
        GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn);
    }
    GuiButton *btn = GuiButton_create(200, self->base.width / 2 - 100, self->base.height / 6 + 168,
                                      StringTranslate_translateKey(var1, "gui.done"));
    GuiScreen_addButton((GuiScreen *)self, btn);
    strncpy(self->screenTitle, StringTranslate_translateKey(var1, "controls.title"),
            sizeof(self->screenTitle) - 1);
}

void GuiControls_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiControls *self = (GuiControls *)self_;
    for (int var2 = 0; var2 < NUM_KEY_BINDINGS; ++var2) {
        strncpy(self->base.buttonList[var2]->displayString,
                GameSettings_getOptionDisplayString(self->options, var2),
                sizeof(self->base.buttonList[var2]->displayString) - 1);
    }
    if (var1->id == 200) {
        Minecraft_displayGuiScreen(self->base.mc, self->parentScreen);
    } else {
        self->buttonId = var1->id;
        snprintf(var1->displayString, sizeof(var1->displayString), "> %s <",
                 GameSettings_getOptionDisplayString(self->options, var1->id));
    }
}

void GuiControls_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiControls *self = (GuiControls *)self_;
    if (self->buttonId >= 0) {
        GameSettings_setKeyBinding(self->options, self->buttonId, var2);
        strncpy(self->base.buttonList[self->buttonId]->displayString,
                GameSettings_getOptionDisplayString(self->options, self->buttonId),
                sizeof(self->base.buttonList[self->buttonId]->displayString) - 1);
        self->buttonId = -1;
    } else {
        GuiScreen_defaultVtable.keyTyped((GuiScreen *)self, var1, var2);
    }
}

void GuiControls_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiControls *self = (GuiControls *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->screenTitle,
                                 self->base.width / 2, 20, 16777215);
    int var4 = GuiControls_func_20080_j(self);
    for (int var5 = 0; var5 < NUM_KEY_BINDINGS; ++var5) {
        GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                             GameSettings_getKeyBindingDescription(self->options, var5),
                             var4 + var5 % 2 * 160 + 70 + 6,
                             self->base.height / 6 + 24 * (var5 >> 1) + 7, -1);
    }
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
