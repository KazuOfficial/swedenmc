#include "GuiYesNo.h"
#include "GuiSmallButton.h"
#include "Minecraft.h"
#include <string.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiYesNo_initGui;
    s_vtable.actionPerformed = GuiYesNo_actionPerformed;
    s_vtable.drawScreen = GuiYesNo_drawScreen;
    s_vtable_init = 1;
}

void GuiYesNo_construct(GuiYesNo *self, GuiScreen *var1, const char *var2, const char *var3,
                        const char *var4, const char *var5, int var6) {
    ensure_vtable();
    self->parentScreen = var1;
    self->base.parentScreen = var1;
    strncpy(self->message1, var2, sizeof(self->message1) - 1);
    self->message1[sizeof(self->message1) - 1] = '\0';
    strncpy(self->message2, var3, sizeof(self->message2) - 1);
    self->message2[sizeof(self->message2) - 1] = '\0';
    strncpy(self->field_22106_k, var4, sizeof(self->field_22106_k) - 1);
    self->field_22106_k[sizeof(self->field_22106_k) - 1] = '\0';
    strncpy(self->field_22105_l, var5, sizeof(self->field_22105_l) - 1);
    self->field_22105_l[sizeof(self->field_22105_l) - 1] = '\0';
    self->worldNumber = var6;
    self->base.vtable = &s_vtable;
}

void GuiYesNo_initGui(GuiScreen *self_) {
    GuiYesNo *self = (GuiYesNo *)self_;
    GuiSmallButton *btn0 = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
    GuiSmallButton_construct(btn0, 0, self->base.width / 2 - 155 + 0, self->base.height / 6 + 96,
                             self->field_22106_k);
    GuiSmallButton *btn1 = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
    GuiSmallButton_construct(btn1, 1, self->base.width / 2 - 155 + 160, self->base.height / 6 + 96,
                             self->field_22105_l);
    GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn0);
    GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn1);
}

void GuiYesNo_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiYesNo *self = (GuiYesNo *)self_;
    if (self->parentScreen && self->parentScreen->vtable &&
        self->parentScreen->vtable->deleteWorld) {
        self->parentScreen->vtable->deleteWorld(self->parentScreen, (var1->id == 0) ? 1 : 0,
                                                self->worldNumber);
    }
}

void GuiYesNo_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiYesNo *self = (GuiYesNo *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->message1,
                                 self->base.width / 2, 70, 16777215);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->message2,
                                 self->base.width / 2, 90, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
