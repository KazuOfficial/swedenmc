#include "GuiErrorScreen.h"

void GuiErrorScreen_updateScreen(GuiErrorScreen *self) { ++self->field_28098_a; }

void GuiErrorScreen_initGui(GuiErrorScreen *self) { (void)self; }

void GuiErrorScreen_actionPerformed(GuiErrorScreen *self, GuiButton *var1) {
    (void)self;
    (void)var1;
}

void GuiErrorScreen_keyTyped(GuiErrorScreen *self, char var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

void GuiErrorScreen_drawScreen(GuiErrorScreen *self, int var1, int var2, float var3) {
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, "Out of memory!",
                                 self->base.width / 2, self->base.height / 4 - 60 + 20, 16777215);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "Minecraft has run out of memory.", self->base.width / 2 - 140,
                         self->base.height / 4 - 60 + 60 + 0, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "This could be caused by a bug in the game or by the",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 18,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "Java Virtual Machine not being allocated enough",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 27,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "memory. If you are playing in a web browser, try",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 36,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "downloading the game and playing it offline.", self->base.width / 2 - 140,
                         self->base.height / 4 - 60 + 60 + 45, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "To prevent level corruption, the current game has quit.",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 63,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, "Please restart the game.",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 81,
                         10526880);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
