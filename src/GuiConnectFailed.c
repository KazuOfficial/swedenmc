#include "GuiConnectFailed.h"
#include "Minecraft.h"
#include "GuiMainMenu.h"
#include "StringTranslate.h"
#include <string.h>
#include <stdio.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.updateScreen = GuiConnectFailed_updateScreen;
    s_vtable.keyTyped = GuiConnectFailed_keyTyped;
    s_vtable.initGui = GuiConnectFailed_initGui;
    s_vtable.actionPerformed = GuiConnectFailed_actionPerformed;
    s_vtable.drawScreen = GuiConnectFailed_drawScreen;
    s_vtable_init = 1;
}

void GuiConnectFailed_construct(GuiConnectFailed *self, const char *var1, const char *var2,
                                const char *var3) {
    ensure_vtable();

    StringTranslate *var4 = StringTranslate_getInstance();
    strncpy(self->errorMessage, StringTranslate_translateKey(var4, var1 ? var1 : ""), 255);
    self->errorMessage[255] = '\0';
    if (var3 != NULL) {

        strncpy(self->errorDetail, StringTranslate_translateKeyFormat(var4, var2 ? var2 : "", var3),
                255);
        self->errorDetail[255] = '\0';
    } else {

        strncpy(self->errorDetail, StringTranslate_translateKey(var4, var2 ? var2 : ""), 255);
        self->errorDetail[255] = '\0';
    }
    self->base.vtable = &s_vtable;
}

void GuiConnectFailed_updateScreen(GuiScreen *self_) { (void)self_; }

void GuiConnectFailed_keyTyped(GuiScreen *self_, char var1, int var2) {
    (void)self_;
    (void)var1;
    (void)var2;
}

void GuiConnectFailed_initGui(GuiScreen *self_) {
    GuiConnectFailed *self = (GuiConnectFailed *)self_;
    self->base.buttonCount = 0;
    StringTranslate *var1 = StringTranslate_getInstance();
    GuiButton *btn =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 120 + 12,
                         StringTranslate_translateKey(var1, "gui.toMenu"));
    GuiScreen_addButton((GuiScreen *)self, btn);
}

void GuiConnectFailed_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiConnectFailed *self = (GuiConnectFailed *)self_;
    if (var1->id == 0) {
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)GuiMainMenu_create());
    }
}

void GuiConnectFailed_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiConnectFailed *self = (GuiConnectFailed *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->errorMessage,
                                 self->base.width / 2, self->base.height / 2 - 50, 16777215);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->errorDetail,
                                 self->base.width / 2, self->base.height / 2 - 10, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
