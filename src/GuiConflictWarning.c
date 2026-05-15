#include "GuiConflictWarning.h"
#include "Minecraft.h"
#include "GuiMainMenu.h"
#include "StatCollector.h"

void GuiConflictWarning_updateScreen(GuiConflictWarning *self) { ++self->updateCounter; }

void GuiConflictWarning_initGui(GuiConflictWarning *self) {
    self->base.buttonCount = 0;
    GuiButton *btn = GuiButton_create(0, self->base.width / 2 - 100,
                                      self->base.height / 4 + 120 + 12, "Back to title screen");
    GuiScreen_addButton((GuiScreen *)self, btn);
}

void GuiConflictWarning_actionPerformed(GuiConflictWarning *self, GuiButton *var1) {
    if (var1->enabled) {
        if (var1->id == 0) {
            GuiMainMenu *menu = GuiMainMenu_create();
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)menu);
        }
    }
}

void GuiConflictWarning_drawScreen(GuiConflictWarning *self, int var1, int var2, float var3) {
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, "Level save conflict",
                                 self->base.width / 2, self->base.height / 4 - 60 + 20, 16777215);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "Minecraft detected a conflict in the level save data.",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 0, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "This could be caused by two copies of the game",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 18,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, "accessing the same level.",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 27,
                         10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         "To prevent level corruption, the current game has quit.",
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 45,
                         10526880);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
