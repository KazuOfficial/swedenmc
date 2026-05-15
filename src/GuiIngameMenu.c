#include "GuiIngameMenu.h"
#include "Minecraft.h"
#include "World.h"
#include "GuiMainMenu.h"
#include "GuiOptions.h"
#include "GuiAchievements.h"
#include "GuiStats.h"
#include "StatCollector.h"
#include "StatFileWriter.h"
#include "StatList.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void GuiIngameMenu_initGui(GuiScreen *self_) {
    GuiIngameMenu *self = (GuiIngameMenu *)self_;
    self->updateCounter2 = 0;
    self->base.buttonCount = 0;
    int var1 = -16;
    GuiButton *btn1 =
        GuiButton_create(1, self->base.width / 2 - 100, self->base.height / 4 + 120 + var1,
                         "Save and quit to title");
    GuiScreen_addButton((GuiScreen *)self, btn1);

    if (Minecraft_isMultiplayerWorld(self->base.mc)) {
        strncpy(self->base.buttonList[0]->displayString, "Disconnect",
                sizeof(self->base.buttonList[0]->displayString) - 1);
    }
    GuiButton *btn4 = GuiButton_create(4, self->base.width / 2 - 100,
                                       self->base.height / 4 + 24 + var1, "Back to game");
    GuiButton *btn0 = GuiButton_create(0, self->base.width / 2 - 100,
                                       self->base.height / 4 + 96 + var1, "Options...");
    GuiButton *btn5 =
        GuiButton_createSized(5, self->base.width / 2 - 100, self->base.height / 4 + 48 + var1, 98,
                              20, StatCollector_translateToLocal("gui.achievements"));
    GuiButton *btn6 =
        GuiButton_createSized(6, self->base.width / 2 + 2, self->base.height / 4 + 48 + var1, 98,
                              20, StatCollector_translateToLocal("gui.stats"));
    GuiScreen_addButton((GuiScreen *)self, btn4);
    GuiScreen_addButton((GuiScreen *)self, btn0);
    GuiScreen_addButton((GuiScreen *)self, btn5);
    GuiScreen_addButton((GuiScreen *)self, btn6);
}

void GuiIngameMenu_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiIngameMenu *self = (GuiIngameMenu *)self_;
    if (var1->id == 0) {
        GuiOptions *opts = (GuiOptions *)calloc(1, sizeof(GuiOptions));
        GuiOptions_construct(opts, (GuiScreen *)self, self->base.mc->gameSettings);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)opts);
    }
    if (var1->id == 1) {
        StatFileWriter_readStat(self->base.mc->statFileWriter, StatList_leaveGameStat, 1);

        if (Minecraft_isMultiplayerWorld(self->base.mc)) {
            World_sendQuittingDisconnectingPacket(self->base.mc->theWorld);
        }
        Minecraft_changeWorld1(self->base.mc, (World *)NULL);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)GuiMainMenu_create());
    }
    if (var1->id == 4) {
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
        Minecraft_setIngameFocus(self->base.mc);
    }
    if (var1->id == 5) {
        GuiAchievements *ach = (GuiAchievements *)calloc(1, sizeof(GuiAchievements));
        GuiAchievements_construct(ach, self->base.mc->statFileWriter);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)ach);
    }
    if (var1->id == 6) {
        GuiStats *stats = (GuiStats *)calloc(1, sizeof(GuiStats));
        GuiStats_construct(stats, (GuiScreen *)self, self->base.mc->statFileWriter);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)stats);
    }
}

void GuiIngameMenu_updateScreen(GuiScreen *self_) {
    GuiIngameMenu *self = (GuiIngameMenu *)self_;
    GuiScreen_defaultVtable.updateScreen((GuiScreen *)self);
    ++self->updateCounter;
}

void GuiIngameMenu_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiIngameMenu *self = (GuiIngameMenu *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);

    int var4 = !World_func_650_a(self->base.mc->theWorld, self->updateCounter2++);
    if (var4 || self->updateCounter < 20) {
        float var5 = ((float)(self->updateCounter % 10) + var3) / 10.0F;
        var5 = MathHelper_sin(var5 * (float)M_PI * 2.0F) * 0.2F + 0.8F;
        int var6 = (int)(255.0F * var5);
        GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, "Saving level..", 8,
                             self->base.height - 16, var6 << 16 | var6 << 8 | var6);
    }
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, "Game menu",
                                 self->base.width / 2, 40, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

static GuiScreenVtable s_vt;
static int s_vt_init = 0;

static void ensure_vt(void) {
    if (s_vt_init)
        return;
    GuiScreen_initVtable(&s_vt);
    s_vt.initGui = GuiIngameMenu_initGui;
    s_vt.actionPerformed = GuiIngameMenu_actionPerformed;
    s_vt.updateScreen = GuiIngameMenu_updateScreen;
    s_vt.drawScreen = GuiIngameMenu_drawScreen;
    s_vt_init = 1;
}

GuiIngameMenu *GuiIngameMenu_create(void) {
    ensure_vt();
    GuiIngameMenu *self = (GuiIngameMenu *)calloc(1, sizeof(GuiIngameMenu));
    if (!self)
        return NULL;
    self->base.vtable = &s_vt;
    self->updateCounter2 = 0;
    self->updateCounter = 0;
    return self;
}

void GuiIngameMenu_free(GuiIngameMenu *self) { free(self); }
